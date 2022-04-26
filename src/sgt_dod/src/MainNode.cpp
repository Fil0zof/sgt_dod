

#include <ros/ros.h>
#include "../include/sgt_dod.h"

void printHelp() {
    std::cout << "h - zobraz toto okno \n"
                 "e - uprav procesovanie cisel \n"
                 "v - zobraz vizualizaciu \n"
                 "x - skonci \n";
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "main");
    ros::NodeHandle handle;

    sgt_dod::RosCommand rosCommand{};

    std::cout << "Vitaj v STUBA Green teame driverless stanku" << std::endl;
    ros::Duration(1).sleep();
    std::cout << "Dozvies sa, co je ROS a ako funguje komunikacia medzi senzormi a riadenim formule" << std::endl;
    ros::Duration(1).sleep();
    std::cout << "Si pripraveny? (stlac enter)";

    getchar();

    std::cout << "Co je ROS?" << std::endl;
    std::cout << "ROS (Robot Operating System) je framework poskytujuci knižnice a nástroje, ktoré pomáhajú vývojárom softvéru vytvárať robotické aplikácie." << std::endl <<
              "Poskytuje hardvérovú abstrakciu, ovládače zariadení, knižnice, vizualizéry, odosielanie správ, správu balíkov a ďalšie." << std::endl <<
              "ROS je licencovaný pod open source, BSD licenciou." << std::endl;

    rosCommand.startNode("sgt_dod", "number_generator_node");
    rosCommand.startNode("sgt_dod", "number_processing_node");

    std::cout << "Podme si to vyskusat" << std::endl;
    getchar();
    std::cout << "====================" << std::endl;

    char command = 'h';
    auto compilationCode = 1;
    while (ros::ok()) {
        switch (command) {
            case 'h':
                printHelp();
                break;
            case 'e':
                rosCommand.stopNode("number_processing_node");
                while (compilationCode) {
                    sgt_dod::execute("gedit src/sgt_dod/src/NumberProcessingNode.cpp");
                    std::cout << "Compiling code" << std::endl;
                    compilationCode = sgt_dod::execute("catkin_make > /dev/null 2>&1");
                    if (compilationCode)
                        std::cerr << "Compilation failed" << std::endl;
                    else
                        std::cout << "Compilation successful" << std::endl;
                }
                compilationCode = 1;
                rosCommand.startNode("sgt_dod", "number_processing_node");
                break;
            case 'v':
                rosCommand.startNode("rviz", "rviz");
                break;
        }
        std::cin.get(command);
        if (command == 'x')
            break;
    }

    std::cout << "Dovidenia" << std::endl;

    rosCommand.stopNode("number_generator_node");
    rosCommand.stopNode("number_processing_node");


    sgt_dod::execute("cp src/sgt_dod/src/NumberProcessingNodeTemplate.cpp src/sgt_dod/src/NumberProcessingNode.cpp");
    return 0;
}