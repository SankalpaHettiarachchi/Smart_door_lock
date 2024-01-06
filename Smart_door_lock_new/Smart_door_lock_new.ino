#include <SoftwareSerial.h>
SoftwareSerial sim(2, 3);
int lock = 8;


void setup() {
  pinMode(lock, OUTPUT);
  sim.begin(9600);
  Serial.begin(9600);
  Serial.println("Type s to send an SMS, r to receive an SMS, and c to make a call");
}
void loop() {
  if (Serial.available() > 0)
    switch (Serial.read())
    {
      case 's':
        SendMessage();
        resetGSM();
        getotp();
        break;
     }
    
  if (sim.available() > 0)
  {
    Serial.write(sim.read());
   }  
}
void SendMessage()
{
  sim.println("AT+CMGF=1");   
  delay(1000);
  sim.println("AT+CMGS=\"+94715917828\"\r");
  delay(1000);
  
  int otp = random(2000, 9999);
  String otpstring = String(otp);
  
  sim.println("OTP : "+ otpstring);
  delay(100);
  
  sim.println((char)26);
  delay(1000);
  
  digitalWrite(lock, HIGH);
  delay(5000);
}

void resetGSM() {
  // Code to reset the GSM module
  sim.println("AT+CFUN=1,1"); // Perform a soft reset of the GSM module
  delay(1000); // Delay for the module to reset (adjust time if needed)
}

void getotp()
{
  //
}
