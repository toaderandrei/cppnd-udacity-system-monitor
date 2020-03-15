#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {

public:
    Process(int, long);

    int Pid();                               // TODO: See src/process.cpp
    std::string User();                      // TODO: See src/process.cpp
    std::string Command();                   // TODO: See src/process.cpp
    double CpuUtilization();                  // TODO: See src/process.cpp
    std::string Ram();                       // TODO: See src/process.cpp
    float RawRam() const;
    long int UpTime();                       // TODO: See src/process.cpp
    bool operator<(Process const &a) const;  // TODO: See src/process.cpp

private:
    int _pid_;
    long _hertz_;
    float utime_ = 0.0;
    float stime_ = 0.0;
    float cutime_ = 0.0;
    float cstime_ = 0.0;
    float starttime_ = 0.0;

    std::vector<std::string> ReadCpuUsages(const int &pid);
};

#endif