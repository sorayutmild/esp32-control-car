#include <ESP32Servo.h> 

const unsigned char motorA = 26, motorB = 27, motorPWM = 25, motorChan = 5;
const unsigned char servoPWM = 17, servoFeedback = 33;
const unsigned char myservo1_pin = 13, myservo2_pin = 14, myservo3_pin = 15, myservo4_pin = 16, myservo1_pin = 17;
Servo myservo, myservo1, myservo2, myservo3, myservo4, myservo5;

/*Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;*/

const unsigned char packetSize = 3;

union packet {
  struct __attribute__((packed)) {
    unsigned char p;
    short i;
  };
  unsigned char b[packetSize];
};

void setup() { 
  Serial.begin(115200);
  // set up motor direction pin
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);
  // set up LED PWM controller channel 0 frequency 500 hz, 10 bit resolution
  ledcSetup(motorChan, 500, 10);
  // attach channel 0 to motor PWM channel
  ledcAttachPin(motorPWM, motorChan);
  // set up servo
  myservo.attach(servoPWM);
  myservo1.attach(myservo1_pin); 
  myservo2.attach(myservo2_pin);
  myservo3.attach(myservo3_pin);
  myservo4.attach(myservo4_pin);
  myservo5.attach(myservo5_pin);
} 

void loop() {
  if (Serial.available() > packetSize - 1) {
    union packet x;
    // fill union
    for (unsigned char i = 0; i < packetSize; i += 1)
      x.b[i] = Serial.read();
    Serial.print((int) x.p); 
    Serial.print(":"); 
    Serial.println((int) x.i);
    // interpret packet
    switch (x.p) {
    case 1:
      // drive command
      if (x.i > 0) {
        // forward
        digitalWrite(motorA, LOW);
        digitalWrite(motorB, HIGH);
      } else if (x.i < 0) {
        // backward
        digitalWrite(motorA, HIGH);
        digitalWrite(motorB, LOW);
        x.i = -x.i;
      } else {
        // stop
        digitalWrite(motorA, LOW);
        digitalWrite(motorB, LOW);
      }
      ledcWrite(motorChan, x.i);
      break;
    case 2:
      // servo command
      myservo.write(x.i);
      break;
    case 3:
      // servo box
      if(x.i==0){myservo1.write(180);}
      else if(x.i==1){myservo2.write(180);}
      else if(x.i==2){myservo3.write(180);}
      else if(x.i==3){myservo4.write(180);}
      else if(x.i==4){myservo5.write(180);}
      
    }
  }
}
