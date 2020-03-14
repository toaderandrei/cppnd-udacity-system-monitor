#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

class Processor {
public:

    float Utilization();

private:
    float prevUserTime;
    float prevIdleTime;
    float prevNiceTime;
    float prevIowaitTime;
    float prevSystemTime;
    float prevIrqTime;
    float prevSoftIrqTime;
    float prevStealTime;

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