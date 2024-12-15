#include <iostream>
#include <fstream>
#include <vector>

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
  
  std::vector<long> fish_A(9, 0L);
  std::vector<long> fish_B(9, 0L);
  char c = '.';
  while (c != -1) {
    c = file.get();
    if (c == -1)
      break;
    
    if (c == ',')
      continue;
    
    fish_A[c - '0'] += 1L;
  }

  std::vector<long>* fish_cur = &fish_A;
  std::vector<long>* fish_new = &fish_B;
  std::vector<long>* temp;
  for (int i = 0; i < 80; i++) {
    for (int j = 0; j < 8; j++)
      (*fish_new)[j] = (*fish_cur)[j+1];
    (*fish_new)[6] += (*fish_cur)[0];
    (*fish_new)[8]  = (*fish_cur)[0];

    temp = fish_cur;
    fish_cur = fish_new;
    fish_new = temp;
  }

  long number_of_lanternfish = 0L;
  for (int i = 0; i < 9; i++)
    number_of_lanternfish += (*fish_cur)[i];

  std::cout << "Part 1\n  Lanternfish after 80 days : " << number_of_lanternfish << std::endl;

  for (int i = 0; i < 256 - 80; i++) {
    for (int j = 0; j < 8; j++)
      (*fish_new)[j] = (*fish_cur)[j+1];
    (*fish_new)[6] += (*fish_cur)[0];
    (*fish_new)[8]  = (*fish_cur)[0];

    temp = fish_cur;
    fish_cur = fish_new;
    fish_new = temp;
  }

  number_of_lanternfish = 0L;
  for (int i = 0; i < 9; i++)
    number_of_lanternfish += (*fish_cur)[i];

  std::cout << "Part 2\n  Lanternfish after 256 days : " << number_of_lanternfish << std::endl;

  return 0;
}