#include <iostream>
#include <string>
#include <vector>

using namespace std;
template <typename ValueType>
class FileReader {
 public:
  FileReader(string filename) : filename_(filename) {}
  ValueType GetValue(string key) {
    std::string line;
    std::string key__;
    ValueType value;
    std::ifstream filestream(filename_);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        while (linestream >> key__ >> value) {
          if (key__ == key) {
            return value;
          }
        }
      }
    }
  }

  ValueType GetValue() {
    string line;
    ValueType value;
    std::ifstream stream(filename_);
    while (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> value;
      return value;
    }
  }
  ValueType GetValue(int id) {
    vector<ValueType> tokens;
    ValueType value;
    std::string line;
    std::ifstream filestream(filename_);
    if (filestream.is_open()) {
      while (std::getline(filestream, line, ' ')) {
        std::istringstream linestream(line);
        linestream >> value;
        tokens.push_back(value);
      }
    }
    return tokens[id];
  }

 private:
  string filename_;
};