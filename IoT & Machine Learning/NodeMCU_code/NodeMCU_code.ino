//.........................The CarbonZ source code is built for the Chainlink Hackathon..............................................................................
//Author: Olufemi Victor Tolulope in Team CarbonZ
//Reach out on linkedIn https://www.linkedin.com/in/olufemi-victor-tolulope/

// WARNING! If you are a newbie and just downloaded this code from my repository and you want to run it, you need to put it in the standard folder where arduino ide keeps its files, else the code won't compile

#include <ESP8266WiFi.h> //wifi library is needed
#include <Adafruit_ADS1X15.h> //For the ADC.
#include <Wire.h> // wire library
#include <WiFiClientSecure.h> // Secure client with SSL for https conversations
#include <ESP8266WebServer.h> // Manage conversations with the web server
#include <ESP8266HTTPClient.h> // Client that handles the requests
#include <NTPClient.h> // catch time from the NTP server
#include <WiFiUdp.h> // 
#include <ArduinoJson.h> // help handle JSON formatting
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
  


Adafruit_ADS1115 ads;     /* I'm Using the for the 16-bit version */
Adafruit_BME280 bme; // I2C

//#include "CO2Sensor.h" // download co2sensor library from https://content.instructables.com/ORIG/FLJ/CVDP/K6HPDU8R/FLJCVDPK6HPDU8R.ino
#define MQ7_Digital D6 //Define port D4 for the MQ7 sensor
#define MQ9_Digital D7 //Define port D5 for the MQ9 sensor
#define MG811_Digital D5 //Just in case you'll like to check out digital values of the Co2 sensor
#define MQ135_Digital D4   //Define port D7 for the MQ135 sensor
#define SEALEVELPRESSURE_HPA (1013.25)

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");


String apiKey = "ZWR8R8U6BVVN6RYX"; // Delta - Enter your Write API key from ThingSpeak - Removed secrets
float co2_prediction = 415.0; //This would help collect response from ML model at AWS Elastic Beanstalk let default value be 415 - CO2
const char *ssid = "Professor"; // replace with your wifi ssid.
const char *pass = "Professor."; //replace with your wifi password
const char* server = "api.thingspeak.com"; // We're using the thingspeak server
const char *host = "api.web3.storage"; // Blockchain Host is web3 storage
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80 - keep this for conversations with https
const int watch_time = 3600; // This is the watch time in seconds where the IOT monitors until it pushes readings and starts over
const char* ml_host = "chemotronix-calibration-env.eba-hp5mrvst.us-east-1.elasticbeanstalk.com";
const int port = 80;
const char* ml_endpoint = "/predict";


// ...........................Connecting to WiFi.............................................................................................................................................

WiFiClient client;

void setup()
{
  Serial.begin(115200); // The Baud rate for reading serial monitor
  //while(!Serial) { } // Wait for serial to initialize. 
  
  delay(1000);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //Only Station No AP, This line hides the viewing of ESP as wifi hotspot to prevent hack ins


  delay(500);
  Serial.println("=== Checking BME sensor ===");

  bool status;
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not detect a BME280 sensor, Fix wiring Connections!");
    while (1);
  }
    Serial.println("=== BME SENSOR Passed ===");

Serial.println("=== Setting up ADS.....Preparing to Initialize ===");

  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
Serial.println("=== ADS is Initialized ===");
delay(500);

pinMode(MQ135_Digital,INPUT); //Set Pins as Inputs
pinMode(MQ7_Digital,INPUT); //Set Pins as Inputs
pinMode(MQ9_Digital,INPUT); //Set Pins as Inputs
pinMode(MG811_Digital,INPUT);
delay(500);

Serial.println("Connecting to ");
Serial.println(ssid);

// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(3600); // use WAT as the time or GMT+1

WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");


}
Serial.println("");
Serial.println("WiFi connected");
Serial.println("");
Serial.print("Connected to ");
Serial.println(ssid);
Serial.print("IP address: ");
Serial.println(WiFi.localIP());  //IP address assigned to your ESP
delay(500);

}

//.............................................READ SENSOR VALUES................................................................................................................................

//-------------------------- Analog readings ---------------------------------------------

void loop()
{
  
timeClient.update();
String formattedTime = timeClient.getFormattedTime();
Serial.print("Formatted Time: ");
Serial.println(formattedTime);

//Get a time structure
time_t epochTime = timeClient.getEpochTime();
struct tm *ptm = gmtime ((time_t *)&epochTime);

int monthDay = ptm->tm_mday;
int currentMonth = ptm->tm_mon+1;
int currentYear = ptm->tm_year+1900;
String formattedDate = String(monthDay) + "-" + String(currentMonth) +  "-" + String(currentYear) + " ";

Serial.print("Formatted Date: ");
Serial.println(formattedDate);

Serial.println("Reading DATA from ADS pins");
delay(5000);
Serial.println("Passed brief delay");
  int16_t adc0, adc1, adc2, adc3;
  Serial.println("Setup the readouts");
  adc0 = ads.readADC_SingleEnded(0); // MQ7 - CO
  delay(500);
  Serial.println("Read A0 successfully");
  adc1 = ads.readADC_SingleEnded(1); // MQ9 - CH4, CO
  delay(500);
   Serial.println("Read A1 successfully");
  adc2 = ads.readADC_SingleEnded(2); // MG811 - Co2
  delay(500);
   Serial.println("Read A2 successfully");
  adc3 = ads.readADC_SingleEnded(3); // MQ135 - CH4, CO
  delay(500);
   Serial.println("Read A3 successfully");
  delay(1000);

  float MQ7_analog = adc0; // Raw analog values from adc0 connected to MQ7
  float MQ9_analog = adc1; // Raw analog values from adc1 connected to MQ9
  float MG811_analog = adc2; //Raw analog values from adc2 connected to MG811
  float MQ135_analog = adc3; // Raw analog values from adc3 connected to MQ135

   

//------------------------------------------- Digital readings -------------------------------------------------------------------------------------------------------------------------

int MQ135_sensor_digital = digitalRead(MQ135_Digital); //Digital value for CO sensor(MQ135)
delay(500);
int mq7_sensor_digital = digitalRead(MQ7_Digital); //Digital value for CO sensor(MQ7)
delay(500);
int mq9_sensor_digital = digitalRead(MQ9_Digital); //Digital value for CO sensor(MQ9)
delay(500);
int mg811_sensor_digital = digitalRead(MG811_Digital); //Digital value for CO sensor(MG811)
delay(500);


//--------------------------------- 

float temperature = bme.readTemperature();
float pressure = bme.readPressure() / 100.0F ;
float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
float humidity = bme.readHumidity();




//--------------------------------------------------  Connecting to AWS ELASTIC BEANSTALK ---------------------------------------------------------------------//
// Construct the JSON body
if (client.connect(ml_host, port)) {
      Serial.println("Connected to server");

      // Create a JSON document
      StaticJsonDocument<200> jsonDoc;



      // Populate the JSON document
      jsonDoc["Temperature"] = temperature;
      jsonDoc["Humidity"] = humidity;
      jsonDoc["MQ7_analog"] = MQ7_analog;
      jsonDoc["MQ9_analog"] = MQ9_analog;
      jsonDoc["MG811_analog"] = MG811_analog;
      jsonDoc["MQ135_analog"] = MQ135_analog;
      jsonDoc["MG811_Digital"] = mg811_sensor_digital;
      jsonDoc["MQ7_Digital"] = mq7_sensor_digital;

      // Serialize the JSON document to a string
      String requestBody;
      serializeJson(jsonDoc, requestBody);
      //String requestBody = "{\"Temperature\":30.76000023,\"Humidity\":67.06835938,\"MQ7_analog\":7354,\"MQ9_analog\":10299,\"MG811_analog\":4364,\"MQ135_analog\":4916,\"MG811_Digital\":0,\"MQ7_Digital\":1}";
      delay(500); // Wait for the server to respond

      // Prepare the request headers and body
      String request = "POST " + String(ml_endpoint) + " HTTP/1.1\r\n" +
                       "Host: " + String(ml_host) + "\r\n" +
                       "Content-Type: application/json\r\n" +
                       "Content-Length: " + String(requestBody.length()) + "\r\n" +
                       "\r\n" +
                       requestBody;

      client.print(request);
      Serial.println(requestBody);

      delay(7000); // Wait for the server to respond

// Skip headers and get the response body
      bool skipHeaders = false;
      String responseBody;
      while (client.available()) {
        String line = client.readStringUntil('\r');
        if (line == "\n") {
          skipHeaders = true;
        }
        if (skipHeaders) {
          responseBody += line;
        }
      }

      responseBody.trim(); // Remove leading and trailing white spaces

      Serial.println();
      Serial.println(responseBody);

      // Assign the response value to the global variable
      co2_prediction = responseBody.toInt();
    

      Serial.println();
      Serial.println("Request sent and response received");
    } else {
      Serial.println("Connection to server failed");
    }

    client.stop(); // Close the connection
    delay(5000);  // Wait for some time before sending the next request
  





//.......................................... Connecting to Thingspeak .......................................................................................................................
 
if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
{
String postStr = apiKey; //Make Sure You write To your Own API key.
postStr += "&field1=";
postStr += String(temperature);
postStr += "&field2=";
postStr += String(humidity);
postStr += "&field3=";
postStr += String(MQ7_analog);
postStr += "&field4=";
postStr += String(MQ9_analog); 
postStr += "&field5=";
postStr += String(MG811_analog);
postStr += "&field6=";
postStr += String(MQ135_analog);
postStr += "&field7=";
postStr += String(mg811_sensor_digital);
postStr += "&field8=";
postStr += String(co2_prediction);
   



client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);

//......................................................SERIAL MONITORING.....................................................................................................................
Serial.print("Readings from the sensors: ");
Serial.println();
Serial.print("Readings from the MQ135 sensor: ");
Serial.println(MQ135_sensor_digital);
Serial.print("raw Readings from the MQ7 sensor: ");
Serial.println(mq7_sensor_digital);
Serial.print("Digital Readings from the Mg811 sensor: ");
Serial.println(mg811_sensor_digital);
Serial.print("Digital Readings from the MQ9 sensor: ");
Serial.println(mq9_sensor_digital);
Serial.println("Data Send to Thingspeak");
Serial.print("AIN0 - MQ7: "); 
Serial.println(adc0);
Serial.print("AIN1 - MQ9: "); 
Serial.println(adc1);
Serial.print("AIN2 - MG811: "); 
Serial.println(adc2);
Serial.print("AIN3 - MQ135: "); 
Serial.println(adc3);
Serial.print("Temperature = ");
Serial.print(temperature);
Serial.println(" ºC");
Serial.print("Pressure = ");
Serial.print(pressure);
Serial.println(" hPa");
Serial.print("Approx. Altitude = ");
Serial.print(altitude);
Serial.println(" m");
Serial.print("Humidity = ");
Serial.print(humidity);
Serial.println(" %");
Serial.print(co2_prediction);
Serial.println(" PPM");


Serial.println();
}
delay(500);
client.stop();
Serial.println("Waiting...");
 
// thingspeak needs minimum 15 sec delay between updates.
delay(16000);



  Serial.println("==========");
  Serial.println("closing connection");
    
  delay(2000);  //Take a deep breath for 2 seconds, you've done a lot

  // Deep sleep mode for 30 seconds, the ESP8266 wakes up by itself when GPIO 16 (D0 in NodeMCU board) is connected to the RESET pin
//  Serial.println("I'm awake, but I'm going into deep sleep mode for 30 seconds");
 // ESP.deepSleep(watch_time * 1000000);


} // This closes the void loop
