/*========================================
 * Author: Grinev Roman
 * Date: JUL.2018
 * File: usb.c
 * Description: USB device stack
 * Version: 0.4
 * Version history:
 * v0.1 initial release
 * v0.2 wrong windows device descriptor
 * request fixed
 * v0.3 STATUS stage now works
 * v0.4 GET_DESCRIPTOR bug fixed
 * Comments:
=========================================*/
#include "usb.h"
#include "usbdesc.h"
#include <xc.h>
#include <string.h>
#include "usb_cdc.h"

#define FULL_SPEED

#define BD_BASE_ADDR 0x200
#define BD_DATA_ADDR 0x280

volatile BD_endpoint_t endpoints[EP_NUM_MAX] __at(BD_BASE_ADDR);
volatile uint8_t ep_data_buffer[136] __at(BD_DATA_ADDR);
USB_SETUP_t usb_setup;

static uint8_t dev_addr;
static uint8_t control_needs_zlp;
static TRANSACTION_STAGE ctl_stage;
USB_STATE state;

static uint16_t status = 0x00;
static uint8_t current_cnf = 0;
static uint8_t alt_if = 0;
static uint8_t active_protocol = 0;

static uint16_t wCount;
static char* ubuf;
static uint8_t ctrl_transaction_owner;

extern int usb_cdc_callback(uint8_t ep, uint8_t* buf, uint8_t len);

void soft_detach();
void init_usb();
void usb_poll();

static void STATUS_OUT_TOKEN();
static void STATUS_IN_TOKEN();
static void reset_usb();

static void SetupStage(USB_SETUP_t* usb_setup);
static void DataInStage();
static uint8_t DataOutStage();
static void WaitForSetupStage(void);

static void process_standart_request(USB_SETUP_t *usb_setup);

void set_transaction_owner(uint8_t owner) {
    ctrl_transaction_owner = owner;
}

void usb_set_tx_packet(uint8_t ep, uint8_t *buf, uint8_t len) {
    if (endpoints[1].in.ADR != buf || buf != 0) memcpy(endpoints[ep].in.ADR, buf, len);
    endpoints[ep].in.CNT = len;
    endpoints[ep].in.STAT.Val &= _DTSMASK;
    endpoints[ep].in.STAT.DTS = ~endpoints[ep].in.STAT.DTS;
    endpoints[ep].in.STAT.Val |= _USIE | _DTSEN;
}

void usb_set_rx_packet(uint8_t ep, uint8_t len) {
    endpoints[ep].out.CNT = len;
    endpoints[ep].out.STAT.Val &= _DTSMASK;
    endpoints[ep].out.STAT.DTS = ~endpoints[ep].out.STAT.DTS;
    endpoints[ep].out.STAT.Val |= _USIE | _DTSEN;
}

static void STATUS_OUT_TOKEN() {
    endpoints[0].out.CNT = 0x00;
    endpoints[0].out.STAT.Val = _USIE | _DTSEN | _DAT1;
}

static void STATUS_IN_TOKEN() {
    endpoints[0].in.CNT = 0x00;
    endpoints[0].in.STAT.Val = _USIE | _DTSEN | _DAT1;
}

void ep0_stall() {
    endpoints[0].in.STAT.Val = _BSTALL | _USIE;
    endpoints[0].out.STAT.Val = _BSTALL | _USIE;
}

static void reset_usb() {
    memset(0x2000, 0x00, 0x200);
    
    UIR = 0;
    UIEbits.TRNIE = 1;
    UIEbits.URSTIE = 1;
    
    UIEbits.SOFIE = 0;
    UIEbits.STALLIE = 1;
    UIEbits.ACTVIE = 1;
    UIEbits.IDLEIE = 1;

    UCFGbits.PPB0 = 0;
    UCFGbits.PPB1 = 0;
    UCFGbits.UPUEN = 1;

#if defined(LOW_SPEED)
    UCFGbits.FSEN = 0;
#elif defined(FULL_SPEED)
    UCFGbits.FSEN = 1;
#endif
    UCONbits.SUSPND = 0;
    UCONbits.RESUME =0;
    UCONbits.PPBRST = 0;
    UCONbits.USBEN = 1;
    UIRbits.TRNIF = 0;
    UIRbits.TRNIF = 0;
    UIRbits.TRNIF = 0;
    UIRbits.TRNIF = 0;
    USBIF=0;
    USBIE=1;

    configure_ep_out(0, &ep_data_buffer[EP0_OUT_OFFSET], EP0_BUFF_SIZE);
    configure_ep_in(0, &ep_data_buffer[EP0_IN_OFFSET], EP0_BUFF_SIZE);
    UEP0 = EP_IN | EP_OUT | EP_HSHK;
    WaitForSetupStage();
    state = ATTACHED;
}

void init_usb() {
    dev_addr = 0;
    UADDR = 0;
    state = DETACHED;
    reset_usb();
}

void configure_ep_in(uint8_t ep, uint8_t* buf, uint8_t len) {
    endpoints[ep].in.ADR = buf;
    endpoints[ep].in.CNT = len;
    endpoints[ep].in.STAT.Val = 0x00;
}

void configure_ep_out(uint8_t ep, uint8_t* buf, uint8_t len) {
    endpoints[ep].out.ADR = buf;
    endpoints[ep].out.CNT = len;
    endpoints[ep].out.STAT.Val = 0x00;
}

void ctl_send(uint8_t* data, uint16_t len) {
    if ((len > EP0_BUFF_SIZE) && (len % EP0_BUFF_SIZE == 0)) control_needs_zlp = 1;
    else control_needs_zlp = 0;
    wCount = len;
    ubuf = data;
    ctl_stage = DATA_IN;
    // reset DATA0/1 SYNC
    endpoints[0].in.STAT.DTS = 0;
    DataInStage();
}

void ctl_recv(char* data, uint16_t len) {
    wCount = len;
    ubuf = data;
    ctl_stage = DATA_OUT;
    usb_set_rx_packet(0, EP1_BUFF_SIZE);
}

void ctl_ack() {
    wCount = 0;
    ubuf = 0;
    ctl_stage=_STATUS;
    STATUS_IN_TOKEN();
    return;
}

static void process_standart_request(USB_SETUP_t* usb_setup) {
    uint16_t len = 0;
    uint16_t request = (usb_setup->bRequest | (usb_setup->bmRequestType << 8));
    switch (request) {
        case STD_CLEAR_FEATURE_INTERFACE:
        case STD_SET_FEATURE_INTERFACE:
        case STD_CLEAR_FEATURE_ENDPOINT:
        case STD_SET_FEATURE_ENDPOINT:
        case STD_CLEAR_FEATURE_ZERO:
        case STD_SET_FEATURE_ZERO:
            ctl_ack();
            break;
        case STD_GET_STATUS_INTERFACE:
            ctl_send(&status, 2);
            break;
        case STD_GET_INTERFACE:
            ctl_send(&alt_if, 1);
            break;
        case STD_SET_INTERFACE:
            ctl_ack();
            alt_if = usb_setup->wValueH;
            break;
        case STD_GET_STATUS_ENDPOINT:
            ctl_send(&status, 2);
            break;
        case STD_GET_STATUS_ZERO:
            ctl_send(&status, 2);
            break;
        case STD_SET_DESCRIPTOR:
            ctl_recv(0, usb_setup->wLen);
            break;
        case STD_GET_DESCRIPTOR_INTERFACE:
        case STD_GET_DESCRIPTOR_ENDPOINT:
        case STD_GET_DESCRIPTOR:
            len = usb_setup->wLen;
            switch (usb_setup->wValueH) {
                case USB_DEVICE_DESCRIPTOR_TYPE:
                    if (state == ADDRESSED) state = DEFAULT;
                    if (len>sizeof (device_dsc)) len = sizeof (device_dsc);
                    if (state == ATTACHED) len = 8;
                    ctl_send(&device_dsc, len);
                    
                    break;
                case USB_CONFIGURATION_DESCRIPTOR_TYPE:
                    if (len>sizeof (cfgDescriptor)) len = sizeof (cfgDescriptor);
                    ctl_send(&cfgDescriptor[0], len);
                    break;
                case USB_STRING_DESCRIPTOR_TYPE:
                    switch (usb_setup->wValueL) {
                        case 0:
                            if (len>sizeof(strLanguage)) len = sizeof (strLanguage);
                            ctl_send(&strLanguage[0], len);
                            break;
                        case 1:
                            if (len>sizeof(strManufacturer)) len = sizeof (strManufacturer);
                            ctl_send(&strManufacturer[0], len);
                            break;
                        case 2:
                            if (len>sizeof(strProduct)) len = sizeof(strProduct);
                            ctl_send(&strProduct[0], len);
                            break;
                        default:
                            ep0_stall();
                            //case 3:
                            //ctl_send(&strSerial[0], len));
                            //break;
                    }
                    break;
                case USB_INTERFACE_DESCRIPTOR_TYPE:
                    ctl_send(&cfgDescriptor[9], 0x9);
                    break;
                case USB_ENDPOINT_DESCRIPTOR_TYPE:
                    if (usb_setup->wValueL == 1) ctl_send(&cfgDescriptor[sizeof (cfgDescriptor) - 14], 0x7);
                    if (usb_setup->wValueL == 0x81) ctl_send(&cfgDescriptor[sizeof (cfgDescriptor) - 7], 0x7);
                    if (usb_setup->wValueL == 0x82) ctl_send(&cfgDescriptor[sizeof (cfgDescriptor) - 30], 0x7);
                    break;
                default:
                    ep0_stall();
            }
            break;
        case STD_GET_CONFIGURATION:
            ctl_send(&current_cnf, 1);
            break;
        case STD_SET_CONFIGURATION:
            state = CONFIGURED;
            cdc_init_endpoints();
            ctl_ack();
            break;
        case STD_SET_ADDRESS:
            state = ADDRESS_PENDING;
            dev_addr = usb_setup->wValueL;
            ctl_ack();
            break;
        default:
            ep0_stall();
    }
}

static void SetupStage(USB_SETUP_t* usb_setup) {
    ctl_stage = SETUP;
    endpoints[0].in.STAT.UOWN = 0;
    endpoints[0].out.STAT.UOWN = 0;
    switch (usb_setup->bmRequestType & 0x60) {
        case REQUEST_TYPE_STANDARD:
            process_standart_request(usb_setup);
            break;
        case REQUEST_TYPE_CLASS:
            process_cdc_request(usb_setup);
            break;
        default: 
            ep0_stall();
    }
    UCONbits.PKTDIS = 0;
}

static void DataInStage() {
    uint8_t current_transfer_len = MIN(wCount, EP0_BUFF_SIZE);
    if (wCount > 0) {
        usb_set_tx_packet(0, ubuf, current_transfer_len);
        ubuf += current_transfer_len;
        wCount -= current_transfer_len;
    } else {
        if (control_needs_zlp){
            usb_set_tx_packet(0, 0, 0);
            control_needs_zlp = 0;
        } else {
            STATUS_OUT_TOKEN();
            ctl_stage=_STATUS;
        }
    }
}

static uint8_t DataOutStage() {
    uint8_t current_transfer_len = endpoints[0].out.CNT;
    memcpy(ubuf, endpoints[0].out.ADR, current_transfer_len);
    ubuf += current_transfer_len;
    wCount -= current_transfer_len;
    if (wCount > 0) {
        usb_set_rx_packet(0, EP0_BUFF_SIZE);
    } else {
        STATUS_IN_TOKEN();
        ctl_stage = _STATUS;
    }
    return current_transfer_len;
}

static void WaitForSetupStage() {
    ctl_stage = WAIT_SETUP;
    control_needs_zlp = 0;
    UCONbits.PKTDIS = 0;

    endpoints[0].in.STAT.Val = 0x00;
    endpoints[0].out.CNT= EP0_BUFF_SIZE;
    endpoints[0].out.STAT.Val = _USIE | _DTSEN;
}

static void XferComplete(USB_SETUP_t* usb_setup) {
    switch (usb_setup->bmRequestType & 0x60) {
        case REQUEST_TYPE_STANDARD:
            break;
        case REQUEST_TYPE_CLASS:
          //  cdc_request_xfer_complete(usb_setup);
            break;
    }
}

static void UnSuspend(void) {
      UCONbits.SUSPND = 0; // Bring USB module out of power conserve
      while (UIRbits.ACTVIF) UIRbits.ACTVIF = 0;
      UIR &= 0xFB;
}

static void Suspend(void) {
       while (!UIRbits.ACTVIF) UIRbits.ACTVIF = 1; // Enable bus activity interrupt
       UIR &= 0xEF;
       UCONbits.SUSPND = 1; // Put USB module in power conserve
}

void usb_poll() {
    uint8_t PID = 0;
    uint8_t ep = 0;
    if (UIRbits.ACTVIF) {
        if (state == SLEEP) {
            state = CONFIGURED;
            UnSuspend();
        }
        UIRbits.ACTVIF = 0;
    }
    if (UIRbits.IDLEIF) {
        if (state == CONFIGURED){
            state = SLEEP;
            Suspend();
            UIRbits.IDLEIF = 0;
        }
    }
    if (UIRbits.STALLIF) {
        if (UEP0bits.EPSTALL) UEP0bits.EPSTALL = 0;
        WaitForSetupStage();
        UIRbits.STALLIF = 0;
    }
    if (UIRbits.URSTIF) {
        reset_usb();
        URSTIF = 0;
    }
    while (UIRbits.TRNIF) {
        ep = (uint8_t) (USTAT >> 3) & 0x7;
        switch (ep) {
            case 0:
                if (USTATbits.DIR) PID = endpoints[0].in.STAT.PID;
                else PID = endpoints[0].out.STAT.PID;
                switch (PID) {
                    case SETUP_PID:
                        memcpy(&usb_setup, endpoints[0].out.ADR, 8);
                        SetupStage(&usb_setup);
                        break;
                    case OUT_PID:
                        if (ctl_stage == DATA_OUT) DataOutStage();
                        else if (ctl_stage == _STATUS) {
                            WaitForSetupStage();
                            XferComplete(&usb_setup);
                        }
                        break;
                    case IN_PID:
                        if (state == ADDRESS_PENDING) {
                            state = ADDRESSED;
                            UADDR = dev_addr;
                        }
                        if (ctl_stage == DATA_IN) DataInStage();
                        else if (ctl_stage == _STATUS) {
                            WaitForSetupStage();
                            XferComplete(&usb_setup);
                        }
                        break;
                }
                break;
            case 1:
                if (USTATbits.DIR) PID = endpoints[1].in.STAT.PID;
                else PID = endpoints[1].out.STAT.PID;
                if (PID == IN_PID) {
                    endpoints[1].in.STAT.UOWN = 0;
                    handle_cdc_in();
                } else
                if (PID == OUT_PID) {
                    endpoints[1].out.STAT.UOWN = 0;
                    //  handle_cdc_out();
                    usb_cdc_callback(1, endpoints[1].out.ADR, endpoints[1].out.CNT);
                    usb_set_rx_packet(1, EP1_BUFF_SIZE);
                }
                break;
            case 2:
                PID = endpoints[2].in.STAT.PID;
                //if (PID == IN_PID) usb_set_tx_packet(2, 0, 0);
                break;
        }
        TRNIF = 0;
    }
}