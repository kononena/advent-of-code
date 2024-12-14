#include <iostream>
#include <fstream>
#include <vector>

int main() {
  std::string filename = "example.txt";
  std::cout << "Using file " << filename << std::endl;
  std::ifstream file;
  file.open(filename, std::ios::in);
  if (!file.is_open()) {
    std::cerr << "Error opening file" << std::endl;
    return 1;
  }
  
  /* Part 1*/

  std::cout << "Part 1\n  Solution : " << std::endl;

  /* Part 2*/

  std::cout << "Part 2\n  Solution : " << std::endl;

  return 0;
}