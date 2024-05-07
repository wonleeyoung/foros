#include <chrono>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include "akit/failover/foros/cluster_node.hpp"
//#include "raft/state.hpp"

#include <nlohmann/json.hpp>
#include <mutex>

// 추가 파트
//#include <state_machine.hpp>
using namespace std::chrono_literals;
using json = nlohmann::json;

class HelloCluster : public akit::failover::foros::ClusterNode {
public:
    uint32_t c_id; // leader 노드 ID 는 저장하기 위한 id
    HelloCluster(const std::string& cluster_name, uint32_t node_id, const std::vector<uint32_t>& node_ids)
    : ClusterNode(cluster_name, node_id, node_ids) {
        sensor_subscriber_ = this->create_subscription<std_msgs::msg::String>(
            "sensoring", 50, std::bind(&HelloCluster::sensor_data_callback, this, std::placeholders::_1));
        RCLCPP_INFO(this->get_logger(), "HelloCluster Node initialized and listening on sensoring topic");
        c_id = node_id;
        
        // peer 로 부터 받아오는 부분
        peer_subscriber_ = this->create_subscription<std_msgs::msg::String>(
            "hello_cluster", 50, std::bind(&HelloCluster::peer_data_callback, this, std::placeholders::_1));
    }
    std::string data;
    std::mutex mutex_;

private:
    void sensor_data_callback(const std_msgs::msg::String::SharedPtr msg) {
        try {

            // 여기에 leader인 경우에만 받게끔하자.
            /*
            if (this->is_activated()){
                std::lock_guard<std::mutex> lock(mutex_); // lock 획득 시도

                auto received_data = json::parse(msg->data);
                data = received_data["data"];
                
                RCLCPP_INFO(this->get_logger(), "Received sensor data: %s", data.c_str());
                // Verify data - Example: Check if the data contains expected string
                if (data.find("Dummy data from sensor") != std::string::npos) {
                    RCLCPP_INFO(this->get_logger(), "Data verified successfully: %s", data.c_str());
                } else {
                    RCLCPP_ERROR(this->get_logger(), "Data verification failed: %s", data.c_str());
                }
                data = data + " from leader node " + std::to_string(c_id);

            }
            */
           
            if(this->is_candidate_from_raft()){
                RCLCPP_INFO(this->get_logger(), "candidate ");
                //this->insert_entry_buffer("123131231321");
            }

            if(this->is_standby()){
                RCLCPP_INFO(this->get_logger(), "standby ");
            }

            if(this->is_follower()){
                RCLCPP_INFO(this->get_logger(), "follower ");
            }

            if(this->is_stay()){
                RCLCPP_INFO(this->get_logger(), "stay ");
            }
            
            if(this->is_leader()){
                RCLCPP_INFO(this->get_logger(), "leader ");
            }
            
            
/*
            if(this->life_active()){
               RCLCPP_INFO(this->get_logger(), "active ");
            }

            if(this->life_standby()){
                RCLCPP_INFO(this->get_logger(), "standby ");
            }

            if(this->life_inactive()){
                RCLCPP_INFO(this->get_logger(), "inactive ");
            }

            if(this->life_unknown()){
                RCLCPP_INFO(this->get_logger(), "unknown ");
            }
*/
 
            if ( this->is_leader() ){
                std::lock_guard<std::mutex> lock(mutex_); // lock 획득 시도

                auto received_data = json::parse(msg->data);
                data = received_data["data"];
                
                //RCLCPP_INFO(this->get_logger(), "Received sensor data: %s", data.c_str());
                // Verify data - Example: Check if the data contains expected string
                if (data.find("Dummy data from sensor") != std::string::npos) {
                    RCLCPP_INFO(this->get_logger(), "Data verified successfully: %s", data.c_str());
                } else {
                    RCLCPP_ERROR(this->get_logger(), "Data verification failed: %s", data.c_str());
                }
                data = data + " from leader node " + std::to_string(c_id);

            }

            if ( this->is_candidate_from_raft() ){
                std::lock_guard<std::mutex> lock(mutex_); // lock 획득 시도

                auto received_data = json::parse(msg->data);
                data = received_data["data"];
                
                //RCLCPP_INFO(this->get_logger(), "Received sensor data: %s", data.c_str());
                // Verify data - Example: Check if the data contains expected string
                if (data.find("Dummy data from sensor") != std::string::npos) {
                    RCLCPP_INFO(this->get_logger(), "Data verified successfully: %s", data.c_str());
                } else {
                    RCLCPP_ERROR(this->get_logger(), "Data verification failed: %s", data.c_str());
                }
                data = data + " from leader node " + std::to_string(c_id);

            }
           

        } catch (const json::parse_error& e) {
            RCLCPP_ERROR(this->get_logger(), "JSON parsing error: '%s'", e.what());
        }
    }

    void peer_data_callback(const std_msgs::msg::String::SharedPtr msg) {
        if (!(this->is_activated())){
            std::lock_guard<std::mutex> lock(mutex_); // lock 획득 시도

            // auto received_data = json::parse(msg->data);
            // std::string data1 = received_data["data"];
            std::string data1 = msg->data;
            //RCLCPP_INFO(this->get_logger(), "i'm from leader node!! : %s", data1.c_str());
            // Verify data - Example: Check if the data contains expected string
            if (data1.find("Dummy data from sensor") != std::string::npos) {
                //RCLCPP_INFO(this->get_logger(), "Data verified successfully!!!: %s", data1.c_str());
            } else {
                //RCLCPP_ERROR(this->get_logger(), "Data verification failed!!!: %s", data1.c_str());
            }
        }
    }
    
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sensor_subscriber_;
    // 이 부분은 다른 node로 부터 데이터를 받아오는 부분
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr peer_subscriber_;



};

int main(int argc, char **argv) {
  const std::string kClusterName = "hello_cluster";
  const std::string kTopicName = "hello_cluster";
  const std::vector<uint32_t> kClusterNodeIds = {0, 1, 2, 3, 4};

  rclcpp::Logger logger = rclcpp::get_logger("hello_cluster_node");
  logger.set_level(rclcpp::Logger::Level::Info);

 // if (argc < 2) {
 //   RCLCPP_ERROR(logger, "Usage : %s {node ID} {size of cluster}", argv[0]);
 //   return -1;
 // }

  uint32_t id = std::stoul(argv[1]);
  if (find(kClusterNodeIds.begin(), kClusterNodeIds.end(), id) ==
      kClusterNodeIds.end()) {
    RCLCPP_ERROR(logger, "Node ID must be among 0, 1, 2");
    return -1;
  }

  rclcpp::init(argc, argv);
  auto node = std::make_shared<HelloCluster>(kClusterName, id, kClusterNodeIds);
  auto publisher = node->create_publisher<std_msgs::msg::String>(kTopicName, 3);

  // 타이머 안에서 node의 subscription data를 가져온다.

  auto timer_duration_ms = std::stoi(argv[2]);
  auto timer_duration = std::chrono::milliseconds(timer_duration_ms);

  auto timer_ = node->create_wall_timer(timer_duration, [&]() {
    std::string sub_data;
    {
        std::lock_guard<std::mutex> lock(node->mutex_);
        sub_data = node->data;
    }

    node->insert_entry_buffer(sub_data);

    auto msg = std_msgs::msg::String();
    //msg.data = std::to_string(id);
    msg.data = sub_data;
    publisher->publish(msg);
    //RCLCPP_INFO(logger, "Publishing cluster message: %s", msg.data.c_str());
  });

  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();

  // wonyeong
  return 0;
}

/*

#include <chrono>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include "akit/failover/foros/cluster_node.hpp"
#include <nlohmann/json.hpp>
using namespace std::chrono_literals;
using json = nlohmann::json;

class HelloCluster : public akit::failover::foros::ClusterNode {
public:
    HelloCluster(const std::string& cluster_name, uint32_t node_id, const std::vector<uint32_t>& node_ids)
    : ClusterNode(cluster_name, node_id, node_ids) {
        sensor_subscriber_ = this->create_subscription<std_msgs::msg::String>(
            "sensoring", 10, std::bind(&HelloCluster::sensor_data_callback, this, std::placeholders::_1));
        RCLCPP_INFO(this->get_logger(), "HelloCluster Node initialized and listening on sensoring topic");
    }

private:
    void sensor_data_callback(const std_msgs::msg::String::SharedPtr msg) {
        try {

            // 여기에 leader인 경우에만 받게끔하자.
            if (this->is_activated()){
                auto received_data = json::parse(msg->data);
                std::string data = received_data["data"];
                RCLCPP_INFO(this->get_logger(), "Received sensor data: %s", data.c_str());
                // Verify data - Example: Check if the data contains expected string
                if (data.find("Dummy data from sensor") != std::string::npos) {
                    RCLCPP_INFO(this->get_logger(), "Data verified successfully: %s", data.c_str());
                } else {
                    RCLCPP_ERROR(this->get_logger(), "Data verification failed: %s", data.c_str());
                }
            }


//            auto received_data = json::parse(msg->data);
//            std::string data = received_data["data"];
//            RCLCPP_INFO(this->get_logger(), "Received sensor data: %s", data.c_str());
//            // Verify data - Example: Check if the data contains expected string
//            if (data.find("Dummy data from sensor") != std::string::npos) {
//                RCLCPP_INFO(this->get_logger(), "Data verified successfully: %s", data.c_str());
//            } else {
//                RCLCPP_ERROR(this->get_logger(), "Data verification failed: %s", data.c_str());
//            }
        } catch (const json::parse_error& e) {
            RCLCPP_ERROR(this->get_logger(), "JSON parsing error: '%s'", e.what());
        }
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sensor_subscriber_;
};

int main(int argc, char **argv) {
  const std::string kClusterName = "hello_cluster";
  const std::string kTopicName = "hello_cluster";
  const std::vector<uint32_t> kClusterNodeIds = {0, 1, 2};

  rclcpp::Logger logger = rclcpp::get_logger("hello_cluster_node");
  logger.set_level(rclcpp::Logger::Level::Info);

  if (argc < 2) {
    RCLCPP_ERROR(logger, "Usage : %s {node ID} {size of cluster}", argv[0]);
    return -1;
  }

  uint32_t id = std::stoul(argv[1]);
  if (find(kClusterNodeIds.begin(), kClusterNodeIds.end(), id) ==
      kClusterNodeIds.end()) {
    RCLCPP_ERROR(logger, "Node ID must be among 0, 1, 2");
    return -1;
  }

  rclcpp::init(argc, argv);
  auto node = std::make_shared<HelloCluster>(kClusterName, id, kClusterNodeIds);



  auto publisher = node->create_publisher<std_msgs::msg::String>(kTopicName, 3);
  auto timer_ = node->create_wall_timer(1s, [&]() {
    auto msg = std_msgs::msg::String();
    msg.data = std::to_string(id);
    publisher->publish(msg);
    RCLCPP_INFO(logger, "Publishing cluster message: %s", msg.data.c_str());
  });

  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}

*/