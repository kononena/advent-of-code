#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

std::vector<int> extract_report(std::string line) {
  int numbers = 1;
  for (int i = 0; i < line.length(); i++)
  if (line[i] == ' ') {
    numbers++;
    line[i] = '\n';
  }
  
  std::vector<int> report(numbers, 0);
  std::istringstream stream(line);
  int j = 0;
  for (std::string elem; getline(stream, elem); )
    report[j++] = stoi(elem);

  return report;
}

bool is_report_safe(std::vector<int> report) {
  int N = report.size();
  if (N <= 1)
    return true;
  
  bool is_increasing = report[1] > report[0];
  for (int i = 0; i < N-1; i++) {
    int a = report[i+1];
    int b = report[i];
    if (((a > b) != is_increasing) || (abs(a - b) > 3) || (a == b))
      return false;
  }

  return true;
}

int main() {
  std::ifstream file;
  file.open("input.txt", std::ios::in);
  if (!file.is_open()) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  long safe_reports = 0;
  for (std::string line; std::getline(file, line); ) {
    auto report = extract_report(line);
    safe_reports += is_report_safe(report) ? 1 : 0;
  }

  std::cout << "Part 1\n  Safe reports : " << safe_reports << std::endl;

  file.clear();
  file.seekg(0);
  safe_reports = 0;
  for (std::string line; std::getline(file, line); ) {
    auto report = extract_report(line);
    if (is_report_safe(report)) {
      safe_reports++;
      continue;
    }

    std::vector<int> dampened_report(report.size() - 1, 0);
    for (int i = 0; i < report.size(); i++) {
      // Copy report into dampened, but exclude i:th element
      int k = 0; // target index
      int j = 0; // source index
      while (j < report.size() && k < report.size() - 1) {
        if (j == i)
          j++;
        dampened_report[k++] = report[j++];
      }

      if (is_report_safe(dampened_report)) {
        safe_reports++;
        break;
      }
    }
  }

  std::cout << "Part 2\n  Safe dampened reports : " << safe_reports << std::endl;

  return 0;
}