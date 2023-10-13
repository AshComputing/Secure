> # Step by step guide
Hi im Ash i coded the Spieed Maix for this project 
let me explain how the code 
works

#### prerequsites for maix board coding
[Maixpy ide(required to code the board to use the model and sety gpio) ](http://dl.sipeed.com/MAIX/MaixPy/ide)
[Kflash utility(required to flash in the ai modiel to flash memory ](https://github.com/sipeed/kflash_gui)



https://github.com/sipeed/kflash_gui
##### Ai training
If you would like to train your own models well good for you make sure You have a decent gpu(NVIDIA) and good processor and ample amount ram 16gb ish minimum if You  do not want to see the blue screen of death 
but this will take a while  most of th image recognition we will be using is the yolo2 model
this where i learned to train model and load it into the Sipeed doc it best let this people explain
https://courses.cs.washington.edu/courses/cse475/19au/labs/yolo2_tutorial.html
however for this project there was really good pre made model for the face done by the manufacturer 
i used it as it was trained on 3 Gtx 1080 ti which i didnt have so my taining would have taken ages
to get the file you would have to use a key gen file that you flash into the Spieed doc 
and get agenrated key heres the link to it 
https://blog.sipeed.com/p/1338.html#more-1338





##### the Import statements
import sensor is to import the local sensor libary
import Image is to import the local image libary
import lcd is to import lcd libary 
import KPU as kpu is the kmodel libary for k210 chip 
from Maix import FPIOA, GPIO (this Fpio is the virtual pin for k210 )
you have to set pin to fpio and link the fpio to gpio 
```
import sensor
import image
import lcd
import KPU as kpu
import time
from Maix import FPIOA, GPIO
import gc
from fpioa_manager import fm
from board import board_info
import utime
```
you have to set pin to fpio and link the fpio to gpio 
this is very complicated to understand you have to see the chinese documentation to understand:)
[Link to chineses diocumentation ](https://maixpy.sipeed.com/zh/libs/Maix/fpioa.html)
but in short you need this to talk to the pin 
the gpioHS17 is a constant u set when u use fm.register its weird but thats how u set the pin 
[Link to chineses diocumentation(GPIO) ](https://maixpy.sipeed.com/zh/libs/Maix/gpio.html)
https://maixpy.sipeed.com/zh/libs/Maix/gpio.html

```
fm.register(13, fm.fpioa.GPIOHS17)#set pin 13 to gpio constant GPIOHS17
esp = GPIO(GPIO.GPIOHS17, GPIO.OUT)
esp.value(0)#set low 

fm.register(1, fm.fpioa.GPIOHS1)#Set Pin 1 To Gpio Constant GPIOHS1
key_gpio = GPIO(GPIO.GPIOHS1, GPIO.IN)
start_processing = False


def set_key_state(*_):
    global start_processing
    start_processing = True


key_gpio.irq(set_key_state, GPIO.IRQ_RISING, GPIO.WAKEUP_NOT_SUPPORT)
```
## Initialization Statements
```
lcd.init() # Initialize lcd
sensor.reset() #Initialize sensor camera
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_hmirror(1) #Set up camera mirroring
sensor.set_vflip(1)   #Set the camera to flip
sensor.run(1) #Enable camera
anchor = (1.889, 2.5245, 2.9465, 3.94056, 3.99987, 5.3658, 5.155437, 6.92275, 6.718375, 9.01025) #anchor
for face detect For face detection Anchor
dst_point = [(44,59),(84,59),(64,82),(47,105),(81,105)] #standard face key point position 
The coordinates of the 5 key points of the standard front face are left eye, right eye, 
nose, left mouth corner, and right mouth corner
a = kpu.init_yolo2(task_fd, 0.5, 0.3, 5, anchor) #Initialize the face detection model
img_lcd=image.Image() # Set display buf
img_face=image.Image(size=(128,128)) #Set up 128 * 128 Face picture buf
a=img_face.pix_to_ai() # Convert picture to kpu Accepted format
record_ftr=[] #Empty list is used to store the current 196-dimensional features
record_ftrs=[] #The empty list is used to store the facial features recorded by the keys.
You can save the features to the SD card in the form of txt and other files, and read this
list to realize the face storage after power failure.
names = ['Mr.1', 'Mr.2', 'Mr.3', 'Mr.4', 'Mr.5', 'Mr.6', 'Mr.7', 'Mr.8', 'Mr.9' , 'Mr.10'] # 
The name tag corresponds to the feature value in the above list.
```
## while loop
```
while(1): # Main loop
    check_key() #Button detection
    esp.value(0)#set low(always low only when facedetected send a high)
    img = sensor.snapshot() #Get a picture from the camera
    clock.tick() #Recording time, used to calculate frame rate
    code = kpu.run_yolo2(task_fd, img) # Run the face detection model to obtain the 
    coordinate position of the face
    if code: # If a face is detected
        for i in code: # Iterative frame
            # Cut face and resize to 128x128
            a = img.draw_rectangle(i.rect()) # Show face frame on screen
            face_cut=img.cut(i.x(),i.y(),i.w(),i.h()) # Crop part of the face image to face_cut
            face_cut_128=face_cut.resize(128,128) # Zoom the cropped face picture to128 * 128 Pixel
            a=face_cut_128.pix_to_ai() # Convert the guessed picture to kpu accepted format
            #a = img.draw_image(face_cut_128, (0,0))
            # Landmark for face 5 points
            fmap = kpu.forward(task_ld, face_cut_128) # Run the face detection model with 5 key points
            plist=fmap[:] # Obtain key prediction results
            le=(i.x()+int(plist[0]*i.w() - 10), i.y()+int(plist[1]*i.h())) # Calculate the position
            of the left eye, where -10 in the w direction is used to compensate for the accuracy
            loss caused by the model conversion
            re=(i.x()+int(plist[2]*i.w()), i.y()+int(plist[3]*i.h())) # Calculate the right eye position
            nose=(i.x()+int(plist[4]*i.w()), i.y()+int(plist[5]*i.h())) #Calculate nose position
            lm=(i.x()+int(plist[6]*i.w()), i.y()+int(plist[7]*i.h())) #Calculate the position of the 
            left corner of the mouth
            rm=(i.x()+int(plist[8]*i.w()), i.y()+int(plist[9]*i.h())) #Right corner of the mouth
            a = img.draw_circle(le[0], le[1], 4)
            a = img.draw_circle(re[0], re[1], 4)
            a = img.draw_circle(nose[0], nose[1], 4)
            a = img.draw_circle(lm[0], lm[1], 4)
            a = img.draw_circle(rm[0], rm[1], 4) # Draw a small circle at the corresponding position
            # align face to standard position
            src_point = [le, re, nose, lm, rm] # The position of the 5 coordinate in the picture
            T=image.get_affine_transform(src_point, dst_point) # Obtain the affine transformation matrix according to the obtained 5-point coordinates and the standard frontal coordinates
            a=image.warp_affine_ai(img, img_face, T) #Perform affine transformation on the original face 
            image and transform it into a positive face image
            a=img_face.ai_to_pix() # Convert the face image to kpu format
            #a = img.draw_image(img_face, (128,0))
            del(face_cut_128) # Release and crop part of the face
            # calculate face feature vector
            fmap = kpu.forward(task_fe, img_face) # Calculate the 196-dimensional feature value of the
            frontal image
            feature=kpu.face_encode(fmap[:]) #Get calculation results
            reg_flag = False
            scores = [] # Store feature comparison score
            for j in range(len(record_ftrs)): #Iterate over existing eigenvalues
                score = kpu.face_compare(record_ftrs[j], feature) #Calculate the current facial feature 
                value and the score of the existing feature value
                scores.append(score) #Add score summary table
            max_score = 0
            index = 0
            for k in range(len(scores)): #Iterate all comparison scores, find the maximum score and 
            index 
            value
                if max_score < scores[k]:
                    max_score = scores[k]
                    index = k
            if max_score > 85: # If the maximum score is greater than 85, it can be considered the 
            same person
                a = img.draw_string(i.x(),i.y(), ("%s :%2.1f" % (names[index], max_score)), color=(0,255,0),scale=2) # Show name and score
                esp.value(1)#face detected so we send a high to esp 32
                
            else:
                a = img.draw_string(i.x(),i.y(), ("X :%2.1f" % (max_score)), color=(255,0,0),scale=2) 
                #Show unknown and score
            if key_pressed == 1: #If a key is detected
                key_pressed = 0 #Reset button state
                record_ftr = feature 
                record_ftrs.append(record_ftr) #Add the current feature to the list of known features
            break
    fps =clock.fps() #Calculate the frame rate
    print("%2.1f fps"%fps) #Print frame rate
    a = lcd.display(img) #Swipe display
    #kpu.memtest()

```





# whole Spieed Maix(new firmware not below v0.4) code for unlocking (Copy this as it has no comments to run )
this is my code used in the smart package collection 
it is sightly diffrent from the code from chinese manufacturer as it is made for the new firmware 
thats why mine has things like fm.register and more
basically how code works it captures ur face when low is detected in pin one and stores ur face data in 
one of the face index example MR.1
it calculates the distance between 5 points left eye , right eye , nose , right mouth, left mouth
then it will always look for face and compare the acuracy in the index 
when the acuuracy is above 85% it will send a logic high to esp at the same time turn the text in lcd green 

```
import sensor
import image
import lcd
import KPU as kpu
import time
from Maix import FPIOA, GPIO
import gc
from fpioa_manager import fm
from board import board_info
import utime

task_fd = kpu.load(0x200000)
task_ld = kpu.load(0x300000)
task_fe = kpu.load(0x400000)
clock = time.clock()

fm.register(13, fm.fpioa.GPIOHS17)
esp = GPIO(GPIO.GPIOHS17, GPIO.OUT)
esp.value(0)

fm.register(1, fm.fpioa.GPIOHS1)
key_gpio = GPIO(GPIO.GPIOHS1, GPIO.IN)
start_processing = False


def set_key_state(*_):
    global start_processing
    start_processing = True


key_gpio.irq(set_key_state, GPIO.IRQ_RISING, GPIO.WAKEUP_NOT_SUPPORT)

lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_hmirror(1)
sensor.set_vflip(0)
sensor.run(1)
anchor = (1.889, 2.5245, 2.9465, 3.94056, 3.99987, 5.3658, 5.155437,
          6.92275, 6.718375, 9.01025)  # anchor for face detect
dst_point = [(44, 59), (84, 59), (64, 82), (47, 105),
             (81, 105)]  # standard face key point position
a = kpu.init_yolo2(task_fd, 0.5, 0.3, 5, anchor)
img_lcd = image.Image()
img_face = image.Image(size=(128, 128))
a = img_face.pix_to_ai()
record_ftr = []
record_ftrs = []
names = ['Mr.1', 'MR.2', 'Mr.3', 'Mr.4', 'Mr.5',
         'Mr.6', 'Mr.7', 'Mr.8', 'Mr.9', 'Mr.10']

while(1):
    img = sensor.snapshot()
    esp.value(0)
    clock.tick()
    code = kpu.run_yolo2(task_fd, img)
    if code:
        for i in code:
            # Cut face and resize to 128x128
            a = img.draw_rectangle(i.rect())
            face_cut = img.cut(i.x(), i.y(), i.w(), i.h())
            face_cut_128 = face_cut.resize(128, 128)
            a = face_cut_128.pix_to_ai()
            #a = img.draw_image(face_cut_128, (0,0))
            # Landmark for face 5 points
            fmap = kpu.forward(task_ld, face_cut_128)
            plist = fmap[:]
            le = (i.x()+int(plist[0]*i.w() - 10), i.y()+int(plist[1]*i.h()))
            re = (i.x()+int(plist[2]*i.w()), i.y()+int(plist[3]*i.h()))
            nose = (i.x()+int(plist[4]*i.w()), i.y()+int(plist[5]*i.h()))
            lm = (i.x()+int(plist[6]*i.w()), i.y()+int(plist[7]*i.h()))
            rm = (i.x()+int(plist[8]*i.w()), i.y()+int(plist[9]*i.h()))
            a = img.draw_circle(le[0], le[1], 4)
            a = img.draw_circle(re[0], re[1], 4)
            a = img.draw_circle(nose[0], nose[1], 4)
            a = img.draw_circle(lm[0], lm[1], 4)
            a = img.draw_circle(rm[0], rm[1], 4)
            # align face to standard position
            src_point = [le, re, nose, lm, rm]
            T = image.get_affine_transform(src_point, dst_point)
            a = image.warp_affine_ai(img, img_face, T)
            a = img_face.ai_to_pix()
            #a = img.draw_image(img_face, (128,0))
            del(face_cut_128)
            # calculate face feature vector
            fmap = kpu.forward(task_fe, img_face)
            feature = kpu.face_encode(fmap[:])
            reg_flag = False
            scores = []
            for j in range(len(record_ftrs)):
                score = kpu.face_compare(record_ftrs[j], feature)
                scores.append(score)
            max_score = 0
            index = 0
            esp.value(0)
            for k in range(len(scores)):
                if max_score < scores[k]:
                    max_score = scores[k]
                    index = k
                esp.value(0)

            if max_score > 85:
                a = img.draw_string(i.x(), i.y(), ("%s :%2.1f" % (
                    names[index], max_score)), color=(0, 255, 0), scale=3)
                esp.value(1)



            else:
                a = img.draw_string(i.x(), i.y(), ("X :%2.1f" % (
                    max_score)), color=(255, 0, 0), scale=2)
                esp.value(0)
            if start_processing:
                record_ftr = feature
                record_ftrs.append(record_ftr)
                start_processing = False
                esp.value(0)

            break
    fps = clock.fps()
    print("%2.1f fps" % fps)
    a = lcd.display(img)
    gc.collect()
    # kpu.memtest()

#a = kpu.deinit(task_fe)
#a = kpu.deinit(task_ld)
#a = kpu.deinit(task_fd)
```
# Orginal Code with Comments(translated from chinese)
```
import sensor,image,lcd  # import Related libraries
import KPU as kpu
import time
from Maix import FPIOA,GPIO
task_fd = kpu.load(0x200000) # From flash 0x200000 Load face detection model
task_ld = kpu.load(0x300000) # From flash 0x300000 Load the five-point key point detection model of the face
task_fe = kpu.load(0x400000) # From flash 0x400000 Load the 196-dimensional eigenvalue model of the face
clock = time.clock()  # Initialize the system clock and calculate the frame rate
key_pin=16 # Set button pin FPIO16
fpioa = FPIOA()
fpioa.set_function(key_pin,FPIOA.GPIO7)
key_gpio=GPIO(GPIO.GPIO7,GPIO.IN)
last_key_state=1
key_pressed=0 # Initialize button pin Set GPIO7 to FPIO16
def check_key(): # Key detection function, used to detect whether the key is pressed in the loop, the falling edge is valid
    global last_key_state
    global key_pressed 
    val=key_gpio.value()
    if last_key_state == 1 and val == 0:
        key_pressed=1
    else:
        key_pressed=0
    last_key_state = val

lcd.init() # Initialize lcd
sensor.reset() #Initialize sensor camera
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_hmirror(1) #Set up camera mirroring
sensor.set_vflip(1)   #Set the camera to flip
sensor.run(1) #Enable camera
anchor = (1.889, 2.5245, 2.9465, 3.94056, 3.99987, 5.3658, 5.155437, 6.92275, 6.718375, 9.01025) #anchor for face detect For face detection Anchor
dst_point = [(44,59),(84,59),(64,82),(47,105),(81,105)] #standard face key point position The coordinates of the 5 key points of the standard front face are left eye, right eye, nose, left mouth corner, and right mouth corner
a = kpu.init_yolo2(task_fd, 0.5, 0.3, 5, anchor) #Initialize the face detection model
img_lcd=image.Image() # Set display buf
img_face=image.Image(size=(128,128)) #Set up 128 * 128 Face picture buf
a=img_face.pix_to_ai() # Convert picture to kpu Accepted format
record_ftr=[] #Empty list is used to store the current 196-dimensional features
record_ftrs=[] #The empty list is used to store the facial features recorded by the keys. You can save the features to the SD card in the form of txt and other files, and read this list to realize the face storage after power failure.
names = ['Mr.1', 'Mr.2', 'Mr.3', 'Mr.4', 'Mr.5', 'Mr.6', 'Mr.7', 'Mr.8', 'Mr.9' , 'Mr.10'] # The name tag corresponds to the feature value in the above list.
while(1): # Main loop
    check_key() #Button detection
    img = sensor.snapshot() #Get a picture from the camera
    clock.tick() #Recording time, used to calculate frame rate
    code = kpu.run_yolo2(task_fd, img) # Run the face detection model to obtain the coordinate position of the face
    if code: # If a face is detected
        for i in code: # Iterative frame
            # Cut face and resize to 128x128
            a = img.draw_rectangle(i.rect()) # Show face frame on screen
            face_cut=img.cut(i.x(),i.y(),i.w(),i.h()) # Crop part of the face image to face_cut
            face_cut_128=face_cut.resize(128,128) # Zoom the cropped face picture to128 * 128 Pixel
            a=face_cut_128.pix_to_ai() # Convert the guessed picture to kpu accepted format
            #a = img.draw_image(face_cut_128, (0,0))
            # Landmark for face 5 points
            fmap = kpu.forward(task_ld, face_cut_128) # Run the face detection model with 5 key points
            plist=fmap[:] # Obtain key prediction results
            le=(i.x()+int(plist[0]*i.w() - 10), i.y()+int(plist[1]*i.h())) # Calculate the position of the left eye, where -10 in the w direction is used to compensate for the accuracy loss caused by the model conversion
            re=(i.x()+int(plist[2]*i.w()), i.y()+int(plist[3]*i.h())) # Calculate the right eye position
            nose=(i.x()+int(plist[4]*i.w()), i.y()+int(plist[5]*i.h())) #Calculate nose position
            lm=(i.x()+int(plist[6]*i.w()), i.y()+int(plist[7]*i.h())) #Calculate the position of the left corner of the mouth
            rm=(i.x()+int(plist[8]*i.w()), i.y()+int(plist[9]*i.h())) #Right corner of the mouth
            a = img.draw_circle(le[0], le[1], 4)
            a = img.draw_circle(re[0], re[1], 4)
            a = img.draw_circle(nose[0], nose[1], 4)
            a = img.draw_circle(lm[0], lm[1], 4)
            a = img.draw_circle(rm[0], rm[1], 4) # Draw a small circle at the corresponding position
            # align face to standard position
            src_point = [le, re, nose, lm, rm] # The position of the 5 coordinate in the picture
            T=image.get_affine_transform(src_point, dst_point) # Obtain the affine transformation matrix according to the obtained 5-point coordinates and the standard frontal coordinates
            a=image.warp_affine_ai(img, img_face, T) #Perform affine transformation on the original face image and transform it into a positive face image
            a=img_face.ai_to_pix() # Convert the face image to kpu format
            #a = img.draw_image(img_face, (128,0))
            del(face_cut_128) # Release and crop part of the face
            # calculate face feature vector
            fmap = kpu.forward(task_fe, img_face) # Calculate the 196-dimensional feature value of the frontal image
            feature=kpu.face_encode(fmap[:]) #Get calculation results
            reg_flag = False
            scores = [] # Store feature comparison score
            for j in range(len(record_ftrs)): #Iterate over existing eigenvalues
                score = kpu.face_compare(record_ftrs[j], feature) #Calculate the current facial feature value and the score of the existing feature value
                scores.append(score) #Add score summary table
            max_score = 0
            index = 0
            for k in range(len(scores)): #Iterate all comparison scores, find the maximum score and index value
                if max_score < scores[k]:
                    max_score = scores[k]
                    index = k
            if max_score > 85: # If the maximum score is greater than 85, it can be considered the same person
                a = img.draw_string(i.x(),i.y(), ("%s :%2.1f" % (names[index], max_score)), color=(0,255,0),scale=2) # Show name and score
            else:
                a = img.draw_string(i.x(),i.y(), ("X :%2.1f" % (max_score)), color=(255,0,0),scale=2) #Show unknown and score
            if key_pressed == 1: #If a key is detected
                key_pressed = 0 #Reset button state
                record_ftr = feature 
                record_ftrs.append(record_ftr) #Add the current feature to the list of known features
            break
    fps =clock.fps() #Calculate the frame rate
    print("%2.1f fps"%fps) #Print frame rate
    a = lcd.display(img) #Swipe display
    #kpu.memtest()

#a = kpu.deinit(task_fe)
#a = kpu.deinit(task_ld)
#a = kpu.deinit(task_fd)
```
# ORginal Code with chinese comments 
```
import sensor,image,lcd  # import 相关库
import KPU as kpu
import time
from Maix import FPIOA,GPIO
task_fd = kpu.load(0x200000) # 从flash 0x200000 加载人脸检测模型
task_ld = kpu.load(0x300000) # 从flash 0x300000 加载人脸五点关键点检测模型
task_fe = kpu.load(0x400000) # 从flash 0x400000 加载人脸196维特征值模型
clock = time.clock()  # 初始化系统时钟，计算帧率
key_pin=16 # 设置按键引脚 FPIO16
fpioa = FPIOA()
fpioa.set_function(key_pin,FPIOA.GPIO7)
key_gpio=GPIO(GPIO.GPIO7,GPIO.IN)
last_key_state=1
key_pressed=0 # 初始化按键引脚 分配GPIO7 到 FPIO16
def check_key(): # 按键检测函数，用于在循环中检测按键是否按下，下降沿有效
    global last_key_state
    global key_pressed 
    val=key_gpio.value()
    if last_key_state == 1 and val == 0:
        key_pressed=1
    else:
        key_pressed=0
    last_key_state = val

lcd.init() # 初始化lcd
sensor.reset() #初始化sensor 摄像头
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_hmirror(1) #设置摄像头镜像
sensor.set_vflip(1)   #设置摄像头翻转
sensor.run(1) #使能摄像头
anchor = (1.889, 2.5245, 2.9465, 3.94056, 3.99987, 5.3658, 5.155437, 6.92275, 6.718375, 9.01025) #anchor for face detect 用于人脸检测的Anchor
dst_point = [(44,59),(84,59),(64,82),(47,105),(81,105)] #standard face key point position 标准正脸的5关键点坐标 分别为 左眼 右眼 鼻子 左嘴角 右嘴角
a = kpu.init_yolo2(task_fd, 0.5, 0.3, 5, anchor) #初始化人脸检测模型
img_lcd=image.Image() # 设置显示buf
img_face=image.Image(size=(128,128)) #设置 128 * 128 人脸图片buf
a=img_face.pix_to_ai() # 将图片转为kpu接受的格式
record_ftr=[] #空列表 用于存储当前196维特征
record_ftrs=[] #空列表 用于存储按键记录下人脸特征， 可以将特征以txt等文件形式保存到sd卡后，读取到此列表，即可实现人脸断电存储。
names = ['Mr.1', 'Mr.2', 'Mr.3', 'Mr.4', 'Mr.5', 'Mr.6', 'Mr.7', 'Mr.8', 'Mr.9' , 'Mr.10'] # 人名标签，与上面列表特征值一一对应。
while(1): # 主循环
    check_key() #按键检测
    img = sensor.snapshot() #从摄像头获取一张图片
    clock.tick() #记录时刻，用于计算帧率
    code = kpu.run_yolo2(task_fd, img) # 运行人脸检测模型，获取人脸坐标位置
    if code: # 如果检测到人脸
        for i in code: # 迭代坐标框
            # Cut face and resize to 128x128
            a = img.draw_rectangle(i.rect()) # 在屏幕显示人脸方框
            face_cut=img.cut(i.x(),i.y(),i.w(),i.h()) # 裁剪人脸部分图片到 face_cut
            face_cut_128=face_cut.resize(128,128) # 将裁出的人脸图片 缩放到128 * 128像素
            a=face_cut_128.pix_to_ai() # 将猜出图片转换为kpu接受的格式
            #a = img.draw_image(face_cut_128, (0,0))
            # Landmark for face 5 points
            fmap = kpu.forward(task_ld, face_cut_128) # 运行人脸5点关键点检测模型
            plist=fmap[:] # 获取关键点预测结果
            le=(i.x()+int(plist[0]*i.w() - 10), i.y()+int(plist[1]*i.h())) # 计算左眼位置， 这里在w方向-10 用来补偿模型转换带来的精度损失
            re=(i.x()+int(plist[2]*i.w()), i.y()+int(plist[3]*i.h())) # 计算右眼位置
            nose=(i.x()+int(plist[4]*i.w()), i.y()+int(plist[5]*i.h())) #计算鼻子位置
            lm=(i.x()+int(plist[6]*i.w()), i.y()+int(plist[7]*i.h())) #计算左嘴角位置
            rm=(i.x()+int(plist[8]*i.w()), i.y()+int(plist[9]*i.h())) #右嘴角位置
            a = img.draw_circle(le[0], le[1], 4)
            a = img.draw_circle(re[0], re[1], 4)
            a = img.draw_circle(nose[0], nose[1], 4)
            a = img.draw_circle(lm[0], lm[1], 4)
            a = img.draw_circle(rm[0], rm[1], 4) # 在相应位置处画小圆圈
            # align face to standard position
            src_point = [le, re, nose, lm, rm] # 图片中 5 坐标的位置
            T=image.get_affine_transform(src_point, dst_point) # 根据获得的5点坐标与标准正脸坐标获取仿射变换矩阵
            a=image.warp_affine_ai(img, img_face, T) #对原始图片人脸图片进行仿射变换，变换为正脸图像
            a=img_face.ai_to_pix() # 将正脸图像转为kpu格式
            #a = img.draw_image(img_face, (128,0))
            del(face_cut_128) # 释放裁剪人脸部分图片
            # calculate face feature vector
            fmap = kpu.forward(task_fe, img_face) # 计算正脸图片的196维特征值
            feature=kpu.face_encode(fmap[:]) #获取计算结果
            reg_flag = False
            scores = [] # 存储特征比对分数
            for j in range(len(record_ftrs)): #迭代已存特征值
                score = kpu.face_compare(record_ftrs[j], feature) #计算当前人脸特征值与已存特征值的分数
                scores.append(score) #添加分数总表
            max_score = 0
            index = 0
            for k in range(len(scores)): #迭代所有比对分数，找到最大分数和索引值
                if max_score < scores[k]:
                    max_score = scores[k]
                    index = k
            if max_score > 85: # 如果最大分数大于85， 可以被认定为同一个人
                a = img.draw_string(i.x(),i.y(), ("%s :%2.1f" % (names[index], max_score)), color=(0,255,0),scale=2) # 显示人名 与 分数
            else:
                a = img.draw_string(i.x(),i.y(), ("X :%2.1f" % (max_score)), color=(255,0,0),scale=2) #显示未知 与 分数
            if key_pressed == 1: #如果检测到按键
                key_pressed = 0 #重置按键状态
                record_ftr = feature 
                record_ftrs.append(record_ftr) #将当前特征添加到已知特征列表
            break
    fps =clock.fps() #计算帧率
    print("%2.1f fps"%fps) #打印帧率
    a = lcd.display(img) #刷屏显示
    #kpu.memtest()

#a = kpu.deinit(task_fe)
#a = kpu.deinit(task_ld)
#a = kpu.deinit(task_fd)
```




