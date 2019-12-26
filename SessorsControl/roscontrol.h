#ifndef ROSCONTROL_H
#define ROSCONTROL_H

#include <QObject>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
using std::string;

class RosControl : public QObject
{
    Q_OBJECT
public:
    explicit RosControl(QObject *parent = nullptr);

    ~RosControl();
    RosControl(string strPublish,string strSubscribe);
    void RunSpin();
    void RosShutdown();

    void callback(const std_msgs::String::ConstPtr& msg);
signals:
    void StartRead(const std_msgs::String::ConstPtr& msg);//自定义信号
public slots:
private:
private:
  ros::NodeHandle n_;
  ros::Publisher pub_;
  ros::Subscriber sub_;
};

extern RosControl *g_RosTopic;

#endif // ROSCONTROL_H
