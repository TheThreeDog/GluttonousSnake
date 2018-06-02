#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "snake.h"
#include <QKeyEvent>
#include <QTimerEvent>
#include "snakethread.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    void die();// 蛇死亡函数
    void createFood();//创建食物的函数
    void eatOrDeath();//判断是否吃食物，是否撞墙
    void restartTimer();//重启定时器
    ~Widget();
protected:
    //键盘事件的处理
    void keyPressEvent(QKeyEvent *event);
    //定时器的处理事件
    void timerEvent(QTimerEvent *event);
private:
    Snake* snake;
    int timerID;   //定时器的标识符
    bool isTimerStart;
    SnakeBody * food;//食物
    int speed;
    QVector<SnakeThread *> threads;
};

#endif // WIDGET_H
