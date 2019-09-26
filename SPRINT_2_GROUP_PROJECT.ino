/*  */

//Libraries
#include <DHT.h>
#include "SD.h"
#include <Wire.h>
#include "RTClib.h"

#define LOG_INTERVAL  2000
#define SYNC_INTERVAL 2000
uint32_t syncTime = 0;


//Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino

//Variables
float temp; //Stores temperature value
bool motion;

RTC_DS1307 RTC;
const int chipSelect = 10;
File logfile;

void setup()
{
    Serial.begin(9600);

    initSDcard();
    createFile();
    initRTC();
    logfile.println("millis,stamp,datetime,motion,temp");
    
    dht.begin();
    
    pinMode(6, INPUT);
    pinMode(4, OUTPUT);  //1,2,3,4 layers
    pinMode(5, OUTPUT);  //2,3,4 layers
    pinMode(8, OUTPUT); //3,4 layers
    pinMode(9, OUTPUT); //4 layers
    
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
}

void loop()
{
  DateTime now;
    uint32_t m = millis();
  logfile.print(m);           
  logfile.print(", ");
  now = RTC.now();
  logfile.print(now.unixtime()); // seconds since 2000
  logfile.print(", ");
  logfile.print(now.year(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.day(), DEC);
  logfile.print(" ");
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);
  
      if (digitalRead(6) == HIGH) {
        motion = 1;
      }
      else {
        motion = 0;
      }
    
    //Read data and store it to variables temp

    temp = dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" Celsius,");
    Serial.print(" Motion: ");
    Serial.println(motion);

      logfile.print(", ");
      logfile.print(motion);
      logfile.print(", ");
      logfile.println(temp);

    
    if (temp >= 25 && motion == 1)
      { 
         digitalWrite(4, HIGH);
         digitalWrite(5, LOW);
         digitalWrite(8, LOW);
         digitalWrite(9, LOW);
      }
    else if (temp < 25 && temp >= 15 && motion == 1)
     {
         digitalWrite(4, HIGH);
         digitalWrite(5, HIGH);
         digitalWrite(8, LOW);
         digitalWrite(9, LOW);
    }
    else if (temp < 15 && temp >= 0 && motion == 1)
    {
         digitalWrite(4, HIGH);
         digitalWrite(5, HIGH);
         digitalWrite(8, HIGH);
         digitalWrite(9, LOW);
    }
    else if (temp < 0 && motion == 1)
    {
         digitalWrite(4, HIGH);
         digitalWrite(5, HIGH);
         digitalWrite(8, HIGH);
         digitalWrite(9, HIGH);
    }
    
    else
    {
         digitalWrite(4, LOW);
         digitalWrite(5, LOW);
         digitalWrite(8, LOW);
         digitalWrite(9, LOW);    
    }
    
    delay(2000); //Delay 2 sec.
    
    if ((millis() - syncTime) < SYNC_INTERVAL) return;
    syncTime = millis();

    logfile.flush();
}

void error(char const *str)
{
  Serial.print("error: ");
  Serial.println(str);

  while (1);
}

void initSDcard()
{
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

}

void createFile()
{
  char filename[] = "MLOG00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[4] = i / 10 + '0';
    filename[5] = i % 10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE);
      break;  // leave the loop!
    }
  }

  if (! logfile) {
    error("couldnt create file");
  }

  Serial.print("Logging to: ");
  Serial.println(filename);
}

void initRTC()
{
  Wire.begin();
  if (!RTC.begin()) {
    logfile.println("RTC failed");
#if ECHO_TO_SERIAL
    Serial.println("RTC failed");
#endif  //ECHO_TO_SERIAL

  }
}
