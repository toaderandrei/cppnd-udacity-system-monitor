#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cstdlib>
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

float LinuxParser::MemoryUtilization() {
    std::string key, line;
    float total_memory = 0.0, memory_free = 0.0;
    float memory_used = 0.0;
    total_memory = Utils::GetValueByKey<float>(kProcDirectory, kMeminfoFilename, Constants::filterMemTotal);
    memory_free = Utils::GetValueByKey<float>(kProcDirectory, kMeminfoFilename, Constants::filterMemoryFree);

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


// TODO: Read and return CPU utilization
vector<double> LinuxParser::CpuUtilization() {
    return Utils::GetCpuData(kProcDirectory, kStatFilename);
}

int LinuxParser::TotalProcesses() {
    return Utils::GetValueByKey<int>(kProcDirectory, kStatFilename, Constants::filterProcesses);
}

int LinuxParser::RunningProcesses() {
    return Utils::GetValueByKey<int>(kProcDirectory, kStatFilename, Constants::filterRunningProcesses);
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
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }