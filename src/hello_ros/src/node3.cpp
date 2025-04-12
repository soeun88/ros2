#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include <pthread.h>

ros::Publisher vel_feedback_pub;
int vel_cmd = 0;

void VelCmdCallback(const std_msgs::Int16::ConstPtr& msg)
{
    ROS_INFO("vel_cmd_check: [%d]", msg->data);
    vel_cmd = msg->data;
}

static void* thread_1(void *unused)
{
    while (ros::ok())
    {
        ROS_INFO("thread 1 working");


        std_msgs::Int16 msg_vel_feedback;

        msg_vel_feedback.data = vel_cmd + 100;

        vel_feedback_pub.publish(msg_vel_feedback);

        usleep(100000);
    }

    ErrorExit:
    return nullptr;
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "node3");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("vel_cmd", 1000, VelCmdCallback);

    vel_feedback_pub = n.advertise<std_msgs::Int16>("vel_feedback", 1000);


    pthread_t th[1];
    if(pthread_create(&th[0], nullptr, thread_1, nullptr) != 0){
        std::perror("pthread_create 0");
        std::exit(1);
    }
    if (pthread_detach(th[0]) != 0){
        std::perror("pthread_detach 0");
        std::exit(1);
    }

    ros::spin();

    return 0;
}