#include <Servo.h>
/////////////////  pins
int calPin = 8; //connect 100k resistor and switch to this pin 
int valPin = A7; //  connect photo resistor and pull down 10k resistor to this  pin
int errorPin = 6; // connect led to indicate you have exceed the maximum or minimum expected values    
/////////////////
///////////////////////// store values
int mode; // mode stores the position of the switch.  The ON position puts you in calibration mode
int val;// val stores the current reading from the 
int dialSet;// dialSet stores the value used to set the position on the gauge dial
int minVal = 1023;// this is minimum value expected
int maxVal = 0;// this is the maximum value expected 
/////////////////////////////////

Servo dial; //this makes a servo object named ‘dial’

void setup() {
  Serial.begin(115200);
/////////////////////////////////// establish witch pins are used for the servo,sensor,switch and error light    
  pinMode(valPin, INPUT);//sensor 
  pinMode(calPin, INPUT);//switch
  pinMode(errorPin, OUTPUT);//led
  dial.attach(9);// This is the pin used to signal the servo (yellow wire)
///////////////////////////////////////////////////
  
}

void loop() {

  mode = digitalRead(calPin);// reads a 1 or 0 from the switch and runs the cal () function if mode=1
 // Serial.print("mode:");
 // Serial.println(mode);

  if (mode == HIGH) 
{
    cal();//calls the calibration function 
  }
  else
  {
gauge();//this function moves the gauge   
  }
errorCheck();// indicate if sensor readings are outside the expected range    
  }


////////////////// user defined functions 
/* the cal() function will read the sensor and store the
maximum and minimum values while the switch is in the ON position*/   
void cal() 
{
  Serial.println("calibration Mode");
  delay(1000);
  val = analogRead(valPin);
  if (val < minVal) 
{
    minVal = val;
  }

  if (val > maxVal) {
    maxVal = val;
  }
  Serial.print("min: ");
  Serial.println(minVal);
  Serial.print("max: ");
  Serial.println(maxVal);
}

void errorCheck()// check if the value read from the senor is outside the expected range
{
  if (dialSet < 0 || dialSet > 180) // if the sensor reading outside the expected values turn the LED on
{
    analogWrite(errorPin, 100);
  }
    else// if the sensor readings are within the expected range turn the LED off
    {
      analogWrite(errorPin, 0);
    }
}
void gauge()//read the sensor and move the dail
{

   dialSet = map(analogRead(valPin), minVal, maxVal, 180, 0);// scale the sensor reading from 180-0
   // Serial.print("setting: ");
   // Serial.println(dialSet);
    delay(50);
    dial.write(dialSet);// move the servo to the appropriate position  
}
