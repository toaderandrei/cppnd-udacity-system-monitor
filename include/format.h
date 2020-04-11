#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
    std::string ElapsedTime(long total_seconds);
    std::string KbsToMbs(float kbs);

    std::string FormatTime(int hours);

    int First_Digit_After_Decimal(double number, unsigned int precision);
};

#endif