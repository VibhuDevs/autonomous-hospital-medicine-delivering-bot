#ifndef ANP_AUTONOMY_NODE_HPP_
#define ANP_AUTONOMY_NODE_HPP_

#include "location_utils.hpp"

#include <behaviortree_cpp/bt_factory.h>
#include <rclcpp/rclcpp.hpp>

#include <memory>
#include <string>

class AutonomyNode : public rclcpp::Node
{
public:
  explicit AutonomyNode(const std::string & node_name);
  void setup();

private:
  void createBehaviorTree();
  void updateBehaviorTree();

  BT::BehaviorTreeFactory factory_;
  BT::Tree tree_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::string locations_file_;
  std::string tree_file_;
  std::string delivery_room_;
  std::map<std::string, Location> locations_;
};

#endif
