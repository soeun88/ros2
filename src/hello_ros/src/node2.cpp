#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include <pthread.h>

ros::Publisher vel_cmd_pub;
ros::Publisher vel_feedback2_pub;

int16_t sensor_1 = 0;
int16_t sensor_2 = 0;
int16_t vel_feedback = 0;

void Sensor1Callback(const std_msgs::Int16::ConstPtr& msg)
{
     ROS_INFO("sensor1_check: [%s]", msg->data);
     sensor_1 = msg->data;
}

void Sensor2Callback(const std_msgs::Int16::ConstPtr& msg)
{
     ROS_INFO("sensor2_check: [%s]", msg->data);
     sensor_2 = msg->data;
}

void VelFeedbackCallback(const std_msgs::Int16::ConstPtr& msg)
{
     ROS_INFO("vel_feedback_check: [%s]", msg->data);
     vel_feedback = msg->data;
}



static void* thread_1(void *unused)
{
     while (ros::ok())
     {
          ROS_INFO("node2 thread 1 working!!!");

          std_msgs::Int16 msg_vel_cmd;

          msg_vel_cmd.data = sensor_1 + sensor_2;

          ROS_INFO("vel com: [%d]",msg_vel_cmd.data);

          vel_cmd_pub.publish(msg_vel_cmd);

          usleep(100000);
     }

     ErrorExit:
     return nullptr;
}

static void* thread_2(void *unused)
{
     while (ros::ok())
     {
          ROS_INFO("node2 thread 2 working!!!");

          std_msgs::Int16 msg_vel_feedback2;

          msg_vel_feedback2.data = vel_feedback/10;

          ROS_INFO("vel_feedback2: [%d]", msg_vel_feedback2.data);

          vel_feedback2_pub.publish(msg_vel_feedback2);

          usleep(1000000);
     }

     ErrorExit:
     return nullptr;
}

int main(int argc, char **argv)
{
     ros::init(argc, argv, "node2");

     ros::NodeHandle n;

     vel_cmd_pub = n.advertise<std_msgs::Int16>("vel_cmd", 1000);
     vel_feedback2_pub = n.advertise<std_msgs::Int16>("vel_feedback2", 1000);

     ros::Subscriber sub_com1 = n.subscribe("sensor_1", 1000, Sensor1Callback);
     ros::Subscriber sub_com2 = n.subscribe("sensor_2", 1000, Sensor2Callback);
     ros::Subscriber sub_com3 = n.subscribe("vel_feedback", 1000, VelFeedbackCallback);

     pthread_t th[2];
     if(pthread_create(&th[0], nullptr, thread_1, nullptr ) != 0){
          std::perror("pthread_create 0");
          std::exit(1);
     }

     if (pthread_detach(th[0]) != 0){{
          std::perror("pthread_detach 0");
          std::exit(1);
     }}

     if(pthread_create(&th[1], nullptr, thread_2, nullptr ) != 0){
          std::perror("pthread_create 1");
          std::exit(1);
     }

     if (pthread_detach(th[1]) != 0){{
          std::perror("pthread_detach 1");
          std::exit(1);
     }}


     ros::spin();

     return 0;
}