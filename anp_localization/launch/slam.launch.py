import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    EmitEvent,
    LogInfo,
    RegisterEventHandler,
)
from launch.conditions import IfCondition
from launch.events import matches_action
from launch.substitutions import (
    AndSubstitution,
    LaunchConfiguration,
    NotSubstitution,
)

from launch_ros.actions import LifecycleNode
from launch_ros.descriptions import ParameterFile
from launch_ros.event_handlers import OnStateTransition
from launch_ros.events.lifecycle import ChangeState

from lifecycle_msgs.msg import Transition


def generate_launch_description():

    pkg_share = get_package_share_directory(
        'anp_localization'
    )

    slam_params_file = os.path.join(
        pkg_share,
        'config',
        'slam_toolbox.yaml'
    )

    autostart = LaunchConfiguration('autostart')
    use_sim_time = LaunchConfiguration('use_sim_time')

    declare_autostart = DeclareLaunchArgument(
        'autostart',
        default_value='true',
        description='Automatically start SLAM Toolbox'
    )

    declare_use_sim_time = DeclareLaunchArgument(
        'use_sim_time',
        default_value='true',
        description='Use Gazebo simulation clock'
    )

    slam_node = LifecycleNode(
        package='slam_toolbox',
    	executable='async_slam_toolbox_node',
    	name='slam_toolbox',
    	namespace='',
    	output='screen',
    	parameters=[
            ParameterFile(slam_params_file, allow_substs=True),
            {'use_sim_time': use_sim_time}
    ]
)

    configure_event = EmitEvent(
        event=ChangeState(
            lifecycle_node_matcher=matches_action(slam_node),
            transition_id=Transition.TRANSITION_CONFIGURE
        ),
        condition=IfCondition(autostart)
    )

    activate_event = RegisterEventHandler(
        OnStateTransition(
            target_lifecycle_node=slam_node,
            start_state='configuring',
            goal_state='inactive',

            entities=[
                LogInfo(
                    msg='SLAM Toolbox configured. Activating...'
                ),
                EmitEvent(
                    event=ChangeState(
                        lifecycle_node_matcher=matches_action(slam_node),
                        transition_id=Transition.TRANSITION_ACTIVATE
                    )
                )
            ]
        ),
        condition=IfCondition(autostart)
    )

    return LaunchDescription([
        declare_autostart,
        declare_use_sim_time,

        slam_node,

        configure_event,
        activate_event,
    ])
