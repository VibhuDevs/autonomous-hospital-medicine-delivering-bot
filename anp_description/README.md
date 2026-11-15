# `anp_description`

Robot model and simulation launch package.

## Robot model

`urdf/anp.urdf.xacro` is the top-level Xacro and includes the base, Gazebo plugin/sensor, shared properties, and ros2_control definitions. The launch entry point `launch/anp_launch.xml` expands the robot model, starts Gazebo with `anp_gazebo`'s hospital world, spawns the robot, starts the configured ROS-Gazebo bridge and wheel controllers, and opens RViz.

## Run

Build and source the workspace, then run:

```bash
ros2 launch anp_description anp_launch.xml
```

This launch file expects `anp_bringup`, `anp_gazebo`, `ros_gz_sim`, `ros_gz_bridge`, `robot_state_publisher`, `controller_manager`, Xacro, and RViz2 to be installed and discoverable. Start localization and Nav2 separately with the launch files in `anp_localization`.
