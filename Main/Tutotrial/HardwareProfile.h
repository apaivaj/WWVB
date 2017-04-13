/*
 * File:   hardwareprofile.h
 * Author: Modtronics Australia
 *         www.modtronicsaustralia.com
 *
 * Created on 16 October 2013, 3:55 PM
 */

#ifndef HARDWAREPROFILE_H
#define	HARDWAREPROFILE_H

#ifdef	__cplusplus
extern "C" {
#endif

/****************************************************************************/
/*               Useful Macros                                              */
#define BITS2WORD(sfrBitfield)   ( *((unsigned int*) &sfrBitfield) )
// Convert a bitfield to a word (unsigned int).
#define BITS2BYTEL(sfrBitfield)   ( ((unsigned char*) &sfrBitfield)[0] )
// Return the low byte (as a unsigned char) of a bitfield.
#define BITS2BYTEH(sfrBitfield)   ( ((unsigned char*) &sfrBitfield)[1] )
// Return the high byte (as a unsigned char) of a bitfield.


/****************************************************************************/
/*               User Configurable Definitions                              */
// If the following line is uncommented the uC will use the internal oscillator,
// comment the line out to use the external oscillator
#define USE_FRC_CLOCK
// Processor clock frequency selection
#define CLOCK_FREQ 32000000ULL		// Use 32MHz clock - default for demo board
//#define CLOCK_FREQ 16000000ULL	// Use 16MHz clock
//#define CLOCK_FREQ 8000000ULL		// Use 8MHz clock
//#define CLOCK_FREQ 4000000ULL		// Use 4MHz clock

#define FOSC   CLOCK_FREQ
#define FCY    (FOSC/2)


#ifdef	__cplusplus
}
#endif

#endif	/* HARDWAREPROFILE_H */

