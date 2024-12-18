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
    value = value * (T)10 + (c - '0');
    c = file.get();
  }

  if (prev == '-')
    value *= -1;
  
  return true;
}
template bool utils::read_next_integer<int>(std::ifstream& file, int& value);
template bool utils::read_next_integer<long>(std::ifstream& file, long& value);

void utils::read_grid_2D(std::ifstream& file, std::vector<char>& grid, int nx_input, int ny_input, int padding, char padding_char) {
  int nx = nx_input + padding * 2;
  int ny = ny_input + padding * 2;
  grid.resize(nx * ny, padding_char);
  for (int y = padding; y < ny - padding; y++)
  for (int x = padding; x < nx - padding; x++)
    file >> grid[x + y * nx];
}

void utils::print_grid_2D(std::vector<char>& grid, int nx, int ny, char delimiter) {
  for (int i = 0; i < nx + 2; i++)
    std::cout << delimiter;
  std::cout << "\n";

  for (int y = 0; y < ny; y++) {
    std::cout << " ";
    for (int x = 0; x < nx; x++)
      std::cout << grid[x + y * nx];
    std::cout << "\n";
  }

  for (int i = 0; i < nx + 2; i++)
    std::cout << delimiter;
  std::cout << std::endl;
}

void utils::Timer::tic() {
  starting_time = std::chrono::high_resolution_clock::now();
}

double utils::Timer::toc() {
  ending_time = std::chrono::high_resolution_clock::now();
  return duration();
}

double utils::Timer::duration() {
  return (ending_time - starting_time).count() / 1e6;
}

void utils::Timer::print(int indentation) {
  for (int i = 0; i < indentation; i++)
    std::cout << "  ";
  std::cout << "Time elapsed : " << duration() << " ms." << std::endl;
}