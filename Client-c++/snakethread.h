/**************************************************************
 * File Name   : snakethread.h
 * Author      : ThreeDog
 * Date        : Wed May 30 12:48:01 2018
 * Description : 蛇的线程类，用于通过网络获取到的数据构造其他蛇
 *
 **************************************************************/
#ifndef _SNAKETHREAD_H_ 
#define _SNAKETHREAD_H_ 
#include <QThread>
#include <QWidget>
#include <QTimerEvent>
#include <snake.h>
#include <QDebug>


class SnakeThread:public QThread
{
public:
    SnakeThread(QWidget * w,QObject * parent = 0);
    Snake * getSnake();
    void setSpeed(const int s);
    int getSpeed();
    ~SnakeThread();
protected:
    void run();
    void timerEvent(QTimerEvent * e);
private:
    int timerId;//时间间隔。
    QWidget * mainWindow;
    Snake * snake;
    int flag;
};

#endif  //SNAKETHREAD
