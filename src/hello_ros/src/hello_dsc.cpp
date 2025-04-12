#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include <sstream>
#include <pthread.h>

ros::Publisher hello_dsc_pub1;
ros::Publisher hello_dsc_pub2;
ros::Publisher hello_dsc_pub3;

ros::Publisher hello_sub_test;

int count = 0;
int16_t thread_num_test = 0;
int16_t true_false_test = 0;

static void* thread_1(void *unused)
{
    while (ros::ok())
    {
        ROS_INFO("thread 1 working!!!");

        std_msgs::String msg;
        std_msgs::Int16 msg_safety;

        std::stringstream ss;

        ss<< "Lidar data" << count;
        msg.data = ss.str();

        msg_safety.data = true_false_test;

        ROS_INFO("%s", msg.data.c_str());
        ROS_INFO("%d", msg_safety.data);

        hello_dsc_pub1.publish(msg);
        hello_dsc_pub2.publish(msg_safety);

        count++;
        if(true_false_test == 0)
        {
            true_false_test = 1;
        }
        else if(true_false_test == 1)
        {
            true_false_test = 0;
        }

        thread_num_test++;
        usleep(1000000);
    }

    ErrorExit:
    return nullptr;
}

static void* thread_2(void *unused)
{
    while (ros::ok())
    {
        ROS_INFO("thread 2 working");
        ROS_INFO("thread_num_test: [%d]", thread_num_test);

        std_msgs::Int16 msg_compute;

        msg_compute.data = 100;

        hello_dsc_pub3.publish(msg_compute);

        usleep(100000);
    }

    ErrorExit:
    return nullptr;
}

int main(int argc, char **argv)
    {
    ros::init(argc, argv, "hello_dsc");

    ros::NodeHandle n1;

    hello_dsc_pub1 = n1.advertise<std_msgs::String>("Lidar_test", 1000);
    hello_dsc_pub2 = n1.advertise<std_msgs::Int16>("safety_test", 1000);
    hello_dsc_pub3 = n1.advertise<std_msgs::Int16>("compute_test_1", 1000);


    pthread_t th[2];
    if(pthread_create(&th[0], nullptr, thread_1, nullptr) != 0){
        std::perror("pthread_create 0");
        std::exit(1);
    }

    if(pthread_create(&th[1], nullptr, thread_2, nullptr) != 0){
        std::perror("pthread_create 1");
        std::exit(1);
    }

    if (pthread_detach(th[0]) != 0){
        std::perror("pthread_detach 0");
        std::exit(1);
    }

    if (pthread_detach(th[1]) != 0){
        std::perror("pthread_detach 1");
        std::exit(1);
    }

    ros::spin();

    return 0;
}