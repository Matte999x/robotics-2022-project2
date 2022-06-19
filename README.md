# Robotics Project 2 - A.Y. 2021-2022
Second project of the Robotics Course (Perception, Localization and Mapping for Mobile Robots) at Politecnico di Milano.  
Description of the project available [here](docs/Project2.pdf).

## Team members
- Pablo Martellucci
- Matteo Pierini
- Sara Vignali

## Required packages
- ira_laser_tools
- gmapping
- navigation (amcl and map_server)

## ROS assets
### Nodes
- **odometry_tf** ([*odometry_tf.cpp*](src/odometry_tf.cpp))  
It broadcasts the TF for the robot base link, starting from the odometry on the */odom* topic.

### Launch files
- [**run.launch**](launch/run.launch)  
It starts the odometry_tf node and broadcasts the three static TF from *base_link* to *laser_front*, *laser_rear* and *laser_merged*. It starts the laserscan_multi_merger node from the ira_laser_tools package to merge the two scans on the */merged_scan* topic.
- [**mapping.launch**](launch/mapping.launch)  
It launches the same nodes of *run.launch* and it creates a map of the environment from the merged scan using slam_gmapping from the gmapping package.
- [**localization.launch**](launch/localization.launch)  
It launches the same nodes of *run.launch* and the map server, that provides the map ([map.yaml](maps/map.yaml)) from the maps folder. It performs localization using amcl (Adaptive Monte Carlo localization algorithm) and the map provided by the map server.