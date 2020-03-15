#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
public:
    Processor &Cpu();

    /**
     * Retrieves the system's processes.
     * @return the system processes.
     */
    std::vector<Process> &Processes();

    /**
     * Retrieves the memory utilization of the system.
     * @return memory utilization.
     */
    float MemoryUtilization();

    /**
     * Retrieves the system's uptime.
     * @return the system uptime.
     */
    long UpTime();

    /**
     * Retrieves the total number of processes.
     * @return total number of processes.
     */
    int TotalProcesses();

    /**
     * Retrieves the number of running processes.
     * @return the total number of running processes.
     */
    int RunningProcesses();

    /**
     * Retrieves the kernel name,
     * @return kernel name.
     */
    std::string Kernel();

    /**
     * Retrieves the name of the operating system.
     * @return the OS name.
     */
    std::string OperatingSystem();

    // TODO: Define any necessary private members
private:
    Processor cpu_;
    std::vector<Process> processes_;
};

#endif