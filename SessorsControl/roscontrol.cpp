#include "roscontrol.h"

RosControl *g_RosTopic = NULL;
RosControl::RosControl(QObject *parent) : QObject(parent)
{

}

RosControl::~RosControl()
{

}

RosControl::RosControl(string strPublish,string strSubscribe)
{
    pub_ = n_.advertise<std_msgs::String>(strPublish, 1000);
    sub_ = n_.subscribe(strSubscribe, 1000, &RosControl::callback, this);
}
//话题回调函数
void RosControl::callback(const std_msgs::String::ConstPtr& msg)
{
    //qDebug("I heard: [%s]\n", msg->data.c_str());
   emit StartRead(msg);
}

void RosControl::RunSpin()
{
    ros::spinOnce();
}

void RosControl::RosShutdown()
{
    ros::shutdown();
}
