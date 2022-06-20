#include <ros/ros.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <nav_msgs/Path.h>
#include "project2/SavePath.h"
#include <nav_msgs/MapMetaData.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <math.h>


class PathGenerationNode {
private:
    ros::NodeHandle nodeHandle;
    ros::Subscriber sub, mapMetaDataSub;
    ros::Publisher pub;

    nav_msgs::Path path;

    ros::ServiceServer savePathService;

    std::string mapFolder;


public:
    PathGenerationNode(std::string mapFolder) {
        // initialize path
        path.poses = std::vector<geometry_msgs::PoseStamped>();

        // subscribe to /amcl_pose topic
        sub = nodeHandle.subscribe("/amcl_pose", 1000, &PathGenerationNode::updatePath, this);

        // advertise /amcl_path topic
        pub = nodeHandle.advertise<nav_msgs::Path>("/amcl_path", 100);
        
        // advertise save_path service
        savePathService = nodeHandle.advertiseService("save_path", &PathGenerationNode::savePathServiceCallback, this);

        // save location of the map        
        this->mapFolder = (mapFolder.back() == '/' || mapFolder == "") ? mapFolder : mapFolder.append("/");
    }

    void updatePath(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& amclPose) {
        // get header and pose of last amcl pose
        geometry_msgs::PoseStamped lastPose;
        lastPose.header = amclPose->header;
        lastPose.pose = amclPose->pose.pose;

        // add last amcl pose to path
        path.poses.push_back(lastPose);
        path.header = lastPose.header;

        // publish updated path
        pub.publish(path);
    }

    bool savePathServiceCallback(project2::SavePath::Request &request, project2::SavePath::Response &response) {
        if (mapFolder == "") {
            ROS_INFO("Cannot save path: no map has been provided as an argument");
        } else {
            ROS_INFO("Saving path on map");
            mapMetaDataSub = nodeHandle.subscribe("/map_metadata", 1000, &PathGenerationNode::savePath, this);
        }
        return true;
    }

    void savePath(const nav_msgs::MapMetaData::ConstPtr& mapMetaData) {
        mapMetaDataSub.shutdown();

        float res = mapMetaData->resolution;
        float x_origin = mapMetaData->origin.position.x;
        float y_origin = mapMetaData->origin.position.y;
        int width = mapMetaData->width;
        int height = mapMetaData->height;

        cv::Mat img = cv::imread(mapFolder + "map.pgm", cv::IMREAD_COLOR);

        for (int i = 1; i < path.poses.size(); i++) {
            geometry_msgs::Pose pose1 = path.poses[i-1].pose;
            geometry_msgs::Pose pose2 = path.poses[i].pose;
            cv::Point p1 = cv::Point(round((pose1.position.x - x_origin) / res), round(height - (pose1.position.y - y_origin) / res));
            cv::Point p2 = cv::Point(round((pose2.position.x - x_origin) / res), round(height - (pose2.position.y - y_origin) / res));
            cv::line(img, p1, p2, cv::Scalar(0, 255, 0));
        }

        cv::imwrite(mapFolder + "map_with_path.png", img);

        ROS_INFO("Map with path saved in %s", (mapFolder + "map_with_path.png").c_str());
    }
};


int main(int argc, char** argv) {
    ros::init(argc, argv, "path_generation");

    std::string mapsFolder;
    if(argc == 1)
        mapsFolder = "";
    else if (argc == 2)
        mapsFolder = argv[1];
    else {
        ROS_ERROR("Too many arguments\nUSAGE: path_generation <map folder>");
        exit(-1);
    }

    PathGenerationNode pathGenerationNode(mapsFolder);

    ros::spin();

    return 0;
}