#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

class Processor {
public:

    float Utilization();  // TODO: See src/processor.cpp
    // TODO: Declare any necessary private members
private:
    double prevUserTime;
    double prevIdleTime;
    double prevNiceTime;
    double prevIowaitTime;
    double prevSystemTime;
    double prevIrqTime;
    double prevSoftIrqTime;
    double prevStealTime;

    void UpdateValues(float user,
                      float nice,
                      float system,
                      float idle,
                      float iowait,
                      float irq,
                      float softirq,
                      float steal);
};

#endif