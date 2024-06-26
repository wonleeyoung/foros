
cmake_minimum_required(VERSION 3.8)
project(hello_cluster)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(foros REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
#find_package(nlohmann_json REQUIRED)


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()


add_executable(sensor_node src/sensor_node.cpp)
ament_target_dependencies(sensor_node
  rclcpp
  std_msgs
  foros
  )



# Add "hello_cluster" exectuable target

add_executable(hello_cluster src/hello_cluster.cpp)
ament_target_dependencies(hello_cluster
	  rclcpp
	    std_msgs
	      foros
	      )


#target_link_libraries(${PROJECT_NAME} #PRIVATE nlohmann_json::nlohmann_json)


# Install
install(TARGETS
  sensor_node
  hello_cluster
  DESTINATION lib/${PROJECT_NAME}
)