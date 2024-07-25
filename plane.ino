/*
  This is the V-TAIL-PID code.
  Copyright (c) 2024 Marco Caneva. All rights reserved.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  he Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

  For inquiries, please contact:
  Marco Caneva
  Email:  marco.caneva@example.com
  Mail:   Marco Caneva
          Via Dei Ciclamini 33
          20147 Milan (MI)
          Italy
*/


#include <Servo.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <QuickPID.h>

//#include <TinyGPS.h>
//TinyGPS gps;
//SoftwareSerial ss(4, 3); // pin 4=RX, pin 3=TX
float x, y, z, pitch, roll, setPitch, setRoll, setHeight;
float Kp = 0.5, Ki = 0.2, Kd = 0.3;



Servo myservo_R;
Servo myservo_L;

Adafruit_MPU6050 mpu;
QuickPID pitchPID(&y, &pitch, &setPitch);
QuickPID rollPID(&x, &roll, &setRoll);

QuickPID heightPID(&z, &setPitch, &setHeight);

sensors_event_t a, g, temp;


void setup() {

  Serial.begin(9600);

  
  /* Probabilente questo snippet va messo in una funzione temporizzata o legato a un task.
  while (ss.available() > 0)
  {
    gps.encode(ss.read);
  }*/

  y = 0;
  setPitch = 0;
  x = 0;
  setRoll = 0;
  //z = gps.altitude.meters();
  z = 0; // SOSTITUIVA IN ASSENZA DI MODULO GPS
  setHeight = z+10;
  rollPID.SetTunings(Kp, Ki, Kd);
  rollPID.SetMode(rollPID.Control::automatic);
  rollPID.SetOutputLimits(-35,35);
  pitchPID.SetTunings(Kp*3, Ki*2, Kd*2);
  pitchPID.SetMode(pitchPID.Control::automatic);
  pitchPID.SetOutputLimits(-30,30);

  heightPID.SetTunings(Kp, Ki, Kd);
  heightPID.SetMode(rollPID.Control::automatic);
  heightPID.SetOutputLimits(-10,10);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  myservo_R.attach(5); // right aileron (left servo)
  myservo_L.attach(3); // Left aileron (right servo)

  

  delay(100);
  
}

void loop() 
{
  mpu.getEvent(&a, &g, &temp);
   x  = a.orientation.x * 10;
   y  = a.orientation.y * 10;
   
  /*
  while (ss.available() > 0)
  {
    gps.encode(ss.read);
  }
  */

  //z = gps.altitude.meters();

  if (z < setHeight) // SOSTITUTIVA IN ATTESA DI MODULO GPS
  {
    z+= 0.3;
  }
  else
  {
    z-= 1.0 ;
  }

  heightPID.Compute();
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

  Serial.print(z);
  Serial.println(setPitch);
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