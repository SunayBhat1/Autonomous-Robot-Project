#include <Servo.h>

Servo myservo;
int pos = 90;
int inc = 1;

const int trigPin = A3;
const int echoPin = A2;

long dist;
bool turn = true;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  myservo.attach(11); 
  myservo.write(pos); 
  digitalWrite(trigPin, LOW);
  delay(1000);
}

void loop() {
  int count = 0;
  
  pos = pos + inc;
  myservo.write(pos); 

  dist = ultra();

  while (dist <=15){
    dist = ultra();
    count++;
    if (count == 3){
      Serial.println("STOP! Object detected");
      delay(500);  
      turn = Avoid();
      if (turn == true) Serial.println("Pulling a Ricky!");
      else Serial.println("Rolling a Larry!");
      delay(1000);
      break;
    }
  }
  Serial.println("loop");
  delay(12);

  if (pos == 135) inc = -1;
  else if (pos == 45) inc = 1;
  
}

long ultra(){
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration*0.034/2;
}

bool Avoid(){
  long plus;
  long sumL = 0;
  long sumR = 0;
  
  pos = 90;
  myservo.write(pos);
  delay(100);
  
  for (pos = 90; pos <= 180; pos++) { 
    myservo.write(pos); 
    plus = ultra();   
    sumR = sumR + plus;         
    delay(15);                  
  }

  pos = 90;
  myservo.write(pos);
  delay(100);
  
  for (pos = 90; pos >= 0; pos--) {
    myservo.write(pos); 
    plus = ultra();   
    sumL = sumL + plus;         
    delay(15);               
  }

  if (sumR < sumL) return true;
  else return false;

  pos = 90;
  
}

