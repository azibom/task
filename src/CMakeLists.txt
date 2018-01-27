cmake_minimum_required(VERSION 2.8.3)
project(task)
find_package(catkin REQUIRED COMPONENTS message_generation std_msgs)
add_message_files(
  FILES
  parsian_robot_status.msg
  vector2D.msg 
  parsian_world_model.msg
  parsian_robot.msg
  result.msg
)
generate_messages(
  DEPENDENCIES
  std_msgs
)
catkin_package(
  CATKIN_DEPENDS message_runtime
)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
)
include_directories(include ${catkin_INCLUDE_DIRS})
add_executable(robot robot.cpp)
target_link_libraries(robot ${catkin_LIBRARIES})

