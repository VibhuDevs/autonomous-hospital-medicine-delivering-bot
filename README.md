# Autonomous Hospital Medicine Delivering Bot

A ROS 2 simulation project for a mobile robot that navigates a hospital world and carries out a simple medicine delivery routine. The repository is split into ROS 2 packages for the robot model, Gazebo world, controller and bridge configuration, localization/navigation, and behavior-tree autonomy.

> This repository describes a simulation and software project. It is not a validated medical device or a system for unsupervised medication handling in a real hospital.

## How the project works

1. `anp_description` defines the ANP robot with Xacro/URDF and provides a launch file that starts Gazebo, spawns the robot, bridges simulation topics, and starts the wheel controllers.
2. `anp_gazebo` provides hospital world assets. `anp_bringup` supplies the differential-drive controller parameters and Gazebo-to-ROS bridge mappings.
3. `anp_localization` provides SLAM, saved-map localization, Nav2 parameters, behavior trees, and RViz configuration.
4. `anp_autonomy` loads named map coordinates and runs a BehaviorTree.CPP sequence: visit the medical shop, wait, navigate to the configured delivery room, wait, then return to the initial pose.

## Packages

| Package | Responsibility |
| --- | --- |
| [`anp_description`](anp_description/README.md) | Robot model and simulation launch entry point |
| [`anp_gazebo`](anp_gazebo/README.md) | Hospital Gazebo worlds and environment assets |
| [`anp_bringup`](anp_bringup/README.md) | Controller parameters and Gazebo topic bridge configuration |
| [`anp_localization`](anp_localization/README.md) | SLAM, AMCL, Nav2 launch/configuration, maps, and RViz |
| [`anp_autonomy`](anp_autonomy/README.md) | Behavior-tree-driven medicine delivery routine |

## Requirements

- ROS 2 installation with `colcon` and `rosdep` (the packages use the ament CMake build type).
- Gazebo Sim with the ROS-Gazebo integration packages (`ros_gz_sim` and `ros_gz_bridge`).
- Navigation and localization packages including Nav2, `slam_toolbox`, and BehaviorTree.CPP.
- Xacro, `robot_state_publisher`, `controller_manager`, `diff_drive_controller`, and RViz2.

Exact package names and versions vary by ROS 2 distribution and Gazebo release. Install the matching dependencies for your distribution before building.

## Build

From the repository root in a ROS 2 workspace:

```bash
source /opt/ros/$ROS_DISTRO/setup.bash
rosdep install --from-paths . --ignore-src -r -y
colcon build --symlink-install
source install/setup.bash
```

## Run the simulation

Start the robot and hospital world:

```bash
ros2 launch anp_description anp_launch.xml
```

Start localization against the included map and Nav2 navigation in separate terminals:

```bash
ros2 launch anp_localization localization.launch.py
ros2 launch anp_localization navigation.launch.py
```

For online mapping in place of the saved map:

```bash
ros2 launch anp_localization slam.launch.py
```

The autonomy package builds an `autonomy_node` executable and provides the behavior tree and named coordinates. Its package currently does not declare a ROS launch file or executable installation instructions, so the autonomy node may need to be started directly using the appropriate installed executable/parameters for your build setup. Set `delivery_room` to `room101` or `room102` and ensure the robot starts localized in the coordinate frame used by `map_locations.yaml`.

## Repository contents

Only the five `anp_*` source packages are included. Generated workspace output, editor databases, and the world backup file are excluded. Each package README documents its purpose, contents, and relevant entry points.

## License

Package license files are retained in each package. Review each package's `LICENSE` before redistributing the combined repository.
