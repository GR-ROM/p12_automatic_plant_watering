/* 
 * File:   usbdesc.h
 * Author: exp10d3r
 *
 * Created on February 14, 2017, 1:24 AM
 */

#ifndef USBDESC_H
#define	USBDESC_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "usb.h"
    
#define MIN(a,b) (((a)<(b))?(a):(b))
    
#define EP1_BUFF_SIZE 8
#define EP0_BUFF_SIZE 8 
    
#define HID_GAMEPAD_BUTTONS 32

#define CDC_DEVICE                  0x02    
    
/* Device Descriptor */
const USB_DEVICE_DESCRIPTOR device_dsc=
{
    0x12,                   // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,  // DEVICE descriptor type
    0x0200,                 // USB Spec Release Number in BCD format
    CDC_DEVICE,             // Class Code
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    EP0_BUFF_SIZE,          // Max packet size for EP0, see usb_config.h
    0x03EB,                 // Vendor ID
    0x612D,                 // Product ID: CDC RS-232 Emulation Demo
    0x0100,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x00,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

static const char cfgDescriptor[] = {
	/* ============== CONFIGURATION 1 =========== */
	/* Configuration 1 descriptor */
	0x09,   // CbLength
	0x02,   // CbDescriptorType
	0x43,   // CwTotalLength 2 EP + Control
	0x00,
	0x02,   // CbNumInterfaces
	0x01,   // CbConfigurationValue
	0x00,   // CiConfiguration
	0xC0,   // CbmAttributes 0xA0
	0x10,   // CMaxPower

	/* Communication Class Interface Descriptor Requirement */
	0x09, // bLength
	0x04, // bDescriptorType
	0x00, // bInterfaceNumber
	0x00, // bAlternateSetting
	0x01, // bNumEndpoints
	0x02, // bInterfaceClass
	0x02, // bInterfaceSubclass
	0x00, // bInterfaceProtocol
	0x00, // iInterface

	/* Header Functional Descriptor */
	0x05, // bFunction Length
	0x24, // bDescriptor type: CS_INTERFACE
	0x00, // bDescriptor subtype: Header Func Desc
	0x10, // bcdCDC:1.1
	0x01,

	/* ACM Functional Descriptor */
	0x04, // bFunctionLength
	0x24, // bDescriptor Type: CS_INTERFACE
	0x02, // bDescriptor Subtype: ACM Func Desc
	0x00, // bmCapabilities

	/* Union Functional Descriptor */
	0x05, // bFunctionLength
	0x24, // bDescriptorType: CS_INTERFACE
	0x06, // bDescriptor Subtype: Union Func Desc
	0x00, // bMasterInterface: Communication Class Interface
	0x01, // bSlaveInterface0: Data Class Interface

	/* Call Management Functional Descriptor */
	0x05, // bFunctionLength
	0x24, // bDescriptor Type: CS_INTERFACE
	0x01, // bDescriptor Subtype: Call Management Func Desc
	0x00, // bmCapabilities: D1 + D0
	0x01, // bDataInterface: Data Class Interface 1

	/* Endpoint 2 descriptor */
	0x07,   // bLength
	0x05,   // bDescriptorType
	0x82,   // bEndpointAddress, Endpoint 02 - IN
	0x03,   // bmAttributes      INT
	0x08,   // wMaxPacketSize
	0x00,
	0xFF,   // bInterval

	/* Data Class Interface Descriptor Requirement */
	0x09, // bLength 
	0x04, // bDescriptorType
	0x01, // bInterfaceNumber
	0x00, // bAlternateSetting
	0x02, // bNumEndpoints
	0x0A, // bInterfaceClass
	0x00, // bInterfaceSubclass
	0x00, // bInterfaceProtocol
	0x00, // iInterface

	/* First alternate setting */
	/* Endpoint 1 descriptor */
	0x07,   // bLength
	0x05,   // bDescriptorType
	0x01,   // bEndpointAddress, Endpoint 01 - OUT
	0x02,   // bmAttributes      BULK
	EP1_BUFF_SIZE,   // wMaxPacketSize
	0x00,
	0x00,   // bInterval

	/* Endpoint 2 descriptor */
	0x07,   // bLength
	0x05,   // bDescriptorType
	0x81,   // bEndpointAddress, Endpoint 01 - IN
	0x02,   // bmAttributes      BULK
	EP1_BUFF_SIZE,   // wMaxPacketSize
	0x00,
	0x00    // bInterval
};


static const uint8_t strLanguage[] = {
	0x04,
	0x03,
	0x04,
	0x09,
};

static const uint8_t strManufacturer[] = {
    0x32,
    0x03,
    USB_UNICODE('R'),
    USB_UNICODE('o'),
    USB_UNICODE('m'),
    USB_UNICODE('a'),
    USB_UNICODE('n'),
    USB_UNICODE(' '),
    USB_UNICODE('G'),
    USB_UNICODE('r'),
    USB_UNICODE('i'),
    USB_UNICODE('n'),
    USB_UNICODE('e'),
    USB_UNICODE('v'),
    USB_UNICODE(' '),
    USB_UNICODE('I'),
    USB_UNICODE('n'),
    USB_UNICODE('d'),
    USB_UNICODE('u'),
    USB_UNICODE('s'),
    USB_UNICODE('t'),
    USB_UNICODE('r'),
    USB_UNICODE('i'),
    USB_UNICODE('e'),
    USB_UNICODE('s'),   
    USB_UNICODE(' '), 
};


static const uint8_t strProduct[] = {
    sizeof(strProduct),
    0x03,
    USB_UNICODE('U'),
    USB_UNICODE('S'),
    USB_UNICODE('B'),
    USB_UNICODE(' '),
    USB_UNICODE('t'),
    USB_UNICODE('o'),
    USB_UNICODE(' '),
    USB_UNICODE('d'),
    USB_UNICODE('u'),
    USB_UNICODE('a'),
    USB_UNICODE('l'),
    USB_UNICODE(' '),
    USB_UNICODE('s'),
    USB_UNICODE('e'),
    USB_UNICODE('r'),
    USB_UNICODE('i'),
    USB_UNICODE('a'),
    USB_UNICODE('l'),
    USB_UNICODE(' '),
    USB_UNICODE('b'),
    USB_UNICODE('r'),
    USB_UNICODE('i'),
    USB_UNICODE('d'),
    USB_UNICODE('g'),
    USB_UNICODE('e'),
};


#ifdef	__cplusplus
}
#endif

#endif	/* USBDESC_H */

