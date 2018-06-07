#include "snakebody.h"
//生成蛇头的构造函数
SnakeBody::SnakeBody(QWidget *parent)
    :QWidget(parent)
{
    this->setFixedSize(20,20);
    //设置背景色自动填充
    this->setAutoFillBackground(true);

    QPalette p;
    //通过调色板设置背景颜色
    //参数（配色模式，颜色）
    p.setColor(QPalette::Background,Qt::blue);
    //应用调色板设置
    this->setPalette(p);
    //把前节点置空
    previousBody = NULL;
    this->show();
    this->move(200,200);
}

//生成蛇身的构造函数
SnakeBody::SnakeBody(SnakeBody *pre, QWidget *parent)
    :QWidget(parent)
{
    this->setFixedSize(20,20);
    //设置背景色自动填充
    this->setAutoFillBackground(true);

    QPalette p;
    //通过调色板设置背景颜色
    //参数（配色模式，颜色）
    p.setColor(QPalette::Background,Qt::red);
    //应用调色板设置
    this->setPalette(p);
    previousBody = pre;
    //把节点移动到前节点的旧位置
    this->move(pre->getOldPos());
    this->show();
}

void SnakeBody::setPreviousBody(SnakeBody *pre)
{
    this->previousBody = pre;
}

SnakeBody *SnakeBody::getPreviousBody()
{
    return previousBody;
}

void SnakeBody::setOldPos(QPoint old_pos)
{
    this->oldPos = old_pos;
}

QPoint SnakeBody::getOldPos()
{
    return this->oldPos;
}
    //节点作为蛇头移动
void SnakeBody::bodyMove(int x, int y)
{
    //先保存旧位置
    oldPos = QPoint(this->x(),this->y());
    this->move(x,y);
}
    //节点作为蛇身移动
void SnakeBody::autoMove()
{
    this->bodyMove(previousBody->getOldPos().x(),previousBody->getOldPos().y());
}
