// Fishfeeder (c) by eHaJo
//
// Fishfeeder is licensed under a
// Creative Commons Attribution-ShareAlike 4.0 International License.
// 
// You should have received a copy of the license along with this
// work. If not, see <http://creativecommons.org/licenses/by-sa/4.0/>.
// 
// With friendly support of ElektorLabs ( www.elektormagazine.de/labs )
//


#include <Stepper.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <RotaryEncoder.h>
#include <OneWire.h>
#include <Wire.h> // must be included here so that Arduino library object file references work
//#include <DallasTemperature.h>
#include <RtcDS3231.h>
#include <TimerOne.h>
#include <SPI.h>
#include <SD.h>

#define SYS_FONT u8x8_font_artossans8_r

/* Define for the Fishfeeder */
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, DISPLAY_RESET);

RtcDS3231<TwoWire> Rtc(Wire);

#define ehajo_bw_width 128
#define ehajo_bw_height 27


/*
#define DEBUGPRINTBEGIN { Serial.begin(115200);  while (!Serial){  } }
#define DEBUGPRINT( x ) { Serial.print( x ); }
#define DEBUGPRINTLN( x ) { Serial.println( x ); }
*/

#define DEBUGPRINTBEGIN {  }
#define DEBUGPRINT( x ) {  }
#define DEBUGPRINTLN( x ) {  }

#define u8g2_print_flash( x , y , F_STRING ) { u8g2.setCursor(x,y); u8g2.print(F_STRING);  }

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


volatile uint8_t SecondFlag=0;
volatile uint8_t MinuteChangedFlag=0;

static const char FILE_FEED_TXT[] = "FEED.TXT";

//OneWire oneWire(ONE_WIRE_BUS);
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
  
  u8g2.setFont(SYS_FONT);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(2);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0); 
  u8g2.setBitmapMode(true /* solid */);
  
  u8g2.firstPage();
  do {
    u8g2.drawXBMP(0,0 , ehajo_bw_width, ehajo_bw_height, ehajo_bw_bits);
    //u8g2.drawStr(10,40,"www.eHaJo.de");
    u8g2_print_flash(10,40,F("www.eHaJo.de"));
 } while ( u8g2.nextPage() );
  
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

  // see if the card is present and can be initialized:
  if (!SD.begin(SS)) {
    u8g2.firstPage();
    do {
   // u8g2.setFont(u8g2_font_ncenB10_tr);
      u8g2.drawStr(0,20,"Error");
      u8g2.drawStr(0,40,"No SD-Card");
    } while ( u8g2.nextPage() );
    while(1);
  } else {
    /* Card found */
  }

  File dataFile = SD.open(FILE_FEED_TXT, FILE_READ);
  // if the file is available, write to it:
  if (dataFile) {
    /* Call the validate function */
     
  } else {
    /* We create a sample file and throw an error */ 
    dataFile = SD.open(FILE_FEED_TXT, FILE_WRITE);
    dataFile.println(F("//eHaJo sample file for feeding"));
    dataFile.println(F("//To feed the fisch at 8pm with 16 1/8 turns and 4 1/8 truns back"));
    dataFile.println(F("//0800 1 16 4"));
    dataFile.close();
    while(1);    
  }

 
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(TimerISR); // do stuff every 200ms
  
  DEBUGPRINTLN(F("Starting...."));
  
  if(false ==  parse_check_sdcard_feeder() ){
    DEBUGPRINTLN(F("File bad!"));
    while(1);
  }
  
  delay(2000);

}

void loop() {

  if( 0 != SecondFlag){
    SecondFlag=0;
    /* This works and will be execute every 1 Second +/- */
    OneSecondTask();
  }
  
  if(0 != MinuteChangedFlag ){
    MinuteChangedFlag = 0;
    /* Read the SD-Card */
    /* Lets find oout what to do next */

    
  }


}



/*
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

  //Serial.print("Please enter Date/Time as YYMMDDhhmmssx\n");

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

  //Rtc.SetDateTime(RtcDateTime(Year,Month,Day,Hour,Minute,Second));

  return 1;
}
*/

void TimerISR(void){

 /* We need to toggle a pin */
 //digitalWrite(SCK, !digitalRead(SCK));
 /* we set a flag in the superloop that one second has passed */
  SecondFlag=0x01;
}

/* This will be called every second +/- */
void OneSecondTask(void)
{
  static uint8_t LastMinute = 0;
  RtcDateTime now = Rtc.GetDateTime();
  char datestring[15];
  char timestring[15];
  if(LastMinute != now.Minute()){
    LastMinute= now.Minute();
    MinuteChangedFlag=0x01;
  }
  

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
  //u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0,24,datestring);
    u8g2.drawStr(0,40,timestring);
  } while ( u8g2.nextPage() );

}

void write_parser_error(uint8_t line, uint8_t pos ){
  char buffer[64]={0,};
  DEBUGPRINTLN(F("Draw error to screen"));
  u8g2.firstPage();
  do {
      snprintf(buffer,sizeof(buffer),"Line %i",line);
      u8g2_print_flash(0,8,F("Error in FEED.TXT"));
      u8g2.drawStr(0,16,buffer);
      if(pos>=0){
        memset (buffer, 0, sizeof(buffer)); 
        snprintf(buffer,sizeof(buffer),"Line %i",line);
        u8g2.drawStr(0,24,buffer);
      }
    } while ( u8g2.nextPage() );
}

//Size of function is (28110 - 27036 ) 
bool parse_check_sdcard_feeder(){
// As specified by ehajo we read in the root
// the FEED.TXT
uint8_t Buffer[16];
uint8_t ParameterStartposition[3];
uint8_t idx=0;
uint16_t lineno=0;
bool fileokay=true;
File dataFile;
/* first we need to check if we have a sdcard */
  dataFile = SD.open("FEED.TXT", FILE_READ);
  if (dataFile){
      DEBUGPRINTLN("Read File");
      /* We need to parse the first line this can be at max 16 chars if it is a valid line */
      memset (Buffer, 0, sizeof(Buffer)); 
      /* we now read the first line */
      int16_t element = 0;
      element = dataFile.read();
      while(element >=0){
        DEBUGPRINT((char)element);
        /* next is to check if we are at the end of the line */
        if(element == '\n'){
          if(idx==0){
            /* found empty line and need to skip it */
            lineno++;  
          } else {
          /* We reached the end of the line */
          /* We need to get token from the line and check if we will get valid data from it */
          DEBUGPRINTLN(F("EoL"));
          uint8_t spacecount = 0;
          for(uint8_t i=0; i<idx;i++){
            /* check for valid elements in string */
            if( ( (Buffer[i]>='0') && ( Buffer[i]<='9') ) || (Buffer[i]==' ') ){
              /* valid element in string */
              if(Buffer[i]==' '){   
                if(spacecount<3){
                  ParameterStartposition[spacecount]=i;
                    if(spacecount>1){
                      if( (ParameterStartposition[spacecount-1]) == ParameterStartposition[spacecount] ){
                        write_parser_error(lineno,i);
                        return false;
                      }
                    }
                  }
                spacecount++;
              }
            } else {
              /* we have a fault at this position */
              write_parser_error(lineno,i);
              return false;
              //break;
            }
          }
          /* Next is to get the token from the file and the position of the elements */
          if( spacecount != 3)    {
            write_parser_error(lineno,-1);
            return false;
          } 
         
          if( ParameterStartposition[0] != 4 ){
             write_parser_error(lineno,-1);      
          } else {
            uint8_t hour = (Buffer[0]-'0')*10+(Buffer[1]-'0');
            uint8_t minute = (Buffer[2]-'0')*10+(Buffer[3]-'0');

            if( (hour>23) || ( minute>59) ){
              write_parser_error(lineno,-1);
              return false;
            }

            
          }
          /* We need to parse the next parameter and make sure it has only 3 chars at max */
          uint16_t parameter[3]={0,};
                 
            for(uint8_t p = ParameterStartposition[0]+1; p<ParameterStartposition[1];p++){
              parameter[0]=parameter[0]*10;
              parameter[0]=parameter[0]+(Buffer[p]-'0');
            }

            for(uint8_t p = ParameterStartposition[1]+1; p<ParameterStartposition[2];p++){
              parameter[1]=parameter[1]*10;
              parameter[1]=parameter[1]+(Buffer[p]-'0');
            }

            for(uint8_t p = ParameterStartposition[2]+1; p<idx;p++){
              parameter[2]=parameter[2]*10;
              parameter[2]=parameter[2]+(Buffer[p]-'0');
            }

            for(uint8_t z=0;z<3;z++){ 
              if(parameter[z]>999){
                write_parser_error(lineno,-1);
                return false;
              }
            }
         
            idx=0;
            lineno++;
          }
        } else {
          if(idx<15){
            if(element!='\r'){
              Buffer[idx]=element;
              idx++;
            }
          } else {
            /* error */
            write_parser_error(lineno,-1);
            return false;
            /* We try to read to the next \n in the file */
          }
        }
      
      element = dataFile.read();     
      } /* end while */
      
  } else {
    /* This is bad , but what to do ? */
    /* make some noise with the stepper?*/
    DEBUGPRINTLN(F("File not found"));
    return false;
  }

  return true;
}

void parse_sdcard_light(){
// As specified by ehajo we read in the root
// the LIGHT.TXT


}

