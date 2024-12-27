#include <iostream>
#include <fstream>
#include <vector>

#include "utils.hpp"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Provide filename" << std::endl;
    return 1;
  }
  std::string input(argv[1]);
  std::string filename;
  if (input[0] == 'e')
    filename = "example.txt";
  else if (input[0] == 'i')
    filename = "input.txt";
  
  std::cout << "Using file " << filename << std::endl;
  std::ifstream file;
  file.open(filename, std::ios::in);
  if (!file.is_open()) {
    std::cerr << "Error opening file" << std::endl;
    return 1;
  }

  bool is_input = filename == "input.txt";
  utils::Timer timer;
  
  /* Part 1 */
  std::vector<std::vector<int>> locks;
  std::vector<std::vector<int>> keys;
  bool is_lock, new_element = true;
  std::vector<int> current = { -1, -1, -1, -1, -1 };
  for (std::string line; std::getline(file, line); ) {
    if (line.empty()) {
      // Flush the key or lock
      if (is_lock)
        locks.push_back(current);
      else
        keys.push_back(current);

      new_element = true;
      for (int i = 0; i < 5; i++)
        current[i] = -1;
      
      continue;
    }

    if (new_element) {
      is_lock = line == "#####";
      new_element = false;
    }

    for (int i = 0; i < 5; i++)
      current[i] += (line[i] == '#') ? 1 : 0;
  }

  long unique_combinations = 0L;
  for (int l = 0; l < locks.size(); l++)
  for (int k = 0; k < keys.size(); k++) {
    bool valid = true;
    for (int i = 0; i < 5; i++)
    if (locks[l][i] + keys[k][i] > 5) {
      valid = false;
      break;
    }

    if (valid)
      unique_combinations++;
  }

  std::cout << "Part 1\n  Unique lock/key pairs : " << unique_combinations << std::endl;

  /* Part 2 */

  std::cout << "Part 2\n  Merry Christmas!" << std::endl;

  return 0;
}