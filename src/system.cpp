#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <linux_parser.h>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor &System::Cpu() {
    return cpu_;
}

vector<Process> &System::Processes() {
    vector<int> pids = LinuxParser::Pids();
    for (const int pid : pids) {
        std::string Uid = LinuxParser::Uid(pid);
        string User = LinuxParser::User(pid);
        long Hertz = sysconf(_SC_CLK_TCK);
        Process process(pid, Hertz);
        processes_.emplace_back(process);
    }

    return processes_;
}

std::string System::Kernel() {
    return LinuxParser::Kernel();
}

float System::MemoryUtilization() {
    return LinuxParser::MemoryUtilization();
}

std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
}

int System::RunningProcesses() {
    return LinuxParser::RunningProcesses();
}

int System::TotalProcesses() {
    return LinuxParser::TotalProcesses();
}

long int System::UpTime() { return LinuxParser::UpTime(); }