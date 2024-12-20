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

  int nxy = 15;
  if (is_input)
    nxy = 141;
  
  std::vector<char> map;
  int sx, sy, ex, ey;
  utils::read_grid_2D(file, map, nxy, nxy, 0, '#');
  for (int y = 0; y < nxy; y++)
  for (int x = 0; x < nxy; x++) {
    if (map[x + y * nxy] == 'S') {
      sx = x; sy = y;
      map[x + y * nxy] = '.';
    } else if (map[x + y * nxy] == 'E') {
      ex = x; ey = y;
      map[x + y * nxy] = '.';
    }
  }
  
  /* Part 1 */
  std::vector<int> scores(nxy * nxy, nxy * nxy);
  scores[sx + sy * nxy] = 0;
  int x = sx, y = sy;
  int next_x, next_y;
  int score;
  const int dir[4] = { 1, -1, nxy, -nxy };
  while (!(x == ex && y == ey)) {
    score = scores[x + y * nxy];
    for (int i = 0; i < 4; i++) {
      int target = x + y * nxy + dir[i];
      if (map[target] == '.' && scores[target] > score) {
        next_x = (target) % nxy;
        next_y = (target) / nxy;
        scores[target] = std::min(scores[target], score + 1);
      }
    }

    x = next_x; y = next_y;
  }

  long good_cheats = 0L;
  int minimum_save = 2;
  if (is_input)
    minimum_save = 100;
  for (int y = 1; y < nxy - 1; y++)
  for (int x = 1; x < nxy - 1; x++)
  if (map[x + y * nxy] == '#') {
    for (int i = 0; i < 4; i++)
    for (int j = i+1; j < 4; j++) {
      int target_i = x + y * nxy + dir[i];
      int target_j = x + y * nxy + dir[j];
      if (scores[target_i] < nxy * nxy && scores[target_j]< nxy * nxy && std::abs(scores[target_i] - scores[target_j]) - 2 >= minimum_save)
        good_cheats++;
    }
  }

  std::cout << "Part 1\n  Cheats saving at least " << minimum_save << " picoseconds : " << good_cheats << std::endl;

  /* Part 2 */
  good_cheats = 0L;
  minimum_save = 74;
  if (is_input)
    minimum_save = 100;
  // Cheats start on a unique track position...
  for (int y = 0; y < nxy; y++)
  for (int x = 0; x < nxy; x++)
  if (map[x + y * nxy] == '.') {
    score = scores[x + y * nxy];
    
    // ...and end on a unique track position within 20 moves
    for (int tx = std::max(0, x - 20); tx < std::min(nxy, x + 21); tx++)
    for (int ty = std::max(0, y - 20); ty < std::min(nxy, y + 21); ty++) {
      int distance = std::abs(x - tx) + std::abs(y - ty);
      if (map[tx + ty * nxy] == '.' && distance <= 20) {
        if (scores[tx + ty * nxy] - score - distance >= minimum_save)
          good_cheats++;
      }
    }
  }

  std::cout << "Part 2\n  Long cheats saving at least " << minimum_save << " picoseconds : " << good_cheats << std::endl;

  return 0;
}