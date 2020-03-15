//
// Created by toaderst on 15-03-20.
//

#ifndef MONITOR_UTILS_H
#define MONITOR_UTILS_H

#include <sstream>
#include <string>
#include <vector>
#include <iterator>

namespace Utils {
    template<typename T>
    void split(const std::string &s, char delim, T result) {
        std::istringstream iss(s);
        std::string item;
        while (std::getline(iss, item, delim)) {
            *result++ = item;
        }
    }

    std::vector <std::string> split(const std::string &s, char delim) {
        std::vector <std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

}

#endif //MONITOR_UTILS_H
