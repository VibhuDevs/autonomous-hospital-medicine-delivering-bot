import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

from launch_ros.actions import LifecycleNode, Node


def generate_launch_description():

    pkg_share = get_package_share_directory('anp_localization')

    params_file = os.path.join(
        pkg_share,
        'config',
        'nav2_params.yaml'
    )
    bt_directory = os.path.join(pkg_share, 'behavior_trees')

    use_sim_time = LaunchConfiguration('use_sim_time')

    declare_use_sim_time = DeclareLaunchArgument(
        'use_sim_time',
        default_value='true',
        description='Use simulation time'
    )

    # ---------------------------------------------------------
    # Controller Server
    # ---------------------------------------------------------

    controller_server = LifecycleNode(
        package='nav2_controller',
        executable='controller_server',
        name='controller_server',
        namespace='',
        output='screen',
        parameters=[
            params_file,
            {
                'use_sim_time': use_sim_time,
                'enable_stamped_cmd_vel': True
            }
        ],
        remappings=[
            ('/tf', 'tf'),
            ('/tf_static', 'tf_static'),
            (
                'cmd_vel',
                '/diff_drive_controller/cmd_vel'
            )
        ]
    )

    # ---------------------------------------------------------
    # Planner Server
    # ---------------------------------------------------------

    planner_server = LifecycleNode(
        package='nav2_planner',
        executable='planner_server',
        name='planner_server',
        namespace='',
        output='screen',
        parameters=[
            params_file,
            {
                'use_sim_time': use_sim_time
            }
        ],
        remappings=[('/tf', 'tf'), ('/tf_static', 'tf_static')]
    )

    # ---------------------------------------------------------
    # Behavior Server
    # ---------------------------------------------------------

    behavior_server = LifecycleNode(
        package='nav2_behaviors',
        executable='behavior_server',
        name='behavior_server',
        namespace='',
        output='screen',
        parameters=[
            params_file,
            {
                'use_sim_time': use_sim_time,
                'enable_stamped_cmd_vel': True
            }
        ],
        remappings=[
            ('/tf', 'tf'),
            ('/tf_static', 'tf_static'),
            (
                'cmd_vel',
                '/diff_drive_controller/cmd_vel'
            )
        ]
    )

    # ---------------------------------------------------------
    # Behavior Tree Navigator
    # ---------------------------------------------------------

    bt_navigator = LifecycleNode(
        package='nav2_bt_navigator',
        executable='bt_navigator',
        name='bt_navigator',
        namespace='',
        output='screen',
        parameters=[
            params_file,
            {
                'use_sim_time': use_sim_time,
                'default_nav_to_pose_bt_xml': os.path.join(
                    bt_directory,
                    'navigate_to_pose_w_replanning_and_recovery.xml'
                ),
                'default_nav_through_poses_bt_xml': os.path.join(
                    bt_directory,
                    'navigate_through_poses_w_replanning_and_recovery.xml'
                )
            }
        ],
        remappings=[('/tf', 'tf'), ('/tf_static', 'tf_static')]
    )

    # ---------------------------------------------------------
    # Lifecycle Manager
    # ---------------------------------------------------------

    lifecycle_manager = Node(
        package='nav2_lifecycle_manager',
        executable='lifecycle_manager',
        name='lifecycle_manager_navigation',
        namespace='',
        output='screen',
        parameters=[
            {
                'use_sim_time': use_sim_time,
                'autostart': True,
                'node_names': [
                    'controller_server',
                    'planner_server',
                    'behavior_server',
                    'bt_navigator'
                ]
            }
        ]
    )

    return LaunchDescription([
        declare_use_sim_time,

        controller_server,
        planner_server,
        behavior_server,
        bt_navigator,

        lifecycle_manager
    ])
