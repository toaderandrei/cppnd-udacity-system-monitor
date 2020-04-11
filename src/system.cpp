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
    Hertz = sysconf(_SC_CLK_TCK);
    UpdateProcesses();
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

void System::UpdateProcesses() {
    std::vector<int> current_pids = LinuxParser::Pids();
    bool updated = false;
    AddNewProcesses(updated, current_pids);
    if (updated) {
        std::sort(processes_.begin(), processes_.end());
    }
}

void System::AddNewProcesses(bool &changed, std::vector<int> &current_pids) {
    for (size_t i = 0; i < current_pids.size(); i++) {
        const int current_pid = current_pids[i];
        if (std::find_if(processes_.begin(), processes_.end(), [current_pid](Process &process) {
            return current_pid == process.Pid();
        }) == processes_.end()) {
            Process process(current_pid, Hertz);
            processes_.emplace_back(process);
            changed = true;
        }
    }
}