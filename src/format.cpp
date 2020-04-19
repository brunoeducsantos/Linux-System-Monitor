#include <string>
#include<iostream>
#include <cmath>
#include "format.h"

using std::string;
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    long hours= floor(seconds/3600);
    long mins= floor(seconds % 3600 /60);
    long secs= floor(seconds%3600%60);
    return  std::to_string(hours)+":"+std::to_string(mins)+":"+std::to_string(secs);
}