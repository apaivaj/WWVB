#include "DisplayCommands.h"
void SendToDisplay(uint8_t*);
void SendErrorToDisplay(uint8_t*);
void SetTime(uint8_t*);
bool BackupData(uint8_t*);

const uint8_t DispAddress = 8;      //Currently using Arduino Uno Address

//Sends Time to Display
void SendToDisplay(uint8_t* time)
{
    static I2C1_MESSAGE_STATUS status;
    I2C1_MasterWrite(
                                time,
                                3,
                                DispAddress,
                                &status);
    
}
//Sends Error Message to Display that memory was unable to be written to
void SendErrorToDisplay(uint8_t* error_in_mem)
{
    static I2C1_MESSAGE_STATUS status;
    I2C1_MasterWrite(
                                error_in_mem,
                                13,
                                DispAddress,
                                &status);
}

//Sets the time based off of what will be decoded from the signal
//at the moment just sets time to 12:00
void SetTime(uint8_t* time)
{
    time[0] = 12;
    time[1] = 0;
    time[2] = 0;
}

//Backs up the value needed for DAC on the Oscillator Board
//Sends back 1 if successful and 0 if it fails
bool BackupData(uint8_t* dac_value)
{
    static I2C1_MESSAGE_STATUS status;
    I2C1_MasterWrite(
                                dac_value,
                                16,
                                10,
                                &status);
    if(status == I2C1_MESSAGE_FAIL)
    {
        return 0;
    }
    return 1;
}