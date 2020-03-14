#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

class Processor {
public:

    double Utilization();  // TODO: See src/processor.cpp
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

    void UpdateValues(double user,
                      double nice,
                      double system,
                      double idle,
                      double iowait,
                      double irq,
                      double softirq,
                      double steal);
};

#endif