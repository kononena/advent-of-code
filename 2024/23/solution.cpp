#include <iostream>
#include <fstream>
#include <vector>

#include "utils.hpp"

#define NAB 26
#define NAB2 NAB*NAB

std::string id_to_name(int id) {
  std::string out = "01";
  out[0] = (id / NAB) + 'a';
  out[1] = (id % NAB) + 'a';
  return out;
}

std::string clique_to_string(const std::vector<int> &computers, int depth) {
  std::string clique = "";
  for (int i = 0; i < depth-1; i++)
    clique += id_to_name(computers[i]) + ",";
  clique += id_to_name(computers[depth-1]);
  return clique;
}

bool any_t(const std::vector<int> &computers, int depth) {
  for (int i = 0; i < depth; i++)
  if ((computers[i] / NAB) == 't' - 'a')
    return true;

  return false;
}

long valid_subsets(const std::vector<bool> &connections, std::vector<int> &computers, int depth, const int target, bool check_t = true, bool exit_early = false) {
  if (depth + 1 == target) {
    if (!check_t || any_t(computers, depth+1))
      return 1L;
    return 0L;
  }
  
  long subsets = 0L;
  const int prev = computers[depth];

  for (int i = prev + 1; i < NAB2; i++) {
    if (!connections[i + prev * NAB2])
      continue;
    
    bool valid = true;
    for (int j = 0; j < depth; j++)
    if (!connections[i + computers[j] * NAB2]) {
      valid = false;
      break;
    }

    if (!valid)
      continue;
    
    computers[depth+1] = i;
    subsets += valid_subsets(connections, computers, depth+1, target, check_t, 
    
    exit_early);

    if (exit_early && subsets > 0L)
      return subsets;
  }

  return subsets;
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
  std::vector<bool> connections(NAB2 * NAB2, false); // use upper triangle only
  for (std::string line; std::getline(file, line); ) {
    int a = (line[0] - 'a') * NAB + (line[1] - 'a');
    int b = (line[3] - 'a') * NAB + (line[4] - 'a');

    if (a > b)
      connections[a + b * NAB2] = true;
    else
      connections[b + a * NAB2] = true;
  }

  timer.tic();
  std::vector<int> computers(3, 0);
  long sets_of_three = 0L;
  for (int i = 0; i < NAB2; i++) {
    computers[0] = i;
    sets_of_three += valid_subsets(connections, computers, 0, 3);
  }
  timer.toc();

  std::cout << "Part 1\n  Inter-connected sets of three : " << sets_of_three << std::endl;
  timer.print();

  /* Part 2 */
  timer.tic();
  int target = 2;
  while (++target < 100) {
    long sets = 0;
    computers.resize(target);

    for (int i = 0; i < NAB2; i++) {
      computers[0] = i;
      sets += valid_subsets(connections, computers, 0, target, false);
    }

    if (sets == 0) {
      target--;
      break;
    }
  }

  std::string password;
  for (int i = 0; i < NAB2; i++) {
    computers[0] = i;
    if (valid_subsets(connections, computers, 0, target, false, true) > 0) {
      password = clique_to_string(computers, target);
      break;
    }
  }
  timer.toc();

  std::cout << "Part 2\n  LAN password : " << password << std::endl;
  timer.print();
  return 0;
}