#ifndef ANP_AUTONOMY_NAVIGATION_BEHAVIORS_HPP_
#define ANP_AUTONOMY_NAVIGATION_BEHAVIORS_HPP_

#include "location_utils.hpp"

#include <behaviortree_cpp/behavior_tree.h>
#include <nav2_msgs/action/navigate_to_pose.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>

#include <atomic>
#include <chrono>
#include <map>
#include <memory>
#include <string>

class GoToPose : public BT::StatefulActionNode
{
public:
  GoToPose(
    const std::string & name, const BT::NodeConfig & config,
    rclcpp::Node::SharedPtr node, std::map<std::string, Location> locations);
  static BT::PortsList providedPorts();

private:
  using NavigateToPose = nav2_msgs::action::NavigateToPose;
  using GoalHandle = rclcpp_action::ClientGoalHandle<NavigateToPose>;

  static constexpr int kPending = 0;
  static constexpr int kSucceeded = 1;
  static constexpr int kFailed = -1;

  rclcpp::Node::SharedPtr node_;
  std::map<std::string, Location> locations_;
  rclcpp_action::Client<NavigateToPose>::SharedPtr action_client_;
  std::shared_ptr<GoalHandle> goal_handle_;
  Location target_location_{};
  bool goal_sent_{false};
  std::atomic<int> result_{kPending};

  bool sendGoal();
  BT::NodeStatus onStart() override;
  BT::NodeStatus onRunning() override;
  void onHalted() override;
};

class Wait : public BT::StatefulActionNode
{
public:
  using BT::StatefulActionNode::StatefulActionNode;
  static BT::PortsList providedPorts();

private:
  std::chrono::steady_clock::time_point deadline_;
  BT::NodeStatus onStart() override;
  BT::NodeStatus onRunning() override;
  void onHalted() override {}
};

class IsDeliveryRoom : public BT::ConditionNode
{
public:
  using BT::ConditionNode::ConditionNode;
  static BT::PortsList providedPorts();

private:
  BT::NodeStatus tick() override;
};

#endif
