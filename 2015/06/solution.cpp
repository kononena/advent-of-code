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

  int n_instructions = is_input ? 300 : 3;
  
  /* Part 1 */
  std::vector<bool> grid(1000 * 1000, false);
  char c;
  int state; // turn on: 0, turn off: 1, toggle: 2
  int xa, xb, ya, yb;
  for (int i = 0; i < n_instructions; i++) {
    c = file.get();
    while (c != 't')
      c = file.get();

    c = file.get();

    if (c == 'o')
      state = 2;
    else {
      while (c != 'o')
        c = file.get();

      c = file.get();
      if (c == 'n')
        state = 0;
      else
        state = 1;
    }

    utils::read_next_integer(file, xa);
    utils::read_next_integer(file, ya);
    utils::read_next_integer(file, xb);
    utils::read_next_integer(file, yb);

    for (int y = ya; y <= yb; y++)
    for (int x = xa; x <= xb; x++) {
      if (state == 0)
        grid[x + y * 1000] = true;
      else if (state == 1)
        grid[x + y * 1000] = false;
      else
        grid[x + y * 1000] = !grid[x + y * 1000];
    }
  }

  int lit_lights = 0;
  for (int i = 0; i < 1000 * 1000; i++)
  if (grid[i])
    lit_lights++;

  std::cout << "Part 1\n  Number of lights lit : " << lit_lights << std::endl;

  /* Part 2 */
  file.clear();
  file.seekg(0);

  std::vector<int> grid_bright(1000 * 1000, 0);
  for (int i = 0; i < n_instructions; i++) {
    c = file.get();
    while (c != 't')
      c = file.get();

    c = file.get();

    if (c == 'o')
      state = 2;
    else {
      while (c != 'o')
        c = file.get();

      c = file.get();
      if (c == 'n')
        state = 0;
      else
        state = 1;
    }

    utils::read_next_integer(file, xa);
    utils::read_next_integer(file, ya);
    utils::read_next_integer(file, xb);
    utils::read_next_integer(file, yb);

    for (int y = ya; y <= yb; y++)
    for (int x = xa; x <= xb; x++) {
      if (state == 0)
        grid_bright[x + y * 1000]++;
      else if (state == 1)
        grid_bright[x + y * 1000] = std::max(0, grid_bright[x + y * 1000] - 1);
      else
        grid_bright[x + y * 1000] += 2;
    }
  }

  long total_brightness = 0L;
  for (int i = 0; i < 1000 * 1000; i++)
    total_brightness += grid_bright[i];

  std::cout << "Part 2\n  Total brightness: " << total_brightness << std::endl;

  return 0;
}