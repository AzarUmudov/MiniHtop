#include "processor.h"
#include "linux_parser.h"
#include <string>

float Processor::Utilization() {  
    std::string user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    auto times = LinuxParser::CpuUtilization();
    
    return 0.0; 
}