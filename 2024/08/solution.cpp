#include <iostream>
#include <fstream>
#include <vector>

inline bool within_bounds(int x, int N) {
  return x >= 0 && x < N;
}

void process_pair(std::vector<bool>& antinodes, long& count, int N, int xa, int ya, int xb, int yb) {
  int x = xa - (xb - xa);
  int y = ya - (yb - ya);
  if (within_bounds(x, N) && within_bounds(y, N) && !antinodes[x + y * N]) {
    count++;
    antinodes[x + y * N] = true;
  }

  x = xb - (xa - xb);
  y = yb - (ya - yb);
  if (within_bounds(x, N) && within_bounds(y, N) && !antinodes[x + y * N]) {
    count++;
    antinodes[x + y * N] = true;
  }
}

void process_multipair(std::vector<bool>& antinodes, long& count, int N, int xa, int ya, int xb, int yb) {
  int x, y;
  for (int n = 0; n < N; n++) {
    x = xa - (xb - xa) * n;
    y = ya - (yb - ya) * n;
    if (!within_bounds(x, N) || !within_bounds(y, N))
      break;
    
    if (!antinodes[x + y * N]) {
      count++;
      antinodes[x + y * N] = true;
    }
  }

  for (int n = 0; n < N; n++) {
    x = xb - (xa - xb) * n;
    y = yb - (ya - yb) * n;
    if (!within_bounds(x, N) || !within_bounds(y, N))
      break;
    
    if (!antinodes[x + y * N]) {
      count++;
      antinodes[x + y * N] = true;
    }
  }
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
  
  int N = 12;
  if (filename == "input.txt")
    N = 50;
  
  std::vector<char> map(N*N, '.');
  for (int y = 0; y < N; y++)
  for (int x = 0; x < N; x++)
    file >> map[x + y * N];
  
  std::vector<bool> antinodes(N*N, false);

  long unique_locations = 0L;
  for (int y = 0; y < N; y++)
  for (int x = 0; x < N; x++) {
    if (map[x + y * N] == '.')
      continue;
    
    char c = map[x + y * N];
    for (int yi = y; yi < N; yi++)
    for (int xi = (yi == y ? x+1 : 0); xi < N; xi++)
    if (map[xi + yi * N] == c)
      process_pair(antinodes, unique_locations, N, x, y, xi, yi);
  }

  std::cout << "Part 1\n  Unique locations : " << unique_locations << std::endl;

  for (int i = 0; i < N*N; i++)
    antinodes[i] = false;
  
  unique_locations = 0L;
  for (int y = 0; y < N; y++)
  for (int x = 0; x < N; x++) {
    if (map[x + y * N] == '.')
      continue;
    
    char c = map[x + y * N];
    for (int yi = y; yi < N; yi++)
    for (int xi = (yi == y ? x+1 : 0); xi < N; xi++)
    if (map[xi + yi * N] == c)
      process_multipair(antinodes, unique_locations, N, x, y, xi, yi);
  }

  std::cout << "Part 2\n  Unique multilocations : " << unique_locations << std::endl;

  return 0;
}