#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>

int main() {
  std::string filename = "input.txt";
  std::cout << "Using file " << filename << std::endl;
  std::ifstream file;
  file.open(filename, std::ios::in);
  if (!file.is_open()) {
    std::cerr << "Error opening file" << std::endl;
    return 1;
  }
  
  std::unordered_set<int> rules;

  // Prepare page ordering rules
  for (std::string line; std::getline(file, line); ) {
    if (line.empty())
      break;

    for (int i = 0; i < line.length(); i++)
    if (line[i] == '|') {
      line[i] = ' ';
      break;
    }

    std::istringstream iss(line);
    int a, b;
    iss >> a >> b;
    rules.insert(a * 100 + b);
  }

  // Process updates
  long sum_of_middle_pages = 0;
  for (std::string line; std::getline(file, line); ) {
    int n_elems = 1;
    for (int i = 0; i < line.length(); i++)
    if (line[i] == ',') {
      line[i] = ' ';
      n_elems++;
    }
    
    std::istringstream iss(line);
    std::vector<int> update(n_elems, 0);
    for (int i = 0; i < n_elems; i++)
      iss >> update[i];

    bool valid = true;
    for (int i = 0; i < n_elems; i++) {
      int b = update[i];

      for (int j = i+1; j < n_elems; j++) {
        int a = update[j];
        if (rules.find(a * 100 + b) != rules.end()) {
          valid = false;
          break;
        }
      }

      if (!valid)
        break;
    }

    if (valid)
      sum_of_middle_pages += update[n_elems / 2];
  }

  std::cout << "Part 1\n  Sum of middle pages : " << sum_of_middle_pages << std::endl;



  // Reset input file
  file.clear();
  file.seekg(0);
  for (std::string line; std::getline(file, line); )
  if (line.empty())
    break;

  // Process updates
  sum_of_middle_pages = 0;
  for (std::string line; std::getline(file, line); ) {
    int n_elems = 1;
    for (int i = 0; i < line.length(); i++)
    if (line[i] == ',') {
      line[i] = ' ';
      n_elems++;
    }
    
    // Problem implies there is a correct ordering for the update
    // Thus the number of applicable rules can be used to find the median
    // Set the score of a page as the number of applicable rules
    std::istringstream iss(line);
    std::vector<int> update(n_elems, 0);
    std::vector<int> scores(n_elems, 0);
    for (int i = 0; i < n_elems; i++)
      iss >> update[i];

    bool valid = true;
    for (int i = 0; i < n_elems; i++) {
      int b = update[i];

      for (int j = 0; j < n_elems; j++) {
        int a = update[j];
        if (rules.find(a * 100 + b) != rules.end()) {
          if (j > i)
            valid = false;
          scores[i]++;
        }
      }
    }

    if (valid)
      continue;
    
    int target = n_elems / 2;
    for (int i = 0; i < n_elems; i++)
    if (scores[i] == target) {
      sum_of_middle_pages += update[i];
      break;
    }
  }

  std::cout << "Part 2\n  Sum of corrected middle pages : " << sum_of_middle_pages << std::endl;

  return 0;
}