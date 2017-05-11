//****************************************
// IR_Remote0038
// MCU: PIC24F
// 2013.10.12
// Regis Hsu
//
// Assign U1RX To Pin RP16 (PIN51, F3)
// Assign U1TX To Pin RP30 (PIN52, F2)
// Link these 2 pins together for test, the LCD will show "LOVE"
//****************************************

#include "Compiler.h"
#include "HardwareProfile.h"

#include "FreeRTOS.h"
#include "task.h"
#include "ir_remote.h"
#include "cn_intr.h"

#if 1
// for stand along test program

#if 0
BYTE IR_ReptFlag, IR_ReceiveFlag;
portTickType tCount;
unsigned char IR_Temp[4], IR_Data[4];

#define IR_LEADTIME_CNT     211   // (9+4.5ms) / 64us
#define IR_DATA_1           211
#define IR_DATA_0           211
#define IR_REPEAT           211

//-----------------------------------------------------------------------------
// Wait0(WORD overtime)
//-----------------------------------------------------------------------------
char Wait0(WORD OverTime)
{
    tCount = 0;
    while(IR_input == 1)
    {
        if(tCount > OverTime)
            return (1);
    }
    return(0);
}
//-----------------------------------------------------------------------------
// Wait1(WORD overtime)
//-----------------------------------------------------------------------------
char Wait1(WORD OverTime)
{
   tCount = 0;
   while(IR_input == 0)
   {
        if(tCount > OverTime)
            return (1);
   }
   return(0);
}

void Irbyte(void)
{
  BYTE i,j=0;
  IR_ReptFlag = 0;

  if(Wait0(60))     // High超過6ms
         return;
  if(Wait1(91))     // Low超過9.1ms
        return;
  if(Wait0(46))     // High超過4.5ms
         return;
  else if(tCount <= 21)
         goto IR_Rept;
  else
         goto IR_Data;
IR_Rept:
  if(Wait1(7))
         return;
  IR_ReptFlag = 1;
  if(Wait0(60))
         return;
IR_Data:
  // Data Format
  for (i=0; i<4; i++)
      IR_Temp[i] = 0;

  //總共處理32 bits
  for(i=0; i<32; i++)
   {
        j = i/8;
        tCount = 0;
        if(Wait1(7) == 1)   //當Low持續700us,表示資料格式信號異常
            return;
        if(Wait0(20) == 1) //當High持續2ms,表示資料格式信號異常
            return;

        //開始偵測
        IR_Temp[j] >>= 1;

        if (tCount < 12)              //當電位'1'小於1.2ms表示此位元為'0'
            IR_Temp[j] |= 0x00;
        else
            IR_Temp[j] |= 0x80;        //當電位'1'大於1.2ms表示此位元為'1'

   }
  for (i=0; i<4; i++)
      IR_Data[i] = IR_Temp[i];

  IR_ReceiveFlag = 1;
}
#endif

void IR_Remote_Test(void)
{
    int i,j;
    int IR_Time, IR_Data_H[48], IR_Data_L[48];

    i = j = 0;
    while(1)
    {
        TRISFbits.TRISF8 = 1;   // set RP15/CN74/RF8 pin as input
        _CNIE = 0;              //disable the Change Notification general interrupt

        T2CON = 0x00;           //Stops the Timer2 and reset control reg.
        T2CONbits.TCKPS = 0x3;  // 1:256 Prescare value, Fcy=8MHz/2=4MHz=0.25us, 1:256= 0.25*256 = 64us
        PR2 = 0xFFFF;           //Load the Period register with the value 0xFFFF

        //wait for Start Leading pulse
        TMR2 = 0x00;            // Clear contents of the timer register
        while (_RF8 == 1);      // wait for falling edge
        T2CONbits.TON = 1;      // Start Timer2
        while (_RF8 == 0);      // wait for rising edge
        //IR_Data_H[0] = TMR2;
        //TMR2 = 0x00;            // Clear contents of the timer register

        while (_RF8 == 1);      // wait for falling edge
        //IR_Data_L[0] = TMR2;
        TMR2 = 0x00;            // Clear contents of the timer register

        for (i=0; i< 4; i++)
            IR_Data_H[i] = 0;

        for (i=0; i< 32; i++)
        {
            j = i /8;
            while (_RF8 == 0);          // wait for rising edge
            //IR_Data_H[i] = TMR2;
            //TMR2 = 0x00;                // Clear contents of the timer register

            while (_RF8 == 1);          // wait for falling edge
            //IR_Data_L[i] = TMR2;
            IR_Time = TMR2;
            TMR2 = 0x00;                // Clear contents of the timer register
            
            IR_Data_H[j] >>= 1;
            
            if (IR_Time > 100)
                IR_Data_H[j] |= 0x80; 
            
        }

    }
}


#endif

#if 0
int IR_Remote_Init(void)
{
    if (!CN_Function_Enable(CN_FUNC_IR_REMOTE))
        return 0;
        
    T2CON = 0x00; //Stops the Timer2 and reset control reg.
    T2CONbits.TCKPS = 0x11 ; //1:256 prescale value = 4MHz/256 = 0.25us * 256 = 64us
    TMR2 = 0x00; //Clear contents of the timer register
    PR2 = 0xFFFF; //Load the Period register with the value 0xFFFF
    // IPC1bits.T2IP = 0x01; //Setup Timer2 interrupt for desired priority level
    // (this example assigns level 1 priority)
    //IFS0bits.T2IF = 0; //Clear the Timer2 interrupt status flag
    //IEC0bits.T2IE = 1; //Enable Timer2 interrupts
    //T2CONbits.TGATE = 1; //Set up Timer2 for operation in Gated
    //Time Accumulation mode
    T2CONbits.TON = 1; //Start Timer2


}

void vTask_IR_Remote(void *pvParameters )
{
    portTickType xLastWakeTime;
    unsigned char i, j;

    i = j = 0;

    IR_Remote_Init();
    xLastWakeTime = xTaskGetTickCount();
    for( ;; )
    {
        vTaskDelayUntil(&xLastWakeTime,500);

        T2CON = 0x00; //Stops the Timer2 and reset control reg.
        T2CONbits.TCKPS = 0x11 ; //1:256 prescale value = 4MHz/256 = 0.25us * 256 = 64us
        TMR2 = 0x00; //Clear contents of the timer register
        PR2 = 0xFFFF; //Load the Period register with the value 0xFFFF
        // IPC1bits.T2IP = 0x01; //Setup Timer2 interrupt for desired priority level
        // (this example assigns level 1 priority)
        //IFS0bits.T2IF = 0; //Clear the Timer2 interrupt status flag
        //IEC0bits.T2IE = 1; //Enable Timer2 interrupts
        //T2CONbits.TGATE = 1; //Set up Timer2 for operation in Gated
        //Time Accumulation mode
        T2CONbits.TON = 1; //Start Timer2
        vTaskDelay(100);       //spec: 60ms for each detection
    
        CN_Function_Release(CN_FUNC_IR_REMOTE);    
    }

}
#endif
