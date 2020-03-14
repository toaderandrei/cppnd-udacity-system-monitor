#include <vector>
#include <linux_parser.h>
#include "processor.h"

//todo for an advanced - check recent stats in the last weeks/days and not
// from the beginning of time.

float Processor::Utilization() {
    std::vector<float> processor_data = LinuxParser::CpuUtilization();
    float user = processor_data[0];
    float nice = processor_data[1];
    float system = processor_data[2];
    float idle = processor_data[3];
    float iowait = processor_data[4];
    float irq = processor_data[5];
    float softirq = processor_data[6];
    float steal = processor_data[7];

    float _prevIdle = this->prevIdleTime + this->prevIowaitTime;
    float _idle = idle + iowait;

    float _prevNonIdleTime = this->prevUserTime + this->prevSystemTime +
                              this->prevNiceTime +
                              this->prevIrqTime + this->prevSoftIrqTime +
                              this->prevStealTime;

    float _nonIdle = user + nice + system + irq + softirq + steal;

    float _prevTotal = _prevIdle + _prevNonIdleTime;
    float _total = _idle + _nonIdle;

    //calculated totalid and idlded time.
    float totald = _total - _prevTotal;
    float idled = _idle - _prevIdle;

    float cpu_percentage = (totald - idled) / totald;
    UpdateValues(user, nice, system, idle, iowait, irq, softirq, steal);
    return cpu_percentage;
}

void Processor::UpdateValues(float user,
                             float nice,
                             float system,
                             float idle,
                             float iowait,
                             float irq,
                             float softirq,
                             float steal) {
    prevUserTime = user;
    prevIdleTime = idle;
    prevSystemTime = system;
    prevIowaitTime = iowait;
    prevNiceTime = nice;
    prevSoftIrqTime = softirq;
    prevIrqTime = irq;
    prevStealTime = steal;
}
