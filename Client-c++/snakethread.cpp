/**************************************************************
 * File Name   : snakethread.cpp
 * Author      : ThreeDog 
 * Date        : Wed May 30 12:48:01 2018
 * Description : 蛇的线程类，用于通过网络获取到的数据构造其他蛇
 *
 **************************************************************/

#include "snakethread.h"

SnakeThread::SnakeThread(QWidget *w, QObject *parent)
    : QObject(parent)
{
    this->mainWindow = w;
    this->snake = new Snake(mainWindow);
    this->snake->setLength(4);
    this->snake->setSnakeName("");
    snake->getHead()->move(200,500);
    timerId = this->startTimer(500);
    this->flag = 80;
}

//通过Json数据生成新蛇
SnakeThread::SnakeThread(QJsonObject obj,QWidget * w,QObject * parent)
    : QObject(parent)
{
    //此构造函数封装不好，严重依赖数据格式，造成耦合性很高。
    this->mainWindow = w;
    this->snake = new Snake(mainWindow);
    this->snake->setLength(obj.value("length").toInt());
    this->snake->setSnakeName(obj.value("name").toString());
    this->snake->setDirection((Snake::DIRECTION)obj.value("direction").toInt());
    this->snake->setSnakeId(obj.value("id").toInt());
    this->snake->setSpeed(obj.value("speed").toInt());
    QPoint pos(obj.value("x").toInt(),obj.value("y").toInt());
    this->snake->getHead()->move(pos);
    QJsonArray posArray = obj.value("positions").toArray();
    for(int i = 0; i < posArray.count(); i++){
        QJsonObject bodyObj = posArray.at(i).toObject();
        int x = bodyObj.value("x").toInt();
        int y = bodyObj.value("y").toInt();
        this->snake->getSnake().at(i)->move(x,y);
    }
    timerId = this->startTimer(this->snake->getSpeed());
    this->flag = 80;
}

Snake *SnakeThread::getSnake()
{
    return this->snake;
}

void SnakeThread::setSpeed(const int s)
{
    this->snake->setSpeed(s);
}

int SnakeThread::getSpeed()
{
    return this->snake->getSpeed();
}

void SnakeThread::die()
{
    this->killTimer(this->timerId);
    delete snake;
    this->snake = NULL;
}

SnakeThread::~SnakeThread()
{

}

void SnakeThread::timerEvent(QTimerEvent *)
{
    //每隔一段时间让蛇前进一次
    snake->goAhead();
    flag--;
    if(flag % 20 == 0){
        switch (snake->getDirection()) {
        case Snake::UP:
            snake->setDirection(Snake::RIGHT);
            break;
        case Snake::RIGHT:
            snake->setDirection(Snake::DOWN);
            break;
        case Snake::DOWN:
            snake->setDirection(Snake::LEFT);
            break;
        case Snake::LEFT:
            snake->setDirection(Snake::UP);
            break;
        }
        if(flag == 0)
            flag = 80;
    }
}
