#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

#include "routine/kinematics.h"
#include "routine/motion.h"

using std::placeholders::_1;

using namespace std;

//---prototype
//===============
void CllbkSensor4(const std_msgs::msg::Float32::SharedPtr msg);
void CllbkSensor5(const std_msgs::msg::Float32::SharedPtr msg);
void CllbkSubPosXRobot(const std_msgs::msg::Float32::SharedPtr msg);
void CllbkSubPosZRobot(const std_msgs::msg::Float32::SharedPtr msg);
void CllbkSubPosXTarget(const std_msgs::msg::Float32::SharedPtr msg);
void CllbkSubPosZTarget(const std_msgs::msg::Float32::SharedPtr msg);

//---Node
//========
rclcpp::Node::SharedPtr routine = nullptr;

//---Sensor
//==========
float sensor[1];

//---Robot Position
//=================
float pos_robot[2];
float pos_target[2];

//---Vars
//========
float _kp_x = 0.5;
float _kp_z = 0.5;

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    routine = rclcpp::Node::make_shared("routine");
    rclcpp::WallRate loop_rate(10);

    //---Subscriber
    //==============
    auto sub_sensor_4 = routine->create_subscription<std_msgs::msg::Float32>("/sensor_data_4", 10, CllbkSensor4);
    auto sub_sensor_5 = routine->create_subscription<std_msgs::msg::Float32>("/sensor_data_5", 10, CllbkSensor5);
    auto sub_pos_x_robot = routine->create_subscription<std_msgs::msg::Float32>("/pos_x_robot", 10, CllbkSubPosXRobot);
    auto sub_pos_y_robot = routine->create_subscription<std_msgs::msg::Float32>("/pos_z_robot", 10, CllbkSubPosZRobot);
    auto sub_pos_x_target = routine->create_subscription<std_msgs::msg::Float32>("/pos_x_target", 10, CllbkSubPosXTarget);
    auto sub_pos_y_target = routine->create_subscription<std_msgs::msg::Float32>("/pos_z_target", 10, CllbkSubPosZTarget);

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
    float delta_x = 0;
    float delta_z = 0;
    float delta_posisi = 0;
    float vx, vz = 0;

    while (rclcpp::ok())
    {
        printf("%f %f || %f %f\n", pos_robot[0], pos_robot[1], pos_target[0], pos_target[1]);

        delta_x = pos_target[0] - pos_robot[0];
        delta_z = pos_target[1] - pos_robot[1];

        delta_posisi = sqrt(pow(delta_x, 2) + pow(delta_z, 2));

        if (delta_posisi < 0.3)
        {
            vx = vz = 0;
        }
        else
        {
            vx = getProportional(_kp_x, delta_x);
            vz = getProportional(_kp_z, delta_z);
        }

        float vMotor[2];
        float vOut[2];
        // JalanPosisiSudut(10, 20, 10, vMotor, vOut);
        inverseKinematics(vx, vz, vMotor);
        printf("V Motor: %f %f\n", vMotor[0], vMotor[1]);
        // printf("VY: %f || VZ: %f\n", vy, vz);
        // printf("%f %f \n", vMotor[0], vMotor[1]);
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

void CllbkSubPosXRobot(const std_msgs::msg::Float32::SharedPtr msg)
{
    printf("%f", msg->data);
    pos_robot[0] = msg->data;
}

void CllbkSubPosZRobot(const std_msgs::msg::Float32::SharedPtr msg)
{
    pos_robot[1] = msg->data;
}

void CllbkSubPosXTarget(const std_msgs::msg::Float32::SharedPtr msg)
{
    pos_target[0] = msg->data;
}

void CllbkSubPosZTarget(const std_msgs::msg::Float32::SharedPtr msg)
{
    pos_target[1] = msg->data;
}