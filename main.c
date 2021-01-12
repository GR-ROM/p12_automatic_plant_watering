/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC18LF14K50
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "usb.h"
#include "usb_cdc.h"
#include <xc.h>
#include <stdio.h>
#include <stdint.h>

#define LED_ON PORTCbits.RC0=1;
#define LED_OFF PORTCbits.RC0=0;

#define __XTAL_FREQ 48000000

#define ADC_SET_CHANNEL(c) (ADCON0bits.CHS=c)

void putch(unsigned char byte){
    send_cdc_buf(&byte, 1);
}

void __interrupt(high_priority) isr(void){
    if (USBIF){
        usb_poll(); 
        USBIF = 0;
    }
    if (ADIF){
        ADIF=0;
    }
}

int usb_cdc_callback(uint8_t ep, uint8_t* buf, uint8_t len){
    char byte;
    if (buf[0] == 'l'){
        LEDA_TOGGLE
    }
    if (buf[0]=='a'){
        printf("Designed by Roman Grinev JUL/2018\r\n");
    }
}

void init_adc(){
    REFCON0bits.FVR1S0=1;
    REFCON0bits.FVR1S1=1;
    REFCON0bits.FVR1EN=1;
    while(!REFCON0bits.FVR1ST) {} // wait 'till FVR is stable
    
    // we use 2 pin as analog inputs
    ADCON2bits.ADCS=0b101; // Fosc/16
    ADCON2bits.ADFM=1; 
    ADCON2bits.ACQT=0b101; // 12Tad
    ADCON1bits.PVCFG0=0; // Vref=VFR 
    ADCON1bits.PVCFG1=1; // Vref=VFR 
    ADCON0bits.ADON=1;
    ADIE=0;
    ADIF=0;
}
    
uint16_t get_adc(unsigned char ch){
    int i;
    uint16_t value=0;
    uint16_t t=0;
//    ADIE=1;
    for (i=0;i!=16;i++){
        ADIF=0;
        ADC_SET_CHANNEL(ch);
        ADCON0bits.GO_nDONE=1;
        while(GO_nDONE){};
        t=ADRESH;
        t<<=8;
        t|=ADRESL;
        value+=t;
    }
    ADIF=0;
    value=value>>4;
    return value;
}

void start_adc(unsigned char ch){
    ADCON0bits.CHS=0;
    ADC_SET_CHANNEL(ch);
    ADCON0bits.GO_nDONE=1;
    ADIF=0;
    ADIE=1;
}

void main(void){
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    uint32_t i=0;
    uint16_t val=0;
    uint16_t voltage;
    uint16_t voltageh;
    
    ANSELHbits.ANS10=1;
    TRISBbits.TRISB4=1;
    ANSELbits.ANS4=0;
    TRISCbits.RC0=0;
    LED_OFF
    TMR2_StopTimer();
    
    init_adc();
    init_cdc();
    init_usb();
    GIEH = 1;
    GIEL = 1;
    PEIE = 1;
    while (1){
        if (i == 50000000) {
            val=get_adc(10);
            TMR2_StopTimer();
            PORTCbits.RC5=0;
            
            voltage=val*4;
            voltageh=voltage/1000;
            voltage=voltage-(voltageh*1000);
            if (voltageh>1 || voltageh==1 && voltage>=900) LED_ON;
            if (voltageh<1 || voltageh==1 && voltage<=850) LED_OFF;
            printf("ADC: %u, voltage: %u.%uv\r\n\0", val, voltageh, voltage);              
            i = 0;       
        }
        if (i == 49000000){
            TMR2_StartTimer();
        }
        i++;
        asm("nop");
    }
}
/**
 End of File
*/