#include "widget.h"
#include "time.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //重设主窗体大小。
    this->move(0,0);
    this->resize(1000,760);
    this->speed = 20;

//    this->setWindowFlags(
//      Qt::FramelessWindowHint
//      |
//      Qt::WindowMinimizeButtonHint
//      );
//    this->setAttribute
//      (Qt::WA_TranslucentBackground);


    snake = new Snake(this);
    isTimerStart = false;
    //开启一个定时器，每隔100毫秒执行一次timerEvent（）；
    timerID = this->startTimer(speed);
    isTimerStart = true;
    createFood();

    SnakeThread * st = new SnakeThread(this);
    threads.append(st);
    st->start();
}

void Widget::die()
{
    this->killTimer(timerID);
    isTimerStart = false;
}

void Widget::createFood()
{
    //配置随机数的种子
    srand((unsigned)time(NULL));
    //随机生成食物的位置
    int x = rand()%50*20;
    int y = rand()%35*20;
    //通过我们计算出的随机数生成坐标
    QPoint p(x,y);
    //检验食物不能生成在蛇头上。
    if(p == snake->getSnake().at(0)->pos()){
        x = rand()%50*20;
        y = rand()%35*20;
    }
    //通过随机坐标生成食物。
    food = new SnakeBody(this);
    //把食物移动到我们生成的坐标上。
    food->move(x,y);

}

void Widget::eatOrDeath()
{
    //先获取蛇头指针
    SnakeBody * head = snake->getSnake().at(0);
    //判断是不是越界
    if(head->x() < 0 || head->x() >= this->width()){
        //QMessageBox::information("游戏结束",this);
        die();
    }
    if(head->y() < 0 || head->y() >= this->height()){
        //QMessageBox::information("游戏结束",this);
        die();
    }
        //蛇头的坐标有没有超过窗体边界
    // 判断有没有撞到其他蛇
    foreach(SnakeThread *snakeThread,this->threads){
        Snake * snake = snakeThread->getSnake();
        foreach(SnakeBody * body , snake->getSnake()){
            if(body->pos() == head->pos()){
                die();
            }
        }
    }
    //判断有没有自杀
        //遍历每一个蛇身，如果有蛇身的坐标和蛇头重合，说明自杀
    for(int i = 1;i < snake->getSnake().count();i++){
        if(snake->getSnake().at(i)->pos() == head->pos()){
            die();
        }
    }
    //判断有没有吃到食物
        //校验蛇头和食物坐标，是否重合，
    if(food->pos() == head->pos()){
        snake->addLength();
        delete food;
        food = NULL;
        this->createFood();
    }
            //如果重合  addLength()；把当前食物删掉，生成新的食物
    //如果没重合，返回即可。
}

//重启定时器
void Widget::restartTimer()
{
    this->killTimer(timerID);
    isTimerStart = false;
    timerID = this->startTimer(this->speed);
    isTimerStart = true;
}

//只要有键盘输入，就会执行这个函数。
void Widget::keyPressEvent(QKeyEvent *event)
{
    //event->key();就是我们输入的按键
    switch(event->key()){
    case Qt::Key_Up :
    //向上移动
        snake->setDirection(Snake::UP);
    break;
    case Qt::Key_Down:
        snake->setDirection(Snake::DOWN);
        break;
    case Qt::Key_Left:
        snake->setDirection(Snake::LEFT);
        break;
    case Qt::Key_Right:
        snake->setDirection(Snake::RIGHT);
        break;
    case Qt::Key_Backspace:
        snake->addLength();
        break;
    case Qt::Key_P:
        if(isTimerStart)//如果定时器是在运行的
        {//暂停
            this->killTimer(timerID);
            isTimerStart = false;
        }else
        {//继续
            timerID = this->startTimer(speed);
            isTimerStart = true;
        }
        break;
    case Qt::Key_1:
        this->speed = 1000;
        this->restartTimer();
        break;
    case Qt::Key_2:
            this->speed = 500;
            this->restartTimer();
        break;
    case Qt::Key_3:
            this->speed = 200;
            this->restartTimer();
        break;
    case Qt::Key_4:
            this->speed = 100;
            this->restartTimer();
        break;
    case Qt::Key_5:
        this->speed = 20;
        this->restartTimer();
        break;
    default:
        event->ignore();
    }
}

void Widget::timerEvent(QTimerEvent *event)
{
    //每隔一段时间让蛇前进一次
    snake->goAhead();
    eatOrDeath();
}

Widget::~Widget()
{

}
