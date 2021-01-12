/* 
 * File:   usb.h
 * Author: exp10d3r
 *
 * Created on March 12, 2016, 10:49 PM
 */

#ifndef USB_H
#define	USB_H

#define NO_PING_PONG

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define GET_STATUS 0x00
#define CLEAR_FEATURE 0x01
#define SET_FEATURE 0x03

#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05    
    
/* USB standard request code */
#define STD_GET_STATUS_ZERO           0x8000
#define STD_GET_STATUS_INTERFACE      0x8100
#define STD_GET_STATUS_ENDPOINT       0x8200

#define STD_CLEAR_FEATURE_ZERO        0x0001
#define STD_CLEAR_FEATURE_INTERFACE   0x0101
#define STD_CLEAR_FEATURE_ENDPOINT    0x0201

#define STD_SET_FEATURE_ZERO          0x0003
#define STD_SET_FEATURE_INTERFACE     0x0103
#define STD_SET_FEATURE_ENDPOINT      0x0203

#define STD_SET_ADDRESS               0x0005
#define STD_GET_DESCRIPTOR            0x8006
#define STD_GET_DESCRIPTOR_DEVICE     0x8006
#define STD_GET_DESCRIPTOR_INTERFACE  0x8106
#define STD_GET_DESCRIPTOR_ENDPOINT   0x8206
#define STD_SET_DESCRIPTOR            0x0007
#define STD_GET_CONFIGURATION         0x8008
#define STD_SET_CONFIGURATION         0x0009
#define STD_GET_INTERFACE             0x810A
#define STD_SET_INTERFACE             0x010B
#define STD_SYNCH_FRAME               0x820C
    
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

   
#define EP_HSHK 1<<4
#define EP_CONDIS 1<<3
#define EP_OUT 1<<2
#define EP_IN 1<<1
#define EP_STALL 1    
    
#define OUT_PID 0x1
#define ACK_PID 0x2
#define DATA0_PID 0x3
#define PING_PID 0x4
#define NYET_PID 0x6
#define DATA2_PID 0x7
#define IN_PID 0x9
#define NAK_PID 0xa
#define DATA1_PID 0xB
#define SETUP_PID 0xD
#define STALL_PID 0xe
#define MDATA_PID 0xf
    
    #define _EP_IN      0x80
#define _EP_OUT     0x00
#define _EP01_OUT   0x01
#define _EP01_IN    0x81
#define _EP02_OUT   0x02
#define _EP02_IN    0x82
#define _EP03_OUT   0x03
#define _EP03_IN    0x83
#define _EP04_OUT   0x04
#define _EP04_IN    0x84
#define _EP05_OUT   0x05
#define _EP05_IN    0x85
#define _EP06_OUT   0x06
#define _EP06_IN    0x86
#define _EP07_OUT   0x07
#define _EP07_IN    0x87
#define _EP08_OUT   0x08
#define _EP08_IN    0x88
#define _EP09_OUT   0x09
#define _EP09_IN    0x89
#define _EP10_OUT   0x0A
#define _EP10_IN    0x8A
#define _EP11_OUT   0x0B
#define _EP11_IN    0x8B
#define _EP12_OUT   0x0C
#define _EP12_IN    0x8C
#define _EP13_OUT   0x0D
#define _EP13_IN    0x8D
#define _EP14_OUT   0x0E
#define _EP14_IN    0x8E
#define _EP15_OUT   0x0F
#define _EP15_IN    0x8F

    
#define _INTERRUPT        0x03            //Interrupt Transfer
    
#define USB_UNICODE(a)			(a), 0x00

#define 	USB_DESCRIPTOR_CONFIGURATION   0x02
#define 	USB_DESCRIPTOR_DEVICE   0x01
#define 	USB_DESCRIPTOR_DEVICE_QUALIFIER   0x06
#define 	USB_DESCRIPTOR_ENDPOINT   0x05
#define 	USB_DESCRIPTOR_INTERFACE   0x04
#define 	USB_DESCRIPTOR_INTERFACE_POWER   0x08
#define 	USB_DESCRIPTOR_OTG   0x09
#define 	USB_DESCRIPTOR_OTHER_SPEED   0x07
#define 	USB_DESCRIPTOR_STRING   0x03
#define 	USB_FEATURE_DEVICE_REMOTE_WAKEUP   1
#define 	USB_FEATURE_DEVICE_REMOTE_WAKEUP   1
#define 	USB_FEATURE_ENDPOINT_HALT   0
#define 	USB_FEATURE_ENDPOINT_STALL   0
#define 	USB_FEATURE_TEST_MODE   2
#define 	USB_FEATURE_TEST_MODE   2
#define 	USB_REQUEST_CLEAR_FEATURE   1
#define 	USB_REQUEST_GET_CONFIGURATION   8
#define 	USB_REQUEST_GET_DESCRIPTOR   6
#define 	USB_REQUEST_GET_INTERFACE   10
#define 	USB_REQUEST_GET_STATUS   0
#define 	USB_REQUEST_SET_ADDRESS   5
#define 	USB_REQUEST_SET_CONFIGURATION   9
#define 	USB_REQUEST_SET_DESCRIPTOR   7
#define 	USB_REQUEST_SET_FEATURE   3
#define 	USB_REQUEST_SET_INTERFACE   11
#define 	USB_REQUEST_SYNCH_FRAME   12
    
#define 	USB_SETUP_DEVICE_TO_HOST   0x80
#define 	USB_SETUP_HOST_TO_DEVICE   0x00
#define 	USB_SETUP_RECIPIENT_DEVICE   0x00
#define 	USB_SETUP_RECIPIENT_ENDPOINT   0x02
#define 	USB_SETUP_RECIPIENT_INTERFACE   0x01
#define 	USB_SETUP_RECIPIENT_OTHER   0x03
#define 	USB_SETUP_TYPE_CLASS   0x20
#define 	USB_SETUP_TYPE_STANDARD   0x00
#define 	USB_SETUP_TYPE_VENDOR   0x40
#define 	USB_TRANSFER_TYPE_BULK   0x02
#define 	USB_TRANSFER_TYPE_CONTROL   0x00
#define 	USB_TRANSFER_TYPE_INTERRUPT   0x03
#define 	USB_TRANSFER_TYPE_ISOCHRONOUS   0x01
    
#define _DEFAULT 0x80
    
    #define DESC_CONFIG_WORD(a) (a&0xFF),((a>>8)&0xFF)
    
    
   
 typedef struct  _USB_DEVICE_DESCRIPTOR
{
    uint8_t bLength;               // Length of this descriptor.
    uint8_t bDescriptorType;       // DEVICE descriptor type (USB_DESCRIPTOR_DEVICE).
    uint16_t bcdUSB;                // USB Spec Release Number (BCD).
    uint8_t bDeviceClass;          // Class code (assigned by the USB-IF). 0xFF-Vendor specific.
    uint8_t bDeviceSubClass;       // Subclass code (assigned by the USB-IF).
    uint8_t bDeviceProtocol;       // Protocol code (assigned by the USB-IF). 0xFF-Vendor specific.
    uint8_t bMaxPacketSize0;       // Maximum packet size for endpoint 0.
    uint16_t idVendor;              // Vendor ID (assigned by the USB-IF).
    uint16_t idProduct;             // Product ID (assigned by the manufacturer).
    uint16_t bcdDevice;             // Device release number (BCD).
    uint8_t iManufacturer;         // Index of String Descriptor describing the manufacturer.
    uint8_t iProduct;              // Index of String Descriptor describing the product.
    uint8_t iSerialNumber;         // Index of String Descriptor with the device's serial number.
    uint8_t bNumConfigurations;    // Number of possible configurations.
} USB_DEVICE_DESCRIPTOR;
    
    typedef struct USB_CONFIGURATION_DESCRIPTOR{
        uint8_t  bLength;             // size of descriptor in bytes          
        uint8_t  bDescriptorType;     // config descriptor code
        uint8_t  wTotalLengthL;       // config+interface+endpoint descriptors
        uint8_t  wTotalLengthH;
        uint8_t  bNumInterfaces;      // Number of interfaces
        uint8_t  bConfigurationValue; // Value to select this interface
        uint8_t  iConfiguration;      // Index of string descriptor
        uint8_t  bmAttributes;        // Powered (self or hub)
        uint8_t  bMaxPower;           // Power usage
    } USB_CONFIGURATION_DESCRIPTOR_t;
    
   typedef struct USB_INTERFACE_DESCRIPTOR {
    unsigned char bLength;            // size of descriptor in bytes
    unsigned char bDescriptorType;    // interface descriptor code
    unsigned char bInterfaceNumber;   // Number of interfaces
    unsigned char bAlternateSetting;  // Value to select alt interface
    unsigned char bNumEndpoints;      // Num endpoints used
    unsigned char bInterfaceClass;    // Class code
    unsigned char bInterfaceSubClass; // Subclass code
    unsigned char bInterfaceProtocol; // Protocol code
    unsigned char iInterface;         // Index of String Descriptor
} USB_INTERFACE_DESCRIPTOR_t;

typedef struct USB_ENDPOINT_DESCRIPTOR {
    unsigned char  bLength;          // size of descriptor in bytes 
    unsigned char  bDescriptorType;  // endpoint descriptor type
    unsigned char  bEndpointAddress; // endpoint bitmap
    unsigned char  bmAttributes;     // attributes bitmap
    unsigned short wMaxPacketSize;   // max packet size
    unsigned char  bInterval;        // polling interval (ignored for control) 
} USB_ENDPOINT_DESCRIPTOR_t;   
    
typedef struct USB_SETUP{
        uint8_t bmRequestType;
        uint8_t bRequest;
        uint8_t wValueL;
        uint8_t wValueH;
        uint8_t wIndexL;
        uint8_t wIndexH;
        uint16_t wLen;
    }USB_SETUP_t;
    
    
enum setupRequestType
{
    REQUEST_TYPE_STANDARD   = 0 << 5,
    REQUEST_TYPE_CLASS      = 1 << 5,
    REQUEST_TYPE_VENDOR     = 2 << 5,
    REQUEST_TYPE_RESERVED   = 3 << 5,
};

/* Buffer Descriptor Status Register Initialization Parameters */
#define _BSTALL     0x04        //Buffer Stall enable
#define _DTSEN      0x08        //Data Toggle Synch enable
#define _INCDIS     0x10        //Address increment disable
#define _KEN        0x20        //SIE keeps buff descriptors enable
#define _DAT0       0x00        //DATA0 packet expected next
#define _DAT1       0x40        //DATA1 packet expected next
#define _DTSMASK    0x40        //DTS Mask
#define _USIE       0x80        //SIE owns buffer
#define _UCPU       0x00        //CPU owns buffer

#define _STAT_MASK  0xFF

/* Buffer Descriptor Status Register Initialization Parameters */
#define _BSTALL     0x04        //Buffer Stall enable
#define _DTSEN      0x08        //Data Toggle Synch enable
#define _INCDIS     0x10        //Address increment disable
#define _KEN        0x20        //SIE keeps buff descriptors enable
#define _DAT0       0x00        //DATA0 packet expected next
#define _DAT1       0x40        //DATA1 packet expected next
#define _DTSMASK    0x40        //DTS Mask
#define _USIE       0x80        //SIE owns buffer
#define _UCPU       0x00        //CPU owns buffer

#define _STAT_MASK  0xFF
    


/* BDT entry structure definition */
typedef union BD_STAT
{
    uint8_t Val;
    struct{
        //If the CPU owns the buffer then these are the values
        unsigned BC8:1;         //bit 8 of the byte count
        unsigned BC9:1;         //bit 9 of the byte count
        unsigned BSTALL:1;      //Buffer Stall Enable
        unsigned DTSEN:1;       //Data Toggle Synch Enable
        unsigned INCDIS:1;      //Address Increment Disable
        unsigned KEN:1;         //BD Keep Enable
        unsigned DTS:1;         //Data Toggle Synch Value
        unsigned UOWN:1;        //USB Ownership
    };
    struct{
        //if the USB module owns the buffer then these are
        // the values
        unsigned BC8:1;         //bit 8 of the byte count
        unsigned BC9:1;         //bit 9 of the byte count
        unsigned PID0:1;        //Packet Identifier
        unsigned PID1:1;
        unsigned PID2:1;
        unsigned PID3:1;
        unsigned :1;
        unsigned UOWN:1;        //USB Ownership
    };
    struct{
        unsigned :2;
        unsigned PID:4;         //Packet Identifier
        unsigned :2;
    };
} BD_STAT_t;                    //Buffer Descriptor Status Register

    typedef struct BD_entry
    {
        BD_STAT_t STAT;
        uint8_t CNT;
        uint8_t* ADR;
    } BD_entry_t;
    
    typedef struct BD_endpoint
    {
        BD_entry_t out;
        BD_entry_t in;
    } BD_endpoint_t;

#define EP0_OUT 0
#define EP0_IN 1
#define EP1_OUT 2
#define EP1_IN 3
#define EP2_OUT 4
#define EP2_IN 5
#define EP3_OUT 6
#define EP3_IN 7
#define EP4_OUT 8
#define EP4_IN 9
#define EP5_OUT 10
#define EP5_IN 11
#define EP6_OUT 12
#define EP6_IN 13
#define EP7_OUT 14
#define EP7_IN 15

      typedef enum {
    DEFAULT = 0,
    POWERED,
    ATTACHED,
    DETACHED,
    ADDRESS_PENDING,
    ADDRESSED,
    CONFIGURED,
    SLEEP
} USB_STATE;

typedef enum {
    SETUP = 0,
    DATA_IN,
    DATA_OUT,
    _STATUS,
    WAIT_SETUP
} TRANSACTION_STAGE;  

#define LEDA_ON PORTCbits.RC2=1;
#define LEDA_OFF PORTCbits.RC2=0;
#define LEDA_TOGGLE PORTCbits.RC2=~PORTCbits.RC2;

#define LEDB_ON PORTCbits.RC1=1;
#define LEDB_OFF PORTCbits.RC1=0;
#define LEDB_TOGGLE PORTCbits.RC1=~PORTCbits.RC1;

#define LEDC_ON PORTCbits.RC0=1;
#define LEDC_OFF PORTCbits.RC0=0;
#define LEDC_TOGGLE PORTCbits.RC0=~PORTCbits.RC0;

#define NO_PING_PONG

#define EP_NUM_MAX 3

#define EP0_BUFF_SIZE 8
#define EP1_BUFF_SIZE 64
#define EP2_BUFF_SIZE 64

#if defined(NO_PING_PONG)
#define EP0_OUT_OFFSET 0
#define EP0_IN_OFFSET EP0_BUFF_SIZE
#define EP1_OUT_OFFSET EP0_IN_OFFSET+EP0_BUFF_SIZE
#define EP1_IN_OFFSET EP1_OUT_OFFSET+EP1_BUFF_SIZE
#define EP2_OUT_OFFSET EP1_IN_OFFSET+EP1_BUFF_SIZE
#define EP2_IN_OFFSET EP2_OUT_OFFSET+EP2_BUFF_SIZE
#elif defined(PING_PONG)
#define EP0_OUT_EVEN_OFFSET 0
#define EP0_OUT_ODD_OFFSET EP0_BUFF_SIZE
#define EP0_IN_EVEN_OFFSET EP0_OUT_ODD_OFFSET+EP0_BUFF_SIZE
#define EP0_IN_ODD_OFFSET EP0_IN_EVEN_OFFSET+EP0_BUFF_SIZE
#define EP1_OUT_EVEN_OFFSET EP0_IN_ODD_OFFSET+EP0_BUFF_SIZE
#define EP1_OUT_ODD_OFFSET EP1_OUT_EVEN_OFFSET+EP1_BUFF_SIZE
#define EP1_IN_EVEN_OFFSET EP1_OUkT_ODD_OFFSET+EP1_BUFF_SIZE
#define EP1_IN_ODD_OFFSET EP1_IN_EVEN_OFFSET+EP1_BUFF_SIZE
#endif
    
void init_usb();
void usb_poll();
void ctl_ack();
void ep0_stall();
void usb_set_tx_packet(uint8_t ep, uint8_t *buf, uint8_t len);
void usb_set_rx_packet(uint8_t ep, uint8_t len);
void configure_ep_in(uint8_t ep, uint8_t* buf, uint8_t len);
void configure_ep_out(uint8_t ep, uint8_t* buf, uint8_t len);
void ctl_send(uint8_t* data, uint16_t len);
void ctl_recv(char* data, uint16_t len);
void usb_set_tx_packet(uint8_t ep, uint8_t *buf, uint8_t len);
//int HID_rxbuffer(char* buf, uint8_t len); replaced by the callback!

#ifdef	__cplusplus
}
#endif

#endif	/* USB_H */

