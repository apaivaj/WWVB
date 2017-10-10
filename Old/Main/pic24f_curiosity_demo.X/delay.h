
 
#ifndef _DELAY_H
#define _DELAY_H


/* Count to generate 1ms delay at 20Mhz clock.
  The value is obtained by toggling the Port pins to generate 2Kz square wave */
#define C_CountForOneMsDelay_U16 300u



/***************************************************************************************************
                             Function prototypes
***************************************************************************************************/
void DELAY_us(uint16_t us_count);
void DELAY_ms(uint16_t ms_count);
void DELAY_sec(uint16_t var_delaySecCount_u16);
/**************************************************************************************************/
#endif