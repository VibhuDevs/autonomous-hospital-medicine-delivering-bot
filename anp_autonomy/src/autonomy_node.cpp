#include "autonomy_node.hpp"
#include "navigation_behaviors.hpp"

#include <ament_index_cpp/get_package_share_directory.hpp>

#include <chrono>
#include <memory>
#include <stdexcept>
#include <utility>

using namespace std::chrono_literals;

AutonomyNode::AutonomyNode(const std::string & node_name)
: Node(node_name)
{
  const auto share = ament_index_cpp::get_package_share_directory("anp_autonomy");
  locations_file_ = declare_parameter<std::string>("locations_file", share + "/config/map_locations.yaml");
  tree_file_ = declare_parameter<std::string>("tree_file", share + "/bt_xml/autonomy_bt.xml");
  delivery_room_ = declare_parameter<std::string>("delivery_room", "room101");
  locations_ = loadLocations(locations_file_);
  if ((delivery_room_ != "room101" && delivery_room_ != "room102") ||
    locations_.find(delivery_room_) == locations_.end())
  {
    throw std::invalid_argument(
      "delivery_room must be 'room101' or 'room102', and that room must exist in map_locations.yaml");
  }
  factory_.registerBuilder<GoToPose>("GoToPose",
    [this](const std::string & name, const BT::NodeConfig & config) {
      return std::make_unique<GoToPose>(name, config, shared_from_this(), locations_);
    });
  factory_.registerNodeType<Wait>("Wait");
  factory_.registerNodeType<IsDeliveryRoom>("IsDeliveryRoom");
  RCLCPP_INFO(get_logger(), "Autonomy node initialized for %s", delivery_room_.c_str());
}

void AutonomyNode::setup()
{
  createBehaviorTree();
  timer_ = create_wall_timer(100ms, std::bind(&AutonomyNode::updateBehaviorTree, this));
}

void AutonomyNode::createBehaviorTree()
{
  tree_ = factory_.createTreeFromFile(tree_file_);
  tree_.rootBlackboard()->set("delivery_room", delivery_room_);
}

void AutonomyNode::updateBehaviorTree()
{
  const auto status = tree_.tickOnce();
  if (status == BT::NodeStatus::SUCCESS) {
    RCLCPP_INFO(get_logger(), "Medicine delivery behavior tree completed");
    timer_->cancel();
  } else if (status == BT::NodeStatus::FAILURE) {
    RCLCPP_ERROR(get_logger(), "Medicine delivery behavior tree failed");
    timer_->cancel();
  }
}

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  try {
    auto node = std::make_shared<AutonomyNode>("autonomy_node");
    node->setup();
    rclcpp::spin(node);
  } catch (const std::exception & e) {
    RCLCPP_FATAL(rclcpp::get_logger("autonomy_node"), "Failed to start autonomy: %s", e.what());
    rclcpp::shutdown();
    return 1;
  }
  rclcpp::shutdown();
  return 0;
}
