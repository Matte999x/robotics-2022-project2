# Robotics Project 2 - A.Y. 2021-2022
Second project of the Robotics Course (Perception, Localization and Mapping for Mobile Robots) at Politecnico di Milano.  
Description of the project available [here](docs/Project2.pdf).

## Team members
- Pablo Martellucci
- Matteo Pierini
- Sara Vignali

## ROS assets
### Nodes
- **odometry_tf** ([*odometry_tf.cpp*](src/odometry_tf.cpp))  
It broadcasts the TF for the robot base link, starting from the odometry on the */odom* topic.