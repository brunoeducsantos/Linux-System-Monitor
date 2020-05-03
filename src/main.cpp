#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include "linux_parser.h"
int main() {
  System system;
  LinuxParser::CpuUtilization();
  NCursesDisplay::Display(system);
}