#include "ros/ros.h"
#include "std_msgs/Int16.h"

#include <pthread.h>

ros::Publisher sensor_1_pub;
void VelFeedback2Callback(const std_msgs::Int16::ConstPtr& msg)
{
    ROS_INFO("vel_feedback2_check: [%d]", msg->data);
}

static void* thread_1(void *unused)
{
    while (ros::ok())
    {
        ROS_INFO("thread 1 working");


        std_msgs::Int16 msg_sensor_1;

        msg_sensor_1.data = 100;

        sensor_1_pub.publish(msg_sensor_1);

        usleep(100000);
    }

    ErrorExit:
    return nullptr;
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "node1_1");
    ros::NodeHandle n;
    sensor_1_pub = n.advertise<std_msgs::Int16>("sensor_1", 1000);

    ros::Subscriber sub_vel_feedback2 = n.subscribe("vel_feedback2", 1000, VelFeedback2Callback);

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