#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include <sstream>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "imu_test");

    ros::NodeHandle n;
    ros::Publisher imu_test_pub = n.advertise<std_msgs::String>("imu_test", 1000);
    ros::Publisher imu_test_pub2 = n.advertise<std_msgs::Int16>("compute_test_2", 1000);

    int count = 0;

    while (ros::ok())
    {
        std_msgs::String msg;
        std_msgs::Int16 msg_compute;

        std::stringstream ss;

        ss << "imu data " << count;
        msg.data = ss.str();
        msg_compute.data = count;

        ROS_INFO("%s", msg.data.c_str());
        imu_test_pub.publish(msg); 
        imu_test_pub2.publish(msg_compute); 

        count++;

        usleep(1000000);
    }

    return 0;
}