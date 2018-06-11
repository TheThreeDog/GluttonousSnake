#include "snake.h"
#include <QDebug>
Snake::Snake(QWidget *parent)
    : QWidget(parent)
{
    p = parent; //父控件指针赋值
    //生成初始的蛇头蛇身
//    this->addLength();
//    this->addLength();
//    this->addLength();
//    this->addLength();
    speed = 500;
    dir = Snake::UP;
}

void Snake::setHead(SnakeBody *s)
{
    this->head = s;
}

SnakeBody *Snake::getHead()
{
    return this->snake_body.at(0);
}
//设置方向
void Snake::setDirection(Snake::DIRECTION d)
{
    //先判断，如果跟当前方向相反，则直接返回。
    if(d == UP && dir == DOWN)
        return ;
    if(d == DOWN && dir == UP)
        return ;
    if(d == LEFT && dir == RIGHT)
        return ;
    if(d == RIGHT && dir == LEFT)
        return ;
    this->dir = d;
}

Snake::DIRECTION Snake::getDirection()
{
    return this->dir;
}

QVector<SnakeBody *> Snake::getSnake()
{
    return this->snake_body;
}
//蛇的前进
void Snake::goAhead()
{
    SnakeBody * hh = snake_body.at(0);
    //通过蛇的方向判断走向
    switch(dir){
    case UP:
        hh->bodyMove(hh->x(),hh->y()-hh->height());
        break;
    case DOWN:
        hh->bodyMove(hh->x(),hh->y()+hh->height());
        break;
    case LEFT:
        hh->bodyMove(hh->x()-hh->width(),hh->y());
        break;
    case RIGHT:
        hh->bodyMove(hh->x()+hh->width(),hh->y());
        break;
    default:
        qDebug()<<"error !??!";
        break;
    }
    //蛇身的移动
    for(int i = 1; i < snake_body.count();i++){
        snake_body.at(i)->autoMove();
    }
    //每次蛇移动后，蛇名也要跟着动
    SnakeBody * h = this->getHead();
    nameLabel->move(h->x(),h->y()-nameLabel->height());

}

//蛇加长的函数！
void Snake::addLength()
{
    //qDebug()<<"enter addLength()!";
    //是往动态数组里面添加 snake_body;
    if(snake_body.count() == 0){
        //生成的蛇头
        SnakeBody * h = new SnakeBody(p);
        //蛇头默认生成在屏幕中间的位置
//        h->move(200,200);
        //把此节点加入到蛇身数组里
        snake_body.append(h);
        //生成后要生成蛇名对应的文字
        nameLabel = new QLabel("小蛇",p);
        nameLabel->show();
        nameLabel->resize(200,20);
        QFont font;
        font.setFamily("微软雅黑");
        font.setPointSize(15);
        nameLabel->setFont(font);
        nameLabel->move(h->x(),h->y()-nameLabel->height());

    }else{
        //生成的是蛇身
        //先获取蛇的长度
        int len = snake_body.count();
        SnakeBody * b = new SnakeBody(snake_body.at(len-1),p);
        snake_body.append(b);
    }
}

void Snake::setSnakeName(const QString name)
{
    this->nameLabel->setText(name);
}

QString Snake::getSnakeName()
{
    return this->nameLabel->text();
}

QLabel* Snake::getSnakeNameLabel()
{
    return this->nameLabel;
}

void Snake::setSpeed(const int speed)
{
    this->speed = speed;
}

int Snake::getSpeed()
{
    return this->speed;
}

void Snake::setSnakeId(int id)
{
    this->id = id;
}

int Snake::getSnakeId()
{
    return this->id;
}








