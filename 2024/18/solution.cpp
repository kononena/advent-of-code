#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <chrono>

#include "utils.hpp"

void perform_bfs(std::vector<char>& map, std::vector<int>& steps, std::queue<int>& pos, int nxy) {
  while (!pos.empty()) {
    int x = pos.front() % nxy;
    int y = pos.front() / nxy;
    pos.pop();

    int score = steps[x + y * nxy];
    int dir[4] = { 1, -1, nxy, -nxy };
    for (int i = 0; i < 4; i++) {
      int xy_new = x + y * nxy + dir[i];
      if (map[xy_new] != '#' && steps[xy_new] > score + 1) {
        pos.push(xy_new);
        steps[xy_new] = score + 1;
      }
    }
  }
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
  else if (input[0] == 'l')
    filename = "large-example.txt";
  
  std::cout << "Using file " << filename << std::endl;
  std::ifstream file;
  file.open(filename, std::ios::in);
  if (!file.is_open()) {
    std::cerr << "Error opening file" << std::endl;
    return 1;
  }

  bool is_input = filename == "input.txt";
  bool is_large = filename == "large-example.txt";
  
  int nxy = 7;
  int n_bytes = 25;
  int bytes_part_1 = 12;
  if (is_input) {
    nxy = 71;
    n_bytes = 3450;
    bytes_part_1 = 1024;
  } else if (is_large) {
    nxy = 213;
    n_bytes = 31051;
    bytes_part_1 = 1024;
  }
  nxy += 2; // padding
  
  std::vector<int> bytes(n_bytes, 0);
  for (int i = 0; i < n_bytes; i++) {
    int x, y;
    utils::read_next_integer(file, x);
    utils::read_next_integer(file, y);
    bytes[i] = (x+1) + (y+1) * nxy; // account for padding
  }

  std::vector<char> map(nxy * nxy, '.');
  // set boundaries
  for (int y = 0; y < nxy; y++) {
    map[        y * nxy] = '#';
    map[nxy-1 + y * nxy] = '#';
  }
  for (int x = 0; x < nxy; x++) {
    map[x                ] = '#';
    map[x + (nxy-1) * nxy] = '#';
  }
  // let bytes fall where they may
  for (int i = 0; i < bytes_part_1; i++)
    map[bytes[i]] = '#';
  
  std::vector<int> steps(nxy * nxy, nxy * nxy);
  std::queue<int> pos;
  pos.push(1 + nxy); // account for padding
  steps[1 + nxy] = 0;

  perform_bfs(map, steps, pos, nxy);

  std::cout << "Part 1\n  Minimum steps to exit : " << steps[(nxy-2) + (nxy-2) * nxy] << std::endl;

  // Removing fallen bytes can only decrease the steps score of a tile
  // Start from all bytes fallen, then iterate backwards
  // !NB! This assumes no bytes fall on the same position
  for (int i = bytes_part_1; i < n_bytes; i++)
    map[bytes[i]] = '#';
  
  for (int y = 0; y < nxy; y++)
  for (int x = 0; x < nxy; x++)
    steps[x + y * nxy] = nxy * nxy;
  steps[1 + nxy] = 0;
  pos.push(1 + nxy);

  int index_of_last_fallen_byte = n_bytes;
  utils::Timer timer;
  timer.tic();
  while (steps[(nxy-2) + (nxy-2) * nxy] == nxy * nxy) {
    map[bytes[--index_of_last_fallen_byte]] = '.';

    // // initialize next search by all previously reached states
    // for (int y = 1; y < nxy; y++)
    // for (int x = 1; x < nxy; x++)
    // if (steps[x + y  * nxy] < nxy * nxy)
    //   pos.push(x + y * nxy);
    
    // optimization : initialize next search by the unblocked position
    // speeds up solution by a factor of ~nxy
    int x = bytes[index_of_last_fallen_byte] % nxy;
    int y = bytes[index_of_last_fallen_byte] / nxy;
    int dir[4] = { 1, -1, nxy, -nxy };
    for (int i = 0; i < 4; i++)
      steps[x + y * nxy] = std::min(steps[x + y * nxy], steps[x + y * nxy + dir[i]]);
    pos.push(x + y * nxy);
    
    perform_bfs(map, steps, pos, nxy);
  }
  timer.toc();

  int x = bytes[index_of_last_fallen_byte] % nxy;
  int y = bytes[index_of_last_fallen_byte] / nxy;
  std::cout << "Part 2\n  First blocking byte : " << x-1 << "," << y-1 << std::endl;
  timer.print();

  return 0;
}