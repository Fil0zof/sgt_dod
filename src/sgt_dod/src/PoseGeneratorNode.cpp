//
// this node generates numbers 0-100 and publishes them to 'generated_numbers' topic each second
//

#include <ros/ros.h>
#include <geometry_msgs/Pose2D.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "pose_generator_node");
    ros::NodeHandle handle;

    ros::Publisher publisher = handle.advertise<geometry_msgs::Pose2D>("generated_pose", 1);

    srand(time(NULL));
    geometry_msgs::Pose2D number;

    while (ros::ok()) {
        number.x = static_cast<float>((rand() % 200) - 100);
        number.y = static_cast<float>((rand() % 200) - 100);
        publisher.publish(number);
        ros::Duration(1).sleep();
    }

}