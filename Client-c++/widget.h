#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "snake.h"
#include <QKeyEvent>
#include <QTimerEvent>
#include <QJsonObject>
#include <QJsonValue>
#include "snakethread.h"
#include "network.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    void die();// 蛇死亡函数
    void createFood(int x, int y);//创建食物的函数
    void eatOrDeath();//判断是否吃食物，是否撞墙
    void restartTimer();//重启定时器
    ~Widget();
public slots:
    void slot_receiveData(QJsonObject jsonObj);//收到数据
protected:
    //键盘事件的处理
    void keyPressEvent(QKeyEvent *event);
    //定时器的处理事件
    void timerEvent(QTimerEvent *event);
    void closeEvent(QCloseEvent *);
private:
    Snake* snake;
    int timerID;    //定时器的标识符
    int snakeId;    //蛇自己的ID
    bool isTimerStart;
    SnakeBody * food;//食物
    QVector<SnakeThread *> threads;
    NetWork * m_pNetWork;
};

#endif // WIDGET_H
