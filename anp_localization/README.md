# `anp_localization`

Mapping, localization, and Nav2 configuration for the simulated robot.

## Launch files

- `launch/slam.launch.py` starts lifecycle-managed SLAM Toolbox; `autostart` and `use_sim_time` default to `true`.
- `launch/localization.launch.py` starts the saved map server and AMCL using `maps/hospital_map_no_obstacles.yaml`.
- `launch/navigation.launch.py` starts the Nav2 controller, planner, behavior server, and BT navigator, using the included navigation behavior trees.

The package also includes Nav2/AMCL/SLAM parameters in `config/` and `params/`, a map image and metadata in `maps/`, and an RViz configuration in `rviz/`. Launch simulation-dependent nodes with `use_sim_time:=true` when using Gazebo. For autonomy, the map frame and named coordinates in `anp_autonomy/config/map_locations.yaml` must agree.

Example:

```bash
ros2 launch anp_localization localization.launch.py use_sim_time:=true
ros2 launch anp_localization navigation.launch.py use_sim_time:=true
```
