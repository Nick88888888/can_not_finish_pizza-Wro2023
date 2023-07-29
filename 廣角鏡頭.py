# Single Color RGB565 Blob Tracking Example
#
# This example shows off single color RGB565 tracking using the OpenMV Cam.

import sensor, image, time, math
from pyb import UART
from pyb import LED
uart = UART(3, 19200)
threshold_index = 0 # 0 for red, 1 for green, 2 for blue


thresholds_green = [(79, 60, -82, -40, -10, 34)] #綠色的值
thresholds_orange = [(55, 26, 17, 114, -23, 121)]#橘色的值
thresholds_black = [(0, 11, -85, 120, -14, 121)]#黑色的值
thresholds_red = [(86, 46, 92, 8, -54, 80)]#紅色的值
thresholds_blue = [(58, 0, 112, -59, -125, -14)]#藍色的值
LED(1).on()
LED(1).on()
LED(2).on()
LED(3).on()
sensor.reset()
sensor.set_vflip(True)
sensor.set_hmirror(True)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)#320x240 
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()
black_arrary_m= [0] * 20
count=0#移動平均要用的
x=0

while(True):
    y = None
    while(y is None):
        Lift_max_x=0
        Lift_max_y=0
        Right_max_x=0
        Right_max_y=0
        Lift_distance=0
        Right_distance=0
        a=0#if see orange line a==1
        angle=0
        left_area=0
        black_m_average=0#移動平均值
        sum1=0
        right_area=0
        x_left_black_value=0
        y_left_black_value=0
        x_right_black_value=0
        y_right_black_value=0
        black_wall_m=0#黑色牆壁的斜率
        clock.tick()
        img = sensor.snapshot()
        for blob in img.find_blobs([thresholds_orange[0]], pixels_threshold=200, area_threshold=200, merge=True):#找橘色線 
            if blob.elongation() > 0.5:
                img.draw_edges(blob.min_corners(), color=(255,0,0))
                img.draw_line(blob.major_axis_line(), color=(0,255,0))
                img.draw_line(blob.minor_axis_line(), color=(0,0,255))
       
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy()) 
            img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
            distance=int(210000/blob.area())
            angle = blob.rotation_deg()
            if(angle>90):
                angle = angle-180
            else:
                angle-4
        for blob in img.find_blobs([thresholds_blue[0]], pixels_threshold=200, area_threshold=200, merge=True):#找藍色線 
  
        
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy()) 
            img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
            distance=int(210000/blob.area())
            angle = blob.rotation_deg()
            if(angle>90):
                angle = angle-180
            else:
                angle-4
                
        img = sensor.snapshot()
        line = img.get_regression([thresholds_black[0]], robust = True)
        if (line):
            rho_err = abs(line.rho())-img.width()/2
            if line.theta()>90:
                theta_err = line.theta()-180
            else:
                theta_err = line.theta()
            img.draw_line(line.line(), color = 127)
            print(rho_err,line.magnitude(),rho_err)
        for blob in img.find_blobs([thresholds_black[0]], pixels_threshold=400, area_threshold=400, merge=True,roi=(100,0,30,240)):#找左黑色牆壁底下的線
          
                
            if blob.cy() > Lift_max_y:
                Lift_max_x = blob.cx()
                Lift_max_y = blob.cy()
            if(blob.corners()[2][1]>=130):
                y=1
            left_area = blob.area()
            Lift_distance = 127200/blob.area()
                    
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy()) 
        img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
        x_left_black_value =  Lift_max_x
        y_left_black_value =  Lift_max_y
            
            
            
            
            #print( x_left_black_value ,y_left_black_value)
            
        for blob in img.find_blobs([thresholds_black[0]], pixels_threshold=300, area_threshold=300, merge=True,roi=(220,0,30,240)):#找右黑色牆壁底下的線
       
            if blob.cy() > Right_max_y:
                Right_max_x = blob.cx()
                Right_max_y = blob.cy()
            if(blob.corners()[2][1]>=130):
                y=1
            right_area = blob.area()  
            Right_distance= 127200/blob.area()    
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy()) 
        x_right_black_value = Right_max_x
        y_right_black_value = Right_max_y
        img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
            #print("blob.corners()[2][1]=",blob.corners()[2][1])
            
            
        if(x_left_black_value>0 and  y_left_black_value>0 and x_right_black_value>0 and y_right_black_value>0):   
            img.draw_line((x_left_black_value,y_left_black_value,x_right_black_value,y_right_black_value),color=(0,255,0))
            #print(y_left_black_value ,y_right_black_value,x_left_black_value ,x_right_black_value)
            sum1 = (left_area+right_area)/10000
            black_wall_m = (y_left_black_value - y_right_black_value) /(x_left_black_value - x_right_black_value)*-250#斜率公式
            #print("sum1=",sum1)
            #print("斜率=",int(black_wall_m*sum1))

        black_arrary_m[count]=black_wall_m*sum1
        if count==19:
            count=0
        else:
            count+=1
        for i in range(20):
            black_m_average=black_m_average+black_arrary_m[i]
        black_m_average = black_m_average/20
        print("black_arrary_m=",black_arrary_m)
        print("black_m_average",black_m_average)
        print("left_area+right_area=",(left_area+right_area)/1000)
        print("左邊的距離=",Lift_distance)
        print("右邊的距離",Right_distance)
        if(y==1):
            if(sum1<=1.5):
                y=None
        elif((Lift_distance+Right_distance)/2<=40):
            uart.write(str('+')+str(100)+","+ str(100+int(angle))+"E\n")
            #print("B")
        else:
            uart.write(str('+')+str(0)+","+ str(100+int(black_wall_m*sum1))+"E\n")
            print("ok")
        time.sleep_ms(20)
        #print("1")



    for i in range(5):#撞黑牆
        uart.write(str('-')+str(60)+","+ str(100)+"E\n")
        time.sleep_ms(20)
    time.sleep_ms(1500)
    for i in range(5):
        uart.write(str('+')+str(60)+","+ str(120)+"E\n")
        time.sleep_ms(20)
    time.sleep_ms(1000)
    for i in range(5):
        uart.write(str('+')+str(0)+","+ str(100)+"E\n")
        time.sleep_ms(20)
    time.sleep_ms(100)
