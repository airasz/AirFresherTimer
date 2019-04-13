#include <Wire.h>
#include <Time.h>
#include <TimeAlarms.h>
//#include "MsTimer2.h"
#include <DS3232RTC.h>

long previousMillis=0;
bool dutySpray=0;
int jam_on, menit_on, detik_on, jam_off, menit_off, detik_off;
// set light task 2
int jam2_on, menit2_on, detik2_on, jam2_off, menit2_off, detik2_off;

#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
return( (val/16*10) + (val%16) );
}
void setup(){
    Serial.begin(115200);
    Serial.println("starting timer toilette");
    setSyncProvider(RTC.get);   // the function to get the time from the RTC

//  setTime(0,14,1,6,5,17); h,m,s ,d,m,y

  //DS3231 seconds, minutes, hours, day, date, month, year 
  //setDS3231time(1,23,22,6,26,1,18);
  jam_on=8;
  menit2_on=50;
  jam2_on=16;
  menit2_on=1;
  jam2_off=20;
  menit2_off=30;
  // setDS3231time(0, 40, 19, 7, 13, 4, 2019);
}
void loop()
{
    if ((previousMillis+1000)==millis()) {
      taskCheck();
      spraying();
      previousMillis=millis();
    }
    
}
void taskCheck(){
    

    
  //   if(((hour() == jam_on && minute()> menit_on )  || (hour() > jam_on   ) ) && (actled==1||actled==3)){ //jika waktu sekarang masuk jam dimana led
  //     if(hour() == jam_off && minute()< menit_off){                                                        //haurs menyala, maka akan
  //       dutySpray=true;}
  //       else if (hour() < jam_off ){
  //         dutySpray=true;
  //       }
  //   }

  // if(((hour() == jam2_on && minute()> menit2_on )  || (hour() > jam2_on ))  && (actled==2||actled==3)){ //jika waktu sekarang masuk jam dimana led
  //   if(hour() == jam2_off && minute()< menit2_off){                                                        //haurs menyala, maka akan
  //     rec_light_CO2_ON();}
  //   else if (hour() < jam2_off ){
  //     rec_light_CO2_ON();
  //   }
  // }
    if(hour()==jam_on&&minute()>menit_on){
      dutySpray=1;  
    }
    else if (hour()>jam_on&&hour()<jam_off){
      dutySpray=1;  
    }
    else if(hour()==jam2_on&&minute()>menit2_on){
      dutySpray=1;  
    }
    else if (hour()>jam2_on&&hour()<jam2_off){
      dutySpray=1;  
      // Serial.println("I am here");
    } else
    {
      dutySpray=false;
    }
    

        // if (hour()==jam_on&&minute()==menit_on){
        //   if (second()<1){
        //     light_CO2_ON();
        //   }
        // }
        // if (hour()==jam_off&&minute()==menit_off){
        //     if (second()<1){
        //       light_CO2_OFF();
        //     }
        //   }
        
        //  if (hour()==jam2_on&&minute()==menit2_on){
        //    if (second()<1){
        //      light_CO2_ON();
        //    }
        //  }

        //  if (hour()==jam2_off&&minute()==menit2_off){
        //    if (second()<1){
        //      light_CO2_OFF();
        //    }
        //  }


 }
 int minuteReach= 20*1000;
 int secondReach=0;
 void spraying(){
   if(dutySpray){
     secondReach++;
     if (secondReach<2){
       //digitalWrite(pin, HIGH);
        Serial.println("Spraying");
        
     }else if(secondReach>60){
       secondReach=0;
     }

   }
 }
 
void readDS3231time(
byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
    Serial.println("time has been saved");
}
