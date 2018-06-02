/**************************************************************
 * File Name   : snakethread.cpp
 * Author      : ThreeDog 
 * Date        : Wed May 30 12:48:01 2018
 * Description : 蛇的线程类，用于通过网络获取到的数据构造其他蛇
 *
 **************************************************************/

#include "snakethread.h"

SnakeThread::SnakeThread(QWidget *w, QObject *parent)
    : QThread(parent)
{
    this->mainWindow = w;
    this->snake = new Snake(mainWindow);
    snake->getHead()->move(200,500);
    timerId = this->startTimer(100);
    this->flag = 80;
}

Snake *SnakeThread::getSnake()
{
    return this->snake;
}

SnakeThread::~SnakeThread()
{

}

void SnakeThread::run()
{

}

void SnakeThread::timerEvent(QTimerEvent *e)
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
//    eatOrDeath();
}
