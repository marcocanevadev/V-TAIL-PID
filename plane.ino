
#include <Servo.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <QuickPID.h>

float x, y, z, pitch, roll, setPitch, setRoll;
float Kp = 0.5, Ki = 0.2, Kd = 0.3;



Servo myservo_R;
Servo myservo_L;

Adafruit_MPU6050 mpu;
QuickPID pitchPID(&y, &pitch, &setPitch);
QuickPID rollPID(&x, &roll, &setRoll);


sensors_event_t a, g, temp;


void setup() {

  Serial.begin(9600);

  y = 0;
  setPitch = 0;
  x = 0;
  setRoll = 0;
  rollPID.SetTunings(Kp, Ki, Kd);
  rollPID.SetMode(rollPID.Control::automatic);
  rollPID.SetOutputLimits(-35,35);
  pitchPID.SetTunings(Kp*3, Ki*2, Kd*2);
  pitchPID.SetMode(pitchPID.Control::automatic);
  pitchPID.SetOutputLimits(-30,30);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  myservo_R.attach(5);
  myservo_L.attach(3);

  delay(100);
  
}

void loop() 
{
  mpu.getEvent(&a, &g, &temp);
   x  = a.orientation.x * 10;
   y  = a.orientation.y * 10;
   //z  = a.orientation.z * 10;



  pitchPID.Compute();
  rollPID.Compute();
  int angleR = 45-pitch;
  int angleL = 45+pitch;

    angleR = angleR+roll;
    angleL = angleL+roll;
  
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
  /*
  Serial.print("y: ");
  Serial.print(y);
  Serial.print(" pitch: ");
  Serial.print(pitch);
  Serial.print("     angle_R: ");
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" roll: ");
  Serial.print(roll);
  Serial.print("     angle_R: ");
  Serial.print(angleR);
  Serial.print(" angle_L: ");
  Serial.println(90-angleL);
*/

  
}