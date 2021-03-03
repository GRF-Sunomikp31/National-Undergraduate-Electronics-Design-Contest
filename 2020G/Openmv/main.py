#代码思路：分别先识别圆和三角形 再识别圆和三角形的颜色模块；测量三角形采用间接法测量   黑色色块问题 三个if不等于1
#如果5次或者十次检测到同一个值就发送  因为我这边要一直发送信息给32，可以让32那边执行
#通信协议:0x5a/0x5a/   X    /     Y     / mode                            /color/distance/0xb3
#       头帧 / 头帧/色块x 默认0/色块Y 默认0/默认0，圆,1、三角形2、正方形3 篮球4 排球5 足球6/默认0 R1G2B3/默认0 物体长度 /尾帧
#三角形和正方形由于旋转的影响可能不准
import sensor, image, time,math
from pyb import UART

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)    #160*120
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()

uart = UART(3, 115200)
uart.init(115200, bits=8, parity=None, stop=1)  #8位数据位，无校验位，1位停止位
threshold_index = 0 # 0 for red, 1 for green, 2 for blue
thresholds = [(16, 71, 15, 102, -23, 97), # red_thresholds
              (11, 29, -33, -17, 0, 48), # green_thresholds
              (0, 42, -23, 16, -46, -5)] # blue_thresholds

while(True):
    clock.tick()
    img = sensor.snapshot().rotation_corr(z_rotation=180)
#参数初始化
    X=0     #物体中心x坐标
    Y=0     #物体中心Y坐标
    mode=0  #形状 默认0 圆1角2方3
    color=0 #颜色 默认0 R1G2B3
    distance=0 #物体x方向距离（矩形怎么办？对角线？   像素单位   利用四个角计算


#检测圆
    for c in img.find_circles(threshold =5000, x_margin = 10, y_margin = 10, r_margin = 10,
            r_min = 2, r_max = 100, r_step = 2):
        area = (c.x()-c.r(), c.y()-c.r(), 2*c.r(), 2*c.r())#area为识别到的圆的区域，即圆的外接矩形框
        statistics = img.get_statistics(roi=area)#像素颜色统计  这里为什么不直接用○的ROI？
        #print(statistics)

        #(0,100,0,120,0,120)是红色的阈值，所以当区域内的众数（也就是最多的颜色），范围在这个阈值内，就说明是红色的圆。
        #l_mode()，a_mode()，b_mode()是L通道，A通道，B通道的众数。
        if 0<statistics.l_mode()<100 and 50<statistics.a_mode()<90 and 30<statistics.b_mode()<100:# red
            img.draw_circle(c.x(), c.y(), c.r(), color = (255, 0, 0))#识别到的红色圆形用红色的圆框出来
            X=c.x()
            Y=c.y()
            mode=1
            color=1
            distance= 2*c.r()
        if 0<statistics.l_mode()<100 and -40<statistics.a_mode()<40 and -80<statistics.b_mode()<40:#green
            img.draw_circle(c.x(), c.y(), c.r(), color = (0, 255, 0))#识别到的绿色圆形用绿色的圆框出来
            X=c.x()
            Y=c.y()
            mode=1
            color=2
            distance= 2*c.r()
        if 0<statistics.l_mode()<100 and -20<statistics.a_mode()<40 and -50<statistics.b_mode()<0:#blue
            img.draw_circle(c.x(), c.y(), c.r(), color = (0, 0, 255))#识别到的蓝色圆形用蓝色的圆框出来
            X=c.x()
            Y=c.y()
            mode=1
            color=3
            distance= 2*c.r()

#检测矩形   问题：现在会出现颜色标记出错的问题
    for d in img.find_rects(threshold = 20000):
        area=(d[0],d[1],d[2],d[3])   #(x, y, w, h)
        statistics1 = img.get_statistics(roi=area)#像素颜色统计
        #print(statistics1)
        x=0
        x=d[2]/d[3]
        print(x)
        q=[0,0]
        Q=0
        for p in d.corners():Q=((p[0]-q[0])**2+(p[1]-q[1])**2)**0.5
        if 0<statistics1.l_mode()<100 and 0<statistics1.a_mode()<90 and 15<statistics1.b_mode()<70 and 0.5<x<1.5:
            img.draw_rectangle(d.rect(), color = (255, 0, 0))
            X=int(d[0]+d[2]/2)
            Y=int(d[1]+d[3]/2)
            mode=3
            color=1
            distance=int(Q)
        if 0<statistics1.l_mode()<100 and -38<statistics1.a_mode()<-18 and 6<statistics1.b_mode()<26  and 0.5<x<1.5:
            img.draw_rectangle(d.rect(), color = (0, 255, 0))
            X=int(d[0]+d[2]/2)
            Y=int(d[1]+d[3]/2)
            mode=3
            color=2
            distance=int(Q)
        if 0<statistics1.l_mode()<100 and -7<statistics1.a_mode()<13 and -57<statistics1.b_mode()<-37  and 0.5<x<1.5:
            img.draw_rectangle(d.rect(), color = (0,0 , 255))
            X=int(d[0]+d[2]/2)
            Y=int(d[1]+d[3]/2)
            mode=3
            color=3
            distance=int(Q)
        print(distance)

#检测三角形    色块查找绿色有影响   黑色产生
    for threshold_index in range(3):
        blob1 = img.find_blobs([thresholds[threshold_index]], pixels_threshold=600)
        if blob1:
            for b in blob1:
                c=b.pixels()/(b.w()*b.h())
                dis_angle=0
                n=0
                theta1 = 0
                theta2 = 0
                for lines in img.find_lines(roi=(b[0],b[1],b[2],b[3]), threshold = 1000, theta_margin = 25, rho_margin = 25):
                    theta1 = lines.theta()
                    n=n+1
                    if n==1:
                        theta2 = lines.theta()
                #print(theta1)
                #print(theta2)
                #print(c)
                dis_angle=abs(theta1-theta2)
                if dis_angle>100:dis_angle=dis_angle-60
                if(70>dis_angle>50  and 0.6>c>0.4 ):
                    img.draw_rectangle(b.rect(), color = (0, 0, 0))
                    #print(threshold_index)
                    #print("triangle")
                    X=b[5]
                    Y=b[6]
                    mode=2
                    color=threshold_index+1
                    distance=(b.w()+b.h())/2
                #print(b[4]) #返回色块的像素数量（int
    if X!=0 | Y!=0:
        data = bytearray([0xb3,0xb3,X,Y,mode,color,distance,0x5b])
        #print(data)
        uart.write(data)    #发送
        #print(X)
        #print(Y)
    print("FPS %f" % clock.fps())
