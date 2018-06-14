# -*- coding:utf-8 -*-
__author__ = 'Threedog'
__Date__ = '2018/5/21 16:46'
import socket
import threading
import time
import json
import random

HOST = ''
PORT = 10001
BODY_SIZE = 20
WIDTH = 1200//20
HEIGHT = 660//20


def recv_sockdata(the_socket):
    total_data = ""
    while True:
        data = the_socket.recv(1024).decode()
        if "END" in data:
            total_data += data[:data.index("END")]
            break
        total_data+=data
    return total_data


# 创建蛇线程
class SnakeThread(threading.Thread,object):
    snakes = {}
    snake_num = 0
    snake_id = 0
    food_x = 0
    food_y = 0
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
        self.snake['x'] = random.randint(20,WIDTH-20) * BODY_SIZE
        self.snake['y'] = random.randint(10,HEIGHT-10) * BODY_SIZE
        self.snake['direction'] = random.randint(0,3)
        self.snake['length'] = 4
        self.snake['speed'] = 500
        self.snake['name'] = '三级狗'
        self.snake['positions'] = []
        SnakeThread.snakes[SnakeThread.snake_id] = self.snake
        # 向新加入的客户端反馈新的蛇自己的信息 同时向新加入的客户端返回当前游戏中所有蛇的信息 食物信息
        self.sock.sendall(json.dumps({
            "snake": self.snake,
            "msg":"join successful",
            "snake_num":SnakeThread.snake_num,
            "all_snakes":SnakeThread.snakes,
            "food_x": SnakeThread.food_x,
            "food_y": SnakeThread.food_y
        }).encode())

        # self.sock.sendall(json.dumps({'snakes':"snakes",'msg':'snakes'}).encode())
        # 向其他蛇发送新蛇的信息
        self.broadcast_msg_without_self({"msg":"newSnake","snake":self.snake})

    def broadcast_msg(self,msg):
        for sock in SnakeThread.socks:
            # print(str(time.time())+"  :  "+str(msg))
            sock.sendall(json.dumps(msg).encode())

    def broadcast_msg_without_self(self,msg):
        for sock in SnakeThread.socks:
            # print(str(time.time())+"  :  "+str(msg))
            if sock == self.sock:
                continue
            sock.sendall(json.dumps(msg).encode())

    def recv_data(self):
        while True:
            data = json.loads(recv_sockdata(self.sock),encoding="utf-8")

            if data['msg'] == 'exit':
                break
            if data['msg'] != 'positions':
                print(data)
            if data['msg'] == "eatFood":
                # 给其他蛇发送蛇加长的消息
                res_all = {}
                res_all['msg'] = 'addLength'
                res_all['id'] = data['id']
                self.broadcast_msg_without_self(res_all)
                # 给记录的蛇加长：
                SnakeThread.snakes[data['id']]['length'] += 1
                # 随机生成一个食物，广播给所有蛇
                res = {}
                res['x'] = SnakeThread.food_x = random.randint(0,WIDTH-1) * BODY_SIZE
                res['y'] = SnakeThread.food_y = random.randint(0,HEIGHT-1) * BODY_SIZE
                res['msg'] = 'createFood'
                self.broadcast_msg(res)
            if data['msg'] == "createFood":
                # 随机生成一个食物，广播给所有蛇
                res = {}
                res['x'] = SnakeThread.food_x = random.randint(0, WIDTH - 1) * BODY_SIZE
                res['y'] = SnakeThread.food_y = random.randint(0, HEIGHT - 1) * BODY_SIZE
                res['msg'] = 'createFood'
                self.broadcast_msg(res)
            if data['msg'] == "positions":
                snake_id = data['id']
                SnakeThread.snakes[snake_id]['positions'] = data['positions']
                SnakeThread.snakes[snake_id]['x'] = data['positions'][0]['x']
                SnakeThread.snakes[snake_id]['y'] = data['positions'][0]['y']
            if data['msg'] == 'die':
                # 蛇如果死了，清除socket  退出循环
                snake_id = data['id']
                SnakeThread.snakes.pop(snake_id)
                self.broadcast_msg_without_self({"id":snake_id,"msg":"die"})
            # self.snake['direction'] = data['direction']
            # self.snake['name'] = data['name']
            # self.snake['speed'] = data['speed']

    def delete_snake(self):
        self.sock.close()
        SnakeThread.socks.remove(self.sock)
        if self.snake['id'] in SnakeThread.snakes.keys():
            SnakeThread.snakes.pop(self.snake['id'])
        SnakeThread.snake_num -= 1

    def run(self):
        print('Accept new connection from {}...'.format(addr))
        # 创建蛇并存在列表中
        self.add_snake()
        # 死循环读入数据并处理
        try:
            self.recv_data()
        except Exception as e:
            print(e) #有的客户端异常断开的情况
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