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

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define heartratePin 36
DFRobot_Heartrate heartrate(DIGITAL_MODE);   // ANALOG_MODE or DIGITAL_MODE
Adafruit_MPU6050 mpu;

const int pushButton = 4;
const int buzzer = 12;

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1); 

const char* ssid = "ok";
const char* password = "12345678";
 
int GMTOffset = 19800;  //Replace with your GMT Offset in seconds
int daylightOffset = 0;  // Replace with your daylight savings offset in seconds

//##############**"Titanic" **##############//
int Titanic_note[] = {
NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_B4, NOTE_E4,
NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_B4, NOTE_F5,
NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_B4, NOTE_E4,
NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_B3, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_B3, NOTE_E4,
NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4
};
int Titanic_duration[] = {
8,8,8,8,8,8,4,8,8,8,8,8,8,4,8,8,8,8,8,8,4,8,8,8,8,4,4,8,8,8,8,8,8,4,8,8,8,8,8,8,4,8,8,8,8,8,8,1,
8,8,8,8,4,4,4,8,4,4,8,8,8,8,4,8,8,4,8,4,8,8,4,8,4,1
};
//###########End of Titanic#############//

//##############**"Crazy Frog" song of Crazy frog album**##############//
int CrazyFrog_note[] = {
  NOTE_D4, 0, NOTE_F4, NOTE_D4, 0, NOTE_D4, NOTE_G4, NOTE_D4, NOTE_C4,
  NOTE_D4, 0, NOTE_A4, NOTE_D4, 0, NOTE_D4, NOTE_AS4, NOTE_A4, NOTE_F4,
  NOTE_D4, NOTE_A4, NOTE_D5, NOTE_D4, NOTE_C4, 0, NOTE_C4, NOTE_A3, NOTE_E4, NOTE_D4,
  0,NOTE_D4,NOTE_D4
};
int CrazyFrog_duration[] = {
  8, 8, 6, 16, 16, 16, 8, 8, 8, 
  8, 8, 6, 16, 16, 16, 8, 8, 8,
  8, 8, 8, 16, 16, 16, 16, 8, 8, 2,
  8,4,4
};
//###########End of Crazy Frog#############//

byte pushState;
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
pinMode(buzzer, OUTPUT);
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
pushState = digitalRead(pushButton);
Serial.println(pushState);
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
        if(((timeinfo->tm_hour == 19)&&(timeinfo->tm_min == 15)&&(timeinfo->tm_sec == 0))||((timeinfo->tm_hour == 12)&&(timeinfo->tm_min == 25)&&(timeinfo->tm_sec == 3))){

           Serial.println("Medicine Reminder");
           while(var==0)
           {
            display.clearDisplay();
            display.setCursor(10,30);
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.print("Medicine Reminder");
            display.display();
            Serial.println(pushState);
            digitalWrite(2, HIGH);
            delay(100);
            
            for (int thisNote = 0; thisNote < (sizeof(CrazyFrog_note)/sizeof(int)); thisNote++) {

              int noteDuration = 1000 / CrazyFrog_duration[thisNote]; //convert duration to time delay
              tone(buzzer, CrazyFrog_note[thisNote], noteDuration);
          
              int pauseBetweenNotes = noteDuration * 1.30;//Here 1.30 is tempo, decrease to play it faster
              delay(pauseBetweenNotes);
            
            if(pushState==1)
            {
                digitalWrite(2, LOW);
                delay(100);
                noTone(buzzer); //stop music on pin 12
                var=1;
                break;
            }
          }
    
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
    Serial.println("Fall Detected!");
    display.setCursor(20,30);
    display.println("Fall Detected!");
    delay(10000);
    Serial.println("Did you fall?");
    display.setCursor(0,0);
    display.println("Did you fall?");
    display.println("If no, Do not press any button.");
    if(pushState == 1){
        digitalWrite(2,HIGH);
        delay(100);
        for (int thisNote = 0; thisNote < (sizeof(Titanic_note)/sizeof(int)); thisNote++) {

            int noteDuration = 1000 / Titanic_duration[thisNote];//convert duration to time delay
            tone(buzzer, Titanic_note[thisNote], noteDuration);

            int pauseBetweenNotes = noteDuration * 2.70;
            delay(pauseBetweenNotes);
            
            delay(30000);
            display.clearDisplay();
            Serial.println("Help?");
            display.println("Send Help?");
            if(pushState == 1){
                Serial.println("Sending help!");
                display.println("Sending help!");
                display.display();
                noTone(buzzer);
        }
    }
  

delay(1000); 
 
}
}