/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include <stdint.h>

#include "leds.h"
#include "buttons.h"
#include "leds_rgb.h"
#include "adc.h"
#include "timer_1ms.h"
#include "i2c.h"
#include "leds.h"
#include "../h/xc.h"





//------------------------------------------------------------------------------
//Application related definitions
//------------------------------------------------------------------------------
//Calculate baud rate of I2C
/*
* I2CxBRG = (FCY/FSCL - FCY/10,000,000) - 1
* Based on FCY = FOSC/2; Doze mode and PLL are disabled.
*/
#define BUTTON_DEBOUCE_TIME_MS      20
#define BUFF_MAX 128
#define Fosc (8000000) // crystal
#define Fcy (Fosc*4/2) // w.PLL (Instruction Per Second)
#define Fsck 400000 // 400kHz I2C
#define I2C1_BRG ((Fcy/2/Fsck)-1)

uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;
unsigned long prev, interval = 1000;

int main(void)
{
    unsigned char *letras;
    unsigned char tx_data[] = {'P', 'I', '\0'};
    // char c='M';
    //Get your pointer to a variable
    letras = tx_data;
    //Set-up LEDs
    InitI2C();
      
    //Configure and enable the I/O pins controlling the general purpose LEDs, and 
    //the PWM outputs controlling the RGB LED.
    LED_Enable(LED_LD1);
    LED_Enable(LED_LD2);
    LED_RGB_Enable(LED_RGB_LD3);
    
    //Enable and configure the ADC so it can sample the potentiometer.
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    
    //Turn on a timer, so to generate periodic interrupts.
    TIMER_SetConfiguration(TIMER_CONFIGURATION_1MS);
     return 0;

}