#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

Servo myServo;
#define relay 4
#define red 16
#define green 15


const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 13, 12, 11, 10 };
byte colPins[COLS] = { 9, 8, 7, 6 };
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27, 16, 2);

SoftwareSerial sim800l(2, 3);
int otp;
String otpstring = "";
int i = 0;
int irsensor = HIGH;
void setup() {
  
  sim800l.begin(4800);
  Serial.begin(115200);
  lcd.init();
  lcd.clear();
  lcd.backlight();
//  Serial.print("Welcome to SIM800L Project");
  sim800l.println("AT");
  updateSerial();
  pinMode(relay, OUTPUT);
  pinMode(red, INPUT);
  pinMode(green, INPUT);
  digitalWrite(relay, LOW);
  delay(500);
  sim800l.println("AT+CSQ");
  updateSerial();
  delay(1000);
  myServo.attach(relay);
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    sim800l.write(Serial.read());
  }
  while (sim800l.available()) {
    Serial.write(sim800l.read());
  }
}

void loop() {
  
  digitalWrite(relay, LOW);
  lcd.clear();
  lcd.print("Press A :");
  String z = "";
  int b = z.length();
  while (b < 1) {
    char customKey = customKeypad.getKey();
    if (customKey == 'A') {
      lcd.setCursor(0, 1);
      z = z + customKey;
      lcd.print(z);
      b = z.length();
      delay(1000);
      lcd.clear();
      lcd.print("Sending OTP");

//      lcd.setCursor(0, 0);
//      lcd.print("OTP Based");
//      lcd.setCursor(0, 1);
//      lcd.print("Door Lock");

      irsensor = LOW;
      if (irsensor == LOW) {
        otp = random(2000, 9999);
        otpstring = String(otp);
//        Serial.println(otpstring);
        SendSMS();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" OTP Sent to");
        lcd.setCursor(0, 1);
        lcd.print(" Your Mobile");
//        Serial.print("OTP is ");
//        delay(100);
//        Serial.println(otpstring);
//        delay(100);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter OTP :");
//        Serial.println("Enter OTP :");
        getotp();
        delay(1000); 
      }
    }
  }
}

void getotp() {

  String y = "";
  int a = y.length();
  while (a < 4) {
    char customKey = customKeypad.getKey();
    if (customKey) {
      lcd.setCursor(0, 1);
      y = y + customKey;
      lcd.print(y);
      Serial.print(y);
      a = y.length();
    }
  }
  
//  Serial.print("Entered OTP is ");
//  Serial.println(y);

  delay(1000);

  if (otpstring == y) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Granted");
    lcd.setCursor(0, 1);
    lcd.print("Door Opening");
    
      for (int angle = 180; angle >= 90; angle -= 1) { 
        myServo.write(angle);
        delay(20); 
      }

      lcd.clear();
      lcd.print("Door Opened");
      delay(3000);
      lcd.clear();
      lcd.print("Door Closing");
      
      for (int angle = 90; angle <= 180; angle += 1) { 
        myServo.write(angle);
        delay(20); 
      }

      lcd.clear();
      lcd.print("Door Locked");
      delay(1000);
      

      digitalWrite(relay, LOW);
//    digitalWrite(red, HIGH);
//    digitalWrite(green, LOW);
//    delay(5000);
//    digitalWrite(relay, LOW);
//    digitalWrite(red, LOW);
//    digitalWrite(green, HIGH);
    
  } else {
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Failed");
    lcd.setCursor(0, 1);
    lcd.print("Try Again !!!");
    delay(3000);
    lcd.clear();
  }
}
void SendSMS() {

  sim800l.println("AT+CMGF=1");   
  delay(1000);
  sim800l.println("AT+CMGS=\"+94776446208\"\r");
  delay(1000);
  
  sim800l.println("OTP : " + otpstring);
  delay(100);
  
  sim800l.println((char)26);
  delay(1000);
  
//  digitalWrite(lock, HIGH);
  delay(5000);
}
void resetGSM() {
  // Code to reset the GSM module
  sim800l.println("AT+CFUN=1,1"); // Perform a soft reset of the GSM module
  delay(1000); // Delay for the module to reset (adjust time if needed)
}
