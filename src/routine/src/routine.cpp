#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32.hpp"
using std::placeholders::_1;
_Float32 dist_sensor_4;
class Routine : public rclcpp::Node
{
  public:
    Routine()
    : Node("routine")
    {
      std::cout << "data dist 4: " << dist_sensor_4 << std::endl;
      sub_data_4 = this->create_subscription<std_msgs::msg::Float32>(
      "/sensor_data_4", 10, std::bind(&Routine::CllbkDataSensor4, this, _1));
      sub_data_4 = this->create_subscription<std_msgs::msg::Float32>(
      "/sensor_data_5", 10, std::bind(&Routine::CllbkDataSensor5, this, _1));
    }

  private:
    void CllbkDataSensor4(const std_msgs::msg::Float32::SharedPtr msg) const
    {
      dist_sensor_4 = msg->data;
    }
    void CllbkDataSensor5(const std_msgs::msg::Float32::SharedPtr msg) const
    {
      // std::cout << "data 5 : " << msg->data << std::endl;
    }
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_data_4;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_data_5;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Routine>());
  
  rclcpp::shutdown();
  return 0;
}