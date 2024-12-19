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
  long floor = 0L;
  char c = file.get();
  while (c != -1) {
    if (c == '(')
      floor++;
    else
      floor--;
    c = file.get();
  }

  std::cout << "Part 1\n  Final floor : " << floor << std::endl;

  /* Part 2 */

  std::cout << "Part 2\n  Solution : " << std::endl;

  return 0;
}