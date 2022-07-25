#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
#endif
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <time.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "DFRobot_Heartrate.h"
#define heartratePin 36
DFRobot_Heartrate heartrate(DIGITAL_MODE);   // ANALOG_MODE or DIGITAL_MODE
Adafruit_MPU6050 mpu;

const int pushButton = 4;

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1); 

const char* ssid = "ok";
const char* password = "12345678";
 
int GMTOffset = 19800;  //Replace with your GMT Offset in seconds
int daylightOffset = 0;  // Replace with your daylight savings offset in seconds


void accelerometerSetup(){
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Initializing MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);

  
}


void setup() { 
Serial.begin(115200); 
pinMode(2,OUTPUT);
pinMode(pushButton, INPUT);
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
  Serial.println(F("SSD1306 allocation failed"));
  for(;;);
}
  delay(2000);
display.clearDisplay();
display.setTextSize(1);
display.setCursor(0,0);
display.setTextColor(WHITE);

WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to Wi-Fi!");

configTime(GMTOffset, daylightOffset, "pool.ntp.org","time.nist.gov");
accelerometerSetup();
}

 
void loop() {

uint8_t rateValue;
rateValue=heartrate.getValue(heartratePin); 

time_t rawtime = time(nullptr);
struct tm* timeinfo = localtime(&rawtime);

Serial.print("Time: ");
Serial.print(timeinfo->tm_hour);
Serial.print(":");
Serial.print(timeinfo->tm_min);
Serial.print(":");
Serial.println(timeinfo->tm_sec);

int var = 0;
        if(((timeinfo->tm_hour == 12)&&(timeinfo->tm_min == 55)&&(timeinfo->tm_sec == 0))||((timeinfo->tm_hour == 12)&&(timeinfo->tm_min == 25)&&(timeinfo->tm_sec == 3))){

           Serial.println("Medicine Reminder");
           while(var==0)
           {
            digitalWrite(2, HIGH);
            delay(100);
            int pushState = digitalRead(pushButton);
            Serial.println(pushState);
            if(pushState==1)
            {
                digitalWrite(2, LOW);
                var=1;
                // break;
            }
        	display.clearDisplay();
            display.setCursor(10,30);
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.print("Medicine Reminder");
            display.display();
           }
      }


display.clearDisplay();
display.setCursor(0,0);
display.setTextSize(2);
display.setTextColor(WHITE);

display.print(timeinfo->tm_hour);
display.print(":");
if( timeinfo->tm_min <10)
    display.print("0");
display.print(timeinfo->tm_min);

display.print(":");

if( timeinfo->tm_sec <10)
    display.print("0");

display.print(timeinfo->tm_sec); 
    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(30,40);
    display.print("HeartRate: ");
    display.print(rateValue);

    display.display();


 
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20,30);
  if(a.acceleration.x > 8 || a.acceleration.x < -8 ){
    display.clearDisplay();
    Serial.println("Fall Detected!");
    display.setCursor(20,30);
    display.println("Fall Detected!");
    display.display();
  }

delay(1000); 
 
}