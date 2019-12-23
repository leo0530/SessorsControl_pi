#include "mainwindow.h"
#include <QApplication>



//话题回调函数
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
    qDebug("node_b is receiving [%s]", msg->data.c_str());
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    ros::init(argc, argv, "listener");
//    ros::NodeHandle n;
//      /*The subscribe() call is how you tell ROS that you want to receive messages on a given topic.  This invokes a call to the ROS master node, which keeps a registry of who is publishing and who is subscribing.  Messages are passed to a callback function, here called chatterCallback.  subscribe() returns a Subscriber object that you must hold on to until you want to unsubscribe.  When all copies of the Subscriber object go out of scope, this callback will automatically be unsubscribed from this topic.The second parameter to the subscribe() function is the size of the message queue.  If messages are arriving faster than they are being processed, this is the number of messages that will be buffered up before beginning to throw away the oldest ones.*/
//    ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);


    MainWindow w;
    w.show();


    return a.exec();
}
