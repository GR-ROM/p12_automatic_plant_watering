/* 
 * File:   usb_cdc.h
 * Author: exp10der
 *
 * Created on July 14, 2018, 10:43 AM
 */

#ifndef USB_CDC_H
#define	USB_CDC_H

#include <stdint.h>
#include "usb.h"

/* CDC Class Specific Request Code */
#define GET_LINE_CODING               0x21
#define SET_LINE_CODING               0x20
#define SET_CONTROL_LINE_STATE        0x22
#define SERIAL_STATE 0x20
    
/* HID Class */
// HID Descriptor Types
    #define HID_INTF_ID             0x00
#define HID_EP 					1
#define HID_INT_OUT_EP_SIZE     64
#define HID_INT_IN_EP_SIZE      64
#define HID_NUM_OF_DSC          1
#define HID_RPT01_SIZE          29
    
    
    /* HID Interface Class Code */
#define HID_INTF                    0x03
    
#define DSC_HID                 0x21 // HID Class Descriptor
#define DSC_REPORT          0x22 // HID Report Descriptor 
#define DSC_HID_PHYSICAL        0x23
         
#define SIZE_OF_REPORT 0x8
#define HID_GET_REPORT 0xA101    
    
#define HID_CLASS               0xA1
// HID Request Codes
#define GET_REPORT              0x01 // Code for Get Report
#define GET_IDLE                0x02 // Code for Get Idle
#define GET_PROTOCOL            0x03 // Code for Get Protocol
#define SET_REPORT              0x09 // Code for Set Report
#define SET_IDLE                0x0A // Code for Set Idle
#define SET_PROTOCOL            0x0B // Code for Set Protocol
    
#define SEND_ENCAPSULATED_COMMAND   0x2100
#define GET_ENCAPSULATED_RESPONSE   0xA101
#define SEND_BREAK                  0x2123
 
typedef struct LineCoding{
	uint32_t DTERRate;
	uint8_t CharFormat;
	uint8_t ParityType;
	uint8_t DataBits;
} LineCoding_t;

#define NETWORK_CONNECTION          0x00
#define RESPONSE_AVAILABLE          0x01
#define SERIAL_STATE                0x20
/* Device Class Code */


/* Communication Interface Class Code */
#define COMM_INTF                   0x02

/* Communication Interface Class SubClass Codes */
#define ABSTRACT_CONTROL_MODEL      0x02

/* Communication Interface Class Control Protocol Codes */
#define V25TER                      0x01    // Common AT commands ("Hayes(TM)")


/* Data Interface Class Codes */
#define DATA_INTF                   0x0A

/* Data Interface Class Protocol Codes */
#define NO_PROTOCOL                 0x00    // No class specific protocol required


/* Communication Feature Selector Codes */
#define ABSTRACT_STATE              0x01
#define COUNTRY_SETTING             0x02

/* Functional Descriptors */
/* Type Values for the bDscType Field */
#define CS_INTERFACE                0x24
#define CS_ENDPOINT                 0x25

/* bDscSubType in Functional Descriptors */
#define DSC_FN_HEADER               0x00
#define DSC_FN_CALL_MGT             0x01
#define DSC_FN_ACM                  0x02    // ACM - Abstract Control Management
#define DSC_FN_DLM                  0x03    // DLM - Direct Line Managment
#define DSC_FN_TELEPHONE_RINGER     0x04
#define DSC_FN_RPT_CAPABILITIES     0x05
#define DSC_FN_UNION                0x06
#define DSC_FN_COUNTRY_SELECTION    0x07
#define DSC_FN_TEL_OP_MODES         0x08
#define DSC_FN_USB_TERMINAL         0x09
/* more.... see Table 25 in USB CDC Specification 1.1 */

/* CDC Bulk IN transfer states */
#define CDC_TX_READY                0
#define CDC_TX_BUSY                 1
#define CDC_TX_BUSY_ZLP             2       // ZLP: Zero Length Packet
#define CDC_TX_COMPLETING           3

/* Line Coding Structure */
#define LINE_CODING_LENGTH          0x07

typedef union _CONTROL_SIGNAL_BITMAP
{
    uint8_t _byte;
    struct
    {
        unsigned DTE_PRESENT;       // [0] Not Present  [1] Present
        unsigned CARRIER_CONTROL;   // [0] Deactivate   [1] Activate
    };
} CONTROL_SIGNAL_BITMAP;


/* Functional Descriptor Structure - See CDC Specification 1.1 for details */

/* Header Functional Descriptor */
typedef struct _USB_CDC_HEADER_FN_DSC
{
    uint8_t bFNLength;
    uint8_t bDscType;
    uint8_t bDscSubType;
    uint16_t bcdCDC;
} USB_CDC_HEADER_FN_DSC;

/* Abstract Control Management Functional Descriptor */
typedef struct _USB_CDC_ACM_FN_DSC
{
    uint8_t bFNLength;
    uint8_t bDscType;
    uint8_t bDscSubType;
    uint8_t bmCapabilities;
} USB_CDC_ACM_FN_DSC;

/* Union Functional Descriptor */
typedef struct _USB_CDC_UNION_FN_DSC
{
    uint8_t bFNLength;
    uint8_t bDscType;
    uint8_t bDscSubType;
    uint8_t bMasterIntf;
    uint8_t bSaveIntf0;
} USB_CDC_UNION_FN_DSC;

/* Call Management Functional Descriptor */
typedef struct _USB_CDC_CALL_MGT_FN_DSC
{
    uint8_t bFNLength;
    uint8_t bDscType;
    uint8_t bDscSubType;
    uint8_t bmCapabilities;
    uint8_t bDataInterface;
} USB_CDC_CALL_MGT_FN_DSC;


void init_cdc();
int cdc_get_rx_bytes();
void cdc_request_xfer_complete(USB_SETUP_t *usb_setup);
void process_cdc_request(USB_SETUP_t *usb_setup);
int recv_cdc_buf(uint8_t* buf, uint8_t len);
void send_cdc_buf(uint8_t* buf, uint8_t len);
void cdc_init_endpoints();
void handle_cdc_out();
void handle_cdc_in();

#endif	/* USB_CDC_H */

