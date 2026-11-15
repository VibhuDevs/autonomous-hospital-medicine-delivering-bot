import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

from launch_ros.actions import LifecycleNode, Node
from launch_ros.descriptions import ParameterFile


def generate_launch_description():

    pkg_share = get_package_share_directory('anp_localization')

    map_file = os.path.join(
        pkg_share,
        'maps',
        'hospital_map_no_obstacles.yaml'
    )

    amcl_params_file = os.path.join(
        pkg_share,
        'config',
        'amcl.yaml'
    )

    use_sim_time = LaunchConfiguration('use_sim_time')

    declare_use_sim_time = DeclareLaunchArgument(
        'use_sim_time',
        default_value='true',
        description='Use simulation time'
    )

    map_server = LifecycleNode(
        package='nav2_map_server',
        executable='map_server',
        name='map_server',
        namespace='',
        output='screen',
        parameters=[
            {
                'yaml_filename': map_file,
                'use_sim_time': use_sim_time
            }
        ]
    )

    amcl = LifecycleNode(
        package='nav2_amcl',
        executable='amcl',
        name='amcl',
        namespace='',
        output='screen',
        parameters=[
            ParameterFile(
                amcl_params_file,
                allow_substs=True
            ),
            {
                'use_sim_time': use_sim_time
            }
        ]
    )

    lifecycle_manager = Node(
        package='nav2_lifecycle_manager',
        executable='lifecycle_manager',
        name='lifecycle_manager_localization',
        namespace='',
        output='screen',
        parameters=[
            {
                'use_sim_time': use_sim_time,
                'autostart': True,
                'node_names': [
                    'map_server',
                    'amcl'
                ]
            }
        ]
    )

    return LaunchDescription([
        declare_use_sim_time,
        map_server,
        amcl,
        lifecycle_manager
    ])
