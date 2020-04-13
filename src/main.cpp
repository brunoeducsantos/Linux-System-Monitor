#include "ncurses_display.h"
#include "system.h"
#include <iostream>
int main() {
  System system;
  float res= system.MemoryUtilization();
  NCursesDisplay::Display(system);
}