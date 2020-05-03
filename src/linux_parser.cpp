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
  string os, kernel, version;  // <----- Declared a new variable version
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >>
        version;  // <-- Extracting the version in a variable version
  }
  return version;  //<---- Return the third variable
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
  int memtotal = filread.GetValue(filterMemTotalString);
  int memfree = filread.GetValue(filterMemFreeString);
  float memperc = (memtotal - memfree) * 1. / memtotal * 1.;
  return memperc;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  FileReader<long> filread(kProcDirectory + kUptimeFilename);
  return filread.GetValue();
}

// Read and return CPU utilization
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

  float cpu_usage = ((total_time * 1. / sysconf(_SC_CLK_TCK)) / seconds);

  return cpu_usage;
}
// Read CPU info utilization
std::vector<long> LinuxParser::InfoCpu() {
  FileReader<long> filread2(kProcDirectory + kStatFilename);
  vector<long> res = filread2.GetVectorValue(characters::empty_);
  long idle = res[5];
  long iowait = res[6];
  long usertime = res[2];

  long nicetime = res[3];
  long irqtime = res[7];
  long softirq = res[8];
  long steal = res[9];

  long nonidle = usertime + nicetime + irqtime + softirq + steal;
  long total_idle = idle + iowait;
  long total = total_idle + nonidle;
  std::vector<long> info;
  info.push_back(total);
  info.push_back(total_idle);
  return info;
}
// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  FileReader<int> filread(kProcDirectory + kStatFilename);
  return filread.GetValue(filterProcesses);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  FileReader<int> filread(kProcDirectory + kStatFilename);
  return filread.GetValue(filterRunningProcesses);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  FileReader<string> filread(kProcDirectory + std::to_string(pid) +
                             kCmdlineFilename);
  return filread.GetValue();
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  FileReader<long> filread(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  float ram = filread.GetValue(filterProcMem) * 1. / 1024.;
  int format_ram = (int)(ram + 0.5);
  return std::to_string(format_ram);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  FileReader<string> filread(kProcDirectory + std::to_string(pid) +
                             kStatusFilename);
  return filread.GetValue(filterUID);
}

// Read and return the user associated with a process
// TODO: fix user mispelling
string LinuxParser::User(int pid) {
  FileReader<string> filread(kPasswordPath);
  vector<string> values = filread.GetVectorValue(characters::dpoint_);
  int size = filread.GetVectorValue(characters::dpoint_).size();
  for (int i = 0; i < size; i++) {
    if (!values[i].empty() && std::find_if(values[i].begin(), values[i].end(), [](unsigned char c) {return !std::isdigit(c);}) == values[i].end()) {
      try {
        if (std::stoi(values[i]) == std::stoi(LinuxParser::Uid(pid))) return values[i + 2];
      } catch (exception e) {
        cerr << "Invalid argument"
             << "\n";
      }
    }
  }
  return "";

}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  FileReader<long> filread(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  return UpTime() -
         filread.GetVectorValue(characters::empty_)[21] / sysconf(_SC_CLK_TCK);
}