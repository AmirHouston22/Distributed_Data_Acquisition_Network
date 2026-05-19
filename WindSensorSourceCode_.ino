#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// Define pins for SPI
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8 // Use -1 if connected to Arduino RESET pin
#define LED1      5
#define LED2      6

// Initialize Adafruit ILI9341
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

const int OutPin  = 15;   // wind sensor analog pin  hooked up to Wind P sensor "OUT" pin
const int TempPin = 2;   // temp sensor analog pin hooked up to Wind P sensor "TMP" pin


//Setting up WiFi  and Web App details
const char* ssid = "UALR-Device";
const char* password = "GoTrojans!2026";

//replace Web App URL below with your own
const char* scriptURL = "https://script.google.com/macros/s/AKfycbyflVgQqse2P4dRit8ZXpS4eqau3sjfNk8KNu4nsb8rJrxQJiD7qU2m-5eZyAK47hkH-g/exec";


void setup() {
    Serial.begin(9600);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    
    tft.begin();
  
    // Set rotation (0-3)
    tft.setRotation(1);
  
    // Clear screen with a background color
    tft.fillScreen(ILI9341_WHITE);
  
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
   
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }
    Serial.println("\nConnected to the WiFi network");
    tft.setCursor(20, 20);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println("Air Velocity:");
}

void loop() {

    // read wind
    int windADunits = analogRead(OutPin);
    Serial.print("RW ");   // print raw A/D for debug
    Serial.print(windADunits);
    Serial.print("\t");
    
    
    // wind formula derived from a wind tunnel data, annemometer and some fancy Excel regressions
    // this scalin doesn't have any temperature correction in it yet
    float windMPH =  pow((((float)windADunits - 264.0) / 85.6814), 3.36814);
    Serial.print(windMPH);
    Serial.print(" MPH\t");    
    float windMS = windMPH * 0.44704; // convert MPH to m/s
    Serial.print(windMS);
    Serial.print(" m/s");

 


    // temp routine and print raw and temp C
    int tempRawAD = analogRead(TempPin);  
    Serial.print("RT ");    // print raw A/D for debug
    Serial.print(tempRawAD);
    Serial.print("\t");
    
     // convert to volts then use formula from datatsheet 
    tempC = (Vout - V0c) / TC   //see the MCP9701 datasheet for V0c and TC
    Vout = ( TempC * .0195 ) + .400

    float tempC = ((((float)tempRawAD * 5.0) / 1024.0) - 0.400) / .0195; 
    Serial.print(tempC);
    Serial.println(" C");
    delay(750);

    if(WiFi.status()==WL_CONNECTED) {

    DigitalWrite(LED1, HIGH);
    DigitalWrite(LED2, HIGH);
      
    tft.setCursor(20, 20);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println("Connected to wifi network");

    tft.fillRect(10, 40, 100, 20 ILI9341_BLACK);

    tft.setCursor(10, 40);
    tft.setTextColor(ILI9341_GREEN);
    tft.print(windMS);
    tft.print(" m/s");
    delay(1000);

    WiFiClientSecure client;
    client.setInsecure();   // skip certificate validation

    HTTPClient http;
    http.begin(client, scriptURL);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.addHeader("Content-Type", "application/json");
    
    float temp = c;
    String jsonData = "{\"method\":\"append\",\"Air Velocity\":" + String(windMS) + "}";
    int httpResponseCode = http.POST(jsonData);
    Serial.println("Response Code: " + String(httpResponseCode));

    String payload = http.getString();
    Serial.println(payload);
    http.end();
    }
    else{
      Serial.println("WiFi Disconnected");
      DigitalWrite(LED1, HIGH);
      delay(1000);
      DigitalWrite(LED2,LOW);
      delay(1000);

      tft.setCursor(10, 40);// adjust if needed
      tft.setTextColor(ILI9341_RED);
      tft.print(" WiFi Disconnected");
      delay(1000);


    }
}



