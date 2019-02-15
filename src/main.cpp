#include <Arduino.h>
#include <USsensor.h>
#include <IRsensor.h>
#include <QueueArray.h>
#include <RunningAverage.h>

//Project Library Main
//15/02/2019.


//Pin Layout
//const int irsense = A0;
//const int trigPin =3;
//const int echoPin =2;


//Running Average Library, used for Buffer implementation.
RunningAverage myRA_a(10);
RunningAverage myRA_b(10);
RunningAverage myRA_a1(10);
RunningAverage myRA_b1(10);
uint32_t samples = 0;
uint32_t samples2 = 0;
 
USsensor US1(11, 12);     //Instantiate USsensor class, define pin numbers.
IRsensor IR1(A0);       //Instantiate IRsensor class, define pin numbers.

volatile bool ReadSensorsFlag;
int WarningInterruptpin = 2; 

float AvrUS; 
float AvrIR;
float Threshold;



void setup() {
  pinMode(WarningInterruptpin, OUTPUT);
  digitalWrite(WarningInterruptpin, LOW);
  attachInterrupt(digitalPinToInterrupt(2), UserWarning ,HIGH);
  
  Serial.begin(9600);
  myRA_a.clear();
  myRA_b.clear();
  myRA_a1.clear();
  myRA_b1.clear();
}


void loop() 
{

  if(AvrIR > Threshold || AvrUS > Threshold){
    //Get Time
    //If values are greater than Threshold for more than X amount of time trigger the ISR.
    //for loop?
    digitalWrite(WarningInterruptpin, HIGH);
  }




  if(ReadSensorsFlag == true){
    AvrUS = myRA_a1.getAverage();
    AvrIR  = myRA_b1.getAverage();
    FirstQueue(); 
  }
  else{
    AvrUS = myRA_a.getAverage();
    AvrIR  = myRA_b.getAverage();
    SecondQueue();
  }
}






void FirstQueue(){

  myRA_a.addValue(US1.Tread());
  myRA_b.addValue(IR1.readIR());
  samples++;

  if(samples == 20){
    samples = 0;
    myRA_a.clear();
    myRA_b.clear();

    ReadSensorsFlag = false;
  }

}
void SecondQueue(){

  myRA_a1.addValue(US1.Tread());
  myRA_b1.addValue(IR1.readIR());
  samples2++;

  if (samples2 == 20){
    samples2 = 0;
    myRA_a1.clear();
    myRA_b1.clear();

    ReadSensorsFlag = true;
  }

}


void UserWarning(){
  noInterrupts();
  //Method to warn user.
  //ISR for Digital Pin 2.
}