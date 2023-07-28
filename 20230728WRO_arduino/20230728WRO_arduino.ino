// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!
char c[20];
int i=0,speed1,heanding=100,m=0;
#include <AFMotor.h>
#include <string.h>
#include <Servo.h> 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED 寬度像素
#define SCREEN_HEIGHT 64 // OLED 高度像素
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
Servo servo1;//控制轉向
Servo servo2;//控制鏡頭
void testdrawstyles(void) {
  display.clearDisplay();
  display.setTextSize(2);             // 設定文字大小
  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  display.setCursor(0,0);             // 設定起始座標
  display.print("start");        // 要顯示的字串
  display.display();                  // 要有這行才會把文字顯示出來
  delay(100);
}
void oled(int k){
  display.clearDisplay();
  display.setTextSize(2);             // 設定文字大小
  display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  display.setCursor(0,0);             // 設定起始座標
  display.print("m=");        // 要顯示的字串
  display.print(k);
  display.display();                  // 要有這行才會把文字顯示出來
  
}
void setup() {
  Serial.begin(19200);           // set up Serial library at 9600 bps
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 一般1306 OLED的位址都是0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // 顯示Adafruit的LOGO，算是開機畫面
  display.display();
  delay(500); 
  // 清除畫面
  display.clearDisplay();
  testdrawstyles();    // 測試文字
  
  Serial.println("Motor test!");
  servo1.attach(9);
  servo2.attach(10);// turn on motor
  motor1.setSpeed(200);
  motor1.run(RELEASE);
  motor2.setSpeed(200);
  motor2.run(RELEASE);
  motor3.setSpeed(200);
  motor3.run(RELEASE);
  motor4.setSpeed(200);
  motor4.run(RELEASE); 
  servo1.write(100);
  servo2.write(58);
}

void loop() {

   if (Serial.available()) 
   {   
       m=0;
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
         while(c[r]!='.')//找.
         {
            r++;
         }
         for(int k=w+1;k<r;k++)
         {
            heanding = heanding*10+(c[k]-48);
         }
         int y=0,p=0,change=1;
         while(c[y]!='E')//找E
         {
           if(c[y]=='-')
           {
            p=1;
           }
           y++;
         }
         for(int k=r+1+p;k<y;k++)
         {
           m=m*10+(c[k]-48);
         }
         if (p==1)
         {
          change=-1;
         }
         m=m*change;
         Serial.println(speed1);
         Serial.println(heanding);
         Serial.println(m);
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
    display.clearDisplay();
    oled(m);    // 測試文字

}
