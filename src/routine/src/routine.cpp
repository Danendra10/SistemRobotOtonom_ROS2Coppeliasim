#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"


#include "routine/kinematics.h"

using std::placeholders::_1;

using namespace std;

//---prototype
//===============
void CllbkSensor4(const std_msgs::msg::Float32::SharedPtr msg);
void CllbkSensor5(const std_msgs::msg::Float32::SharedPtr msg);

//---Node
//========
rclcpp::Node::SharedPtr routine = nullptr;

//---Sensor
//==========
float sensor[1];

//---Vars
//========
float _kp_trans = 0.5;
float _kp_rot = 6;

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    routine = rclcpp::Node::make_shared("routine");
    rclcpp::WallRate loop_rate(10);

    //---Subscriber
    //==============
    auto sub_sensor_4 = routine->create_subscription<std_msgs::msg::Float32>("/sensor_data_4", 10, CllbkSensor4);
    auto sub_sensor_5 = routine->create_subscription<std_msgs::msg::Float32>("/sensor_data_5", 10, CllbkSensor5);

    //---Publisher
    //=============
    auto pub_v_left = routine->create_publisher<std_msgs::msg::Float32>("/v_left", 10);
    auto pub_v_right = routine->create_publisher<std_msgs::msg::Float32>("/v_right", 10);

    //---Message
    //===========
    std_msgs::msg::Float32 msg_motor_left;
    std_msgs::msg::Float32 msg_motor_right;

    //---Variable
    //============
    float delta_sensor = 0;
    float distance = 0;
    float delta_rotation = 0;
    float vy, vz = 0;

    while (rclcpp::ok())
    {
        // printf("Sensor 4 : %f | Sensor 5 : %f\n", sensor[0], sensor[1]);
        delta_sensor = sensor[0] - sensor[1];

        if(sensor[0] > sensor[1])
            distance = sensor[1] -0.4;
        else
            distance = sensor[0] -0.4;

        if(distance > 1)
            distance = 0;
        
        //---PID
        //======
        vz = getProportional(_kp_rot, delta_sensor);
        vy = getProportional(_kp_trans, distance);

        float vMotor[2];
        inverseKinematics(vy, vz, vMotor);
        printf("VY: %f || VZ: %f\n", vy, vz);
        printf("%f %f \n", vMotor[0], vMotor[1]);
        float vLeft = vMotor[0];
        float vRight = vMotor[1];
        msg_motor_left.data = vLeft;
        msg_motor_right.data = vRight;

        pub_v_left->publish(msg_motor_left);
        pub_v_right->publish(msg_motor_right);

        rclcpp::spin_some(routine);
        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}

void CllbkSensor4(const std_msgs::msg::Float32::SharedPtr msg)
{
 sensor[0] = msg->data;
}

void CllbkSensor5(const std_msgs::msg::Float32::SharedPtr msg)
{
 sensor[1] = msg->data;
}