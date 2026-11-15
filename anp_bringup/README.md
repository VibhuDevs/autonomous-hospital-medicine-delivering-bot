# `anp_bringup`

Shared simulation bringup configuration for the ANP robot.

## Contents

- `config/anp_controllers.yaml` configures `joint_state_broadcaster` and `diff_drive_controller`, including wheel joints, geometry, odometry frames, and command limits.
- `config/anp_gazebo_bridge.yaml` bridges the simulation clock and laser scan from Gazebo to ROS 2.

The main simulation launch file is in `anp_description` and references this package's bridge configuration. Build and source this package in the same ROS 2 workspace as the robot description and Gazebo packages. The configuration files are intended to be installed under the package share directory.
