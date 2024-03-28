
#include <Servo.h>

Servo myservo_R;
Servo myservo_L;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  myservo_R.attach(5);
  myservo_L.attach(3);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin A0:
  int analogValue1 = analogRead(A0);
  int analogValue2 = analogRead(A1);

  int angle = map(analogValue1, 0,1023, -30, 30);

  int angle3 = map(analogValue2, 0, 1023, -30, 30);

  int angleR = 45+angle;
  int angleL = 45-angle;
  angleR = angleR+angle3;
  angleL = angleL+angle3;

  if (angleR > 90){
    angleR = 90;
  }
  if (angleL >90){
    angleL = 90;
  }
  if (angleR <0 ){
    angleR = 0;
  }
  if (angleL <0 ){
    angleL = 0;
  }

  myservo_R.write(angleR);
  myservo_L.write(angleL);
  // print 
  Serial.print("POT1: ");
  Serial.print(analogValue1);
  Serial.print(" POT2: ");
  Serial.print(analogValue2);
  Serial.print("     angle_R: ");
  Serial.print(angleR);
  Serial.print(" angle_L: ");
  Serial.println(angleL);


  
}