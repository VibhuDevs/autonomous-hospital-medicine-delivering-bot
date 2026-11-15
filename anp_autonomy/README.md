# `anp_autonomy`

BehaviorTree.CPP-based autonomy for the simulated medicine-delivery robot.

## What it does

`autonomy_node` loads named poses from `config/map_locations.yaml`, creates the tree in `bt_xml/autonomy_bt.xml`, and ticks it at 10 Hz. The tree sends navigation goals to the medical shop, waits five seconds, selects room 101 or 102 from the `delivery_room` parameter, waits again, and returns to `initial_pose`. Custom tree nodes implement navigation, waiting, and delivery-room selection.

## Key files

- `src/autonomy_node.cpp` — node setup and behavior-tree tick loop.
- `src/navigation_behaviors.cpp` — custom behavior-tree actions/condition.
- `include/` — node, location, and behavior declarations.
- `bt_xml/autonomy_bt.xml` — delivery sequence.
- `config/map_locations.yaml` — named `[x, y, theta]` goals.

## Configuration and dependencies

The node parameters `locations_file`, `tree_file`, and `delivery_room` can override defaults. `delivery_room` must be `room101` or `room102`, and the corresponding location must exist in the YAML. The package depends on ROS 2 C++, Nav2 action messages, BehaviorTree.CPP, and yaml-cpp.

Build with the workspace using `colcon build --packages-up-to anp_autonomy`. The package installs the executable, headers, config, and tree. Ensure Nav2 is active and the coordinates match the currently loaded map before running the node.
