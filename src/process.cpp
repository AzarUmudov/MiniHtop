#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <sstream>

#include "process.h"
#include "linux_parser.h"


using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid): pid_(pid) { 
    command_ = LinuxParser::Command(pid_);
    user_ = LinuxParser::User(pid_);
    ram_ = LinuxParser::Ram(pid_);
  	uptime_ = LinuxParser::UpTime(pid_);
  	auto values = LinuxParser::CpuTimes(pid_);
  	utime_ = values[0];
  	stime_ = values[1];
  	cutime_ = values[2];
  	cstime_ = values[3];
  	starttime_ = values[4];
    cpu_utilization_ = CpuUtilization();
  
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { 
  long uptime = LinuxParser::UpTime();
  float total_time = utime_ + stime_ + cutime_ + cstime_;

  float seconds = uptime - (starttime_ / sysconf(_SC_CLK_TCK));
  float cpu_usage = (total_time / sysconf(_SC_CLK_TCK)) / seconds;
	
  return cpu_usage;
}

void Process::UpdateProcess(){
   command_ = LinuxParser::Command(pid_);
   user_ = LinuxParser::User(pid_);
   ram_ =LinuxParser::Ram(pid_);
   uptime_ = LinuxParser::UpTime(pid_);
   auto values = LinuxParser::CpuTimes(pid_);
   utime_ = values[0];
   stime_ = values[1];
   cutime_ = values[2];
   cstime_ = values[3];
   starttime_ = values[4];
   cpu_utilization_ = CpuUtilization();
}

string Process::Command() { return command_; }

string Process::Ram() { return (ram_ + " MB"); }

string Process::User() { return user_; }

long int Process::UpTime() { return uptime_; }

bool Process::operator<(Process const& a) const { 
  //return std::stof(ram_) > std::stof(a.ram_); 
  return cpu_utilization_ > a.cpu_utilization_;
}