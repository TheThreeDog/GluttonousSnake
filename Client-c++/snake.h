﻿#ifndef SNAKE_H
#define SNAKE_H

//蛇这个类的头文件

#include <QWidget>
#include <QLabel>
#include "snakebody.h"
class Snake : public QWidget
{
    Q_OBJECT
public:
    enum DIRECTION{UP=0,DOWN,LEFT,RIGHT};
    explicit Snake(QWidget *parent = 0);
    //设置蛇头节点
    void setHead(SnakeBody* );
    //获取蛇头节点
    SnakeBody* getHead();
    //设置当前的行进方向
    void setDirection(DIRECTION dir);
    //获取当前的行进方向
    DIRECTION getDirection();
    //获取存放节点的数组
    QVector<SnakeBody *> getSnake();
    //蛇的移动函数
    void goAhead();
    //蛇加长
    void addLength();
    //设置蛇名
    void setSnakeName(const QString name);
    //获取蛇名
    QString getSnakeName();
    //获取蛇名节点
    QLabel * getSnakeNameLabel();
    //设置速度
    void setSpeed(const int speed);
    //获取速度
    int getSpeed();
    //设置蛇ID
    void setSnakeId(int id);
    //获取蛇ID
    int getSnakeId();
    //设置蛇的长度
    void setLength(const int len);
    //获取蛇的长度
    int getLength();

signals:

public slots:
private:
    QWidget * p;    //父控件指针
    SnakeBody * head;
    DIRECTION dir ;
    QVector<SnakeBody *> snake_body;
    QLabel * nameLabel;
    int speed;
    int id;

};

#endif // SNAKE_H
