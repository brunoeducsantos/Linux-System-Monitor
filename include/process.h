#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  
  Process(int pid,float cpu_uti, string command, string ram, string user,long uptime): pid_(pid), cpu_uti_(cpu_uti),command_(command),ram_(ram),user_(user),uptime_(uptime){}
  
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

 private:
    int pid_;
    float cpu_uti_;
    string command_;
    string ram_;
    string user_;
    long uptime_;
};

#endif