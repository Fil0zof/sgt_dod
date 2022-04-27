

#include <ros/ros.h>
#include "../include/sgt_dod.h"

void printHelp() {
    std::cout << "-----------------------------\n"
                 "Pomoc\n"
                 "h - zobraz toto okno\n"
                 "e - uprav procesovanie údajov\n"
                 "x - skonči" << std::endl;
}

void readLineString(std::string &lineString, const std::string &expectedString) {
    lineString.clear();
    do {
        getline(std::cin, lineString);
        if (lineString != expectedString)
            std::cerr << "Nesprávny príkaz. Zadaj príkaz '" << expectedString << "': ";
    } while (lineString != expectedString);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "main");
    ros::NodeHandle handle;

    sgt_dod::RosCommand rosCommand{};

    std::cout << "Vitaj v STUBA Green teame driverless stánku.\n"
                 "Tu sa dozvieš, čo je ROS a ako funguje komunikácia medzi senzormi a riadením formule\n"
                 "Si pripravený? (stlac enter)" << std::endl;;
    getchar();
    std::cout << "Čo je ROS?\n"
                 "ROS (Robot Operating System) je framework poskytujúci knižnice a nástroje, ktoré pomáhajú vývojárom softvéru vytvárať robotické aplikácie.\n"
                 "Poskytuje hardvérovú abstrakciu, ovládače zariadení, knižnice, vizualizéry, odosielanie správ, správu balíkov a ďalšie." << std::endl;
    getchar();
    std::cout << "ROS je postavený na uzloch (Node), ktoré môžu publikovat (publish) na témy (Topic), alebo tému odoberat (subscribe).\n"
                 "Uzol nie je nič iné, ako spustiteľný súbor.\n"
                 "Každá téma má určitý typ správy." << std::endl;
    getchar();
    std::cout << "Poďme sa pozrieť na názornú ukážku" << std::endl;
    getchar();

    rosCommand.startNode("sgt_dod", "pose_generator_node");
    rosCommand.startNode("sgt_dod", "pose_processing_node");

    int compilationCode = 1;
    int step = 1;

    std::string lineString;
    while (step <= 3) {
        std::cout << "\n==================== Krok " << step << "====================" << std::endl;
        switch (step) {
            case 1:
                std::cout << "Teraz boli spustené 2 uzly:\n"
                             "1. pose_generator_node\n"
                             "2. pose_processing_node" << std::endl;
                std::cout << "Príkaz 'rosnode list' zobrazí všetky aktívne uzly\n"
                             "Zadaj príkaz 'rosnode list': ";
                readLineString(lineString, "rosnode list");
                sgt_dod::execute(lineString);
                std::cout << "\nMôžeme vidieť, že sú spustené 4 uzly:\n"
                             "1. /rosout - toto je master uzol, ten nás teraz nezaujíma\n"
                             "2. /main - tento node ta sprevádza (je spustený v tomto termináli a vypisuje tieto správy)\n"
                             "3. /pose_generator_node - posiela správy typu Pose2D na tému /generated_pose\n"
                             "4. /pose_processing_node - počúva na tému /generated_pose. V tomto uzle sa môže diať mágia nad prijatymi správami, ktoré sú posielané na tému /processed_pose\n"
                          << std::endl;
                break;
            case 2:
                std::cout << "O týchto uzloch môžeme zistiť bližšie informácie príkazom 'rosnode info /nazov_uzla'\n"
                             "Zadaj príkaz 'rosnode info /pose_generator_node': ";
                readLineString(lineString, "rosnode info /pose_generator_node");
                sgt_dod::execute(lineString, "12");
                std::cout << "Na výpise máme názov uzla, publikované a odoberané témy.\n"
                             "Každý uzol posiela správy na tému /rosout - tam sa posielajú logovacie správy, ale to nás teraz nezaujíma.\n"
                             "Čo nás zaujíma je, že uzol /pose_generator_node posiela správy typu Pose2D na tému /generated_pose." << std::endl;
                getchar();
                std::cout << "Teraz zadaj príkaz 'rosnode info /pose_processing_node': ";
                readLineString(lineString, "rosnode info /pose_processing_node");
                sgt_dod::execute(lineString, "10");
                std::cout << "Tu si všimni, že uzol odoberá tému /generated_pose.\n"
                             "Tiež posiela rovnaký typ správy na tému /processed_pose." << std::endl;
                break;
            case 3:
                std::cout << "Teraz sa pozrieme na konkrétne témy.\n"
                             "Správy, ktoré sú posielané na tému vieme zobrazit príkazom 'rostopic echo /nazov_topicu'\n";
                std::cout << "Zadaj príkaz 'rostopic echo /generated_pose': ";
                readLineString(lineString, "rostopic echo /generated_pose");
                sgt_dod::executeInNewTerminal(lineString, "Generated pose");
                std::cout << "Na novom okne sa vypisujú správy, ktoré chodia na tému /generated_pose" << std::endl;

                std::cout << "Teraz zadaj prikaz 'rostopic echo /processed_pose': ";
                readLineString(lineString, "rostopic echo /processed_pose");
                sgt_dod::executeInNewTerminal(lineString, "Porcessed pose");
                std::cout << "Na novom okne sa vypisujú správy, ktoré chodia na tému /processed_pose" << std::endl;
                std::cout << "Na oboch témach sú presne rovnaké hodnoty. \n"
                             "V ďalšom kroku budeš mať možnosť upraviť procesovanie údajov v uzle /pose_processing_node" << std::endl;
                break;
        }
        std::cout << "\nPodme ďalej (stlač enter)";
        getchar();
        step++;
    }

    std::cout << "=======================================" << std::endl;
    std::cout << "\n\nNa záver si môžes vyskúšať upravit programu uzlu /pose_processing_node\n"
                 "Po zobrazení Pomoci zadaj 'e'\n"
                 "Otvorí sa ti okno editora, v ktorom môžeš upraviť hodnoty, posielané na tému /processed_pose" << std::endl;

    char command = 'h';
    while (ros::ok()) {
        switch (command) {
            case 'h':
                printHelp();
                break;
            case 'e':
                rosCommand.stopNode("pose_processing_node");
                while (compilationCode) {
                    sgt_dod::execute("gedit src/sgt_dod/src/PoseProcessingNode.cpp");
                    std::cout << "Prebieha kompilácia kódu" << std::endl;
                    compilationCode = sgt_dod::execute("catkin_make > /dev/null");
                    if (compilationCode)
                        std::cerr << "Chyba počas kompilácie" << std::endl;
                    else
                        std::cout << "Kompilácia prebehla úspešne" << std::endl;
                }
                compilationCode = 1;
                rosCommand.startNode("sgt_dod", "pose_processing_node");
                break;
        }

        std::cin.get(command);
        if (command == 'x')
            break;
    }

    std::cout << "\nPrajeme pekný deň\n"
                 "Budeme radi, ak sa ku nám pridáš" << std::endl;

    rosCommand.stopNode("pose_generator_node");
    rosCommand.stopNode("pose_processing_node");

    sgt_dod::execute("cp src/sgt_dod/src/PoseProcessingNodeTemplate.cpp src/sgt_dod/src/PoseProcessingNode.cpp");
    return 0;
}