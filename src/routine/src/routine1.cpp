#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32.hpp"
// #include "routine_main/msg/robot_pos.hpp"

using std::placeholders::_1;
float dist_sensor_4;
float dist_sensor_5;
float robot_pos_x;
float robot_pos_y;
float status_rotasi;
float status_jaga_jarak;

std_msgs::msg::Float32 msg_status_rotasi;
std_msgs::msg::Float32 msg_status_jaga_jarak;

rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_status_rotasi;
rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_status_jaga_jarak;
class Routine : public rclcpp::Node
{
public:
  Routine()
      : Node("routine")
  {

    // std::cout << "Routine node started" << std::endl;
    sub_data_4 = this->create_subscription<std_msgs::msg::Float32>(
        "/sensor_data_4", 10, std::bind(&Routine::CllbkDataSensor4, this, _1));
    sub_data_5 = this->create_subscription<std_msgs::msg::Float32>(
        "/sensor_data_5", 10, std::bind(&Routine::CllbkDataSensor5, this, _1));
    sub_robot_pos_x = this->create_subscription<std_msgs::msg::Float32>(
        "/robot_pos_x", 10, std::bind(&Routine::CllbckRobotPosX, this, _1));
    sub_robot_pos_y = this->create_subscription<std_msgs::msg::Float32>(
        "/robot_pos_y", 10, std::bind(&Routine::CllbckRobotPosY, this, _1));
    pub_status_jaga_jarak = this->create_publisher<std_msgs::msg::Float32>(
        "/status_jaga_jarak", 10);
    pub_status_rotasi = this->create_publisher<std_msgs::msg::Float32>(
        "/status_rotasi", 10);

    // std::cout << "status jaa jarak: " << status_jaga_jarak << std::endl;
    // std::cout << "status rotasi: " << status_rotasi << std::endl;
    // std::cout << "data Sensor: " << dist_sensor_4 << " " << dist_sensor_5 << std::endl;
  }

private:
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_data_4;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_data_5;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_robot_pos_x;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_robot_pos_y;

  void CllbkDataSensor4(const std_msgs::msg::Float32::SharedPtr msg) const
  {
    // std::cout << "dist 4" << msg->data << std::endl;
    dist_sensor_4 = msg->data;
  }
  void CllbkDataSensor5(const std_msgs::msg::Float32::SharedPtr msg) const
  {
    // std::cout << "dist 5" << msg->data << std::endl;
    dist_sensor_5 = msg->data;
  }
  void CllbckRobotPosX(const std_msgs::msg::Float32::SharedPtr msg) const
  {
    robot_pos_x = msg->data;
  }
  void CllbckRobotPosY(const std_msgs::msg::Float32::SharedPtr msg) const
  {
    robot_pos_y = msg->data;
  }
  float Pythagoras(float _x1, float _y1, float _x2, float _y2)
  {
    float delta_x = _x1 - _x2;
    float delta_y = _y1 - _y2;

    return sqrt(delta_x * delta_x + delta_y * delta_y);
  }
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  while (rclcpp::ok())
  {

    
    if (dist_sensor_4 < 0.5 && dist_sensor_5 < 0.5)
      status_jaga_jarak = 1;
    else
      status_jaga_jarak = 0;

    if (fabs(dist_sensor_4 - dist_sensor_5) > 0.05)
    {
      if (dist_sensor_4 > dist_sensor_5)
      {
        // rotasi ke kiri
        status_rotasi = 1;
      }
      else
      {
        status_rotasi = 2;
      }
    }
    else
      status_rotasi = 0;
    msg_status_rotasi.data = status_rotasi;
    std::cout << "Rotasi" << msg_status_rotasi.data << std::endl;
    pub_status_rotasi->publish(msg_status_rotasi);

    msg_status_jaga_jarak.data = status_jaga_jarak;
    std::cout << "Jaga Jarak" << msg_status_jaga_jarak.data << std::endl;
    pub_status_jaga_jarak->publish(msg_status_jaga_jarak);
  }
  rclcpp::spin(std::make_shared<Routine>());
  rclcpp::shutdown();
  return 0;
}