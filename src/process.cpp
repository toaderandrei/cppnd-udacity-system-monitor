#include "process.h"

#include <string>
#include <vector>

#include "linux_parser.h"
#include "debugging.h"

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

  return ((total_time / _hertz_) / seconds);
}

string Process::Command() { return LinuxParser::Command(_pid_); }

string Process::Ram() { return LinuxParser::Ram(_pid_); }

string Process::User() { return LinuxParser::User(_pid_); }

long int Process::UpTime() {
  long system_uptime = LinuxParser::UpTime();
  long process_uptime = system_uptime - (starttime_ / _hertz_);
  return process_uptime;
}

bool Process::operator<(Process const &a) const {
  return LinuxParser::GetRawRam(a._pid_) < LinuxParser::GetRawRam(this->_pid_);
}

std::vector<std::string> Process::ReadCpuUsages(const int &pid) {
  return LinuxParser::CpuUsages(pid);
}