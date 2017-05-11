
// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

#include "Compiler.h"
#include "HardwareProfile.h"

#include "i2c.h"
#include "i2c_func.h"
#include "lcd_i2c_1602.h"

UINT8 _Addr;
UINT8 _displayfunction;
UINT8 _displaycontrol;
UINT8 _displaymode;
UINT8 _numlines;
UINT8 _cols;
UINT8 _rows;
UINT8 _backlightval;

/*
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
void setup()
{
  lcd.init();                      // initialize the lcd

  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello, world!");
}
*/

//****************************************
//整數轉字符串
//****************************************
void Lcd_printf(UINT8 *s,int temp_data)
{
	if(temp_data<0)
	{
		temp_data=-temp_data;
		*s='-';
	}
	else *s=' ';
	*++s =temp_data/100+0x30;
	temp_data=temp_data%100;     //取余運算
	*++s =temp_data/10+0x30;
	temp_data=temp_data%10;      //取余運算
	*++s =temp_data+0x30;
}

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

// write either command or data
void Lcd_1602_send(UINT8 value, UINT8 mode) {
	UINT8 highnib = value&0xf0;
	UINT8 lownib = (value<<4)&0xf0;
    Lcd_1602_write4bits((highnib)|mode);
	Lcd_1602_write4bits((lownib)|mode);
}

void Lcd_1602_write4bits(UINT8 value) {
	Lcd_1602_expanderWrite(value);
	Lcd_1602_pulseEnable(value);
}

void Lcd_1602_expanderWrite(UINT8 _data){
    Lcd_i2c_write(0x4E,  _data | _backlightval);
}

void Lcd_1602_pulseEnable(UINT8 _data){
	Lcd_1602_expanderWrite(_data | En);	// En high
	delay_ms(1);		// enable pulse must be >450ns

	Lcd_1602_expanderWrite(_data & ~En);	// En low
	delay_ms(50);		// commands need > 37us to settle
}

void Lcd_1602_command(UINT8 value) {
	Lcd_1602_send(value, 0);
}

void Lcd_1602_write(UINT8 value) {
	Lcd_1602_send(value, Rs);
}

/********** high level commands, for the user! */
void Lcd_1602_clear(){
	Lcd_1602_command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	delay_ms(2);  // this command takes a long time!
}

void Lcd_1602_home(){
	Lcd_1602_command(LCD_RETURNHOME);  // set cursor position to zero
	delay_ms(2);  // this command takes a long time!
}

void Lcd_1602_setCursor(UINT8 col, UINT8 row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > _numlines ) {
		row = _numlines-1;    // we count rows starting w/0
	}
	Lcd_1602_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void Lcd_1602_noDisplay() {
	_displaycontrol &= ~LCD_DISPLAYON;
	Lcd_1602_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Lcd_1602_display() {
	_displaycontrol |= LCD_DISPLAYON;
	Lcd_1602_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void Lcd_1602_noCursor() {
	_displaycontrol &= ~LCD_CURSORON;
	Lcd_1602_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Lcd_1602_cursor() {
	_displaycontrol |= LCD_CURSORON;
	Lcd_1602_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void Lcd_1602_noBlink() {
	_displaycontrol &= ~LCD_BLINKON;
	Lcd_1602_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Lcd_1602_blink() {
	_displaycontrol |= LCD_BLINKON;
	Lcd_1602_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void Lcd_1602_scrollDisplayLeft(void) {
	Lcd_1602_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void Lcd_1602_scrollDisplayRight(void) {
	Lcd_1602_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void Lcd_1602_leftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	Lcd_1602_command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void Lcd_1602_rightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	Lcd_1602_command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void Lcd_1602_autoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	Lcd_1602_command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void Lcd_1602_noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	Lcd_1602_command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void Lcd_1602_createChar(UINT8 location, UINT8 charmap[]) {
    UINT8 i;
	location &= 0x7; // we only have 8 locations 0-7
	Lcd_1602_command(LCD_SETCGRAMADDR | (location << 3));
	for (i=0; i<8; i++) {
		Lcd_1602_write(charmap[i]);
	}
}

// Turn the (optional) backlight off/on
void Lcd_1602_noBacklight(void) {
	_backlightval=LCD_NOBACKLIGHT;
	Lcd_1602_expanderWrite(0);
}

void Lcd_1602_backlight(void) {
	_backlightval=LCD_BACKLIGHT;
	Lcd_1602_expanderWrite(0);
}

//////////////////////////////
//****************************************
//LCD1602寫入一個字符
//****************************************
void Lcd_1602_DisplayOneChar(UINT8 X,UINT8 Y,UINT8 DData)
{
	Y &= 1;
	X &= 15;
	if (Y) X |= 0x40;
	X |= 0x80;
	Lcd_1602_command(X);
	Lcd_1602_write(DData);
}

//****************************************
//LCD1602顯示字符串
//****************************************
void Lcd_1602_DisplayListChar(UINT8 X,UINT8 Y,UINT8 *DData, UINT8 L)
{
	UINT8 ListLength=0;

        Y &= 0x1;
	X &= 0xF;

        while(L--)
	{
		Lcd_1602_DisplayOneChar(X,Y,DData[ListLength]);
		ListLength++;
		X++;
	}
}

//**************************************
//在1602上顯示10位數據
//**************************************
void Lcd_1602_Display10BitData(int value,UINT8 x,UINT8 y)
{
    UINT8   dis[4];		//顯示數字(-511至512)的字符數組
	value /= 64;			//轉換為10位數據
	Lcd_printf(dis, value);         //轉換數據顯示
	Lcd_1602_DisplayListChar(x,y,dis,4);	//啟始列，行，顯示數組，顯示長度
}

void Lcd_1602_DisplayData(int value,UINT8 x,UINT8 y)
{
    UINT8   dis[4];		//顯示數字(-511至512)的字符數組

    Lcd_printf(dis, value);         //轉換數據顯示
    Lcd_1602_DisplayListChar(x,y,dis,4);	//啟始列，行，顯示數組，顯示長度
}




/// High level functions

void Lcd_1602_Init(UINT8 lcd_Addr,UINT8 lcd_cols,UINT8 lcd_rows, UINT8 dotsize)
{
  _Addr = lcd_Addr;
  _cols = lcd_cols;
  _rows = lcd_rows;
  _backlightval = LCD_NOBACKLIGHT;

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
	delay_ms(50);

	// Now we pull both RS and R/W low to begin commands
	// expanderWrite(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	// delay(1000);

        Lcd_1602_expanderWrite(_backlightval);
        delay_ms(100);

        //put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

  // we start in 8bit mode, try to set 4 bit mode
   Lcd_1602_write4bits(0x03 << 4);
   delay_ms(5); // wait min 4.1ms

   // second try
   Lcd_1602_write4bits(0x03 << 4);
   delay_ms(5); // wait min 4.1ms

   // third go!
   Lcd_1602_write4bits(0x03 << 4);
   delay_ms(1);

   // finally, set to 4-bit interface
   Lcd_1602_write4bits(0x02 << 4);


	// set # lines, font size, etc.
	Lcd_1602_command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	//_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
        _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	Lcd_1602_display();

	// clear it off
	Lcd_1602_clear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	Lcd_1602_command(LCD_ENTRYMODESET | _displaymode);

	Lcd_1602_home();

        Lcd_1602_DisplayOneChar(0,0,'A');
        Lcd_1602_DisplayOneChar(0,1,'G');

}
