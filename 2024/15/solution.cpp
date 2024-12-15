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

void move_wide_y(std::vector<char>& warehouse, int nx, int x, int y, int dy) {
  char self = warehouse[x + y * nx];
  if (self == '.')
    return;
  
  char targetA = warehouse[x + (y + dy) * nx];
  int ddx = 0;
  if (targetA == '[')
    ddx = 1;
  else if (targetA == ']')
    ddx = -1;
  char targetB = warehouse[(x + ddx) + (y + dy) * nx];

  if (targetA != '.') {
    move_wide_y(warehouse, nx, x,     y+dy, dy);
    move_wide_y(warehouse, nx, x+ddx, y+dy, dy);
  }

  warehouse[x + y * nx] = '.';
  warehouse[x + (y + dy) * nx] = self;
}

bool attempt_to_move_wide_y(std::vector<char>& warehouse, int nx, int x, int y, int dy) {
  char self = warehouse[x + y * nx];
  if (self == '#')
    return false;
  
  char targetA = warehouse[ x        + (y + dy) * nx];
  int ddx = 0;
  if (targetA == '[')
    ddx = 1;
  else if (targetA == ']')
    ddx = -1;
  char targetB = warehouse[(x + ddx) + (y + dy) * nx];

  if (targetA == '#' || targetB == '#')
    return false;

  bool can_move_A = targetA == '.' || attempt_to_move_wide_y(warehouse, nx, x,     y+dy, dy);
  bool can_move_B = targetB == '.' || attempt_to_move_wide_y(warehouse, nx, x+ddx, y+dy, dy);
  return can_move_A && can_move_B;
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
  for (int y = 0; y < ny; y++)
  for (int x = 0; x < nx; x++)
  if (warehouse[x + y * nx] == 'O')
    GPS += x + y * 100;

  std::cout << "Part 1\n  Sum of GPS coordinates : " << GPS << std::endl;

  file.clear();
  file.seekg(0);
  std::vector<char> wide_warehouse(nx*ny*2, '.');
  
  for (int y = 0; y < ny; y++)
  for (int x = 0; x < nx; x++) {
    file >> c;
    switch (c) {
      case '#':
        wide_warehouse[x * 2     + y * nx * 2] = '#';
        wide_warehouse[x * 2 + 1 + y * nx * 2] = '#';
        break;
      case 'O':
        wide_warehouse[x * 2     + y * nx * 2] = '[';
        wide_warehouse[x * 2 + 1 + y * nx * 2] = ']';
        break;
      case '@':
        wide_warehouse[x * 2     + y * nx * 2] = '@';
        rx = x * 2;
        ry = y;
        break;
    }
  }

  c = '.';

  while (c != -1) {
    c = get_next_char(file);
    if (c == -1)
      break;
    
    unpack_movement(c, dx, dy);
    if (dy == 0) {
      if (attempt_to_move(wide_warehouse, nx * 2, rx, ry, dx, dy))
        rx += dx;
    }
    else {
      if (attempt_to_move_wide_y(wide_warehouse, nx * 2, rx, ry, dy)) {
        move_wide_y(wide_warehouse, nx * 2, rx, ry, dy);
        ry += dy;
      }
    }
  }
  
  GPS = 0L;
  for (int y = 0; y < ny; y++)
  for (int x = 0; x < nx * 2; x++)
  if (wide_warehouse[x + y * nx * 2] == '[')
    GPS += x + y * 100;

  std::cout << "Part 2\n  Sum of wide GPS coordinates : " << GPS << std::endl;

  return 0;
}