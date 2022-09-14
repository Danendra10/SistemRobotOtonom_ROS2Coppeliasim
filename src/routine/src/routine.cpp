#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

#include <termios.h>
#include <sys/ioctl.h>

// ---- Prototype
//===============
rclcpp::Node::SharedPtr routine = nullptr;
std_msgs::msg::Int32 vel_data;

// ---- Prototype Function
//========================
int keypress();

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
    while (rclcpp::ok())
    {
        if (keypress())
        {
            auto c = std::cin.get();
            std::cout << c;
            switch (c)
            {
            case 'j':
                // printf("Yang masuk: %d", c);
                vel_data.data = 0;
                // vel_data.data = 0;
                pub_key_comm->publish(vel_data);
                break;
            case 'b':
                // printf("Yang masuk: %d", c);
                vel_data.data = 1;
                // vel_data.data = 0;
                pub_key_comm->publish(vel_data);
                break;
            case 'n':
                // printf("Yang masuk: %d", c);
                vel_data.data = 2;
                // vel_data.data = 0;
                pub_key_comm->publish(vel_data);
                break;
            case 'm':
                // printf("Yang masuk: %d", c);
                vel_data.data = 3;
                // vel_data.data = 0;
                pub_key_comm->publish(vel_data);
                break;
            case 'o':
                // printf("Yang masuk: %d", c);
                vel_data.data = 4;
                // vel_data.data = 0;
                pub_key_comm->publish(vel_data);
                break;
            case 'p':
                // printf("Yang masuk: %d", c);
                vel_data.data = 5;
                // vel_data.data = 0;
                pub_key_comm->publish(vel_data);
                break;
            case ' ':
                // printf("Yang masuk: %d", c);
                vel_data.data = 6;
                // vel_data.data = 0;
                pub_key_comm->publish(vel_data);
                break;
            }
        }
    }

    rclcpp::spin(routine);
    rclcpp::shutdown();
    return 0;
}