int tempPin = A0;    //LM35 requires an analog pin
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo

const float mFactor = 2.0*2910;  //dividing by this factor converts echo time to converts to meters 
//const float inFactor = 2*74;
const float speedRate = 0.6;
const float speedConst = 331.4;
const float knownD = 0.5;
const float tempAdj = 273.15;
float measuredTemp = 0;

long duration, cm, inches;
 
void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
 
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  //the duration is time to travel known distance and back.
  float dist = 2.0 * knownD;

  //conver duration to seconds;
  float seconds = 1000000. * duration;

  //speed = dist / time ==> dist/ duration
  float sp = dist / seconds;

  //speed = speedConst + speedRate * temp ==> temp = (speed - speedCosnt)/ speedRate;
  float obtainedTemp = (sp - speedConst)/speedRate; 
  
  // Convert the time into a distance
  float m = duration / mFactor;     // Divide by 29.1 or multiply by 0.0343
  //  we don't need some garbage non-SI units inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135

  //get the temperature sensor reading.
  measuredTemp = analogRead(tempPin) *500./1024.;
  
  Serial.print(measuredTemp);
  Serial.println(" is the meassured air temperature");
  Serial.println();
  Serial.print(m);
  Serial.println("m");

  if (((m - knownD) <= 0.05) && ((m - knownD) >= -0.05)){
    Serial.print(obtainedTemp);
    Serial.println(" is the air temperature obtained.");
    Serial.print("They vary by ");
    Serial.print((measuredTemp - obtainedTemp)/measuredTemp*100);
    Serial.println(" %");
    
  }
  else if ((m - knownD) <= -0.05){
    Serial.println("distance too small for accurate measurement, move the sensor farther from the object");
  }

  else {
    Serial.println("distance too large for accurate measurement, move the sensor closer to the object");
  }
  Serial.println();
  
  delay(2000);
}