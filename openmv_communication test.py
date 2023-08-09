# Single Color RGB565 Blob Tracking Example
#
# This example shows off single color RGB565 tracking using the OpenMV Cam.

import sensor, image, time, math
from pyb import UART
from pyb import LED
uart = UART(3, 19200)



thresholds_green = [(79, 60, -82, -40, -10, 34)] #綠色的值
thresholds_orange = [(55, 26, 17, 114, -23, 121)]#橘色的值
thresholds_black = [(0, 11, -85, 120, -14, 121)]#黑色的值
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
a = [0]*10

count=0
a_sum=0

while(True):
    angle=0
    x=0
    a_sum=0
    if count==10:
        count=0
    clock.tick()
    img = sensor.snapshot()
    for blob in img.find_blobs([thresholds_green[0]], pixels_threshold=200, area_threshold=200, merge=True):#找橘色線 
        if blob.elongation() > 0.5:
            img.draw_edges(blob.min_corners(), color=(255,0,0))
            img.draw_line(blob.major_axis_line(), color=(0,255,0))
            img.draw_line(blob.minor_axis_line(), color=(0,0,255))
   
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy()) 
        img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
        distance=int(210000/blob.area())
        angle = blob.rotation_deg()
        if blob.area()>300:
            if(angle>90):
                angle = angle-180
            else:
                angle-4
            #print(angle-4)
            if(blob.cy()>150):
                x=1
    if x==1:
        a[count] =1     
    else:
        a[count]=0
    for i in range(9):
        a_sum=a_sum+a[i]
    )
    
    if (uart.any()):
        print(uart.read())
    else:
        if  a_sum>0:
            uart.write(str('+')+str(100)+","+ str(100+angle)+"E\n")
        else:
            uart.write(str('B')
    count= count+1
    time.sleep_ms(100)
#   print("a_sum=",a_sum)
     