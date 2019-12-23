#include "mainwindow.h"
#include <QApplication>



//话题回调函数
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
    qDebug("node_b is receiving [%s]", msg->data.c_str());
}

void chatterCallback2(const geometry_msgs::Twist::ConstPtr& cmd_vel)//这是一个回调函数，当接收到 chatter 话题的时候就会被调用
    //消息是以 boost shared_ptr 指针的形式传输，这就意味着你可以存储它而又不需要复制数据。
{
    static int  cnt = 0;
    cnt++;
    printf ("接收手机消息 %d： %f, %f, %f, %f, %f, %f\n",cnt,cmd_vel->linear.x,cmd_vel->linear.y,cmd_vel->linear.z,
            cmd_vel->angular.x,cmd_vel->angular.y,cmd_vel->angular.z);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

<<<<<<< HEAD
//    ros::init(argc, argv, "listener");
//    ros::NodeHandle n;
//      /*The subscribe() call is how you tell ROS that you want to receive messages on a given topic.  This invokes a call to the ROS master node, which keeps a registry of who is publishing and who is subscribing.  Messages are passed to a callback function, here called chatterCallback.  subscribe() returns a Subscriber object that you must hold on to until you want to unsubscribe.  When all copies of the Subscriber object go out of scope, this callback will automatically be unsubscribed from this topic.The second parameter to the subscribe() function is the size of the message queue.  If messages are arriving faster than they are being processed, this is the number of messages that will be buffered up before beginning to throw away the oldest ones.*/
//    ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
=======
    ros::init(argc, argv, "cmd_vel2");
    ros::NodeHandle n;
      /*The subscribe() call is how you tell ROS that you want to receive messages on a given topic.  This invokes a call to the ROS master node, which keeps a registry of who is publishing and who is subscribing.  Messages are passed to a callback function, here called chatterCallback.  subscribe() returns a Subscriber object that you must hold on to until you want to unsubscribe.  When all copies of the Subscriber object go out of scope, this callback will automatically be unsubscribed from this topic.The second parameter to the subscribe() function is the size of the message queue.  If messages are arriving faster than they are being processed, this is the number of messages that will be buffered up before beginning to throw away the oldest ones.*/
 //  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
    ros::Subscriber sub = n.subscribe("/turtle1/cmd_vel", 1000, chatterCallback2);
>>>>>>> d6ce994b76666b698932b5ae636c320701333735


    MainWindow w;
    w.show();


    return a.exec();
}
