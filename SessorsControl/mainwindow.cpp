#include "mainwindow.h"
#include "ui_mainwindow.h"

uint8_t PacketBufferGlobal[96] = {0};//接受数据数组
uint8_t SerBuf[64] = {0};//发送数据数组
////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bOptimumIsOpen = OpenOptimum();
    if(bOptimumIsOpen == false)
    {
        qDebug()<<"光照传感器打开失败";
    }
//    OpenCo2Com();//打开co2传感器串口

 //   InitRos();//初始化节点

//    rosTimer = new  QTimer(this);
//    rosTimer->start(100);
//    connect( rosTimer, SIGNAL(timeout()),this, SLOT(RunRosSpin()) );

//    fd = spi.Open(0,5000000,1);//打开spi设备

//    timerSpi = new QTimer(this);
//    connect(timerSpi, SIGNAL(timeout()), this, SLOT(updateSpi()));
//    timerSpi->start(1000);

    //初始化创建QUdpSocket对象
     mSocket = new QUdpSocket();
}

MainWindow::~MainWindow()
{
    if(bOptimumIsOpen)
        usb.Close();
 //   CloseCo2Com();//关闭co2传感器串口

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
//     QByteArray msg = "Hello world!";
//     qint64 len = mSocket->writeDatagram(msg, QHostAddress("192.168.1.168"), 2333);

}

void MainWindow::updateSpi()
{

    if(!bOptimumIsOpen)
        return;
    int ret = usb.Measure(20);
    if (ret != 0)
    {
        qDebug()<<"optimum sessor Measure failure!";
        return;
    }

    double lx,glx;
    usb.GetLxAndGLx(lx,glx);

    qDebug()<<QString::number(lx,'f',2)/*<<QString::number(glx,'f',2)*/;

//    unsigned char tx_Data[10]={1,2,3,4,5,6,7,8,9,10}; //定义读写的数据
//    unsigned char rx_Data[10]={0,0,0,0,0,0,0,0,0,0};

//    int len = spi.SPIDataRW(0,tx_Data,rx_Data,7); //向总线中写入7个数据

//    char *str1 = (char *)rx_Data;

//    QString str2 = QString(QLatin1String(str1));

//    static int cnt = 0;
//    qDebug()<<QString::number(cnt++)<<str2<<"\n";
}

bool MainWindow::SendSPICommandByte(uint8_t SPICommandByte)
{
    bool result = false;

    uint8_t b = 0;
   do
   {
       SerBuf[0] = SPICommandByte;
       spi.SPIDataRW(0,SerBuf,PacketBufferGlobal,1);
       if ((int)SerBuf[0] != 0xF3)
       {
          usleep(10);
           b += 1;
       }
   }
   while ((int)SerBuf[0] != 0xF3 && (int)b <= 9);

   if ((int)PacketBufferGlobal[1] == 0xF3)
   {
       result = true;
   }

    return result;
}

ushort MainWindow::Get_MODBUS_CRC(uint8_t *ByteArray, uint8_t NumOfBytes)
{
   ushort num = 65535;
   uint8_t b = 0;
   uint8_t b2 = (uint8_t)((int)NumOfBytes - 1);
   if (b <= b2)
   {
       b2 += 1;
       do
       {
           num = (ushort)((uint)num ^ (uint)((ushort)ByteArray[(int)b]));
           uint8_t b3 = 0;
           uint8_t b4 = 7;
           if (b3 <= b4)
           {
               b4 += 1;
               do
               {
                   if (((int)num & 1) == 1)
                   {
                       num = (ushort)((uint)((int)num) >> 1);
                       num = (ushort)((int)num ^ 40961);
                   }
                   else
                   {
                       num = (ushort)((uint)((int)num) >> 1);
                   }
                   b3 += 1;
               }
               while (b3 != b4);
           }
           b += 1;
       }
       while (b != b2);
   }
   return num;
}

bool MainWindow::WritePacketBufferBytes(uint8_t NumOfBytes)
{
   SerBuf[0] = 0x01;
   uint8_t b = 1;
   uint16_t cnt = 0;
   if (b <= NumOfBytes)
   {
       uint8_t b2 = NumOfBytes+1;

       do
       {
           uint8_t data = 0;
           spi.SPIDataRW(0,SerBuf,&data,1);//
           PacketBufferGlobal[cnt++] = data;
           b += 1;
           usleep(20);//delay 20 us
       }
       while (b != b2);
   }
   return true;
}

void MainWindow::OperateSpi(bool bStart)//bStart:true,startup;false,stop
{
   uint8_t data = 0;
   if(bStart)
   {
       //turn fan on
       SerBuf[0] = 0x03;
       spi.SPIDataRW(0,SerBuf,&data,1);//
       usleep(20);
       SerBuf[0] = 0x03;
       spi.SPIDataRW(0,SerBuf,&data,1);//

       usleep(20);
       //turn laser on
       SerBuf[0] = 0x03;
       spi.SPIDataRW(0,SerBuf,&data,1);//
       usleep(20);
       SerBuf[0] = 0x07;
       spi.SPIDataRW(0,SerBuf,&data,1);//
   }
   else {
       //turn fan off
       SerBuf[0] = 0x03;
       spi.SPIDataRW(0,SerBuf,&data,1);//
       usleep(20);
       SerBuf[0] = 0x02;
       spi.SPIDataRW(0,SerBuf,&data,1);//

       usleep(20);
       //turn laser off
       SerBuf[0] = 0x03;
       spi.SPIDataRW(0,SerBuf,&data,1);//
       usleep(20);
       SerBuf[0] = 0x06;
       spi.SPIDataRW(0,SerBuf,&data,1);//
   }
}

bool MainWindow::OpenOptimum()
{
    double WArray[2048];
    int dataLenght;
    int ret = -1;
    ret = usb.Init(WArray, &dataLenght);
    if (ret != 0)
    {
        qDebug()<<"optimum sessor init failure!";
        return false;
    }

    return true;
}


void MainWindow::on_pushButton_clicked()
{
    QFile file;
    QDateTime time =QDateTime::currentDateTime();
    QString strDate = time.toString("yyyy-MM-dd hh-mm-ss");

    QString strPath;// = "/home/corvin/Desktop/raspberry_files/" + strDate + ".csv";
    strPath.append("/home/corvin/Desktop/raspberry_files/");
    strPath.append(strDate);
    strPath.append(".csv");

    file.setFileName(strPath);

    if(!file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append))
    {
        qDebug()<<"open file failed!";
        return;
    }

    QTextStream textStream;
    textStream.setDevice(&file);

    textStream<<strDate<<","<<"1"<<","<<"2"<<endl;//用","隔开，内容写如相邻的三个单元格
    textStream.flush();

    file.close();

}
