#include <iostream>
#include <fstream>
#include <vector>

#include "utils.hpp"

void update_secret(uint32_t &secret) {
  secret = ((secret <<  6) ^ secret) % 16777216u;
  secret = ((secret >>  5) ^ secret) % 16777216u;
  secret = ((secret << 11) ^ secret) % 16777216u;
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

  int N = is_input ? 1729 : 4;
  std::vector<uint32_t> secrets(N, 0);
  for (int i = 0; i < N; i++)
    file >> secrets[i];
  
  /* Part 1 */
  for (int iteration = 0; iteration < 2000; iteration++)
  for (int i = 0; i < N; i++)
    update_secret(secrets[i]);
  
  long secret_sum = 0L;
  for (int i = 0; i < N; i++)
    secret_sum += secrets[i];

  std::cout << "Part 1\n  Sum of 2000th secrets : " << secret_sum << std::endl;

  /* Part 2 */

  std::cout << "Part 2\n  Solution : " << std::endl;

  return 0;
}