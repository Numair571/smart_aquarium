#include <Servo.h>
#include <ThingSpeak.h>

int waterLevelPin = A0;
int servoMotorPin = D5;
int foodDispenseInterval = 300000; 

char thingSpeakAPIKey[] = "YOUR_THINGSPEAK_API_KEY";
int channelID = YOUR_THINGSPEAK_CHANNEL_ID;

Servo servoMotor;
unsigned long lastDispenseTime = 0; 
void setup() {
  Serial.begin(9600);
  servoMotor.attach(servoMotorPin);
  ThingSpeak.begin(client);
  Serial.println("Smart Aquarium System Online");
}

void loop() {
  int waterLevel = analogRead(waterLevelPin);
  int servoPosition = map(waterLevel, 0, 1023, 0, 180);
  servoMotor.write(servoPosition);

  updateThingSpeak(waterLevel);

 
  if (millis() - lastDispenseTime >= foodDispenseInterval) {
    dispenseFood();
    lastDispenseTime = millis(); 
  }

  delay(1000);
}

void updateThingSpeak(int waterLevel) {
  String data = String("field1=") + String(waterLevel);
  int status = ThingSpeak.writeFields(channelID, data, thingSpeakAPIKey);
  Serial.print("ThingSpeak Update: ");
  if (status == 200)
    Serial.println("Success");
  else
    Serial.println("Failed");
}

void dispenseFood() {
  
  servoMotor.write(90); 
  delay(2000); 
  servoMotor.write(0); 
}
