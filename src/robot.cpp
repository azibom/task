#include "ros/ros.h"
#include "iostream"
#include "math.h"
#include "task/parsian_robot_status.h"
#include "task/parsian_robot.h"
#include "task/parsian_world_model.h"
#include "task/vector2D.h"
#include "task/result.h"
# define buf  10
# define threshold_y 5
# define threshold_x 5
using namespace std;
ros::Publisher publisher;
task::result msg3;
int robot_status,robot_x,robot_y,ball_y,ball_x,vision_sensor,decide_ar[11],sum,decide_pr,counter_aa=1;
bool robot_sensor=0,decide=1;
void a(const task::parsian_robot_status&msg)
{
    robot_status=msg.status;
    robot_sensor=msg.spinCatchBall;
    if(msg.battery<=3||msg.kickFault==0||msg.chipFault==0||msg.m1Fault==0||msg.m1Fault==0||msg.m2Fault==0||msg.m3Fault==0||msg.En4Fault==0||msg.En1Fault==0||msg.En2Fault==0||msg.En3Fault==0||msg.En4Fault==0)
        decide=0;
}
void b(const task::parsian_robot&msg1)
{
    robot_x=msg1.pos.x;
    robot_y=msg1.pos.y;
}
void c(const task::parsian_world_model&msg2)
{
    ball_x=msg2.ball.pos.x;
    ball_y=msg2.ball.pos.y;
    if(abs(ball_x-robot_x)<threshold_x && abs(ball_y-robot_y)<threshold_y )
        vision_sensor=1;
    else
        vision_sensor=0;
    if(vision_sensor==0 && robot_sensor==1)
        decide=0;
    if(counter_aa> buf ){
        decide_pr=decide;
        sum=0;
        for(int aa=1;aa<=buf;aa++)
            sum=decide_ar[aa]+sum;
        if(sum<=buf/2)
            decide=0;
        else
            decide=1;
        for(int bb=1;bb<buf;bb++)
            decide_ar[bb]=decide_ar[bb+1];
        decide_ar[buf]=decide_pr;
    }
    else
    {
        decide_ar[counter_aa]=decide;
        ROS_INFO_STREAM(" loading... ");
        counter_aa++;
    }
    msg3.r[robot_status] = decide;
    publisher.publish(msg3);
    decide=1;
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "robot");
    ros::NodeHandle n;
    publisher = n.advertise<task::result>("result", 1000);
    ros::Subscriber sub1 = n.subscribe("task/parsian_robot_status",1000,&a);
    ros::Subscriber sub2 = n.subscribe("task/parsian_robot",1000,&b);
    ros::Subscriber sub3 = n.subscribe("task/parsian_world_model",1000,&c);
    ros::spin();
}
