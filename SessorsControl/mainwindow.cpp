#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    OpenCo2Com();//打开co2传感器串口

    rosTimer = new  QTimer(this);
    rosTimer->start(100);
    connect( rosTimer, SIGNAL(timeout()),this, SLOT(RunRosSpin()) );

    fd = spi.Open(0,5000000,1);//打开spi设备

    timerSpi = new QTimer(this);
    connect(timerSpi, SIGNAL(timeout()), this, SLOT(updateSpi()));
    timerSpi->start(1000);

    //初始化创建QUdpSocket对象
     mSocket = new QUdpSocket();
}

MainWindow::~MainWindow()
{
    CloseCo2Com();//关闭co2传感器串口
    if(rosTimer->isActive())
    {
        rosTimer->stop();
        delete rosTimer;

        ros::shutdown();//关闭ros
    }

    if(fd > 0)
    {
        spi.Close(fd);//关闭spi设备
    }
    delete ui;
}

void MainWindow::OpenCo2Com()
{
    serial = new QSerialPort();
    serial->close();
    serial->setPortName("/dev/ttyUSB0");
    serial->open(QIODevice::ReadWrite);
    serial->setBaudRate(9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    connect(serial,&QSerialPort::readyRead,this,&MainWindow::readCo2Com);

}

void  MainWindow::CloseCo2Com()
{
    if(serial)
    {
        serial->close();
        delete serial;
    }
}

void MainWindow::readCo2Com()
{

    static int cc = 0;

    QByteArray buf;
    buf = serial->readAll();
    qDebug() << buf;
    QString a = QString(buf);

    qDebug()<<QString::number(cc++)<<a<<"\n";

}

void MainWindow::RunRosSpin()
{
     ros::spinOnce();
     QByteArray msg = "Hello world!";
     qint64 len = mSocket->writeDatagram(msg, QHostAddress("192.168.1.168"), 2333);

}

void MainWindow::updateSpi()
{
    unsigned char tx_Data[10]={1,2,3,4,5,6,7,8,9,10}; //定义读写的数据
    unsigned char rx_Data[10]={0,0,0,0,0,0,0,0,0,0};

    int len = spi.SPIDataRW(0,tx_Data,rx_Data,7); //向总线中写入7个数据

    char *str1 = (char *)rx_Data;

    QString str2 = QString(QLatin1String(str1));

    static int cnt = 0;
    qDebug()<<QString::number(cnt++)<<str2<<"\n";
}


