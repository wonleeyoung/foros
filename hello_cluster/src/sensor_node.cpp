
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <chrono>
#include <nlohmann/json.hpp>

using namespace std::chrono_literals;
using json = nlohmann::json;

using namespace std;

int imsi =1;



class SensorNode : public rclcpp::Node {
public:
    SensorNode(int sensor_id) : Node("sensor_node_" + std::to_string(sensor_id)), sensor_id_(sensor_id) {
        publisher_ = this->create_publisher<std_msgs::msg::String>("sensoring", 10);
        timer_ = this->create_wall_timer(2ms, std::bind(&SensorNode::timer_callback, this));
    }

private:
    void timer_callback() {
        json data = {
            {"sensor_id", sensor_id_},
            {"data", "Dummy data from sensor " + std::to_string(sensor_id_) + "   at " + to_string(imsi) }
        };
        auto message = std_msgs::msg::String();
        message.data = data.dump();
        
        imsi++;
        publisher_->publish(message);
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    }

    int sensor_id_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv) {
    rclcpp::init(argc, argv);
    int sensor_id = 3; // Now takes sensor_id from command line arguments
	
    auto sensor_node = std::make_shared<SensorNode>(sensor_id);
    rclcpp::spin(sensor_node);
    rclcpp::shutdown();
    return 0;
}