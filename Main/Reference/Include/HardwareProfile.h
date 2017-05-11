/********************************************************************
 FileName:      HardwareProfile.h
 Dependencies:  See INCLUDES section
 Processor:     PIC18, PIC24, or PIC32 USB Microcontrollers
 Hardware:      See supported boards in the list found in the code below
                  The firmware may be modified for use on other USB 
                    platforms by editing this file (HardwareProfile.h)
 Compiler:  	Microchip C18 (for PIC18), C30 (for PIC24), 
                  or C32 (for PIC32)
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the �Company�) for its PIC� Microcontroller is intended and
 supplied to you, the Company�s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Description
  2.9   Initial Release
********************************************************************/

#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

#include "../Microchip/Include/Compiler.h"
//#define DEMO_BOARD USER_DEFINED_BOARD

#if !defined(DEMO_BOARD)
    #if defined(__C32__)
        #if defined(__32MX460F512L__)
            #if defined(PIC32MX460F512L_PIM)
                #include "HardwareProfile - PIC32MX460F512L PIM.h"
            #elif defined(PIC32_USB_STARTER_KIT)
                #include "HardwareProfile - PIC32 USB Starter Kit.h"
            #endif
        #elif defined(__32MX795F512L__)
            #if defined(PIC32MX795F512L_PIM)
                #include "HardwareProfile - PIC32MX795F512L PIM.h"
            #elif defined(CEREBOT_32MX7)
                #include "HardwareProfile - Cerebot 32MX7.h"
            #elif defined(PIC32_USB_STARTER_KIT)
                //PIC32 USB Starter Kit II
                #include "HardwareProfile - PIC32 USB Starter Kit.h"
            #elif defined(PIC32_ETHERNET_STARTER_KIT)
                #include "HardwareProfile - PIC32 Ethernet Starter Kit.h"
            #endif
        #endif
    #endif

    #if defined(__C30__) || defined __XC16__
        #if defined(__PIC24FJ256GB110__)
            #if defined(PIC24F_ADK_FOR_ANDROID)
                #include "HardwareProfile - PIC24F ADK for Android.h"
            #else
                #include "HardwareProfile - PIC24FJ256GB110 PIM.h"
            #endif
        #elif defined(__PIC24FJ256GB210__)
            #include "HardwareProfile - PIC24FJ256GB210 PIM.h"
        #elif defined(__PIC24FJ256GB106__)
            #include "HardwareProfile - PIC24F Starter Kit.h"
        #elif defined(__PIC24FJ64GB004__)
            #include "HardwareProfile - PIC24FJ64GB004 PIM.h"
        #elif defined(__PIC24FJ64GB502__)
            #include "HardwareProfile - PIC24FJ64GB502 Microstick.h"
        #elif defined(__PIC24FJ256DA210__)
            #include "HardwareProfile - PIC24FJ256DA210 Development Board.h"
        #elif defined(__dsPIC33EP512MU810__)
            #if defined(DSPIC33EP512MU810_PIM)
                #include "HardwareProfile - dsPIC33EP512MU810 PIM.h"
            #elif defined(DSPIC33E_USB_STARTER_KIT)
                #include "HardwareProfile - dsPIC33E USB Starter Kit.h"
            #endif
        #elif defined(__PIC24EP512GU810__)
            #if defined(PIC24EP512GU810_PIM)
                #include "HardwareProfile - PIC24EP512GU810 PIM.h"
            #endif
        #endif
    #endif
#endif

#if !defined(DEMO_BOARD)
#endif

#endif  //HARDWARE_PROFILE_H


/*
 delay_us(x) and delay_ms(x) for C30
 */
 #ifndef __DELAY_H
 #define FOSC  8000000LL  // clock-frequecy in Hz with suffix LL (64-bit-long), eg. 32000000LL for 32MHz
 #define FCY       (FOSC/2)  // MCU is running at FCY MIPS
 #define delay_us(x) __delay32(((x*FCY)/1000000L)) // delays x us
 #define delay_ms(x) __delay32(((x*FCY)/1000L))  // delays x ms
 #define __DELAY_H 1
 #include <libpic30.h>
 #endif