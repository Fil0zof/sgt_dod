
#include <ros/ros.h>
#include <geometry_msgs/Pose2D.h>

ros::Publisher publisher;

void ProcessNumbers(const geometry_msgs::Pose2D &generatedNumber) {
    float x = generatedNumber.x;
    float y = generatedNumber.y;

    /**
     * Sem napis svoj kod.
     * Zmen hodnotu premennych 'x' a 'y'
     *
     * napr. posielaj polovicne hodnoty
     *      x/=2; y/=2;
     */



    geometry_msgs::Pose2D processedNumber;
    processedNumber.x = x;
    processedNumber.y = y;
    publisher.publish(processedNumber);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "pose_processing_node");
    ros::NodeHandle handle;

    publisher = handle.advertise<geometry_msgs::Pose2D>("processed_pose", 1);
    ros::Subscriber subscriber = handle.subscribe("generated_pose", 1, ProcessNumbers);

    ros::spin();
}

