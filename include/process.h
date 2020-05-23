#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

class Process {
 public:
  Process(int pid);
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  
  void UpdateProcess();
  
 private:
    int pid_ = 0;
    std::string user_ = {};
    std::string command_ = {};
    float cpu_utilization_;
    std::string ram_ = {};
    long int uptime_;
	float utime_;
  	float stime_;
  	float cutime_;
  	float cstime_;
  	float starttime_;
    
};

#endif