

#ifndef _LCD_I2C_1602_
#define _LCD_I2C_1602_


#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

void Lcd_1602_Init(UINT8 lcd_Addr,UINT8 lcd_cols,UINT8 lcd_rows, UINT8 dotsize);
void Lcd_1602_write4bits(UINT8 value);
void Lcd_1602_expanderWrite(UINT8 _data);
void Lcd_1602_pulseEnable(UINT8 _data);
void Lcd_1602_DisplayOneChar(UINT8 X,UINT8 Y,UINT8 DData);
void Lcd_1602_DisplayListChar(UINT8 X,UINT8 Y,UINT8 *DData, UINT8 L);
void Lcd_1602_Display10BitData(int value,UINT8 x,UINT8 y);
void Lcd_1602_DisplayData(int value,UINT8 x,UINT8 y);
#endif

