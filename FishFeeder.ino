// Fishfeeder (c) by eHaJo
//
// Fishfeeder is licensed under a
// Creative Commons Attribution-ShareAlike 4.0 International License.
// 
// You should have received a copy of the license along with this
// work. If not, see <http://creativecommons.org/licenses/by-sa/4.0/>.

#include <Stepper.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <RotaryEncoder.h>
#include <OneWire.h>
#include <Wire.h> // must be included here so that Arduino library object file references work
//#include <DallasTemperature.h>
#include <RtcDS3231.h>

/* Define for the Fishfeeder */
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, DISPLAY_RESET);

RtcDS3231<TwoWire> Rtc(Wire);

#define ehajo_bw_width 128
#define ehajo_bw_height 27

static const unsigned char ehajo_bw_bits[] U8X8_PROGMEM= {
   0x00, 0x00, 0x00, 0xe0, 0x3f, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xfc,
   0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x7f, 0xfc, 0x1f, 0x00,
   0x00, 0x00, 0x00, 0xfe, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0,
   0x7f, 0xfc, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xf0, 0x7f, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xfe,
   0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0xd0, 0x07, 0x00,
   0x00, 0x00, 0x00, 0xd0, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x28, 0x00, 0x80,
   0x0f, 0xc0, 0x03, 0x40, 0x3f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x7a, 0x00,
   0x80, 0xff, 0x03, 0x80, 0x0f, 0xc0, 0x03, 0xf0, 0xff, 0x00, 0x00, 0x80,
   0x0f, 0xc0, 0xff, 0x03, 0xe0, 0xff, 0x07, 0x80, 0x0f, 0xc0, 0x03, 0xfc,
   0xff, 0x01, 0x00, 0x00, 0x0f, 0xf0, 0xff, 0x0f, 0xf0, 0xff, 0x1f, 0x80,
   0x07, 0xc0, 0x07, 0xfc, 0xff, 0x03, 0x00, 0x80, 0x0f, 0xf0, 0xff, 0x1f,
   0xf8, 0x83, 0x1f, 0x80, 0x0f, 0xc0, 0x03, 0xfc, 0xf2, 0x07, 0x00, 0x00,
   0x0f, 0xf8, 0xcb, 0x3f, 0xf8, 0x00, 0x3e, 0x80, 0x07, 0xc0, 0x07, 0x38,
   0xc0, 0x07, 0x00, 0x80, 0x0f, 0xfc, 0x00, 0x3f, 0x7c, 0x00, 0x3c, 0x80,
   0x0f, 0xc0, 0x03, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x0f, 0x7e, 0x00, 0x7e,
   0x3c, 0x00, 0x7c, 0x80, 0xbf, 0xf7, 0x07, 0x80, 0xd6, 0x07, 0x00, 0x80,
   0x0f, 0x3e, 0x00, 0xfc, 0x7e, 0x00, 0x7c, 0x80, 0xff, 0xff, 0x03, 0xf0,
   0xff, 0x07, 0x00, 0x00, 0x0f, 0x3e, 0x00, 0xf8, 0xe3, 0xff, 0x7f, 0x80,
   0xff, 0xff, 0x07, 0xfc, 0xff, 0x07, 0x08, 0x80, 0x0f, 0x3f, 0x00, 0xf8,
   0xe3, 0xff, 0x7f, 0x80, 0xff, 0xff, 0x03, 0xfe, 0xff, 0x07, 0x1c, 0x00,
   0x0f, 0x1e, 0x00, 0xf8, 0xe3, 0xff, 0x7f, 0x80, 0xaf, 0xea, 0x07, 0xff,
   0xf2, 0x07, 0x3c, 0x80, 0x0f, 0x3f, 0x00, 0xf8, 0x76, 0xaa, 0x0a, 0x80,
   0x0f, 0xc0, 0x03, 0x3f, 0x80, 0x07, 0x3c, 0x00, 0x0f, 0x3e, 0x00, 0xf8,
   0x3e, 0x00, 0x00, 0x80, 0x07, 0xc0, 0x07, 0x1f, 0x80, 0x07, 0x3e, 0x80,
   0x0f, 0x3e, 0x00, 0xf8, 0x3c, 0x00, 0x78, 0x80, 0x0f, 0xc0, 0x03, 0x0f,
   0xc0, 0x07, 0x3c, 0x80, 0x0f, 0x3e, 0x00, 0x7c, 0x7c, 0x00, 0x7c, 0x80,
   0x07, 0xc0, 0x07, 0x1f, 0xe0, 0x07, 0x3e, 0x80, 0x0f, 0x7e, 0x00, 0x7e,
   0xfc, 0x00, 0xc6, 0x80, 0x0f, 0xc0, 0x03, 0x1f, 0xf0, 0x07, 0x7c, 0xc0,
   0x0f, 0xfc, 0x00, 0x7f, 0xf8, 0x41, 0x87, 0xa0, 0x2f, 0xf0, 0x0f, 0xff,
   0xfe, 0x3f, 0xfe, 0xe0, 0x07, 0xf8, 0xd7, 0x3f, 0xf0, 0xff, 0xc7, 0xf0,
   0x7f, 0xfc, 0x1f, 0xfe, 0xff, 0x7f, 0xfc, 0xff, 0x07, 0xf0, 0xff, 0x1f,
   0xe0, 0xff, 0x7f, 0xf0, 0x7f, 0xfc, 0x3f, 0xfe, 0xff, 0x7f, 0xfc, 0xff,
   0x03, 0xf0, 0xff, 0x0f, 0xc0, 0xff, 0x3b, 0xf0, 0x7f, 0xfc, 0x1f, 0xf8,
   0x9f, 0x3f, 0xf0, 0xff, 0x01, 0xc0, 0xff, 0x03, 0x00, 0x54, 0x00, 0x40,
   0x15, 0x50, 0x05, 0xe0, 0x01, 0x00, 0x80, 0x3f, 0x00, 0x00, 0xbe, 0x00 };  

void u8g2_bitmap_overlay(uint8_t a);
void u8g2_ascii_1( void );
int SetDateStuff();

OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);

const int stepsPerRevolution = 4096;
const int quarterRevolution = stepsPerRevolution / 4;

RotaryEncoder encoder(ENC_A, ENC_B);
Stepper FishStepperB(stepsPerRevolution, MOTOR1_1, MOTOR1_3, MOTOR1_2, MOTOR1_4);
Stepper FishStepperA(stepsPerRevolution, MOTOR2_1, MOTOR2_3, MOTOR2_2, MOTOR2_4);

void setup() {
  
  pinMode(ENC_BUTTON, INPUT);
  Rtc.Begin();
  u8g2.begin(); 
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(2);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0); 
  u8g2.firstPage();  
  do {
     u8g2_bitmap_overlay(0);
     u8g2.setFont(u8g2_font_ncenB10_tr);
     u8g2.drawStr(10,40,"www.eHaJo.de");
  } while( u8g2.nextPage() );
  pinMode(ENC_A, INPUT_PULLUP); 
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(ENC_BUTTON, INPUT_PULLUP);
  //sensors.begin();

  FishStepperA.setSpeed(6);
  FishStepperB.setSpeed(6);
  pinMode(LED_SKIP, OUTPUT);
  pinMode(LED_PWM, OUTPUT);
  digitalWrite(LED_SKIP, 0);
  digitalWrite(LED_PWM, 1);
  delay(1000);
  /*if(SetDateStuff()==1)
    Serial.print("Date and time set.\n");
  else
    Serial.print("Error setting date and time.\n");
  */
  
  FishStepperA.step(stepsPerRevolution);
}

void loop() {

  RtcDateTime now = Rtc.GetDateTime();
  char datestring[15];
  char timestring[15];

  snprintf_P(datestring, 
          15,
          PSTR("%02u.%02u.%04u"),
          now.Day(),
          now.Month(),
          now.Year() );
  snprintf_P(timestring, 
          15,
          PSTR("%02u:%02u:%02u"),
          now.Hour(),
          now.Minute(),
          now.Second() );          
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0,24,datestring);
    u8g2.drawStr(0,40,timestring);
  } while ( u8g2.nextPage() );
delay(100);
}


void u8g2_ascii_1() {
  char s[2] = " ";
  uint8_t x, y;
  u8g2.drawStr( 0, 0, "ASCII page 1");
  for( y = 0; y < 6; y++ ) {
    for( x = 0; x < 16; x++ ) {
      s[0] = y*16 + x + 32;
      u8g2.drawStr(x*7, y*10+10, s);
    }
  }
}

void u8g2_bitmap_overlay(uint8_t a) {
  
  u8g2.setBitmapMode(true /* solid */);
  u8g2.drawXBMP(a,0 , ehajo_bw_width, ehajo_bw_height, ehajo_bw_bits);
 
}

int SetDateStuff() 
{

  byte Year;
  byte Month;
  byte Day;
  byte Hour;
  byte Minute;
  byte Second;

  boolean GotString = false;
  char InChar;
  byte Temp1, Temp2;
  char InString[15];

  Serial.print("Please enter Date/Time as YYMMDDhhmmssx\n");

  byte j=0;
  while (!GotString) {
    if (Serial.available()) {
      InChar = Serial.read();
      InString[j] = InChar;
      j += 1;
      if (InChar == 'x') {
        GotString = true;
      }
      if(j>14)
        return -1;
    }
  }
  Serial.println(InString);
  // Read Year first
  Temp1 = (byte)InString[0] -48;
  Temp2 = (byte)InString[1] -48;
  Year = Temp1*10 + Temp2;
  // now month
  Temp1 = (byte)InString[2] -48;
  Temp2 = (byte)InString[3] -48;
  Month = Temp1*10 + Temp2;
  // now date
  Temp1 = (byte)InString[4] -48;
  Temp2 = (byte)InString[5] -48;
  Day = Temp1*10 + Temp2;
  // now Hour
  Temp1 = (byte)InString[6] -48;
  Temp2 = (byte)InString[7] -48;
  Hour = Temp1*10 + Temp2;
  // now Minute
  Temp1 = (byte)InString[8] -48;
  Temp2 = (byte)InString[9] -48;
  Minute = Temp1*10 + Temp2;
  // now Second
  Temp1 = (byte)InString[10] -48;
  Temp2 = (byte)InString[11] -48;
  Second = Temp1*10 + Temp2;

  Rtc.SetDateTime(RtcDateTime(Year,Month,Day,Hour,Minute,Second));

  return 1;
}



