#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <Utils.h>
#include "cstdint"
#include "parser_utils.h"
#include "linux_parser.h"
#include "constants.h"

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


// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids2() {
    vector<int> pids;
    std::string line;
    int pid;
    std::ifstream stream;
    stream.open(LinuxParser::kProcDirectory.c_str());
    if (stream.is_open()) {
        while (getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> pid;
            pids.emplace_back(pid);
        }
    }

    return pids;
}

float LinuxParser::MemoryUtilization() {
    std::string key, line;
    float total_memory, memory_free;
    float memory_used = 0.0;
    total_memory = ParserUtils::GetValueByKey<float>(kProcDirectory, kMeminfoFilename, Constants::filterMemTotal);
    memory_free = ParserUtils::GetValueByKey<float>(kProcDirectory, kMeminfoFilename, Constants::filterMemoryFree);

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
                    linestream >> user >> nice >> system
                               >> idle >> iowait >> irq
                               >> softirq >> steal >> guest;
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
    return cpuData;
}

int LinuxParser::TotalProcesses() {
    return ParserUtils::GetValueByKey<int>(kProcDirectory, kStatFilename, Constants::filterProcesses);
}

int LinuxParser::RunningProcesses() {
    return ParserUtils::GetValueByKey<int>(kProcDirectory, kStatFilename, Constants::filterRunningProcesses);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    const std::string UidDirectory = kProcDirectory + std::to_string(pid);
    const auto uid = ParserUtils::GetValueByKey<std::string>(UidDirectory, kStatusFilename, "Uid:");
    return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
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
    return key;
}