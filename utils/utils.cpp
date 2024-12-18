#include "utils.hpp"

template <typename T>
bool utils::read_next_integer(std::ifstream& file, T& value) {
  char c = '.', prev;
  while (c < '0' || c > '9') {
    if (c == -1)
      return false;
    prev = c;
    c = file.get();
  }

  value = (T)0;
  while (c >= '0' && c <= '9') {
    value = value * 10 + (c - '0');
    c = file.get();
  }

  if (prev == '-')
    value *= -1;
  
  return true;
}

void utils::read_grid_2D(std::ifstream& file, std::vector<char>& grid, int nx_input, int ny_input, int padding, char padding_char) {
  int nx = nx_input + padding * 2;
  int ny = ny_input + padding * 2;
  grid.resize(nx * ny, padding_char);
  for (int y = 1; y < ny - 1; y++)
  for (int x = 1; x < nx - 1; x++)
    file >> grid[x + y * nx];
}