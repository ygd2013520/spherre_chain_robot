#!/usr/bin/env python
# coding: utf-8

# # 颜色分拣玩法

# ### 导入头文件

# In[28]:


#!/usr/bin/env python
# coding: utf-8
import cv2 as cv
import threading
from time import sleep
from dofbot_config import *
import ipywidgets as widgets
from IPython.display import display
from identify_target import identify_GetTarget


# ### 创建实例,初始化参数

# In[29]:


# 创建获取目标实例
target      = identify_GetTarget()
# 创建相机标定实例
calibration = Arm_Calibration()
# 初始化一些参数
num=0
dp    = []
xy=[90,135]
msg   = {}
threshold = 140
model = "General"
color_list = {}
# 初始化HSV值
color_hsv  = {"red"   : ((0, 43, 46), (10, 255, 255)),
              "green" : ((35, 43, 46), (77, 255, 255)),
              "blue"  : ((100, 43, 46), (124, 255, 255)),
              "yellow": ((26, 43, 46), (34, 255, 255))}
HSV_path="/home/dofbot/dofbot_ws/src/dofbot_color_identify/scripts/HSV_config.txt"
# XYT参数路径
XYT_path="/home/dofbot/dofbot_ws/src/dofbot_color_identify/scripts/XYT_config.txt"
# png path
calibration_path="/home/dofbot/dofbot_ws/src/dofbot_color_identify/scripts/calibration.png"
calibration_ok_path="/home/dofbot/dofbot_ws/src/dofbot_color_identify/scripts/calibration_ok.png"

try: read_HSV(HSV_path,color_hsv)
except Exception: print("Read HSV_config Error !!!")
try: xy, thresh = read_XYT(XYT_path)
except Exception: print("Read XYT_config Error !!!")


# ### 初始化机械臂位置

# In[30]:


import Arm_Lib
# 创建机械臂驱动实例
arm = Arm_Lib.Arm_Device()
joints_0 = [xy[0], xy[1], 0, 0, 90, 30]
arm.Arm_serial_servo_write6_array(joints_0, 1000)


# ### 创建控件

# In[31]:


button_layout      = widgets.Layout(width='320px', height='60px', align_self='center')
output = widgets.Output()
# 调整滑杆
joint1_slider      = widgets.IntSlider(description='joint1 :'   ,    value=xy[0]     , min=70 , max=110, step=1, orientation='horizontal')
joint2_slider      = widgets.IntSlider(description='joint2 :'   ,    value=xy[1]     , min=115, max=155, step=1, orientation='horizontal')
threshold_slider   = widgets.IntSlider(description='threshold :',    value=threshold , min=0  , max=255, step=1, orientation='horizontal')

# 进入标定模式
calibration_model  = widgets.Button(description='calibration_model',  button_style='primary', layout=button_layout)
calibration_ok     = widgets.Button(description='calibration_ok',     button_style='success', layout=button_layout)
calibration_cancel = widgets.Button(description='calibration_cancel', button_style='danger', layout=button_layout)
# 选择抓取颜色
color_list_one     = widgets.Dropdown(options=['red', 'green', 'blue', 'yellow', 'none'], value='none', disabled=False)
color_list_two     = widgets.Dropdown(options=['red', 'green', 'blue', 'yellow', 'none'], value='none', disabled=False)
color_list_three   = widgets.Dropdown(options=['red', 'green', 'blue', 'yellow', 'none'], value='none', disabled=False)
color_list_four    = widgets.Dropdown(options=['red', 'green', 'blue', 'yellow', 'none'], value='none', disabled=False)
# 目标检测抓取
target_detection   = widgets.Button(description='target_detection', button_style='info', layout=button_layout)
reset_color_list   = widgets.Button(description='reset_color_list', button_style='info', layout=button_layout)
grap = widgets.Button(description='grap', button_style='success', layout=button_layout)
# 退出
exit_button = widgets.Button(description='Exit', button_style='danger', layout=button_layout)
imgbox = widgets.Image(format='jpg', height=480, width=640, layout=widgets.Layout(align_self='center'))
color_down = widgets.HBox([exit_button, reset_color_list], layout=widgets.Layout(align_self='center'));
color_img = widgets.VBox([imgbox, color_down], layout=widgets.Layout(align_self='center'));
color_identify = widgets.VBox(
    [joint1_slider, joint2_slider, threshold_slider, calibration_model, calibration_ok, calibration_cancel,
     color_list_one, color_list_two, color_list_three, color_list_four, target_detection, grap],
    layout=widgets.Layout(align_self='center'));
controls_box = widgets.HBox([color_img, color_identify], layout=widgets.Layout(align_self='center'))


# ### 标定回调

# In[32]:


def calibration_model_Callback(value):
    global model
    model = 'Calibration'
    with output: print(model)
def calibration_OK_Callback(value):
    global model
    model = 'calibration_OK'
    with output: print(model)
def calibration_cancel_Callback(value):
    global model
    model = 'calibration_Cancel'
    with output: print(model)
calibration_model.on_click(calibration_model_Callback)
calibration_ok.on_click(calibration_OK_Callback)
calibration_cancel.on_click(calibration_cancel_Callback)


# ### 颜色选择序列

# In[33]:


# 选择颜色
def color_list_one_Callback(value):
    global model,color_list
    model="General"
    if not isinstance(value['new'],str):return
    if value['new'] == "none":
        if '1' in color_list:del color_list['1']
    elif value['new'] == "red":
        color_list['1'] = "red"
    elif value['new']== "green":
        color_list['1'] = "green"
    elif value['new'] == "blue":
        color_list['1'] = "blue"
    elif value['new'] == "yellow":
        color_list['1'] = "yellow"
    with output:
        print("color_list_three_Callback clicked.",color_list)
def color_list_two_Callback(value):
    global model,color_list
    model="General"
    if not isinstance(value['new'],str):return
    if value['new'] == "none":
        if '2' in color_list:del color_list['2']
    elif value['new'] == "red":
        color_list['2'] = "red"
    elif value['new'] == "green":
        color_list['2'] = "green"
    elif value['new'] == "blue":
        color_list['2'] = "blue"
    elif value['new'] == "yellow":
        color_list['2'] = "yellow"
    with output:
        print("color_list_three_Callback clicked.",color_list)
def color_list_three_Callback(value):
    global model,color_list
    model="General"
    if not isinstance(value['new'],str):return
    if value['new'] == "none":
        if '3' in color_list:del color_list['3']
    elif value['new'] == "red":
        color_list['3'] = "red"
    elif value['new'] == "green":
        color_list['3'] = "green"
    elif value['new'] == "blue":
        color_list['3'] = "blue"
    elif value['new'] == "yellow":
        color_list['3'] = "yellow"
    with output:
        print("color_list_three_Callback clicked.",color_list)
def color_list_four_Callback(value):
    global model,color_list
    model="General"
    if not isinstance(value['new'],str):return
    if value['new'] == "none":
        if '4' in color_list:del color_list['4']
    elif value['new'] == "red":
        color_list['4'] = "red"
    elif value['new'] == "green":
        color_list['4'] = "green"
    elif value['new'] == "blue":
        color_list['4'] = "blue"
    elif value['new'] == "yellow":
        color_list['4'] = "yellow"
    with output:
        print("color_list_four_Callback clicked.",color_list)
color_list_one.observe(color_list_one_Callback)
color_list_two.observe(color_list_two_Callback)
color_list_three.observe(color_list_three_Callback)
color_list_four.observe(color_list_four_Callback)


# ### 抓取控制

# In[34]:


# 抓取控制
def target_detection_Callback(value):
    global model
    model = 'Detection'
    with output: print(model)
def reset_color_list_Callback(value):
    global model
    model = 'Reset_list'
    with output: print(model)
def grap_Callback(value):
    global model
    model = 'Grap'
    with output: print(model)
def exit_button_Callback(value):
    global model
    model = 'Exit'
    with output: print(model)
target_detection.on_click(target_detection_Callback)
reset_color_list.on_click(reset_color_list_Callback)
grap.on_click(grap_Callback)
exit_button.on_click(exit_button_Callback)


# ### 主程序

# In[35]:


def camera():
    global color_hsv,model,dp,msg,color_list
    # 打开摄像头
    capture = cv.VideoCapture(0)
    # 当摄像头正常打开的情况下循环执行
    while capture.isOpened():
        try:
            # 读取相机的每一帧
            _, img = capture.read()
            # 统一图像大小
            img = cv.resize(img, (640, 480))
            xy=[joint1_slider.value,joint2_slider.value]
            if model == 'Calibration':
                img = cv.imread(calibration_path)
                _, img = calibration.calibration_map(img,xy,threshold_slider.value)
                #cv.imwrite(calibration_path, img)
            if model == 'calibration_OK':
                try: write_XYT(XYT_path,xy, threshold_slider.value)
                except Exception: print("File XYT_config Error !!!")
                img = cv.imread(calibration_ok_path)
                dp, img = calibration.calibration_map(img,xy,threshold_slider.value)
                #cv.imwrite(calibration_ok_path, img)
                model="General"
            if len(dp) != 0: img = calibration.Perspective_transform(dp, img)
            if model == 'calibration_Cancel':  
                dp = []
                msg= {}
                model="General"
            if len(dp)!= 0 and len(color_list)!= 0 and model == 'Detection':
                img, msg = target.select_color(img, color_hsv,color_list)
            if model=="Reset_list":
                msg={}
                color_list = {}
                model="General"
            if len(msg)!= 0 and model == 'Grap':
                threading.Thread(target=target.target_run, args=(msg,xy)).start()
                msg={}
                model="Detection"
                print("grapping!!!")
            if model == 'Exit':
                cv.destroyAllWindows()
                capture.release()
                break
            imgbox.value = cv.imencode('.jpg', img)[1].tobytes()
        except KeyboardInterrupt:capture.release()

#create client
import socket
def create_client(msg):
    client = socket.socket(socket.AF_INET,socket.SOCK_STREAM) #声明socket类型，同时生成链接对象
    print(socket.AF_INET,socket.SOCK_STREAM)
    client.connect(('192.168.31.106',4321)) #建立一个链接，连接到本地的6969端口
    #msg = 'verify$did:chisid:0xd9aeaa982fc21ea9addaf09e4f0c6a23a08d306a'#strip默认取出字符串的
    client.send(msg.encode('utf-8'))  #发送一条信息 python3 只接收btye流
    data = client.recv(1024) #接收一个信息，并指定接收的大小 为1024字节
    print('recv:',data.decode()) #输出我接收的信息
    client.close() #关闭这个链接
            
#create socket server
def create_server():
    global model,color_list,msg
    num = 1
    # 初始化机械手
    while True:
        model = "Calibration"
        sleep(2)
        model = "calibration_OK"
        sleep(2)
        if model == "General":
            break
    print("Calibration completed!!!")
    model = "Detection"
    sleep(1)
    print("Init completed!!!")
    # 建立一个服务端
    server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    server.bind(('0.0.0.0',8099)) #绑定要监听的端口
    server.listen(5) #开始监听 表示可以使用五个链接排队
    while True:# conn就是客户端链接过来而在服务端为期生成的一个链接实例
        conn,addr = server.accept() #等待链接,多个链接的时候就会出现问题,其实返回了两个值
        #print(conn,addr)
        color_list['1'] = "yellow"
#         color_list['2'] = "red"
#         color_list['3'] = "blue"
#         color_list['4'] = "green"
        try:
            data = conn.recv(1024)  #接收数据
            print('recive:',data.decode()) #打印接收到的数据
            if data.decode() == 'arrived':
                if len(msg)!= 0:
                    #verify small car
                    msg = 'verify$did:chisid:0xd9aeaa982fc21ea9addaf09e4f0c6a23a08d306a'
                    create_client(msg)
                    model = 'Grap'
                    conn.send("begin grap".encode())
                    print("status: begin grap")
                    sleep(2)
                else:
                    conn.send("nothing".encode())
                    print("status: nothing get")
            elif data.decode() == 'iscompleted':
                if target.iscompleted == True:
                    #complete go chain
                    msg = "set$Get Components Completed , Count: %d" % num
                    create_client(msg)
                    num = num + 1
                    conn.send("yes".encode())
                    print("status: completed grap")
                else:
                    conn.send("no".encode())
                    print("status: grap not completed")
            else:
                conn.send("Message Error".encode())
        except ConnectionResetError as e:
            print('关闭了正在占线的链接！')
            #break
        conn.close()


# ### 启动

# In[36]:


# display(controls_box)
display(controls_box,output)
threading.Thread(target=camera, ).start()
threading.Thread(target=create_server, ).start()


# In[ ]:





# In[ ]:




