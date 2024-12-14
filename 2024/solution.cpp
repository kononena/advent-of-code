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
  
  

  return 0;
}