#include "navigation_behaviors.hpp"

#include <cmath>
#include <utility>

GoToPose::GoToPose(
  const std::string & name, const BT::NodeConfig & config,
  rclcpp::Node::SharedPtr node, std::map<std::string, Location> locations)
: BT::StatefulActionNode(name, config), node_(std::move(node)), locations_(std::move(locations)),
  action_client_(rclcpp_action::create_client<NavigateToPose>(node_, "navigate_to_pose"))
{}

BT::PortsList GoToPose::providedPorts()
{
  return {BT::InputPort<std::string>("location")};
}

BT::NodeStatus GoToPose::onStart()
{
  std::string location_name;
  if (!getInput("location", location_name)) {
    RCLCPP_ERROR(node_->get_logger(), "GoToPose requires a location input");
    return BT::NodeStatus::FAILURE;
  }
  const auto location = locations_.find(location_name);
  if (location == locations_.end()) {
    RCLCPP_ERROR(node_->get_logger(), "Unknown navigation location '%s'", location_name.c_str());
    return BT::NodeStatus::FAILURE;
  }

  target_location_ = location->second;
  result_.store(kPending);
  goal_sent_ = false;
  sendGoal();
  RCLCPP_INFO(node_->get_logger(), "Starting navigation to '%s'", location_name.c_str());
  return BT::NodeStatus::RUNNING;
}

bool GoToPose::sendGoal()
{
  if (!action_client_->action_server_is_ready()) {
    return false;
  }

  NavigateToPose::Goal goal;
  goal.pose.header.frame_id = "map";
  goal.pose.header.stamp = node_->now();
  goal.pose.pose.position.x = target_location_.x;
  goal.pose.pose.position.y = target_location_.y;
  goal.pose.pose.position.z = 0.0;
  goal.pose.pose.orientation.x = 0.0;
  goal.pose.pose.orientation.y = 0.0;
  goal.pose.pose.orientation.z = std::sin(target_location_.theta / 2.0);
  goal.pose.pose.orientation.w = std::cos(target_location_.theta / 2.0);

  rclcpp_action::Client<NavigateToPose>::SendGoalOptions options;
  options.goal_response_callback = [this](std::shared_ptr<GoalHandle> handle) {
    if (handle) {
      goal_handle_ = std::move(handle);
    } else {
      result_.store(kFailed);
      RCLCPP_ERROR(node_->get_logger(), "Nav2 rejected the navigation goal");
    }
  };
  options.result_callback = [this](const GoalHandle::WrappedResult & result) {
    result_.store(result.code == rclcpp_action::ResultCode::SUCCEEDED ? kSucceeded : kFailed);
  };
  action_client_->async_send_goal(goal, options);
  goal_sent_ = true;
  return true;
}

BT::NodeStatus GoToPose::onRunning()
{
  if (!goal_sent_) {
    sendGoal();
    return BT::NodeStatus::RUNNING;
  }
  const auto result = result_.load();
  if (result == kSucceeded) {
    goal_handle_.reset();
    return BT::NodeStatus::SUCCESS;
  }
  if (result == kFailed) {
    goal_handle_.reset();
    return BT::NodeStatus::FAILURE;
  }
  return BT::NodeStatus::RUNNING;
}

void GoToPose::onHalted()
{
  if (goal_handle_) {
    action_client_->async_cancel_goal(goal_handle_);
    goal_handle_.reset();
  }
  result_.store(kFailed);
  goal_sent_ = false;
}

BT::PortsList Wait::providedPorts()
{
  return {BT::InputPort<double>("seconds")};
}

BT::NodeStatus Wait::onStart()
{
  double seconds = 0.0;
  if (!getInput("seconds", seconds) || seconds < 0.0) {
    RCLCPP_ERROR(rclcpp::get_logger("Wait"), "Wait requires a non-negative seconds input");
    return BT::NodeStatus::FAILURE;
  }
  deadline_ = std::chrono::steady_clock::now() +
    std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(seconds));
  return onRunning();
}

BT::NodeStatus Wait::onRunning()
{
  return std::chrono::steady_clock::now() >= deadline_ ?
         BT::NodeStatus::SUCCESS : BT::NodeStatus::RUNNING;
}

BT::PortsList IsDeliveryRoom::providedPorts()
{
  return {
    BT::InputPort<std::string>("delivery_room"),
    BT::InputPort<std::string>("expected")};
}

BT::NodeStatus IsDeliveryRoom::tick()
{
  std::string selected_room;
  std::string expected_room;
  if (!getInput("delivery_room", selected_room) || !getInput("expected", expected_room)) {
    return BT::NodeStatus::FAILURE;
  }
  return selected_room == expected_room ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
}
