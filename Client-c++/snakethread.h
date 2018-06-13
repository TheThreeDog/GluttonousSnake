/**************************************************************
 * File Name   : snakethread.h
 * Author      : ThreeDog
 * Date        : Wed May 30 12:48:01 2018
 * Description : 蛇的线程类，用于通过网络获取到的数据构造其他蛇
 *
 **************************************************************/
#ifndef _SNAKETHREAD_H_ 
#define _SNAKETHREAD_H_ 
#include <QWidget>
#include <QTimerEvent>
#include <snake.h>
#include <QDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>


class SnakeThread:public QObject
{
public:
    SnakeThread(QWidget * w,QObject * parent = 0);
    //通过Json数据生成新蛇
    SnakeThread(QJsonObject obj,QWidget * w,QObject * parent = 0);
    Snake * getSnake();
    void setSpeed(const int s);
    int getSpeed();
    ~SnakeThread();
protected:
    void timerEvent(QTimerEvent * e);
private:
    int timerId;//时间间隔。
    QWidget * mainWindow;
    Snake * snake;
    int flag;
};

#endif  //SNAKETHREAD
