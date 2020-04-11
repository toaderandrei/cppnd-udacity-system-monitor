#ifndef MONITOR_CONSTANTS_H
#define MONITOR_CONSTANTS_H

#include "iostream"

namespace Constants {

    constexpr char filterProcesses[] = "processes";
    constexpr char filterRunningProcesses[] = "procs_running";
    constexpr char filterMemTotal[] = "MemTotal:";
    constexpr char filterMemoryFree[] = "MemFree:";
    constexpr char filterCpu[] = "cpu";
    constexpr char filterUID[] = "Uid:";
    constexpr char filterProcMem[] = "VmData:";
    constexpr char kCmdlineFilename[] = "/cmdline";


}  // namespace ParserConsts

#endif //MONITOR_CONSTANTS_H
