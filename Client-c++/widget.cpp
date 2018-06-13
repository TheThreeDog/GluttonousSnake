#include "widget.h"
#include "time.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
//    this->speed = 20;
    this->snakeId = 0;
    this->snake = NULL;
    this->show();

//    snake = new Snake(this);
//    snake->addLength();
//    snake->addLength();
//    snake->addLength();
//    snake->addLength();
//    snake->move(200,200);
//    isTimerStart = false;
////    开启一个定时器，每隔speed毫秒执行一次timerEvent（）；
//    timerID = this->startTimer(speed);
//    isTimerStart = true;
//    createFood();

    //重设主窗体大小。
    this->move(0,0);
    this->resize(1200,660);
    this->food = NULL;

//    SnakeThread * st = new SnakeThread(this);
//    st->getSnake()->setLength(4);
//    threads.append(st);

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
//    qDebug()<<"create food !";
    //通过从网络收到的坐标生成食物。
    if(food == NULL)
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
//        delete food;
//        food = NULL;
//        this->createFood();
        QJsonObject jsonObj;
        jsonObj.insert("msg","eatFood");
        jsonObj.insert("id",this->snakeId);
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
    timerID = this->startTimer(this->snake->getSpeed());
    isTimerStart = true;
}

//只要有键盘输入，就会执行这个函数。
void Widget::keyPressEvent(QKeyEvent *event)
{
    if(this->snake == NULL)
        return ;
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
            timerID = this->startTimer(this->snake->getSpeed());
            isTimerStart = true;
        }
        break;
    case Qt::Key_1:
        if(isTimerStart){
            this->snake->setSpeed(1000);
            this->restartTimer();
        }
        break;
    case Qt::Key_2:
        if(isTimerStart){
            this->snake->setSpeed(500);
            this->restartTimer();
        }
        break;
    case Qt::Key_3:
        if(isTimerStart){
            this->snake->setSpeed(200);
            this->restartTimer();
        }
        break;
    case Qt::Key_4:
        if(isTimerStart){
            this->snake->setSpeed(100);
            this->restartTimer();
        }
        break;
    case Qt::Key_5:
        if(isTimerStart){
            this->snake->setSpeed(20);
            this->restartTimer();
        }
        break;
    default:
        event->ignore();
    }
}

void Widget::timerEvent(QTimerEvent *)
{
    //每隔一段时间让蛇前进一次
    this->snake->goAhead();
    QJsonObject json;
    json.insert("msg","positions");
    json.insert("id",this->snake->getSnakeId());
    QJsonArray positions;
    foreach(SnakeBody * body,this->snake->getSnake()){
        QJsonObject posJson;
        posJson.insert("x",body->x());
        posJson.insert("y",body->y());
        positions.append(posJson);
    }
    json.insert("positions",positions);
    this->m_pNetWork->slot_socketSend(json);
    eatOrDeath();
}

void Widget::closeEvent(QCloseEvent *)
{
    if(m_pNetWork != NULL){
        QJsonObject obj;
        obj.insert("msg","exit");
        this->m_pNetWork->slot_socketSend(obj);
        this->m_pNetWork->disconnect();
    }
}

Widget::~Widget()
{

}

//接收到数据的处理
void Widget::slot_receiveData(QJsonObject jsonObj)
{
//    qDebug()<<"receive data!";
    QString msg = jsonObj.value("msg").toString();
    // 加入成功
    if(msg == "join successful"){
        // 生成蛇
        QJsonObject snakeObj = jsonObj.value("snake").toObject();
//        qDebug()<<snakeObj;
        snakeId = snakeObj.value("id").toInt();
        snake = new Snake(this);
        snake->setSnakeId(snakeId);
        snake->setSpeed(snakeObj.value("speed").toInt());
        snake->setDirection((Snake::DIRECTION)snakeObj.value("direction").toInt());
        QPoint pos(snakeObj.value("x").toInt(),snakeObj.value("y").toInt());
        for(int i = 0; i < snakeObj.value("length").toInt(); i++){
            snake->addLength();
        }
        snake->getHead()->move(pos);
        snake->setSnakeName(snakeObj.value("name").toString());

        isTimerStart = false;
        timerID = this->startTimer(snake->getSpeed());
        isTimerStart = true;
        // 如果这是第一条蛇，先生成一个食物
        int snakeNum = jsonObj.value("snake_num").toInt();
        if(snakeNum == 1){
            //就去请求一个食物
            QJsonObject jsonObj;
            jsonObj.insert("msg","createFood");
            this->m_pNetWork->slot_socketSend(jsonObj);
        }else{
            //否则的话就获取当前的食物的位置
            int x = jsonObj.value("food_x").toInt();
            int y = jsonObj.value("food_y").toInt();
            this->createFood(x,y);
        }
        QJsonObject allSnakesObj = jsonObj.value("all_snakes").toObject();
        foreach(QString s_id,allSnakesObj.keys()){
            int id = s_id.toInt();
            if( id == this->snakeId )//自己不做渲染
                continue;
            QJsonObject obj = allSnakesObj.value(s_id).toObject();
            SnakeThread * st = new SnakeThread(obj,this);//通过Json生成新蛇
            threads.append(st);
        }
//        qDebug()<<allSnakesObj;
    }else if(msg == "createFood"){
        //食物被吃掉了，创建一个新的
        int x = jsonObj.value("x").toInt();
        int y = jsonObj.value("y").toInt();
        this->createFood(x,y);
    }else if(msg == "addLength"){
        //有蛇吃了食物，对应的蛇长度+1
        int targetId = jsonObj.value("id").toInt();
        foreach(SnakeThread * thread,threads){
            if (thread->getSnake()->getSnakeId() == targetId)
                thread->getSnake()->addLength();
        }
    }else if(msg == "newSnake"){
        //有新蛇加入
        QJsonObject snakeObj = jsonObj.value("snake").toObject();
        SnakeThread * st = new SnakeThread(snakeObj,this);
        threads.append(st);
    }
}
