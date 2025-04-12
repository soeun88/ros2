#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include <sstream>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "node1_2");

    ros::NodeHandle n;

    ros::Publisher sensor2_pub = n.advertise<std_msgs::Int16>("sensor_2", 1000);

    int sensor2_value = 0;

    while (ros::ok())
    {
        std_msgs::String msg;
        sensor2_value += 2;
        msg.data = sensor2_value;
        sensor2_pub.publish(msg);

        usleep(1000000);
    }

    return 0;
}