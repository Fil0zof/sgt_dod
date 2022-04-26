//
// this node generates numbers 0-100 and publishes them to 'generated_numbers' topic each second
//

#include <ros/ros.h>
#include <std_msgs/Int8.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "number_generator_node");
    ros::NodeHandle handle;

    ros::Publisher publisher = handle.advertise<std_msgs::Int8>("generated_numbers", 1);

    srand(time(NULL));
    std_msgs::Int8 number;

    while (ros::ok()) {
        number.data = (rand() % 200) - 100;
        publisher.publish(number);
        ros::Duration(1).sleep();
    }

}