//
// Created by toaderst on 08-03-20.
//

#ifndef MONITOR_PARSER_UTILS_H
#define MONITOR_PARSER_UTILS_H


#include <iostream>
#include <fstream>
#include <sstream>

namespace ParserUtils {

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
                        return value;
                    }
                }
            }
        }
        return value;
    }
}


#endif //MONITOR_PARSER_UTILS_H
