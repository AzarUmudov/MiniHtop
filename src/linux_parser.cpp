#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

string LinuxParser::Kernel() {
  string os, kernel;
  string line, x;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> x >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {
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
  string line, key, value;
  float memTotal, memFree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "MemTotal"){
      	memTotal = stof(value);
      }
      else if(key=="MemFree"){
      	memFree = stof(value);
      }
    }
  }
  
  return (memTotal-memFree)/memTotal; 
}

long LinuxParser::UpTime() { 
  string line, value;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
  	std::istringstream linestream(line);
    linestream >> value;
  }
  return std::stol(value); 

}

vector<string> LinuxParser::CpuUtilization() { 
  string line, key;
  vector<string> values; 

  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if(key== "cpu"){
        string val;
        while(linestream >> val){
          values.push_back(val);
        }
        return values;
      }
    }
  }
  
  return {}; 
}

int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "processes"){
        return std::stoi(value);
      }
    }
  }

  return 0; 
}

int LinuxParser::RunningProcesses() {
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "procs_running"){
        return std::stoi(value);
      }
    }
  }
  
  return 0; 
}

string LinuxParser::Command(int pid) { 
  string pid_s = to_string(pid);
  string line, cmd;
  std::ifstream stream(kProcDirectory + pid_s + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cmd;
    return cmd;
  }
  
  return string(); 
}

string LinuxParser::Ram(int pid) { 
  string pid_s = to_string(pid);
  int mem;
  string line, key, value;
  std::ifstream stream(kProcDirectory + pid_s + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "VmSize"){
        mem = stof(value)*0.001;
        return to_string(mem); 
      }
    }
  }

  return string(); 
  
}

string LinuxParser::Uid(int pid) { 
  string pid_s = to_string(pid);
  string line, key, value;
  std::ifstream stream(kProcDirectory + pid_s + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "Uid"){
        return value;
      }
    }
  }
  
  return string(); 
}

string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line, user, x, value;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> value;
      if(value == uid){
        return user;
      }
    }
  }

  return string(); 
}

long LinuxParser::UpTime(int pid) { 
  string pid_s = to_string(pid);
  string line, starttime;
  std::ifstream stream(kProcDirectory + pid_s + kStatFilename);
  if(stream.is_open()){
    int counter = 0;
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      
      while(linestream >> starttime){
      	counter++;
        
        if(counter == 14){
          auto upTime = std::stoi(starttime);
          return upTime/sysconf(_SC_CLK_TCK);
      	}
      }
    }
  }
  
  return 0; 
  
}

vector<float> LinuxParser::CpuTimes(int pid) { 
  vector<float> cpu_times = {};
  string pid_s = to_string(pid);
  string line, starttime;
  std::ifstream stream(kProcDirectory + pid_s + kStatFilename);
  if(stream.is_open()){
    int counter = 0;
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> starttime){
      	counter++;
        if(counter == 14 || counter == 15 || counter == 16 || counter == 17 || counter == 22){
          auto upTime = std::stof(starttime);
          cpu_times.emplace_back(upTime);
      	}
      }
    }
  }
  
  return cpu_times; 
}