#include <string>
#include <vector>
#include <format.h>
#include <constants.h>

#include "process.h"
#include "linux_parser.h"
#include "parser_utils.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, long hertz) : _pid_(pid), _hertz_(hertz) {
    vector<std::string> cpu_usage_values = ReadCpuUsages(pid);
    utime_ = stof(cpu_usage_values[13]);
    stime_ = stof(cpu_usage_values[14]);
    cutime_ = stof(cpu_usage_values[15]);
    cstime_ = stof(cpu_usage_values[16]);
    starttime_ = stof(cpu_usage_values[21]);
}

int Process::Pid() { return _pid_; }

double Process::CpuUtilization() {
    long system_uptime = LinuxParser::UpTime();
    double total_time = utime_ + stime_ + cutime_ + cstime_;
    double seconds = system_uptime - (starttime_ / _hertz_);
    return 100 * ((total_time / _hertz_) / seconds);
}

string Process::Command() {
    return ParserUtils::GetFirstValueByKey<std::string>(LinuxParser::kProcDirectory + std::to_string(_pid_),
                                                        Constants::kCmdlineFilename,
                                                        50);
}

string Process::Ram() {

    float raw_ram = GetRawRam();
    return Format::KbsToMbs(raw_ram);
}

float Process::GetRawRam() const {
    return ParserUtils::GetValueByKey<float>(LinuxParser::kProcDirectory + std::to_string(_pid_),
                                             LinuxParser::kStatusFilename,
                                             Constants::filterProcMem);
}

string Process::User() {
    return LinuxParser::User(_pid_);
}

long int Process::UpTime() {
    long system_uptime = LinuxParser::UpTime();
    long process_uptime = system_uptime - (starttime_ / _hertz_);
    return process_uptime;
}

bool Process::operator<(Process const &a) const {
    return a.GetRawRam() < this->GetRawRam();
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