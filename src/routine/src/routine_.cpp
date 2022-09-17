#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/string.hpp"

#include <termios.h>
#include <sys/ioctl.h>
#include <sensor_msgs/ChannelFloat32>

// ---- Prototype
//===============
rclcpp::Node::SharedPtr routine = nullptr;
rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_sensor_data;
std_msgs::msg::Int32 vel_data;

// ---- Prototype Function
//========================
int keypress();
void printSomething();
float CllbkDataSensor();

void topic_callback(const std_msgs::msg::String & msg)
{
  std::cout << " " << msg.data;
}

// float CllbkDataSensor(const std_msgs::msg::Float32::ConstSharedPtr & msg)
// {
//     printf("%f", msg);
// }
void printSomething()
{
    std::cout << "Hi Timer" << std::endl;
}

int keypress()
{
    static const int STDIN = 0;
    static bool initialized = false;

    if (!initialized)
    {
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    routine = rclcpp::Node::make_shared("routine");
    auto pub_key_comm = routine->create_publisher<std_msgs::msg::Int32>("/pub_key_command", 10);
    auto sub_data_sensor = routine->create_subscription<std_msgs::msg::String>("/sensor_data", 10, topic_callback);
    // sub_sensor_data = routine->create_subscription<std_msgs::msg::Float32>("/sensor_data", 10, CllbkDataSensor);
    while (rclcpp::ok())
    {
        if (keypress())
        {
            auto c = std::cin.get();
            std::cout << c;
            switch (c)
            {
            case 'j':                
                vel_data.data = 0;
                pub_key_comm->publish(vel_data);
                break;
            case 'b':                
                vel_data.data = 1;
                pub_key_comm->publish(vel_data);
                break;
            case 'n':                
                vel_data.data = 2;
                pub_key_comm->publish(vel_data);
                break;
            case 'm':    
                vel_data.data = 3;
                pub_key_comm->publish(vel_data);
                break;
            case 'o':                
                vel_data.data = 4;
                pub_key_comm->publish(vel_data);
                break;
            case 'p':                
                vel_data.data = 5;
                pub_key_comm->publish(vel_data);
                break;
            case ' ':                
                vel_data.data = 6;
                pub_key_comm->publish(vel_data);
                break;
            }
        }
    }
    // auto timer_ = rclcpp::create_timer(routine, ros_clock, 0.5, 
    // [=](){
    //     printSomething();
    // });
    rclcpp::spin(routine);
    rclcpp::shutdown();
    return 0;
}