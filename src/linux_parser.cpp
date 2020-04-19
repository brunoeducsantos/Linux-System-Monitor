#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <vector>
using std::stof;
using std::string;
using std::to_string;
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
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  int memtotal = 0, memfree = 0;
  string line;
  string key_;
  string mem;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> key_;
    if (key_ == "MemTotal") {
      linestream >> mem;
      memtotal = std::stoi(mem);
    }
    if (key_ == "MemFree") {
      linestream >> mem;
      memfree = std::stoi(mem);
      stream.close();
    }
  }
  float memperc = (memtotal - memfree) * 1. / memtotal * 1.;
  return memperc;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return std::stol(uptime);
  }
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
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string line;
  std::string key_;
  std::string processes;
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key_;
    if (key_ == "processes") {
      linestream >> processes;
      return std::stoi(processes);
    }
  }
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string line;
  std::string key_;
  std::string procs;
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key_;
    if (key_ == "procs_running") {
      linestream >> procs;
      return std::stoi(procs);
    }
  }
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }