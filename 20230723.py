# Single Color RGB565 Blob Tracking Example
#
# This example shows off single color RGB565 tracking using the OpenMV Cam.

import sensor, image, time, math
from pyb import UART
from pyb import LED
uart = UART(3, 19200)
threshold_index = 0 # 0 for red, 1 for green, 2 for blue


thresholds_green = [(79, 60, -82, -40, -10, 34)] #綠色的值
thresholds_orange = [(98, 11, 5, 114, -23, 121)]#橘色的值
thresholds_black = [(41, 21, 10, -94, 45, -13)]#黑色的值
thresholds_red = [(86, 46, 92, 8, -54, 80)]#紅色的值
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

# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. "merge=True" merges all overlapping blobs in the image.

while(True):
    distance=0
    a=0#if see orange line a==1
    angle=0
    x_left_black_value=0
    y_left_black_value=0
    x_right_black_value=0
    y_right_black_value=0
    black_wall_m=0#黑色牆壁的斜率
    clock.tick()
    img = sensor.snapshot()
    #for blob in img.find_blobs([thresholds_green[0]], pixels_threshold=100, area_threshold=50, merge=True):#找綠色方塊
        ## These values depend on the blob not being circular - otherwise they will be shaky.
        #if blob.elongation() > 0.5:
            #img.draw_edges(blob.min_corners(), color=(255,0,0))
            #img.draw_line(blob.major_axis_line(), color=(0,255,0))
            #img.draw_line(blob.minor_axis_line(), color=(0,0,255))
        ## These values are stable all the time.
        #img.draw_rectangle(blob.rect())
        #img.draw_cross(blob.cx(), blob.cy())
        ## Note - the blob rotation is unique to 0-180 only.
        #img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
        #distance=int(210000/blob.area())
        #print(distance)
        #angle = int(blob.cx()/5.3)+70
        #uart.write(str(int(distance)+15)+","+ str(angle)+"E\n")
        #time.sleep_ms(10)
    #for blob in img.find_blobs([thresholds_orange[0]], pixels_threshold=200, area_threshold=200, merge=True):#找綠色線 
        #if blob.elongation() > 0.5:
            #img.draw_edges(blob.min_corners(), color=(255,0,0))
            #img.draw_line(blob.major_axis_line(), color=(0,255,0))
            #img.draw_line(blob.minor_axis_line(), color=(0,0,255))
   
        #img.draw_rectangle(blob.rect())
        #img.draw_cross(blob.cx(), blob.cy()) 
        #img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
        #distance=int(210000/blob.area())
        #angle = blob.rotation_deg()
        #if blob.area()>500:
            #if(angle>90):
                #angle = angle-180
            #else:
                #angle
        #print(angle)
        #angle = int(blob.cx()/5.3)+70
        #if(blob.cy()>200):
            #a=1
    for blob in img.find_blobs([thresholds_red[0]], pixels_threshold=200, area_threshold=200, merge=True,roi=(0,0,80,240)):#找左黑色牆壁底下的線紅色先代替
        if blob.elongation() > 0.5:
            img.draw_edges(blob.min_corners(), color=(255,0,0))
            img.draw_line(blob.major_axis_line(), color=(0,255,0))
            img.draw_line(blob.minor_axis_line(), color=(0,0,255))
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy()) 
        img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
        x_left_black_value = blob.cx()
        y_left_black_value = blob.cy()
        distance=int(210000/blob.area())
        angle = blob.rotation_deg()
        
    for blob in img.find_blobs([thresholds_green[0]], pixels_threshold=300, area_threshold=300, merge=True,roi=(240,0,80,240)):#找右黑色牆壁底下的線紅色先代替
        if blob.elongation() > 0.5:
            img.draw_edges(blob.min_corners(), color=(255,0,0))
            img.draw_line(blob.major_axis_line(), color=(0,255,0))
            img.draw_line(blob.minor_axis_line(), color=(0,0,255))
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy()) 
        x_right_black_value = blob.cx()
        y_right_black_value = blob.cy()
        img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
        distance=int(210000/blob.area())
        angle = blob.rotation_deg()
    if(x_left_black_value>0 and  y_left_black_value>0 and x_right_black_value>0 and y_right_black_value>0):   
        img.draw_line((x_left_black_value,y_left_black_value,x_right_black_value,y_right_black_value))
        black_wall_m = y_left_black_value - y_right_black_value / x_left_black_value - x_right_black_value
        print("斜率=",black_wall_m)
    #if(a==1):
         #uart.write(str(255)+","+ str(100+angle)+"E\n")
    #else:
        #uart.write(str(255)+","+ str(100)+"E\n")
    #time.sleep_ms(10)
    