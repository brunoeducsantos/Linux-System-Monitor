#include "processor.h"

#include "linux_parser.h"
// Return the aggregate CPU utilization
float Processor::Utilization() {
    long totald = LinuxParser::InfoCpu()[0] - prev_total_;
    long idled = LinuxParser::InfoCpu()[1] - prev_total_idle_;

    prev_total_= LinuxParser::InfoCpu()[0];
    prev_total_idle_= LinuxParser::InfoCpu()[1];
    return (totald-idled)*1./totald;

}