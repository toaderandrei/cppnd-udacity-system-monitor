#include <vector>
#include <linux_parser.h>
#include "processor.h"

//todo for an advanced - check recent stats in the last weeks/days and not
// from the beginning of time.

// TODO: Return the aggregate CPU utilization
double Processor::Utilization() {
    std::vector<double> processor_data = LinuxParser::CpuUtilization();
    double user = processor_data[0];
    double nice = processor_data[1];
    double system = processor_data[2];
    double idle = processor_data[3];
    double iowait = processor_data[4];
    double irq = processor_data[5];
    double softirq = processor_data[6];
    double steal = processor_data[7];

    double _prevIdle = this->prevIdleTime + this->prevIowaitTime;
    double _idle = idle + iowait;

    double _prevNonIdleTime = this->prevUserTime + this->prevSystemTime +
                              this->prevNiceTime +
                              this->prevIrqTime + this->prevSoftIrqTime +
                              this->prevStealTime;

    double _nonIdle = user + nice + system + irq + softirq + steal;

    double _prevTotal = _prevIdle + _prevNonIdleTime;
    double _total = _idle + _nonIdle;

    //calculated totalid and idlded time.
    double totald = _total - _prevTotal;
    double idled = _idle - _prevIdle;

    double cpu_percentage = (totald - idled) / totald;
    UpdateValues(user, nice, system, idle, iowait, irq, softirq, steal);
    return cpu_percentage;
}

void Processor::UpdateValues(double user,
                             double nice,
                             double system,
                             double idle,
                             double iowait,
                             double irq,
                             double softirq,
                             double steal) {
    prevUserTime = user;
    prevIdleTime = idle;
    prevSystemTime = system;
    prevIowaitTime = iowait;
    prevNiceTime = nice;
    prevSoftIrqTime = softirq;
    prevIrqTime = irq;
    prevStealTime = steal;
}
