// Program for a pick and place robot
// which is searching for an object using an ultrasonic sensor in a defined area
// and picking up this object and placing it in a defined area

#include <Servo.h>

// defined pins
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
int LEDblue=9;
int LEDgreen=10;
int LEDred=11;
int trigPin=6;
int echoPin=7;
int photocellPin=0;

//  input of the keyboard
char input_1;
char input_2;

//  measured distance
float distance;
float distance_;

int x=0;  // steps of the angles
int object=0;  // found objects

// Arrays of the measurements
float angleArrays[100];
float distanceArrays[100];
float testArrays[10];

// height of the robot
float height=5.5;
// length of the robot arms
float length_1=13.2;
float length_2=15.2;

float r;  // needed of the calculation of the angles

// angles of the arms
float alpha_2;
float angle_1=165;
int angle_4=100;
float alpha=124;
float alpha_=0;

// angles of the arms
float beta=124;
float beta_=0;

//  needed for the photodiode
int photoBefore;
int photoAfter;
int photocellReading;

void setup()
{
  // Definition of the pins
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);
  pinMode(trigPin, OUTPUT);
  pinMode(LEDblue, OUTPUT);
  pinMode(LEDgreen, OUTPUT);
  pinMode(LEDred, OUTPUT);

  Serial.begin(9600);
  // x represents the different steps (angles) which are getting scanned
  for (x; x<100; x++)
  {
    angleArrays[x]=0;
    distanceArrays[x]=0;
  }
  for (int p=0; p<10; p++)
  {
    testArrays[p]=0;
  }
  x=0;
  servo1.write(angle_1);
  delay(500);
  servo2.write(alpha);
  delay(500);
  servo3.write(beta);
  delay(500);
  // Starting postion of the servos
  servo4.write(angle_4);  
  red();
  // Waiting for entering '1' to start the program
  Serial.println("Enter 1 to start");
  while (Serial.available()==0)
  {
  }
  input_1=Serial.read();
  while (input_1!='1')
  {
  }
}

void loop() 
{
  // picking up three object in a row and waiting for the input '2' to repeat
  if (object>=3) 
  {
    red();
    object=0;
    Serial.println("Enter 2 to restart");
    while (Serial.available()==0)
    {
    }
    input_2=Serial.read();
    while (input_2!='2')
    {
    }
    delay(500);
  }
  blue();
  servo1.write(angle_1);
  Serial.print("Current angle: ");  // prints the current angle
  Serial.println(angle_1);
  delay(50);
  //  starts the scanning process
  ultrasonic();  // measures the distance
  Serial.print("distance: ");
  Serial.print((distance+5));  // correction of the distance
  Serial.println(" cm");
  //  object should be within a range of 22 cm
  if (distance<=22.00)
  {
    Serial.println("                               Object found!!!");
    Serial.println("                    Data are getting stored!!!");
    angleArrays[x]=angle_1;  // stores the measured angle in an array
    distanceArrays[x]=distance;  // stores the measured distance in an array
    x++;
  }
  angle_1=angle_1-0.5;  // correction of the angle
  if (angle_1<53)
  {
    if (x>=5)
    {
      if (x%2==1)  // odd number
      {
        //  starts the picking process of the object
        x=x/2;
        angle_1=angleArrays[x]+8.5;  // correction
        distance=distanceArrays[x]+4;  // correction
        servo1.write(angle_1);
        x=0;
        delay(300);
        Serial.print("                        distance: ");
        Serial.print((distance+1));  // correction of the distance
        Serial.print(" cm ");
        Serial.print("angle: ");
        Serial.println(angle_1);
        Serial.println("                                   Start picking");
        Serial.println("                                Processing......");
        pick();
        for (int p=0; p<10; p++)
        {
          ultrasonic_();
          delay(50);
          testArrays[p]=distance_;
          delay(15);
        }
        distance_=0;
        for (int p=0; p<10; p++)
        {
          distance_=distance_+testArrays[p];
        }
        distance_=distance_/10;
        if (distance_>22)
        {
          Serial.println("                                   Place started");
          Serial.println("                                   Waiting......");
          photoMeasurementBefore();
          place();
          photoMeasurementAfter();
          if (photoBefore>(photoAfter+500))
          {
            green();
            object++;
          }
          else
          {
            red();
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("                                   Object lost");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
          }
        }
        else if (distance_<=22)
        {
          red();
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("                                   Object lost");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
        }
        endPosition();
      }
      else if (x%2==0)  // even number
      {
        x=x/2;
        angle_1=angleArrays[x]+8;
        distance=distanceArrays[x]+4;
        servo1.write(angle_1);
        x=0;
        delay(300);
        Serial.print("                        distance: ");
        Serial.print((distance+1));
        Serial.print(" cm ");
        Serial.print("angle: ");
        Serial.print(angle_1);
        Serial.println(" degree");
        Serial.println("                                 Picking started");
        Serial.println("                                   Waiting......");
        pick();
        for (int p=0; p<10; p++)
        {
          ultrasonic_();
          delay(50);
          testArrays[p]=distance_;
          delay(15);
        }
        distance_=0;
        for (int p=0; p<10; p++)
        {
          distance_=distance_+testArrays[p];
        }
        distance_=distance_/10;
        if (distance_>22)
        {
          Serial.println("                                 Placing started");
          Serial.println("                                   Waiting......");
          photoMeasurementBefore();
          place();
          photoMeasurementAfter();
          if (photoBefore>(photoAfter+500))
          {
            green();
            object++;
          }
          else
          {
            red();
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("                                   Object lost");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
            Serial.println("--------------------------------------------------------------------------");
          }
        }
        else if (distance_<=22)
        {
          red();
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("                                   Object lost");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
          Serial.println("--------------------------------------------------------------------------");
        }
      }
      endPosition();
    }
    else if (x<5)
    {
      angle_1=165;
      servo1.write(angle_1);
      delay(800);
    }
  }
}

float ultrasonic()
{
  // measures the distance
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  distance=pulseIn(echoPin,HIGH)/58.00;
  return distance;
  delay(15);
}

float ultrasonic_()
{
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  distance_=pulseIn(echoPin,HIGH)/58.00;
  return distance_;
  delay(15);
}

void pick()
{
  // picking up the object from the measured distance and angle
  // by calulating the needed angles of the servos and controlling them
  r=sqrt(pow(height,2)+pow(distance,2));
  alpha_1=(acos((pow(length_1,2)+pow(r,2)-pow(length_2,2))/(2*length_1*r)))*5729577/100000;
  alpha_2=(acos(height/r))*5729577/100000;
  alpha_=alpha_1+alpha_2;
  beta_=(acos((pow(length_1,2)+pow(length_2,2)-pow(r,2))/(2*length_1*length_2)))*5729577/100000;
  alpha_=180-alpha_-2;
  beta_=180-beta_-15;
  Serial.print("alpha: ");
  Serial.print(alpha_);
  Serial.println(" degree");
  Serial.print("beta: ");
  Serial.print(beta_);
  Serial.println(" degree");
  for (int j=0; j<=beta_; j++)
  {
    beta=124-j;
    servo3.write(beta);
    delay(5);
  }    
  delay(300);
  if (alpha_<0)
  {
    for (int i=0; i<=abs(alpha_); i++)
    {
      alpha=124+i;
      servo2.write(alpha);
      delay(25);
    }
  }
  else if (alpha_>=0)
  {
    for (int i=0; i<=alpha_; i++)
    {
      alpha=124-i;
      servo2.write(alpha);
      delay(25);
    }      
  }
  delay(500);
  for (int k=0; k<=60; k++)
  {
    angle_4=angle_4-1;
    servo4.write(angle_4);
    delay(15);
  }
  delay(300);
  if (alpha_<0)
  {
    for (int i=0; i<=(26+alpha_); i++)
    {
      alpha++;
      servo2.write(alpha);
      delay(10);
    }
  }
  else if (alpha_>=0)
  {
    for (int i=0; i<=(26+alpha_); i++)
    {
      alpha++;
      servo2.write(alpha);
      delay(10);
    }
  }
  delay(300);
  if ((beta_-54)<0)
  {
    for (int j=0; j<=(54-beta_); j++)
    {
      beta--;
      servo3.write(beta);
      delay(10);
    }
  }
  else if ((beta_-54)>=0)
  {
    for (int j=0; j<=(beta_-54); j++)
    {
      beta++;
      servo3.write(beta);
      delay(10);
    }
  }
  delay(300);
}

void place()
{
  // placing the object in the vessel
  for (int h=angle_1-33; h>=0; h--)
  {
    angle_1--;
    servo1.write(angle_1);
    delay(10);    
  }
  delay(300);
  for (int i=0; i<=70; i++)
  {
    alpha--;
    servo2.write(alpha);
    delay(10);
  }
  delay(300);
  for (int k=0; k<=60; k++)
  {
    angle_4=angle_4+1;
    servo4.write(angle_4);
    delay(10);
  }
}

void endPosition()
{
  // end position of the robot after placing the object
  angle_1=165;
  angle_4=100;
  alpha=124;
  beta=124;
  servo4.write(angle_4);
  delay(500);
  servo2.write(alpha);
  delay(500);
  servo3.write(beta);
  delay(500);
  servo1.write(angle_1);
  delay(500);
}

void photoMeasurementBefore()
{
  // measures the voltage of the photodiode before placing the object in the vessel 
  // to evaluate the success of placing the object in the vessel 
  photoBefore=0;
  for (int w=0; w<=3; w++)
  {
    photoBefore=analogRead(photocellPin)+photoBefore;      
    delay(50);
  }
}

void photoMeasurementAfter()
{
  // measures the voltage of the photodiode after placing the object in the vessel
  // to evaluate the success of placing the object in the vessel 
  photoAfter=0;
  for (int w=0; w<=3; w++)
  {
    photoAfter=analogRead(photocellPin)+photoAfter;      
    delay(50);
  }
}

void blue()
{
  // Defined color of the LED
  analogWrite(LEDred, 0);
  analogWrite(LEDgreen, 0);
  analogWrite(LEDblue, 255);
}

void red()
{
  // Defined color of the LED
  analogWrite(LEDred, 255);
  analogWrite(LEDgreen, 0);
  analogWrite(LEDblue, 0);
}

void green()
{
  // Defined color of the LED
  analogWrite(LEDred, 0);
  analogWrite(LEDgreen, 255);
  analogWrite(LEDblue, 0);
}