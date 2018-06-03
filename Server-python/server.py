# -*- coding:utf-8 -*-
__author__ = 'Threedog'
__Date__ = '2018/5/21 16:46'
import socket
import threading
import time
import json

HOST = ''
PORT = 10001


# 创建蛇线程
class SnakeThread(threading.Thread,object):
    snakes = {}
    snake_num = 0
    snake_id = 0
    socks = []

    def __init__(self,sock,addr):
        threading.Thread.__init__(self)
        self.sock = sock
        self.addr = addr
        self.snake = {}
        SnakeThread.socks.append(sock)

    def add_snake(self):
        SnakeThread.snake_num += 1
        SnakeThread.snake_id += 1
        self.snake['id'] = SnakeThread.snake_id
        SnakeThread.snakes[SnakeThread.snake_id] = self.snake
        self.sock.send(str({"snake_id": SnakeThread.snake_id,"replay":"join successful"}).encode(encoding='utf-8'))

    def recv_data(self):
        while True:
            data = self.sock.recv(1024)
            if data.decode() == 'exit':
                break
            print(data.decode())
            data = eval(data.decode())
            self.snake['direction'] = data['direction']
            self.snake['name'] = data['name']
            self.snake['speed'] = data['speed']
            for sock in SnakeThread.socks:
                # print(str(time.time())+"  :  "+str(SnakeThread.snakes))
                sock.sendall(str(self.snake).encode())

    def delete_snake(self):
        self.sock.close()
        SnakeThread.socks.remove(self.sock)
        SnakeThread.snakes.pop(self.snake['id'])
        SnakeThread.snake_num -= 1


    def run(self):
        print('Accept new connection from {}...'.format(addr))
        # 创建蛇并存在列表中
        self.add_snake()
        # 死循环读入数据并处理
        try:
            self.recv_data()
        except:
            pass
        # 退出循环后，执行关闭线程处理
        self.delete_snake()
        print('Connection from {} closed.'.format(addr))


if __name__ == '__main__':
    #创建Socket
    tcpSocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    #绑定地址
    tcpSocket.bind((HOST,PORT))
    #监听端口，传入的参数指定等待连接的最大数量
    tcpSocket.listen(16)
    threads = []
    print('Waiting for connection...')
    #服务器程序通过一个永久循环来接受来自客户端的连接
    while True:
        # 接受一个新连接:
        sock,addr = tcpSocket.accept()
        # 创建新线程来处理TCP连接:每个连接都必须创建新线程（或进程）来处理，
        #否则，单线程在处理连接的过程中，无法接受其他客户端的连接：
        snake_thread = SnakeThread(sock,addr)
        snake_thread.start()
        threads.append(snake_thread)
        print("当前连接数："+str(SnakeThread.snake_num))