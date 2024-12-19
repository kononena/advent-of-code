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

  int n_packages = 2;
  if (is_input)
    n_packages = 1000;
  
  /* Part 1 */
  long wrapping_paper = 0L;
  int l, w, h;
  for (int i = 0; i < n_packages; i++) {
    utils::read_next_integer(file, l);
    utils::read_next_integer(file, w);
    utils::read_next_integer(file, h);

    wrapping_paper += 2 * (l*w + l*h + w*h);
    wrapping_paper += std::min(l * std::min(w, h), w * h);
  }

  std::cout << "Part 1\n  Total wrapping paper : " << wrapping_paper << std::endl;

  /* Part 2 */

  std::cout << "Part 2\n  Solution : " << std::endl;

  return 0;
}