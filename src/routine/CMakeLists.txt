cmake_minimum_required(VERSION 3.5)
project(routine)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


include_directories(
  include
)

set(ROUTINE_SRCS
  src/routine.cpp
)
# find dependencies
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(routine ${ROUTINE_SRCS})
ament_target_dependencies(routine rclcpp std_msgs)

install(TARGETS
  routine
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
