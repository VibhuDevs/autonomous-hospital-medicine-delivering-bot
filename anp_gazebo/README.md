# `anp_gazebo`

Gazebo Sim hospital environment assets for the ANP medicine-delivery robot.

## Contents

- `worlds/hospital_world.sdf` — primary hospital simulation world.
- `worlds/hospital_world_no_obstacles.sdf` — simplified world variant.
- `worlds/media/` and `worlds/yosemite/` — world textures and supporting environment assets.

The primary world is selected by `anp_description/launch/anp_launch.xml`. That launch file also sets `GZ_SIM_RESOURCE_PATH` to this package's `worlds` directory. Build and source the package before launching the simulation. The world and its assets are simulation content and do not represent a real hospital layout.
