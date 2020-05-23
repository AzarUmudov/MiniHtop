#ifndef SUPPORT_PROCESSES_H
#define SUPPORT_PROCESSES_H

#include "process.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

class Support_Processes {
 private:
  vector<int> current_pids_;
  vector<Process> all_processes_;
  bool compareProcesses(Process& p1, Process& p2);
  void UpdateProcesses();
  void AddNewProcesses(bool&);
  void RemoveFinishedProcesses(bool&);

 public:
  Support_Processes();
  vector<Process>& GetProcesses();
};

#endif