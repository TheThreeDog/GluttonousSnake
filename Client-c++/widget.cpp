#include "widget.h"
#include "time.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //重设主窗体大小。
    this->move(0,0);
    this->resize(1200,660);
    this->speed = 20;
    this->snakeId = 0;

    snake = new Snake(this);
    isTimerStart = false;
    //开启一个定时器，每隔speed毫秒执行一次timerEvent（）；
    timerID = this->startTimer(speed);
    isTimerStart = true;
//    createFood();
    this->food = NULL;

    SnakeThread * st = new SnakeThread(this);
    threads.append(st);
    st->start();

    // 启动网络、网络收到的数据，在主类中处理
    m_pNetWork = new NetWork(this);
    connect(m_pNetWork,SIGNAL(sig_socketReceived(QJsonObject)),this,SLOT(slot_receiveData(QJsonObject)));
    m_pNetWork->startup();

}

void Widget::die()
{
    this->killTimer(timerID);
    isTimerStart = false;
}

void Widget::createFood(int x, int y)
{
    qDebug()<<"create food !";
    //通过从网络收到的坐标生成食物。
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
    if( food != NULL && food->pos() == head->pos()){
        snake->addLength();
        delete food;
        food = NULL;
//        this->createFood();
        QJsonObject jsonObj;
        jsonObj.insert("msg","createFood");
        this->m_pNetWork->slot_socketSend(jsonObj);
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
        if(isTimerStart){
            this->speed = 1000;
            this->restartTimer();
        }
        break;
    case Qt::Key_2:
        if(isTimerStart){
            this->speed = 500;
            this->restartTimer();
        }
        break;
    case Qt::Key_3:
        if(isTimerStart){
            this->speed = 200;
            this->restartTimer();
        }
        break;
    case Qt::Key_4:
        if(isTimerStart){
            this->speed = 100;
            this->restartTimer();
        }
        break;
    case Qt::Key_5:
        if(isTimerStart){
            this->speed = 20;
            this->restartTimer();
        }
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

void Widget::closeEvent(QCloseEvent *)
{
    this->m_pNetWork->disconnect();
}

Widget::~Widget()
{

}

//接收到数据的处理
void Widget::slot_receiveData(QJsonObject jsonObj)
{
    qDebug()<<"receive data!";
    QString msg = jsonObj.value("msg").toString();
    //加入成功
    if(msg == "join successful"){
        int id = jsonObj.value("snake_id").toInt();
        int snakeNum = jsonObj.value("snake_num").toInt();
        this-> snakeId = id;
        if(snakeNum == 1){
            //如果这是第一条蛇，先生成一个食物
            //就去请求一个食物
            QJsonObject jsonObj;
            jsonObj.insert("msg","createFood");
            this->m_pNetWork->slot_socketSend(jsonObj);
        }
    }
    if(msg == "createFood"){
        int x = jsonObj.value("x").toInt();
        int y = jsonObj.value("y").toInt();
        this->createFood(x,y);
    }
}
