// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!
char c[20];
int Rcm_avange[6],middle_avange[6],Lcm_avange[6];
int d=0;
int count=0;
int i=0,speed1=60,heanding=100,openmv_speed1=60,openmv_heanding=100;
int Rcm,Lcm,Rcount=0,middlecm;//Rcm左超音波真的距離  Lcmu右超音波真的距離
long RTime,LTime,middleTime;
int change=1,a=0;//找負號change是用來變號的
int find_value=0;
int q=0;
int sum_R=0,sum_L=0;
int sum_middle=0;
void detect()
{
  digitalWrite(14,0);
  delayMicroseconds(2);
  digitalWrite(14,1);
  delayMicroseconds(10);
  digitalWrite(14,0);
  
  RTime = pulseIn(17,1);
  
  digitalWrite(15,0);
  delayMicroseconds(2);
  digitalWrite(15,1);
  delayMicroseconds(10);
  digitalWrite(15,0);
  LTime = pulseIn(18,1);

  digitalWrite(16,0);
  delayMicroseconds(2);
  digitalWrite(16,1);
  delayMicroseconds(10);
  digitalWrite(16,0);
  middleTime = pulseIn(19,1);

  
  Rcm = RTime/58;
  Lcm = LTime/58;
  middlecm = middleTime/58;

  Rcm_avange[count] = Rcm;
  Lcm_avange[count] = Lcm;
  count++;
  if(count==7)
  {
    count=0;
  }
  
  for(int i=0;i<=5;i++)
  {
    sum_R=sum_R+Rcm_avange[i];
    sum_L=sum_L+Lcm_avange[i];
  }
  sum_R = sum_R/6;
  sum_L = sum_L/6;

  middle_avange[count] = middlecm;
  
  for(int i=0;i<=5;i++)
  {
    sum_middle=sum_middle+middle_avange[i];
  }
  sum_middle = sum_middle/6;
}
#include <AFMotor.h>
#include <string.h>
#include <Servo.h> 
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
Servo servo1;//控制轉向

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

  //設定超音波
  pinMode(14,1);//右邊
  pinMode(15,1);//左邊
  pinMode(16,1);//中間
  pinMode(17,0);//右邊
  pinMode(18,0);//左邊
  pinMode(19,0);//中間
  for(int i=0;i<=5;i++)
  {
    middle_avange[i] = 100;
  }
}

void loop() {
  
   detect();
//   if (Serial.available()) 
//   {   
//       
//       c[i] = Serial.read();
//       if(c[i]==10)
//       {
//        openmv_speed1=0;openmv_heanding=0;
//        i=0;
//        change=1;
//        for(int j=0;j<=strlen(c);j++)
//        {
//          if(c[j]=='-')
//          {
//            change=-1;
//          }
//          Serial.println(c[j]);
//        }
//   
//        int w=0;
//         while(c[w]!=',')//找逗號
//         {
//            w++;
//         }
//         for(int k=1;k<w;k++)//逗號前加
//         {
//            openmv_speed1 = openmv_speed1*10+(c[k]-48);
//         } 
//         int r=0;
//         while(c[r]!='E')//找E
//         {
//            r++;
//         }
//         for(int k=w+1;k<r;k++)
//         {
//            openmv_heanding = openmv_heanding*10+(c[k]-48);
//         }
//         Serial.println(openmv_speed1*change);
//         Serial.println(change);
//         Serial.println(openmv_heanding);
//         if(openmv_speed1>0 && openmv_heanding>0)
//         {
//          find_value=1;
//         }
//         else
//         {
//          find_value=0;
//         }
//       }
//       else
//       {
//        i++;
//        
//       }
//    }
    if (change==1)
      {
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        motor3.run(BACKWARD);
        motor4.run(BACKWARD);
      }
    else
      {
        motor1.run(BACKWARD);
        motor2.run(BACKWARD);
        motor3.run(FORWARD);
        motor4.run(FORWARD);
      }
      
    if(find_value==0)
    {
          if(q>=1)
          { 
            
            
            if(sum_middle<80)
            {
              while(sum_middle<130)
              {
                  detect();
                  speed1=65;
                  if(q==2)
                  { 
                    servo1.write(125);
                  }
                  else
                  {
                    servo1.write(75);
                  }
                  motor1.setSpeed(speed1);
                  motor2.setSpeed(speed1);   
                  motor3.setSpeed(speed1);
                  motor4.setSpeed(speed1);
              }
              
            }
            else
            {
            
              speed1=90;
              if(q==2)
              {
                heanding = 140-int(sum_L*1.3)+int((d-sum_L)*600);
                d=sum_L;
              }
              else
              {
                heanding = int(sum_R*1.3)-int((d-sum_R)*600)+65;
                 d=sum_R;
              }
            
              motor1.setSpeed(speed1);
              motor2.setSpeed(speed1);   
              motor3.setSpeed(speed1);
              motor4.setSpeed(speed1);
              if(heanding<=65)
              {
                heanding=65;
              }
              else if(heanding>=125)
              {
                heanding=125;
              }
              servo1.write(heanding);
            }
         
          
          Serial.print("heanding=");
          Serial.println(heanding);
          
          Serial.print("Lcm=");
          Serial.print(Lcm);
          Serial.print("Rcm=");
          Serial.print(Rcm);
          Serial.print("middlecm=");
          Serial.println(middlecm);
          delay(10); 
          }
        else
        {
            if(sum_middle<55)
            {
              if(sum_L>sum_R)
              {
                q=1;
                while(sum_middle<60)
                {
                detect();
                servo1.write(70);
                motor1.setSpeed(speed1);
                motor2.setSpeed(speed1);   
                motor3.setSpeed(speed1);
                motor4.setSpeed(speed1);
                }
                delay(500);
              }
              else
              {
                q=2;
                while(sum_middle<60)
                {
                detect();
                servo1.write(125);
                motor1.setSpeed(speed1);
                motor2.setSpeed(speed1);   
                motor3.setSpeed(speed1);
                motor4.setSpeed(speed1);
                }
                delay(500);
              }
              
             
            }
            else
            {
              servo1.write(100);
              motor1.setSpeed(speed1);
              motor2.setSpeed(speed1);   
              motor3.setSpeed(speed1);
              motor4.setSpeed(speed1);   
            }
        }
    }
    else
    {   
        if(openmv_heanding<=65)
        {
          openmv_heanding=65;
        }
        else if(openmv_heanding>=125)
        {
          openmv_heanding=125;
        }
        motor1.setSpeed(openmv_speed1);
        motor2.setSpeed(openmv_speed1);   
        motor3.setSpeed(openmv_speed1);
        motor4.setSpeed(openmv_speed1); 
        servo1.write(openmv_heanding);
    }
    
    
    
}
