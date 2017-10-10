#include <Wire.h>
#include <ds3231.h>
#include <LiquidCrystal_I2C.h>
#define BUFF_MAX 128

//****************************************Define I2C LCD Display *********************************
#define I2C_ADDR    0x27  // Define I2C Address for the PCF8574T 
#define Rs_pin  0
#define Rw_pin  1
#define En_pin  2
#define BACKLIGHT_PIN  3
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

#define  LED_OFF  0
#define  LED_ON  1

/*-----( Declare objects )-----*/  
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
//************************************ END LCD DISPLAY *******************************************

uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;
unsigned long prev, interval = 1000;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    DS3231_init(DS3231_INTCN);
    memset(recv, 0, BUFF_MAX);
    Serial.println("GET time");

  //**************************LCD Setup********************************
   lcd.begin (16,2);  // initialize the lcd 
   // Switch on the backlight
   lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
   lcd.setBacklight(LED_ON);
  //***************************END LCD Setup********************************
    Serial.println("Setting time");
    setTheTime("300102220042017");     // ssmmhhWDDMMYYYY set time once in the given format
    lcd.clear();
}

void loop()
{
    char tempF[6]; 
    float temperature;
    char buff[BUFF_MAX];
    unsigned long now = millis();
    struct ts t;
    // show time once in a while
    if (now - prev > interval){
        DS3231_get(&t); //Get time
//        temperature = DS3231_get_treg(); //Get temperature
//        dtostrf(temperature, 5, 1, tempF);

//        lcd.clear();
        lcd.setCursor(0,0);
        
        if(t.hour<10)
        {
          lcd.print("0");
        }
        lcd.print(t.hour);
        lcd.print(":");
        if(t.min<10)
        {
          lcd.print("0");
        }
        lcd.print(t.min);
        lcd.print(":");
        if(t.sec<10)
        {
          lcd.print("0");
        }
        lcd.print(t.sec);
        
        lcd.print(' ');
        
        lcd.setCursor(0,1); //Go to second line of the LCD Screen
        printMonth(t.mon);
        lcd.print(t.mday);
        lcd.print(' ');
        lcd.print(t.year);
//
//        lcd.print(tempF);
//        lcd.print((char)223);
//        lcd.print("C ");
        prev = now;
    }
}

void setTheTime(char *cmd)
{
    struct ts t;
    // ssmmhhWDDMMYYYY  set time
      t.sec = inp2toi(cmd, 0);
      t.min = inp2toi(cmd, 2);
      t.hour = inp2toi(cmd, 4);
      t.wday = inp2toi(cmd, 6);
      t.mday = inp2toi(cmd, 7);
      t.mon = inp2toi(cmd, 9);
      t.year = inp2toi(cmd, 11) * 100 + inp2toi(cmd, 13);
      DS3231_set(t);
      Serial.println("OK");
}

void printMonth(int month)
{
  switch(month)
  {
    case 1: lcd.print("January ");break;
    case 2: lcd.print("February ");break;
    case 3: lcd.print("March ");break;
    case 4: lcd.print("April ");break;
    case 5: lcd.print("May ");break;
    case 6: lcd.print("June ");break;
    case 7: lcd.print("July ");break;
    case 8: lcd.print("August ");break;
    case 9: lcd.print("September ");break;
    case 10: lcd.print("October ");break;
    case 11: lcd.print("November ");break;
    case 12: lcd.print("December ");break;
    default: lcd.print("Error ");break;
  } 
}
