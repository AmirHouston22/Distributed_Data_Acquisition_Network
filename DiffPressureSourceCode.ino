#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ModbusMaster.h>

// Define pins for SPI
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8 // Use -1 if connected to Arduino RESET pin
#define LED1      5
#define LED2      6
#define MAX485_RE_DE  2

// Initialize Adafruit ILI9341
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

//Setting up WiFi  and Web App details
const char* ssid = "UALR-Device";
const char* password = "GoTrojans!2026";

//replace Web App URL below with your own
const char* scriptURL = "https://script.google.com/macros/s/AKfycbyflVgQqse2P4dRit8ZXpS4eqau3sjfNk8KNu4nsb8rJrxQJiD7qU2m-5eZyAK47hkH-g/exec";

ModbusMaster node;

void preTransmission(){digitalWrite(MAX485_RE_DE, HIGH); }
void postTransmission(){digitalWrite(MAX485_RE_DE, LOW); }

void setup() {
  pinMode(MAX485_RE_DE, OUTPUT);
  digitalWrite(MAX485_RE_DE, LOW);
  Serial.begin(115200);
  Serial1.begin(96000, SERIAL_8N1, 1, 0);

  node.begin(1, Serial1);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  tft.begin();
;  
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
    tft.println("Pressure:");
  
}

void loop() {
 uint8_t result = node.readHoldingRegisters(0,1);

 if(WiFi.status()==WL_CONNECTED) {

    DigitalWrite(LED1, HIGH);
    DigitalWrite(LED2, HIGH);
      
    tft.setCursor(20, 20);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println("Connected to wifi network");

  if (result == node.ku8MBSuccess) {
    // If successful, grab the data out of the buffer
    float pressure = node.getResponseBuffer(0); 

    tft.fillRect(10, 40, 100, 20 ILI9341_BLACK);

    tft.setCursor(10, 40);
    tft.setTextColor(ILI9341_GREEN);
    tft.print(pressure);
    tft.print(" pascal");
    delay(1000);
  } 
    else {
    // If it failed, print the specific error hex code
    Serial.print("Error: ");
    Serial.println(result, HEX);

    tft.fillRect(10, 40, 100, 20 ILI9341_BLACK);

    tft.setCursor(10, 40);
    tft.setTextColor(ILI9341_RED);
    tft.print("Error: ");
    tft.print(result, HEX);
    delay(1000);
  }

    WiFiClientSecure client;
    client.setInsecure();   // skip certificate validation

    HTTPClient http;
    http.begin(client, scriptURL);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.addHeader("Content-Type", "application/json");
    
    float temp = c;
    String jsonData = "{\"method\":\"append\",\"Pressure\":" + String(pressure) + "}";
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


