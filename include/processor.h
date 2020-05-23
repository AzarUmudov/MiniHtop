#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization(); 
  
 private:
  int previdle_;
  int previowait_;
  int prevuser_;
  int prevnice_;
  int prevsystem_;
  int previrq_;
  int prevsoftirq_;
  int prevsteal_;

  void PrevValues(std::vector<float>);
};

#endif