#ifndef PROCESSOR_H
#define PROCESSOR_H
#include<process.h>

class Processor {
 public:
  
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    long prev_total_=0;
    long prev_total_idle_=0;
};

#endif