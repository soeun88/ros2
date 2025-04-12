#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include <pthread.h>

ros::Publisher vel_cmd_pub;

int16_t compute_1 = 0;
int16_t compute_2 = 0;

void SensorCallback(const std_msgs::String::ConstPtr& msg)
{
     ROS_INFO("hello_dsc_check: [%s]", msg->data.c_str());
}

void SensorSafetyCallback(const std_msgs::Int16::ConstPtr& msg)
{
     ROS_INFO("safety_check: [%d]", msg->data);
}

void ImuCallback(const std_msgs::String::ConstPtr& msg)
{
     ROS_INFO("imu_check: [%s]", msg->data.c_str());
}

void Compute1Callback(const std_msgs::Int16::ConstPtr& msg)
{
     ROS_INFO("compute1_check: [%s]", msg->data);
     compute_1 = msg->data;
}

void Compute2Callback(const std_msgs::Int16::ConstPtr& msg)
{
     ROS_INFO("compute2_check: [%s]", msg->data);
     compute_2 = msg->data;
}


static void* thread_1(void *unused)
{
     while (ros::ok())
     {
          ROS_INFO("Controller thread 1 working!!!");

          std_msgs::Int16 msg_vel_com;

          msg_vel_com.data = compute_1 + compute_2;

          ROS_INFO("vel com: [%d]", msg_vel_com.data);

          vel_cmd_pub.publish(msg_vel_com);

          usleep(100000);
     }

     ErrorExit:
     return nullptr;
}

int main(int argc, char **argv)
{
     ros::init(argc, argv, "controller");

     ros::NodeHandle n;
     ros::Subscriber sub_lidar = n.subscribe("Lidar_test", 1000, SensorCallback);
     ros::Subscriber sub_safety = n.subscribe("safety_test", 1000, SensorSafetyCallback);
     ros::Subscriber sub_imu = n.subscribe("imu_test", 1000, ImuCallback);

     ros::Subscriber sub_com1 = n.subscribe("compute_test_1", 1000, Compute1Callback);
     ros::Subscriber sub_com2 = n.subscribe("compute_test_2", 1000, Compute2Callback);

     vel_cmd_pub = n.advertise<std_msgs::Int16>("vel_cmd",1000);

     pthread_t th[1];
     if(pthread_create(&th[0], nullptr, thread_1, nullptr ) != 0){
          std::perror("pthread_create 0");
          std::exit(1);
     }

     if (pthread_detach(th[0]) != 0){{
          std::perror("pthread_detach 0");
          std::exit(1);
     }}

     ros::spin();

     return 0;
}