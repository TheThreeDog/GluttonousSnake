# -*- coding:utf-8 -*-
__author__ = 'Threedog'
__Date__ = '2018/5/28 19:35'

import socket
import time
import threading

tcp_client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)


server_addr = ('127.0.0.1',10001)
tcp_client.connect(server_addr)

while True:
    send_data = {}
    send_data['id'] = 10
    send_data['direction'] = 'UP'
    send_data['name'] = '小妖精'
    send_data['speed'] = 1
    a = input()
    # if a == 'exit':
    #     tcp_client.send("exit".encode())
    #     tcp_client.close()
    #     break
    send_data['speed'] = int(a)
    tcp_client.send(str(send_data).encode())

    resv_data = tcp_client.recv(1024)
    print("receive data : {}".format(resv_data.decode()))
    time.sleep(1)




