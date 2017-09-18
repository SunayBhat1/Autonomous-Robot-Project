/*
 * Mini Smart Car Project
 * 
 * By: Sunay Bhat
 * Created 24 August 2017
 * 
 * This code is used for controlling an Arduino based smart car. The hardware utilized for this
 * this project includes:
 *   -Arduino UNO
 *   -L298 Motor Driver Module
 *   -Emgreat® Motor Robot Car Chassis Kit (with two DC gear motors)
 *   -HC-SR04 Ultrasonic Senor
 *   -SG90 TowerPro Micro-Servo
 *   -18V Battery Source (made by combining two 9V Alkaline batteries in series)
 */
 
/*MIT License

Copyright (c) [2017] [Sunay Bhat]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Servo.h>

//Initialize servo object, set pin (See Servo example for functionality details)
Servo myservo;
int pos = 90;
int inc = 1;
const int servoPin = A5;

//Motor Right Pins
const int motorR1  = 3 ;  // Pin 'IN4' of L293
const int motorR2  = 5;  // Pin 'IN3' of L293

//Motor Left Pins
const int motorL1  = 11; // Pin 'IN1' of L293
const int motorL2  = 6;  // Pin 'IN2' of L293

//Ultrasonic Sensor Pins
const int trigPin = A3;
const int echoPin = A2;

long dist;
int turn = 0;

void setup(){
  //Init serial for debugging
  Serial.begin(115200);
  Serial.println("Hello!");
  
  myservo.attach(servoPin);
  myservo.write(pos); 
  
  //Set motor control/ultrasonic pins as inputs/outputs. See L298 Tutorial.pdf for pin map.
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Init ultrasonic pin for readings
  digitalWrite(trigPin, LOW);
  delay(5000);

  //Move robot forward, function call
  Forward();
}


void loop(){
  //reset loop count var
  int count = 0;

  //Increment servo sweep scan while moving
  pos = pos + inc;
  myservo.write(pos);

  //Get ultrasonic reading
  dist = Ultra();

  //Object avoidance loop is object detected within 20cm. Stay in loop till avoided
  while (dist <= 20){
    
    //Stop robot
    Stop();
    Serial.println(dist);
    dist = Ultra(); //Obtain distance again confirm. Failsafe against faulty readings. 
    count++;

    //Once 30 bad readings detected consecutively, assume object detection.
    if (count == 30){
      Serial.println("Object Detected!");
      Stop();
      delay(500);
      Back(200); //Reverse
      turn = Avoid(); //Call object avoid function, returns direction to turn/go.
      if (turn == 1) TurnR(200); 
      else if (turn == 2) TurnL(200);
      else if (turn == 3) TurnR(500); //180 degree turn
      delay(1000);   
      Forward();  //Continue moving
    }
    
    Forward();  //Continue moving
  }

  delay(50);

  //Increment servo
  if (pos == 115) inc = -5;
  else if (pos == 65) inc = 5;
}

//Function below for moving and turning. Might need to modify based on individual experimentation.
//Note: Will behave differently as battery depletes. 
void Back(int x){
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 50);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 50);
  delay(x);
  Stop();
}

void Forward(){
  analogWrite(motorR1, 50);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 50);
  analogWrite(motorL2, 0);
}

void TurnR(int x){
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 65);
  analogWrite(motorL1, 65);
  analogWrite(motorL2, 0);
  delay(x);
  Stop();
}

void TurnL(int x){
  analogWrite(motorR1, 65);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 65);
  delay(x);
  Stop();
}

void Stop(){
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 0);
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0);
}

//Ultrasonic reading acquisition. See HC-SR04 documentation for details. 
long Ultra(){
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return (duration*0.034/2);
}

/*Object avoidance function. Sweeps both left and right 90 degrees summing all readings under
 * 100cm. Whichever side has lower sum, turn in that direction. If both sums are too high
 * (greater than 4000), then turn around. Otherwise continue straight (will presumabely trigger 
 * loop again if worng decision). 
 */
int Avoid(){
  long plus;
  long sumL = 0;
  long sumR = 0;
  
  pos = 90;
  myservo.write(pos);
  delay(100);
  
  for (pos = 90; pos <= 180; pos++) { 
    myservo.write(pos); 
    plus = Ultra();   
    if (plus < 100) sumR = sumR + plus;         
    delay(15);                  
  }

  pos = 90;
  myservo.write(pos);
  delay(100);
  
  for (pos = 90; pos >= 0; pos--) {
    myservo.write(pos); 
    plus = Ultra();  
    if (plus < 100) sumL = sumL + plus;         
    delay(15);               
  }

  pos = 90;
  myservo.write(pos);
  delay(100);

  long diff = sumR - sumL;

  if (diff < -300 ) return 1;
  else if (diff > 300) return 2;
  else{
    if (sumL > 4000 && sumR > 4000) return 3;
    else return 0;
  }
}

