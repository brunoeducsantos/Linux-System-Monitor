#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <cmath>
#include <experimental/filesystem>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "filereader.h"
using std::stof;
using std::string;
using std::vector;
namespace filesystem = std::experimental::filesystem;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// Get linux folder PIDS
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  for (const auto& dir_entry :
       filesystem::directory_iterator((kProcDirectory.c_str()))) {
    // Is this a directory?
    if (filesystem::is_directory(dir_entry)) {
      string filename = dir_entry.path().filename().string();
      if (std::all_of(filename.begin(), filename.end(), ::isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  FileReader<int> filread(kProcDirectory + kMeminfoFilename);
  int memtotal = filread.GetValue("MemTotal");
  int memfree = filread.GetValue("MemFree");
  float memperc = (memtotal - memfree) * 1. / memtotal * 1.;
  return memperc;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  FileReader<long> filread(kProcDirectory + kUptimeFilename);
  return filread.GetValue();
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) {
  char c = ' ';
  FileReader<long> filread(kProcDirectory + std::to_string(pid) +
                           kStatFilename);

  vector<long> res = filread.GetVectorValue(c);
  long utime = res[13];
  long stime = res[14];
  long cutime = res[15];
  long cstime = res[16];
  long starttime = res[21];
  
  long total_time = utime + stime;
  total_time = total_time + cutime + cstime;
  float seconds = UpTime() - (starttime / sysconf(_SC_CLK_TCK));
  float cpu_usage = 100 * ((total_time / sysconf(_SC_CLK_TCK)) / 1*seconds);
  return cpu_usage;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  FileReader<int> filread(kProcDirectory + kStatFilename);
  return filread.GetValue("processes");
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  FileReader<int> filread(kProcDirectory + kStatFilename);
  return filread.GetValue("procs_running");
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  FileReader<string> filread(kProcDirectory + std::to_string(pid) +
                             kCmdlineFilename);
  return filread.GetValue();
}

// Read and return the memory used by a process
//TODO: Fix large invalid numbers
string LinuxParser::Ram(int pid) {
  FileReader<long> filread(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  float ram = filread.GetValue("VmSize") / 1024.;
  return std::to_string(ram);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  FileReader<string> filread(kProcDirectory + std::to_string(pid) +
                             kStatusFilename);
  return filread.GetValue("Uid");
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  FileReader<long> filread(kPasswordPath);
  char c  = ':';
  vector<long> values= filread.GetVectorValue(c);
  for (int i = 0; i < filread.GetVectorValue(c).size(); i++) {
    if (values[i] == std::stol(LinuxParser::Uid(pid)))
      return std::to_string(values[i - 2]);
  }
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  FileReader<long> filread(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  char c = ' ';
  return UpTime() - filread.GetVectorValue(c)[21] / sysconf(_SC_CLK_TCK);
}