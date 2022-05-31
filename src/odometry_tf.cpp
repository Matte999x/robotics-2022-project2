#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf2_ros/transform_broadcaster.h>


class OdometryTransformNode {
private:
    ros::NodeHandle nodeHandle;
    ros::Subscriber sub;
    tf2_ros::TransformBroadcaster tf_broadcaster;


public:
    OdometryTransformNode() {
        // subscribe to /odom topic
        sub = nodeHandle.subscribe("/odom", 1000, &OdometryTransformNode::broadcastOdometryTransform, this);
    }

    void broadcastOdometryTransform (const nav_msgs::Odometry::ConstPtr& odometryMsg) {
        // broadcast TF odom->base_link
        geometry_msgs::TransformStamped odom_tf;
        odom_tf.header.stamp = odometryMsg->header.stamp;
        odom_tf.header.frame_id = "odom";
        odom_tf.child_frame_id = "base_link";
        odom_tf.transform.translation.x = odometryMsg->pose.pose.position.x;
        odom_tf.transform.translation.y = odometryMsg->pose.pose.position.y;
        odom_tf.transform.translation.z = odometryMsg->pose.pose.position.z;
        odom_tf.transform.rotation.x = odometryMsg->pose.pose.orientation.x;
        odom_tf.transform.rotation.y = odometryMsg->pose.pose.orientation.y;
        odom_tf.transform.rotation.z = odometryMsg->pose.pose.orientation.z;
        odom_tf.transform.rotation.w = odometryMsg->pose.pose.orientation.w;
        tf_broadcaster.sendTransform(odom_tf);
    }
};


int main(int argc, char** argv) {
    ros::init(argc, argv, "odometry_tf");

    OdometryTransformNode odometryTransformNode;

    ros::spin();

    return 0;
}
