//
// Created by toaderst on 08-03-20.
//

#ifndef MONITOR_PARSER_UTILS_H
#define MONITOR_PARSER_UTILS_H


#include <iostream>
#include <fstream>
#include "linux_parser.h"
#include "processor.h"

namespace Utils {
    template<typename T>
    T GetValueByKey(
            const std::string &directory,
            const std::string &filename,
            const std::string &filter) {
        std::string key, line;
        T value;
        std::ifstream stream(directory + filename);
        if (stream.is_open()) {
            while (getline(stream, line)) {
                std::istringstream linestream(line);
                while (linestream >> key) {
                    if (key == filter) {
                        linestream >> value;
                        break;
                    }
                }
            }
        }
        return value;
    }

    std::vector<float> GetCpuData(const std::string &directory,
                                   const std::string &filename) {

        const std::string &filter = "cpu";

        std::vector<float> cpuData;
        std::string key, line;
        float idle, user, nice, iowait, system, irq, softirq, steal, guest;
        std::ifstream stream(directory + filename);

        if (stream.is_open()) {
            while (getline(stream, line)) {
                std::istringstream linestream(line);
                while (linestream >> key) {
                    if (key == filter) {
                        linestream >> user >> nice >> system
                                   >> idle >> iowait >> irq
                                   >> softirq >> steal >> guest;
                        cpuData.emplace_back(user);
                        cpuData.emplace_back(nice);
                        cpuData.emplace_back(system);
                        cpuData.emplace_back(idle);
                        cpuData.emplace_back(iowait);
                        cpuData.emplace_back(irq);
                        cpuData.emplace_back(softirq);
                        cpuData.emplace_back(steal);
                        cpuData.emplace_back(guest);
                    }
                }
            }
        }
        return cpuData;
    }
}


#endif //MONITOR_PARSER_UTILS_H
