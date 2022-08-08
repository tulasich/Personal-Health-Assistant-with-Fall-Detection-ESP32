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
#include <ESP_Mail_Client.h>

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "mrminiprjt@gmail.com"
#define AUTHOR_PASSWORD "cukfquajeyfkeolr"

/* Recipient's email*/
#define RECIPIENT_EMAIL "19r11a04n7@gcet.edu.in"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

#define heartratePin 36
DFRobot_Heartrate heartrate(DIGITAL_MODE);   // ANALOG_MODE or DIGITAL_MODE
Adafruit_MPU6050 mpu;

const int pushButton = 4;
const int buzzer = 12;
int pushState;
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

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
    }
    Serial.println("----------------\n");
  }
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
uint8_t rateValue; //heartrate
rateValue=heartrate.getValue(heartratePin); 

time_t rawtime = time(nullptr); //time
struct tm* timeinfo = localtime(&rawtime);

Serial.print("Time: ");
Serial.print(timeinfo->tm_hour);
Serial.print(":");
Serial.print(timeinfo->tm_min);
Serial.print(":");
Serial.println(timeinfo->tm_sec);

int var = 0;
if(((timeinfo->tm_hour == 13)&&(timeinfo->tm_min == 42)&&(timeinfo->tm_sec == 3))||((timeinfo->tm_hour == 11)&&(timeinfo->tm_min == 8)&&(timeinfo->tm_sec == 3))){

           Serial.println("Medicine Reminder");
           while(var==0)
           {
            
            byte pushState = digitalRead(pushButton);
            Serial.println(pushState);
            digitalWrite(2, HIGH);
            digitalWrite(buzzer, HIGH);
            delay(50);
            digitalWrite(buzzer,LOW);
            delay(50);
            if(pushState==1)
            {
                digitalWrite(2, LOW);
                delay(100);
                digitalWrite(buzzer, LOW);
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
  
  if(a.acceleration.x > 8 || a.acceleration.x < -8 || a.acceleration.y > 8 || a.acceleration.y < -8){
    display.clearDisplay();
    Serial.println("Fall Detected!");
    display.setCursor(20,30);
    display.println("Fall Detected!");
    display.display();
      /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
    byte fall = 0;
    while (fall == 0){

        for(int i=0; i<100; i++ ){
            digitalWrite(buzzer, HIGH);
            delay(40);
            digitalWrite(buzzer, LOW);
            delay(40);
            smtp.debug(1);
        }
        /* Set the callback function to get the sending results */
            smtp.callback(smtpCallback);

        /* Declare the session config data */
            ESP_Mail_Session session;

        /* Set the session config */
        session.server.host_name = SMTP_HOST;
        session.server.port = SMTP_PORT;
        session.login.email = AUTHOR_EMAIL;
        session.login.password = AUTHOR_PASSWORD;
        session.login.user_domain = ""; 

        /* Declare the message class */
        SMTP_Message message;

        /* Set the message headers */
        message.sender.name = "ESP";
        message.sender.email = AUTHOR_EMAIL;
        message.subject = "Fall Detected";
        message.addRecipient("kalyan", RECIPIENT_EMAIL); 

        /*Send HTML message*/
        String htmlMsg = "<div style=\"color:#2f4468;\"><h1>FAll Detected!</h1><p>- Sent from ESP board</p></div>";
        message.html.content = htmlMsg.c_str();
        message.html.content = htmlMsg.c_str();
        message.text.charSet = "us-ascii";
        message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit; 

        
        //Send raw text message
       /* String textMsg = "Hello World! - Sent from ESP board";
        message.text.content = textMsg.c_str();
        message.text.charSet = "us-ascii";
        message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
        
        message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
        message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;
*/
        /* Set the custom message header */
       // message.addHeader("Message-ID: <abcde.fghij@gmail.com>"); 

        /* Connect to server with the session config */
        if (!smtp.connect(&session))
            return;

        /* Start sending Email and close the session */
        if (!MailClient.sendMail(&smtp, &message))
            Serial.println("Error sending Email, " + smtp.errorReason());
            delay(2000);

            fall = 1;
/*    boolean fall = 0;
    while(fall == 0){
        display.clearDisplay();
        Serial.println("Did you fall?");
        display.setCursor(0,0);
        display.println("Did you fall?");
        display.println("If no, Do not press any button.");
        pushState = digitalRead(pushButton);
        display.display();
        Serial.println(pushState);
    
          digitalWrite(2,HIGH);
            delay(50);
            digitalWrite(2,LOW);
            delay(50);
            display.clearDisplay();
            Serial.println("Help?");
            display.println("Send Help?");
            display.display();
            if(pushState == 1){
            Serial.println("Sending help!");
            display.println("Sending help!");
            display.display();
            }
        }
        display.clearDisplay();
        display.println("Click to turn off buzzer");
        display.display();
        delay(200);
        if(pushState == 1){
            digitalWrite(buzzer,LOW);
            fall = 1;
        } */


    }
  }
    
delay(1000); 
 
}


/* Fall Detection - Yes/No Setup - MAil
Accelerometer work
GPS */