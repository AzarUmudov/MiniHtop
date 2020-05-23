#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include "support_processes.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System(){
    kernel_ = LinuxParser::Kernel();
    operating_system_ = LinuxParser::OperatingSystem();
}

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 
  Support_Processes sp;
  processes_ = sp.GetProcesses();
  return processes_;
}

std::string System::Kernel() { return kernel_; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return operating_system_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }