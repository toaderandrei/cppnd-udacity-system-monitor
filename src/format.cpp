#include <string>

#include "format.h"

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