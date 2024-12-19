#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "utils.hpp"

void add_or_increment(std::unordered_map<int, int>& map, int val) {
  if (map.find(val) == map.end())
    map[val] = 1;
  else
    map[val] += 1;
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
  std::unordered_map<int, int> deliveries;
  deliveries[0] = 1;
  int x = 0, y = 0;
  char c = file.get();
  while (c != -1) {
    switch (c) {
      case 'v':
        y += 1;
        break;
      case '^':
        y -= 1;
        break;
      case '<':
        x -= 1;
        break;
      case '>':
        x += 1;
        break;
    }

    add_or_increment(deliveries, x + (y << 15));
    c = file.get();
  }

  long unique_houses = deliveries.size();

  std::cout << "Part 1\n  Houses with any presents : " << unique_houses << std::endl;

  /* Part 2 */

  std::cout << "Part 2\n  Solution : " << std::endl;

  return 0;
}