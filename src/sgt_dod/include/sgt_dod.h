
#ifndef SGT_DOD_H
#define SGT_DOD_H

#include <iostream>
#include <thread>
#include <vector>

namespace sgt_dod {

    class RosCommand {
    private:
        std::vector<std::pair<std::string, std::thread>> nameThreadPairs;

    public:
        ~RosCommand() {
            for (auto &pair: nameThreadPairs) {
                pair.second.join();
            }
        }

        void stopNode(const std::string &nodeName) {
            for (int i = 0; i < nameThreadPairs.size(); ++i) {
                if (nameThreadPairs.at(i).first == nodeName) {
                    std::string fullCommand("rosnode kill ");
                    fullCommand.append(nodeName).append(" > /dev/null 2>&1");
                    system(fullCommand.data());
                    nameThreadPairs.at(i).second.detach();
                    nameThreadPairs.erase(nameThreadPairs.begin() + i);
                }
            }
        }

        void startNode(const std::string &packaged, const std::string &nodeName) {
            std::string command("rosrun ");
            command.append(packaged).append(" ").append(nodeName).append(" > /dev/null 2>&1");
            this->nameThreadPairs.emplace_back(nodeName, [command]() {
                system(command.data());
            });
        }
    };

    int execute(const std::string &command) {
        std::string fullCommand("cd ~/sgt/dod/ && ");
        fullCommand.append(command);
        int commandResponse = system(fullCommand.data());
        return commandResponse;
    }

    int execute(const std::string &command, const std::string &nLines) {
        std::string fullCommand("cd ~/sgt/dod/ && ");
        fullCommand.append(command).append(" | head -n ").append(nLines);
        int commandResponse = system(fullCommand.data());
        return commandResponse;
    }


    int executeInNewTerminal(const std::string &command, const std::string &terminalTitle) {
        std::string fullCommand("gnome-terminal ");
        fullCommand.append(" --title ").append(terminalTitle).append(" -- ").append(command);
        return system(fullCommand.data());
    }

}

#endif //SGT_DOD_H
