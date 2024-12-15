#include <iostream>
#include <fstream>
#include <vector>

char get_next_char(std::ifstream& file) {
  char c = file.get();
  while (c == '\n')
    c = file.get();
  return c;
}

void unpack_movement(char movement, int& dx, int& dy) {
  dx = 0;
  dy = 0;
  switch (movement) {
    case '^':
      dy = -1;
      break;
    case '<':
      dx = -1;
      break;
    case '>':
      dx = 1;
      break;
    case 'v':
      dy = 1;
      break;
    default:
      std::cerr << "Invalid movement character : " << movement << std::endl;
      break;
  }
}

bool attempt_to_move(std::vector<char>& warehouse, int nx, int x, int y, int dx, int dy) {
  char self = warehouse[x + y * nx];
  char target = warehouse[(x + dx) + (y + dy) * nx];
  if (target == '#')
    return false;
  else if (target == '.' || attempt_to_move(warehouse, nx, x+dx, y+dy, dx, dy)) {
    warehouse[x + y * nx] = '.';
    warehouse[(x + dx) + (y + dy) * nx] = self;
    return true;
  }

  return false;
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

  int nx = 10;
  int ny = 10;
  if (is_input) {
    nx = 50;
    ny = 50;
  }
  
  std::vector<char> warehouse(nx*ny, '.');
  int rx = 0;
  int ry = 0;
  for (int y = 0; y < ny; y++)
  for (int x = 0; x < nx; x++) {
    file >> warehouse[x + y * nx];
    if (warehouse[x + y * nx] == '@') {
      rx = x;
      ry = y;
    }
  }
  
  char c = '.';

  int dx, dy;
  while (c != -1) {
    c = get_next_char(file);
    if (c == -1)
      break;
    unpack_movement(c, dx, dy);
    if (attempt_to_move(warehouse, nx, rx, ry, dx, dy)) {
      rx += dx;
      ry += dy;
    }
  }

  long GPS = 0L;
  for (int y = 0; y < ny; y++) {
    for (int x = 0; x < nx; x++)
    if (warehouse[x + y * nx] == 'O')
      GPS += x + y * 100;
  }

  std::cout << "Part 1\n  Sum of GPS coordinates : " << GPS << std::endl;

  /* Part 2 */

  std::cout << "Part 2\n  Solution : " << std::endl;

  return 0;
}