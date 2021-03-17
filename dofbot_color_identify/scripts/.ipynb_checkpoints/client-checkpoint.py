#!/usr/bin/python
# -*- coding: UTF-8 -*-
#from time import sleep
import socket# 客户端 发送一个数据，再接收一个数据
n = 0
while True:
    name = input("""1: arrived      2: iscompleted     0: eixt\n""")
    print("input: " + name)
    if name == "0":
        break
    client = socket.socket(socket.AF_INET,socket.SOCK_STREAM) #声明socket类型，同时生成链接对象
    print(socket.AF_INET,socket.SOCK_STREAM)
    client.connect(('127.0.0.1',8099)) #建立一个链接，连接到本地的端口
    if name == "1":
        msg = "arrived"
    elif name == "2":
        msg = "iscompleted"
    else:
        print("Please input again!\n")
        client.close() #关闭这个链接
        continue
    client.send(msg.encode('utf-8'))  #发送一条信息 python3 只接收btye流
    data = client.recv(1024) #接收一个信息，并指定接收的大小 为1024字节
    print('recv:',data.decode()) #输出我接收的信息
    
    client.close() #关闭这个链接