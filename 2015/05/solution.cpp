#include <iostream>
#include <fstream>
#include <vector>

#include "utils.hpp"

bool is_nice(std::string line) {
  int vowels = 0;
  char c;
  for (int i = 0; i < line.length(); i++) {
    c = line[i];

    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
      vowels++;
    
    if (vowels >= 3)
      break;
  }

  if (vowels < 3)
    return false;

  bool repeat = false;
  char ca = line[0];
  for (int i = 1; i < line.length(); i++) {
    c = line[i];

    if (c == ca) {
      repeat = true;
      break;
    }

    ca = c;
  }

  if (!repeat)
    return false;

  ca = line[0];
  std::string pair = "01";
  for (int i = 1; i < line.length(); i++) {
    c = line[i];
    pair[0] = ca;
    pair[1] = c;

    if (pair == "ab" || pair == "cd" || pair == "pq" || pair == "xy")
      return false;
    
    ca = c;
  }

  return true;
}

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
  int nice = 0;
  for (std::string line; std::getline(file, line); )
  if (is_nice(line))
    nice++;

  std::cout << "Part 1\n  Nice strings : " << nice << std::endl;

  /* Part 2 */

  std::cout << "Part 2\n  Solution : " << std::endl;

  return 0;
}