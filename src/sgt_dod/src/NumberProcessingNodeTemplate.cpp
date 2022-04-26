
#include <ros/ros.h>
#include <std_msgs/Int8.h>

ros::Publisher publisher;

void ProcessNumbers(const std_msgs::Int8 &generatedNumber) {
    int number = generatedNumber.data;

    /**
     * Sem napis svoj kod.
     * Zmen hodnotu premennej 'number'
     *
     * napr. number /= 2;
     */



    std_msgs::Int8 processedNumber;
    processedNumber.data = number;
    publisher.publish(processedNumber);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "number_processing_node");
    ros::NodeHandle handle;

    publisher = handle.advertise<std_msgs::Int8>("processed_numbers", 1);
    ros::Subscriber subscriber = handle.subscribe("generated_numbers", 1, ProcessNumbers);

    ros::spin();
}