#ifndef SNAKEBODY_H
#define SNAKEBODY_H

#include <QWidget>

class SnakeBody : public QWidget
{
    Q_OBJECT
public:
    //构造函数
    explicit SnakeBody(QWidget *parent = 0);
    //指定前节点的构造函数
    SnakeBody(SnakeBody *pre,QWidget *parent = 0);
    //设置前一节点
    void setPreviousBody(SnakeBody * pre);
    //获取前一节点的指针
    SnakeBody * getPreviousBody();
    //设置旧的位置
    void setOldPos(QPoint old_pos);
    //获取旧的位置
    QPoint getOldPos();
    //节点作为蛇头移动
    void bodyMove(int x,int y);
    //节点作为蛇身移动
    void autoMove();


signals:

public slots:

private:
    SnakeBody *previousBody;
    QPoint oldPos;

};

#endif // SNAKEBODY_H
