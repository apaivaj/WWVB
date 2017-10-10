
#ifndef _I2C_FUNC_
#define _I2C_FUNC_

//This file contains the function prototypes for the i2c function
#define PAGESIZE	32

//Low Level Functions
unsigned char IdleI2C(void);
unsigned char StartI2C(void);
unsigned char WriteI2C(unsigned char);
unsigned char StopI2C(void);
unsigned char RestartI2C(void);
unsigned char getsI2C(unsigned char*, unsigned char);
unsigned char NotAckI2C(void);
unsigned char InitI2C(void);
unsigned char ACKStatus(void);
unsigned char getI2C(void);
unsigned char AckI2C(void);
unsigned char EEAckPolling(unsigned char);
unsigned char putstringI2C(unsigned char*);

//High Level Functions for Low Density Devices
unsigned char LDByteReadI2C(unsigned char, unsigned char, unsigned char*, unsigned char);
unsigned char LDByteWriteI2C(unsigned char, unsigned char, unsigned char);
unsigned char LDPageWriteI2C(unsigned char, unsigned char, unsigned char*);
unsigned char LDSequentialReadI2C(unsigned char, unsigned char, unsigned char*, unsigned char);

//High Level Functions for High Density Devices
unsigned char HDByteReadI2C(unsigned char, unsigned char, unsigned char, unsigned char*, unsigned char);
unsigned char HDByteWriteI2C(unsigned char, unsigned char, unsigned char, unsigned char);
unsigned char HDPageWriteI2C(unsigned char, unsigned char, unsigned char, unsigned char*);
unsigned char HDSequentialReadI2C(unsigned char, unsigned char, unsigned char, unsigned char*, unsigned char);

#endif

