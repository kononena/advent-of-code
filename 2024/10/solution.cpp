#include <iostream>
#include <fstream>
#include <vector>

int score(std::vector<char> map, std::vector<bool>& reached, int nx, int x, int y, char val, bool update_value) {
  if (val == '-' || map[x + y * nx] != val)
    return 0;
  if (val == '9') {
    int val = reached[x + y * nx] ? 0 : 1;
    reached[x + y * nx] = update_value;
    return val;
  }
  
  int trails = 0;
  trails += score(map, reached, nx, x+1, y  , val + 1, update_value);
  trails += score(map, reached, nx, x-1, y  , val + 1, update_value);
  trails += score(map, reached, nx, x  , y+1, val + 1, update_value);
  trails += score(map, reached, nx, x  , y-1, val + 1, update_value);
  return trails;
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
  
  int nx = 8;
  int ny = 8;
  if (is_input) {
    nx = 45;
    ny = 45;
  }
  nx += 2;
  ny += 2;

  std::vector<char> map(nx*ny, '-');
  for (int y = 1; y < ny - 1; y++)
  for (int x = 1; x < nx - 1; x++)
    file >> map[x + y * nx];
  
  int trailhead_scores = 0;
  int distinct_scores = 0;
  for (int y = 1; y < ny - 1; y++)
  for (int x = 1; x < nx - 1; x++) {
    if (map[x + y * nx] != '0')
      continue;
    
    std::vector<bool> reached(nx*ny, false);
    distinct_scores += score(map, reached, nx, x, y, '0', false);
    trailhead_scores += score(map, reached, nx, x, y, '0', true);
  }

  std::cout << "Part 1\n  Sum of trailhead scores : " << trailhead_scores << std::endl;

  /* Part 2*/

  std::cout << "Part 2\n  Sum of distinct trailhead scores : " << distinct_scores << std::endl;

  return 0;
}