#include <string>

#include "format.h"
#include<bits/stdc++.h>

using std::string;

string Format::ElapsedTime(long seconds) {
    std::string elapsed_time;
    long int days = 0;
    int hours = 0;
    int minutes = 0;
    days = seconds / 86400;
    hours = (seconds - (days * 86400)) / 3600;
    minutes = (seconds - (days * 86400) - (hours * 3600)) / 60;
    seconds = (seconds - (days * 86400) - (hours * 3600) - (minutes * 60));
    if (days > 0) {
        elapsed_time += std::to_string(days);
        elapsed_time += ":";
        elapsed_time += std::to_string(hours);
        elapsed_time += ":";
        elapsed_time += std::to_string(minutes);
        elapsed_time += ":";
        elapsed_time += std::to_string(seconds);
    } else {
        elapsed_time += std::to_string(hours);
        elapsed_time += ":";
        elapsed_time += std::to_string(minutes);
        elapsed_time += ":";
        elapsed_time += std::to_string(seconds);
    }
    return elapsed_time;
}

string Format::KbsToMbs(float kbs) {

    float mbs = kbs / 1000;
    std::string mbs_string = std::to_string(mbs);
    std::size_t found = mbs_string.find('.');
    if (found != std::string::npos) {
        mbs_string = mbs_string.substr(0, found + 3);
    }
    return mbs_string;
}