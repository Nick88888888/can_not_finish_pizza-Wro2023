// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!
char c[15];
int i=0,speed1,heanding=100;
#include <AFMotor.h>
#include <string.h>
#include <Servo.h> 
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
Servo servo1;
void setup() {
  Serial.begin(19200);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");
  servo1.attach(9);
  // turn on motor
  motor1.setSpeed(200);
  motor1.run(RELEASE);
  motor2.setSpeed(200);
  motor2.run(RELEASE);
  motor3.setSpeed(200);
  motor3.run(RELEASE);
  motor4.setSpeed(200);
  motor4.run(RELEASE); 
  servo1.write(100);
}

void loop() {
   
   if (Serial.available()) 
   {   
       
       c[i] = Serial.read();
       if(c[i]==10)
       {
        speed1=0;heanding=0;
        i=0;
        for(int j=0;j<=strlen(c);j++)
        {
          Serial.println(c[j]);
        }
        int w=0;
         while(c[w]!=',')//找逗號
         {
            w++;
         }
         for(int k=0;k<w;k++)//逗號前加
         {
            speed1 = speed1*10+(c[k]-48);
         }
         int r=0;
         while(c[r]!='E')//找E
         {
            r++;
         }
         for(int k=w+1;k<r;k++)
         {
            heanding = heanding*10+(c[k]-48);
         }
         Serial.println(speed1);
         Serial.println(heanding);
       }
       else
       {
        i++;
       }
   }
   
    uint8_t i;
    //Serial.print("tick");
    
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);
    
    motor1.setSpeed(speed1);
    motor2.setSpeed(speed1);   
    motor3.setSpeed(speed1);
    motor4.setSpeed(speed1);      
    
    if(heanding<=70)
    {
      heanding=70;
    }
    else if(heanding>=130)
    {
      heanding=130;
    }
    servo1.write(heanding);
//    delay(2000);
//    servo1.write(85);
//    delay(1000);
//    servo1.write(100);
//    delay(2000);
//    servo1.write(115);
//    delay(1000);
//    servo1.write(100);
//    delay(2000);
}
