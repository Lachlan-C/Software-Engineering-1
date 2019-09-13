/*  */

//Libraries
#include <DHT.h>

//Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino

//Variables
float temp; //Stores temperature value

void setup()
{
    Serial.begin(9600);
    dht.begin();
    pinMode(8, OUTPUT);  //1,2,3,4 layers
    pinMode(9, OUTPUT);  //2,3,4 layers
    pinMode(10, OUTPUT); //3,4 layers
    pinMode(11, OUTPUT); //4 layers
}

void loop()
{
    //Read data and store it to variables hum and temp

    temp = dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    if (temp >= 25)
      { 
         digitalWrite(8, HIGH);
         digitalWrite(9, LOW);
         digitalWrite(10, LOW);
         digitalWrite(11, LOW);
      }
    else if (temp < 25 && temp >= 15)
     {
         digitalWrite(8, HIGH);
         digitalWrite(9, HIGH);
         digitalWrite(10, LOW);
         digitalWrite(11, LOW);
    }
    else if (temp < 15 && temp >= 0)
    {
         digitalWrite(8, HIGH);
         digitalWrite(9, HIGH);
         digitalWrite(10, HIGH);
         digitalWrite(11, LOW);
    }
    else if (temp < 0)
    {
         digitalWrite(8, HIGH);
         digitalWrite(9, HIGH);
         digitalWrite(10, HIGH);
         digitalWrite(11, HIGH);
    }
    delay(2000); //Delay 2 sec.
}
