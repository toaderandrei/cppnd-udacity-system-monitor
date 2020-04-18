#include "linux_parser.h"

#include <Utils.h>
#include <dirent.h>
#include <format.h>
#include <unistd.h>

#include <cstdlib>
#include <string>
#include <vector>

#include "constants.h"
#include "cstdint"
#include "parser_utils.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  std::string key, line;
  float total_memory;
  float memory_free;
  float memory_used = 0.0;
  total_memory = ParserUtils::GetValueByKey<float>(
      kProcDirectory, kMeminfoFilename, Constants::filterMemTotal);
  memory_free = ParserUtils::GetValueByKey<float>(
      kProcDirectory, kMeminfoFilename, Constants::filterMemoryFree);

  if (total_memory != 0.0) {
    memory_used = (total_memory - memory_free) / total_memory;
  }
  return memory_used;
}

long LinuxParser::UpTime() {
  std::string line;
  std::string value1;
  long uptime = 0;
  std::ifstream stream(LinuxParser::kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    if (getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> value1;
      uptime = strtol(value1.c_str(), nullptr, 10);
    }
  }
  ParserUtils::CloseStream(&stream);
  return uptime;
}

vector<float> LinuxParser::CpuUtilization() {
  const std::string &filter = "cpu";

  std::vector<float> cpuData;
  std::string key, line;
  float idle, user, nice, iowait, system, irq, softirq, steal, guest;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == filter) {
          linestream >> user >> nice >> system >> idle >> iowait >> irq >>
              softirq >> steal >> guest;
          cpuData.emplace_back(user);
          cpuData.emplace_back(nice);
          cpuData.emplace_back(system);
          cpuData.emplace_back(idle);
          cpuData.emplace_back(iowait);
          cpuData.emplace_back(irq);
          cpuData.emplace_back(softirq);
          cpuData.emplace_back(steal);
          cpuData.emplace_back(guest);
        }
      }
    }
  }
  ParserUtils::CloseStream(&stream);
  return cpuData;
}

int LinuxParser::TotalProcesses() {
  return ParserUtils::GetValueByKey<int>(kProcDirectory, kStatFilename,
                                         Constants::filterProcesses);
}

int LinuxParser::RunningProcesses() {
  return ParserUtils::GetValueByKey<int>(kProcDirectory, kStatFilename,
                                         Constants::filterRunningProcesses);
}

long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

long LinuxParser::ActiveJiffies(int pid) {
  vector<std::string> cpu_usages = CpuUsages(pid);

  long jiffies{0};
  if (cpu_usages.size() > 21) {
    long user = stol(cpu_usages[13]);
    long kernel = stol(cpu_usages[14]);
    long children_user = stol(cpu_usages[15]);
    long children_kernel = stol(cpu_usages[16]);
    jiffies = user + kernel + children_user + children_kernel;
  }
  return jiffies;
}

vector<std::string> LinuxParser::CpuUsages(int pid) {
  std::vector<std::string> cpu_usages;
  std::string line;
  std::string value;
  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) +
                       LinuxParser::kStatFilename);

  if (stream.is_open()) {
    if (getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        cpu_usages.emplace_back(value);
      }
    }
  }
  ParserUtils::CloseStream(&stream);
  return cpu_usages;
}

long LinuxParser::ActiveJiffies() {
  vector<float> cpu_utilization = CpuUtilization();
  return cpu_utilization[CPUStates::kUser_] +
         cpu_utilization[CPUStates::kNice_] +
         cpu_utilization[CPUStates::kSystem_] +
         cpu_utilization[CPUStates::kIRQ_] +
         cpu_utilization[CPUStates::kSoftIRQ_] +
         cpu_utilization[CPUStates::kSteal_] +
         cpu_utilization[CPUStates::kGuest_] +
         cpu_utilization[CPUStates::kGuestNice_];
}

long LinuxParser::IdleJiffies() {
  vector<float> cpu_utilization = CpuUtilization();
  return cpu_utilization[CPUStates::kIdle_] +
         cpu_utilization[CPUStates::kIOwait_];
}

string LinuxParser::Command(int pid) {
  return ParserUtils::GetFirstValueByKey<std::string>(
      LinuxParser::kProcDirectory + std::to_string(pid),
      Constants::kCmdlineFilename, 50);
}

string LinuxParser::Ram(int pid) {
  float raw_ram = GetRawRam(pid);
  return Format::KbsToMbs(raw_ram);
}

float LinuxParser::GetRawRam(int _pid_) {
  return ParserUtils::GetValueByKey<float>(
      LinuxParser::kProcDirectory + std::to_string(_pid_),
      LinuxParser::kStatusFilename, Constants::filterProcMem);
}

string LinuxParser::Uid(int pid) {
  const std::string UidDirectory = kProcDirectory + std::to_string(pid);
  const auto uid = ParserUtils::GetValueByKey<std::string>(
      UidDirectory, kStatusFilename, "Uid:");
  return uid;
}

string LinuxParser::User(int pid) {
  std::string user;
  std::string key, x, value, line;
  string filter = Uid(pid);
  std::ifstream stream(LinuxParser::kPasswordPath);

  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::vector<std::string> words = Utils::split(line, ':');

      if (words[2] == filter) {
        return words[0];
      }
    }
  }
  ParserUtils::CloseStream(&stream);
  return key;
}