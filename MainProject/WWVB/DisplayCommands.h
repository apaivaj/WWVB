/* 
 * File:   DisplayCommands.h
 * Author: HubCenter
 *
 * Created on October 10, 2017, 6:15 AM
 */

#ifndef DISPLAYCOMMANDS_H
#define	DISPLAYCOMMANDS_H


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include <libpic30.h>


void SendToDisplay(uint8_t*);
void SendErrorToDisplay(uint8_t*);
void SetTime(uint8_t*);
bool BackupData(uint8_t*);




#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* DISPLAYCOMMANDS_H */

