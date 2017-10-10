

#include "i2c.h"
#include "i2c_func.h"
#include "lcd_i2c_1602.h"
#define _Addr 0x27;
#define _cols 16;
#define _rows 2;
#define _backlightval 0x00;
#define _displaycontrol 0x04;
//UINT8 _Addr;
unsigned char _displayfunction;
//UINT8 _displaycontrol;
unsigned char _displaymode;
unsigned char _numlines;
//UINT8 _cols;
//UINT8 _rows;
//UINT8 _backlightval;

/*void Lcd_printf(UINT8 *s,int temp_data)
{
	if(temp_data<0)
	{
		temp_data=-temp_data;
		*s='-';
	}
	else *s=' ';
	*++s =temp_data/100+0x30;
	temp_data=temp_data%100;     //????
	*++s =temp_data/10+0x30;
	temp_data=temp_data%10;      //????
	*++s =temp_data+0x30;
}
*/
/************ low level data pushing commands **********/
unsigned char Lcd_i2c_write(unsigned char ControlByte, unsigned char data)
{
	unsigned char ErrorCode;

	IdleI2C();				//Ensure Module is Idle
	StartI2C();				//Generate Start COndition
	WriteI2C(ControlByte);			//Write Control byte
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status

	WriteI2C(data);				//Write Data
	IdleI2C();
	StopI2C();				//Initiate Stop Condition
	// Regis EEAckPolling(ControlByte);		//Perform ACK polling
	return(ErrorCode);
}
void Lcd_1602_expanderWrite(unsigned char _data){
    Lcd_i2c_write(0x4E,  _data | 0x00);
}

void Lcd_1602_pulseEnable(unsigned char _data){
	Lcd_1602_expanderWrite(_data | En);	// En high
//	delay_ms(5);		// enable pulse must be >450ns

	Lcd_1602_expanderWrite(_data & ~En);	// En low
//	delay_ms(50);		// commands need > 37us to settle
}

void Lcd_1602_write4bits (unsigned char value) {
	Lcd_1602_expanderWrite(value);
	Lcd_1602_pulseEnable(value);
}
void Lcd_1602_send (unsigned char value, unsigned char mode) {
	unsigned char highnib = value&0xf0;
	unsigned char lownib = (value<<4)&0xf0;
    Lcd_1602_write4bits((highnib)| mode);
	Lcd_1602_write4bits((lownib)| mode);
}


void Lcd_1602_command (unsigned char value) {
	Lcd_1602_send(value, 0);
}

void Lcd_1602_display() {
	Lcd_1602_command (LCD_DISPLAYCONTROL);
}

void Lcd_1602_clear(){
	Lcd_1602_command(0x01);// clear display, set cursor position to zero
	//delay_ms (2);  // this command takes a long time!
}

void Lcd_1602_home(){
	Lcd_1602_command(LCD_RETURNHOME);  // set cursor position to zero
	//delay_ms(2);  // this command takes a long time!
}

void Lcd_1602_DisplayOneChar(unsigned char X,unsigned char Y,unsigned char DData)
{
	Y &= 1;
	X &= 15;
	if (Y) X |= 0x40;
	X |= 0x80;
	Lcd_1602_command(X);
    Lcd_1602_send(DData, Rs);
}

void Lcd_1602_Init(unsigned char lcd_Addr,unsigned char lcd_cols,unsigned char lcd_rows, unsigned char dotsize)
{
    
 /* _Addr = lcd_Addr;
  _cols = lcd_cols;
  _rows = lcd_rows;*/
//  _backlightval == 0x00;

  _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;


    if (lcd_rows > 1) {
		_displayfunction |= LCD_2LINE;
	}
	_numlines = lcd_rows;

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (lcd_rows == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
//	delay_ms(50);

	// Now we pull both RS and R/W low to begin commands
	// expanderWrite(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	// delay(1000);
     
        //Lcd_i2c_write(0x4E, _backlightval);
       // delay_ms(100);

        //put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

  // we start in 8bit mode, try to set 4 bit mode
  // Lcd_1602_write4bits(0x03 << 4);
   //delay_ms(5); // wait min 4.1ms

   // second try
   //Lcd_1602_write4bits(0x03 << 4);
   //delay_ms(5); // wait min 4.1ms

   // third go!
   //Lcd_1602_write4bits(0x03 << 4);
  // delay_ms(1);

   // finally, set to 4-bit interface
  // Lcd_1602_write4bits(0x02 << 4);


	// set # lines, font size, etc.
	//Lcd_1602_command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	//_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    //_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	//Lcd_1602_display();

	// clear it off
	//Lcd_1602_clear();

	// Initialize to default text direction (for roman languages)
	//_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	//Lcd_1602_command(LCD_ENTRYMODESET | _displaymode);

	//Lcd_1602_home();
   // Lcd_1602_DisplayOneChar(0,0,'A');
    //Lcd_1602_DisplayOneChar(0,1,'G');

}


