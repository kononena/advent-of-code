#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>

#include "utils.hpp"

long number_of_solutions(std::unordered_set<std::string>& towels, std::string design, size_t max_towel_length) {
  int ND = design.length();
  std::vector<long> solutions(ND + 1, 0);
  solutions[ND] = 1;

  for (int i = ND - 1; i >= 0; i--) {
    std::string towel = "";
    for (int j = 0; j < max_towel_length && i + j < ND; j++) {
      towel += design[i + j];
      if (towels.find(towel) != towels.end())
        solutions[i] += solutions[i + j + 1];
    }
  }

  return solutions[0];
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
  
  /* Parse input */
  // std::vector<std::string> towels;
  std::unordered_set<std::string> towels;
  std::vector<std::string> designs;
  std::string towel;
  size_t max_towel_length = 0;
  char c;
  while (true) {
    file >> towel;
    c = towel.back();

    if (c == ',')
      towel.pop_back();
    
    towels.insert(towel);
    max_towel_length = std::max(max_towel_length, towel.length());
    if (c != ',')
      break;
  }
  while (true) {
    file >> towel;
    if (file.eof())
      break;
    
    designs.push_back(towel);
  }

  /* Part 1 */
  timer.tic();
  long number_of_designs = 0;
  for (int i = 0; i < designs.size(); i++)
  if (number_of_solutions(towels, designs[i], max_towel_length) > 0L)
    number_of_designs++;
  timer.toc();

  // Greater than 179
  std::cout << "Part 1\n  Number of possible designs : " << number_of_designs << std::endl;
  timer.print();

  /* Part 2 */
  timer.tic();
  number_of_designs = 0;
  for (int i = 0; i < designs.size(); i++)
    number_of_designs += number_of_solutions(towels, designs[i], max_towel_length);
  timer.toc();

  std::cout << "Part 2\n  Number of distinct designs : " << number_of_designs << std::endl;
  timer.print();

  return 0;
}