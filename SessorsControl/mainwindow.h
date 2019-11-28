#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QUdpSocket>
#include <QByteArray>
#include <QSerialPort>
#include <QDateTime>
#include "spi_control.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
using std::string;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void readCo2Com();
    void RunRosSpin();
    void updateSpi();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;//co2串口
    QUdpSocket *mSocket;//udp通信
    SPI_Control spi;//spi控制
    int fd;
    QTimer *readTimer;//co2传感器定时器
    QTimer *rosTimer;//ros定时器
    QTimer *timerSpi;
public:
    void OpenCo2Com();//打开co2传感器串口
    void CloseCo2Com();//关闭co2传感器串口
};

#endif // MAINWINDOW_H
