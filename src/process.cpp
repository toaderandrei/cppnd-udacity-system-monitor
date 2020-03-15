#include <string>
#include <vector>
#include <format.h>

#include "process.h"
#include "linux_parser.h"
#include "parser_utils.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, long hertz) : _pid_(pid), _hertz_(hertz) {
    vector<std::string> cpu_usage_values = ReadCpuUsages(pid);
    utime_ = stof(cpu_usage_values[14]);
    stime_ = stof(cpu_usage_values[15]);
    cutime_ = stof(cpu_usage_values[16]);
    cstime_ = stof(cpu_usage_values[17]);
    starttime_ = stof(cpu_usage_values[22]);
}

int Process::Pid() { return _pid_; }

double Process::CpuUtilization() {

    long system_uptime = LinuxParser::UpTime();
    double total_time = utime_ + stime_ + cutime_ + cstime_;
    double seconds = system_uptime - (starttime_ / _hertz_);
    double cpu_usage = 100 * ((total_time / _hertz_) / seconds);
    return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() {

    this->ram = ParserUtils::GetValueByKey<float>(LinuxParser::kProcDirectory + std::to_string(_pid_),
                                                  LinuxParser::kStatusFilename,
                                                  "VmSize:");
    return Format::KbsToMbs(this->ram);
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(_pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    long process_uptime = 0;
    long system_uptime = LinuxParser::UpTime();
    process_uptime = system_uptime - (starttime_ / _hertz_);
    return process_uptime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a) const {
    bool compare = this->ram > a.ram;
    return compare;
}

std::vector<std::string> Process::ReadCpuUsages(const int &pid) {
    std::vector<std::string> cpu_usages;
    std::string line;
    std::string value;
    std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename);
    if (stream.is_open()) {
        if (getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> value) {
                cpu_usages.emplace_back(value);
            }
        }
    }
    return cpu_usages;
}