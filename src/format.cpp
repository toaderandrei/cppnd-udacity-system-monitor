#include <string>

#include "format.h"
#include <iomanip>
#include <cmath>

using std::string;

string Format::ElapsedTime(long total_seconds) {
    std::string elapsed_time;
    int days = 0;
    int hours = 0;
    int minutes = 0;
    days = total_seconds / 86400;
    hours = (total_seconds - (days * 86400)) / 3600;
    minutes = (total_seconds - (days * 86400) - (hours * 3600)) / 60;
    int seconds = (total_seconds - (days * 86400) - (hours * 3600) - (minutes * 60));
    if (days > 0) {
        elapsed_time += FormatTime(days);
        elapsed_time += ":";
        elapsed_time += FormatTime(hours);
        elapsed_time += ":";
        elapsed_time += FormatTime(minutes);
        elapsed_time += ":";
        elapsed_time += FormatTime(seconds);
    } else {
        elapsed_time += FormatTime(hours);
        elapsed_time += ":";
        elapsed_time += FormatTime(minutes);
        elapsed_time += ":";
        elapsed_time += FormatTime(seconds);
    }
    return elapsed_time;
}

string Format::FormatTime(int hours) {
    std::string formatted_time = std::to_string(hours);
    if (hours < 10) {
        return "0" + formatted_time;
    }
    return formatted_time;
}

string Format::KbsToMbs(float kbs) {
    float mbs = kbs / 1024;
    std::stringstream mb_stream;
    int floor_mbs = First_Digit_After_Decimal(mbs, 1);
    if (floor_mbs == 0) {
        mb_stream << std::fixed << std::setprecision(0) << mbs;
    } else {
        mb_stream << std::fixed << std::setprecision(1) << mbs;
    }

    return mb_stream.str();
}

int Format::First_Digit_After_Decimal(double number, unsigned int precision) {
    double trunc = number - static_cast<int>(number);
    return static_cast<int>(trunc * pow(10, precision));
}