// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!
char c[15];
int i=0,speed1,heanding=100;
int Rcm,Lcm,Rcount=0;//Rcm左超音波真的距離  Lcmu右超音波真的距離
long RTime,LTime;
void detect()
{
  digitalWrite(14,0);
  delayMicroseconds(2);
  digitalWrite(14,1);
  delayMicroseconds(10);
  digitalWrite(14,0);
  
  RTime = pulseIn(15,1);
  delayMicroseconds(2);
  digitalWrite(14,1);
  delayMicroseconds(10);
  digitalWrite(14,0);
  LTime = pulseIn(16,1);
  Rcm = RTime/58;
  Lcm = LTime/58;
}
#include <AFMotor.h>
#include <string.h>
#include <Servo.h> 
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
Servo servo1;//控制轉向
Servo servo2;//控制鏡頭
void setup() {
  Serial.begin(19200);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");
  servo1.attach(9);
  servo2.attach(10);
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
  servo2.write(60);

  pinMode(14,1);//設定超音波
  pinMode(15,0);//右邊
  pinMode(16,0);//左邊
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
    detect();
    Serial.print("Lcm=");
    Serial.print(Lcm);
    Serial.print("Rcm=");
    Serial.println(Rcm);
}
