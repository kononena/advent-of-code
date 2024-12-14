#include <iostream>
#include <fstream>
#include <vector>

template <typename T>
void read_integer(std::string line, int& start, T& output) {
  char c = line[start];
  while (c < '0' || c > '9')
    c = line[++start];

  while (c >= '0' && c <= '9') {
    output = output * (T)10 + (c - '0');
    c = line[++start];
  }
}

template <typename T>
void read_next_pair(std::string line, int& start, std::pair<T, T>& pair) {
  read_integer(line, start, pair.first);
  read_integer(line, start, pair.second);
}

long tokens_simple(long Ax, long Ay, long Bx, long By, long Px, long Py) {
  // Ax a + Bx b = Px
  // Ay a + By b = Py
  long aN = Px * By - Py * Bx;
  long aD = Ax * By - Ay * Bx;

  if (aD == 0 || aN % aD != 0L)
    return 0L;

  long bN = Px * Ay - Py * Ax;
  long bD = -aD;

  if (bN % bD != 0L)
    return 0L;

  long a = aN / aD;
  long b = bN / bD;

  return 3L * a + b;
}

int main() {
  std::string filename = "input.txt";
  std::cout << "Using file " << filename << std::endl;
  std::ifstream file;
  file.open(filename, std::ios::in);
  if (!file.is_open()) {
    std::cerr << "Error opening file" << std::endl;
    return 1;
  }

  int state = 0;
  long tokens = 0L;
  std::pair<long, long> A;
  std::pair<long, long> B;
  std::pair<long, long> P;
  for (std::string line; std::getline(file, line); ) {
    if (line.empty())
      continue;
    
    int start = 0;

    if (state == 0) {
      read_next_pair(line, start, A);
      state++;
    } else if (state == 1) {
      read_next_pair(line, start, B);
      state++;
    } else if (state == 2) {
      read_next_pair(line, start, P);
      tokens += tokens_simple(A.first, A.second, B.first, B.second, P.first, P.second);
      A.first = 0L; A.second = 0L;
      B.first = 0L; B.second = 0L;
      P.first = 0L; P.second = 0L;
      state = 0;
    }
  }

  std::cout << "Part 1\n  Fewest tokens : " << tokens << std::endl;

  file.clear();
  file.seekg(0);
  state = 0;
  tokens = 0L;
  for (std::string line; std::getline(file, line); ) {
    if (line.empty())
      continue;
    
    int start = 0;

    if (state == 0) {
      read_next_pair(line, start, A);
      state++;
    } else if (state == 1) {
      read_next_pair(line, start, B);
      state++;
    } else if (state == 2) {
      read_next_pair(line, start, P);
      tokens += tokens_simple(A.first, A.second, B.first, B.second, P.first + 10000000000000L, P.second + 10000000000000L);
      A.first = 0L; A.second = 0L;
      B.first = 0L; B.second = 0L;
      P.first = 0L; P.second = 0L;
      state = 0;
    }
  }

  std::cout << "Part 2\n  Fewest tokens : " << tokens << std::endl;

  return 0;
}