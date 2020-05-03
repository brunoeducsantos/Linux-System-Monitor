#include "processor.h"

#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  float sum = 0.;
  for (auto& pid : LinuxParser::Pids()) {
    sum = LinuxParser::CpuUtilization(pid);
  }
  return sum;
}