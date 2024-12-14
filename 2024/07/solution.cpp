#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

int main() {
  std::string filename = "input.txt";
  std::cout << "Using file " << filename << std::endl;
  std::ifstream file;
  file.open(filename, std::ios::in);
  if (!file.is_open()) {
    std::cerr << "Error opening file" << std::endl;
    return 1;
  }
  
  int N = 9;
  if (filename == "input.txt")
    N = 850;
  
  std::string line;
  long target;
  long calibration_result = 0L;
  for (int Li = 0; Li < N; Li++) {
    std::getline(file, line);
    long number = 0;
    for (long i = 0; i < line.length(); i++) {
      if (line[i] == ':')
        line[i] = ' ';
      else if (line[i] == ' ')
        number++;
    }
    
    std::istringstream iss(line);
    iss >> target;
    std::vector<long> elements(number, 0);
    for (long i = 0; i < number; i++)
      iss >> elements[i];
    
    std::vector<long> possibilities_cur;
    std::vector<long> possibilities_new;
    possibilities_cur.push_back(elements[0]);
    for (long i = 1; i < number; i++) {
      long elem = elements[i];
      for (long j = 0; j < possibilities_cur.size(); j++) {
        long pos = possibilities_cur[j];
        possibilities_new.push_back(pos + elem);
        possibilities_new.push_back(pos * elem);
      }
      possibilities_cur = possibilities_new;
      possibilities_new.clear();
    }

    for (long j = 0; j < possibilities_cur.size(); j++) {
      if (possibilities_cur[j] == target) {
        calibration_result += target;
        break;
      }
    }
  }

  std::cout << "Part 1\n  Calibration result : " << calibration_result << std::endl;

  file.clear();
  file.seekg(0);
  calibration_result = 0L;
  for (int Li = 0; Li < N; Li++) {
    std::getline(file, line);
    long number = 0;
    for (long i = 0; i < line.length(); i++) {
      if (line[i] == ':')
        line[i] = ' ';
      else if (line[i] == ' ')
        number++;
    }
    
    std::istringstream iss(line);
    iss >> target;
    std::vector<long> elements(number, 0);
    for (long i = 0; i < number; i++)
      iss >> elements[i];
    
    std::vector<long> possibilities_cur;
    std::vector<long> possibilities_new;
    possibilities_cur.push_back(elements[0]);
    for (long i = 1; i < number; i++) {
      long elem = elements[i];
      for (long j = 0; j < possibilities_cur.size(); j++) {
        long pos = possibilities_cur[j];
        possibilities_new.push_back(pos + elem);
        possibilities_new.push_back(pos * elem);
        possibilities_new.push_back(std::stol(std::to_string(pos) + std::to_string(elem)));
      }
      possibilities_cur = possibilities_new;
      possibilities_new.clear();
    }

    for (long j = 0; j < possibilities_cur.size(); j++) {
      if (possibilities_cur[j] == target) {
        calibration_result += target;
        break;
      }
    }
  }

  std::cout << "Part 2\n  Calibration result : " << calibration_result << std::endl;

  return 0;
}