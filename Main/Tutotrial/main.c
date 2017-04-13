/*
 * File:   main.c - Interrupts Tutorial
 * Author: Modtronics Australia
 *         www.modtronicsaustralia.com
 *
 * Created on 16 October 2013, 3:55 PM
 */

#include "HardwareProfile.h"
#include <stdio.h>
#include <stdlib.h>

#include <xc.h>
#include <libpic30.h>
#include <PPS.h>
#include <ports.h>


// Microcontroller config words (fuses) - get these wrong and nothing will work correctly, if at all!!
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx1 & FWDTEN_OFF & WINDIS_OFF & FWPSA_PR32 & WDTPS_PS8192)
_CONFIG2(IESO_OFF & FNOSC_FRCPLL & OSCIOFNC_OFF & POSCMOD_NONE & PLL96MHZ_ON & PLLDIV_DIV2 & FCKSM_CSECME & IOL1WAY_OFF)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_FST & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_LPRC & DSBOREN_OFF & DSWDTEN_OFF)


/*
 *      Function Prototypes
 */
inline void clockSetup( void );

/*
 *  main function and program entry point
 */
int main(int argc, char** argv) {

    // Setup the microcontrollers clocking
    clockSetup( );

    // Turn off analogue functions on all pins
    AD1PCFG = 0xFFFF;

    // Enable the LED pin as an output
    TRISBbits.TRISB9 = 0;

    // Enable the switch as an input
    TRISBbits.TRISB13 = 1;

    // Turn the LED on
    LATBbits.LATB9 = 1;


    // Now it is time to configure the INT1 functionality
    CloseINT1();

    // Step 1 - configure the PPS function to assign INT1 to an IO pin

    PPSUnLock; // Unlock the PPS functionality
    // Configure INT1 to the correct IO pin
    iPPSInput( IN_FN_PPS_INT1, IN_PIN_PPS_RP13 );
    PPSLock; // Lock the PPS functionality

    /*  Step 2 - Setup the interrupt with the following attributes:
     *              - Interrupt enabled
     *              - Interrupt on a rising edge (low-to-high)
     *              - With a priority of 2 (this is arbitrary in this example)
     */
    ConfigINT1( INT_ENABLE & RISING_EDGE_INT & INT_PRI_2 );

    // Loop forever and toggle the LED everytime the button is pressed
    while(1)
    {
        // Do nothing as this program is entirely interrupt driven
    }


    return (EXIT_SUCCESS);
}

/*
 *  This function is responsible for setting up the microcontroller clocking
 *  and PLL
 */
inline void clockSetup( void )
{

    unsigned int pllCounter;
    OSCCONBITS OSCCONbitsCopy;

    // Copy the current Clock Setup
    OSCCONbitsCopy = OSCCONbits;
    // Slow output clock down to 4Mhz
    CLKDIVbits.CPDIV = 3;
    // Enable the PLL - Note: Fuse bits don't do this
    CLKDIVbits.PLLEN = 1;
    // Wait for the PLL to stabalise
    for (pllCounter = 0; pllCounter < 600; pllCounter++);

    // Check to see what clock setup is defined - either internal or external
    #ifdef USE_FRC_CLOCK
        // Setup the uC to use the internal FRCPLL mode
        OSCCONbitsCopy.NOSC = 1;
        OSCCONbitsCopy.OSWEN = 1;
    #else
        // Setup the uC to use the external crystal with the PLL
        OSCCONbitsCopy.NOSC = 3;
        OSCCONbitsCopy.OSWEN = 1;
    #endif

    // Switch over to the new clock setup
    __builtin_write_OSCCONH( BITS2BYTEH( OSCCONbitsCopy ) );
    __builtin_write_OSCCONL( BITS2BYTEL( OSCCONbitsCopy ) );
    // Wait for this transfer to take place
    while (OSCCONbits.COSC != OSCCONbits.NOSC);
    // Setup the DIV bits for the FRC, this values means the config word needs to be: PLLDIV_DIV2
    CLKDIVbits.RCDIV0 = 0;

    // Setup the PLL divider for the correct clock frequency
    if (CLOCK_FREQ == 32000000)
    {
        CLKDIVbits.CPDIV = 0;
    }
    else if (CLOCK_FREQ == 16000000)
    {
        CLKDIVbits.CPDIV = 1;
    }
    else if (CLOCK_FREQ == 8000000)
    {
        CLKDIVbits.CPDIV = 2;
    }
    else if (CLOCK_FREQ == 4000000)
    {
        CLKDIVbits.CPDIV = 3;
    }

    // Check that the PLL is enabled again and locked properly to the new setup
    CLKDIVbits.PLLEN = 1;
    // Note - don't want to do this check if we are running in the MPLAB X simulator as it won't work
    #ifndef __MPLAB_SIM
        while(_LOCK != 1);
    #endif

    // At this point the PIC24FJ64GB004 clock setup will be complete with the PLL
    // enabled and ready for operation with USB2.0
}



/*
 *  This function is the ISR (interrupt service routine) responsible for dealing with the
 *  external interrupt INT1
 */
void __attribute__((__interrupt__, auto_psv )) _ISR _INT1Interrupt (void)
{
    // Clear the interrupt status flag associated with this interrupt - very important
    Int1_Clear_Intr_Status_Bit;

    // Temporarily disable this interrupt - so it doesn't trigger multiple times due to button debounce
    DisableINT1;

    // Invert LED
    LATBbits.LATB9 = ~PORTBbits.RB9;
    // Delay to await for the button position to stabilised
    __delay_ms(200);

    // Clear the interrupt status flag associated with this interrupt - very important
    Int1_Clear_Intr_Status_Bit;

    // Re-enable the interrupt
    EnableINT1;
}

