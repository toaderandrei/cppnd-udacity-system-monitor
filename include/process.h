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

    int Pid();
    std::string User();
    std::string Command();
    double CpuUtilization();
    std::string Ram();
    long int UpTime();
    bool operator<(Process const &a) const;
private:
    int _pid_;
    float _hertz_;
    float utime_ = 0.0;
    float stime_ = 0.0;
    float cutime_ = 0.0;
    float cstime_ = 0.0;
    float starttime_ = 0.0;
    float ram;
    std::vector<std::string> ReadCpuUsages(const int &pid);
};

#endif