#ifndef PROCESSOR_H
#define PROCESSOR_H
#include<process.h>

class Processor {
 public:
  
  float Utilization();  

  // Previous total and total_idle
 private:
    long prev_total_=0;
    long prev_total_idle_=0;
};

#endif