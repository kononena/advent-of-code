#include <iostream>
#include <fstream>
#include <vector>

int main() {
  std::ifstream file;
  file.open("input.txt", std::ios::in);
  if (!file.is_open()) {
    std::cerr << "Error opening file\n";
    return 1;
  }
  
  char c = file.get();
  int state = 0;
  int a = 0;
  int b = 0;
  long sum = 0;

  while (c != -1) {
    int d = c - '0';

    if (c == 'm')
      state = 1;
    else if (c == 'u' && state == 1)
      state++;
    else if (c == 'l' && state == 2)
      state++;
    else if (c == '(' && state == 3) {
      state++;
      a = 0;
    }
    else if (d >= 0 && d < 10 && state == 4)
      a = a * 10 + d;
    else if (c == ',' && state == 4) {
      state = 0;
      if (a != 0) {
        state = 5;
        b = 0;
      }
    }
    else if (d >= 0 && d < 10 && state == 5)
      b = b * 10 + d;
    else if (c == ')' && state == 5) {
      sum += a * b;
      state = 0;
    }
    else
      state = 0;

    c = file.get();
  }

  std::cout << "Part 1\n  Result of multiplications : " << sum << std::endl;

  file.clear();
  file.seekg(0);
  c = file.get();

  int state_do = 0;
  bool enabled = true;
  sum = 0;

  while (c != -1) {
    int d = c - '0';

    if (c == 'd')
      state_do = 1;
    else if (c == 'o' && state_do == 1)
      state_do++;
    else if (c == '(' && state_do == 2)
      state_do++;
    else if (c == ')' && state_do == 3) {
      enabled = true;
      state_do = 0;
    }
    else if (c == 'n' && state_do == 2)
      state_do = 4;
    else if (c == '\'' && state_do == 4)
      state_do++;
    else if (c == 't' && state_do == 5)
      state_do++;
    else if (c == '(' && state_do == 6)
      state_do++;
    else if (c == ')' && state_do == 7) {
      enabled = false;
      state_do = 0;
    }
    else
      state_do = 0;

    if (enabled) {
      if (c == 'm')
        state = 1;
      else if (c == 'u' && state == 1)
        state++;
      else if (c == 'l' && state == 2)
        state++;
      else if (c == '(' && state == 3) {
        state++;
        a = 0;
      }
      else if (d >= 0 && d < 10 && state == 4)
        a = a * 10 + d;
      else if (c == ',' && state == 4) {
        state = 0;
        if (a != 0) {
          state = 5;
          b = 0;
        }
      }
      else if (d >= 0 && d < 10 && state == 5)
        b = b * 10 + d;
      else if (c == ')' && state == 5) {
        sum += a * b;
        state = 0;
      }
      else
        state = 0;
    }

    c = file.get();
  }

  std::cout << "Part 2\n  Result of enabled multiplications : " << sum << std::endl;

  return 0;
}