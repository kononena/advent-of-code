#include <iostream>
#include <fstream>
#include <vector>

long count_appearances(std::vector<char> grid, int nx, int ny, std::string target) {
  long appearances = 0;

  for (int x = 1; x < nx - 1; x++)
  for (int y = 1; y < ny - 1; y++) {
    int sx = x;
    int sy = y;

    for (int ox = -1; ox <= 1; ox++)
    for (int oy = -1; oy <= 1; oy++) {
      char c;
      int t = 0;

      for (; t < target.length(); t++) {
        int tx = sx + ox * t;
        int ty = sy + oy * t;
        c = grid[tx + ty * nx];
        if (c != target[t])
          break;
      }

      if (t == target.length())
        appearances++;
    }
  }

  return appearances;
}

long count_cross_appearances(std::vector<char> grid, int nx, int ny) {
  long appearances = 0;

  for (int x = 0; x < nx; x++)
  for (int y = 0; y < ny; y++) {
    if (grid[x + y * nx] != 'A')
      continue;
    
    std::string subgrid = "...---...";
    // 0 1 2
    // 3 4 5
    // 6 7 8

    for (int ox = -1; ox <= 1; ox++)
    for (int oy = -1; oy <= 1; oy++)
      subgrid[(ox + 1) + (oy + 1) * 3] = grid[(x + ox) + (y + oy) * nx];
    
    if (
      (subgrid[0] == 'M' || subgrid[8] == 'M') &
      (subgrid[0] == 'S' || subgrid[8] == 'S') &
      (subgrid[2] == 'M' || subgrid[6] == 'M') &
      (subgrid[2] == 'S' || subgrid[6] == 'S')
    )
      appearances++;
  }

  return appearances;
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
  
  int nx = 12;
  int ny = 12;
  if (filename == "input.txt") {
    nx = 142;
    ny = 142;
  }

  std::vector<char> grid(nx*ny, '-');
  for (int x = 1; x < nx-1; x++)
  for (int y = 1; y < ny-1; y++)
    file >> grid[x + y * nx];

  std::string target = "XMAS";
  long appearances = count_appearances(grid, nx, ny, target);

  std::cout << "Part 1\n  XMAS appearances : " << appearances << std::endl;

  appearances = count_cross_appearances(grid, nx, ny);

  std::cout << "Part 2\n  X-MAS appearances : " << appearances << std::endl;

  return 0;
}