#include <iostream>
#include <fstream>
#include <vector>

void rotate(int& dx, int& dy) {
  // maps: (0, -1) -> (1, 0) -> (0, 1) -> (-1, 0) ->
  int temp = -dy;
  dy = dx;
  dx = temp;
}

void set_map(std::vector<char>& file_map, std::vector<char>& map) {
  for (int i = 0; i < file_map.size(); i++)
    map[i] = file_map[i];
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
    nx = 132;
    ny = 132;
  }

  std::vector<char> file_map(nx*ny, '-');
  std::vector<char> map(nx*ny, '-');

  int gx, gy;
  for (int yi = 1; yi < ny - 1; yi++)
  for (int xi = 1; xi < nx - 1; xi++) {
    file >> file_map[xi + yi * nx];
    if (file_map[xi + yi * nx] == '^') {
      gx = xi;
      gy = yi;
    }
  }

  set_map(file_map, map);
  int x = gx, y = gy;
  
  int dx = 0, dy = -1;
  long distinct_positions = 1L;
  map[x + y * nx] = 'X';
  char c = map[x + y * nx];
  while (c != '-') {
    c = map[(x + dx) + (y + dy) * nx];

    if (c == '#') {
      rotate(dx, dy);
      continue;
    }
    
    x += dx;
    y += dy;
    
    if (c == '.') {
      distinct_positions++;
      map[x + y * nx] = 'X';
    }
  }

  std::cout << "Part 1\n  Distinct positions : " << distinct_positions << std::endl;
  
  distinct_positions = 0L;
  for (int xi = 1; xi < nx - 1; xi++)
  for (int yi = 1; yi < ny - 1; yi++) {
    c = file_map[xi + yi * nx];
    if (c == '#' || c == '^')
      continue;
    
    set_map(file_map, map);
    map[xi + yi * nx] = '#';
    x = gx; y = gy;
    dx = 0; dy = -1;

    std::vector<bool> encountered_states(nx*ny*4, false);
    int state_index = 0;

    while (c != '-') {
      c = map[(x + dx) + (y + dy) * nx];

      if (c == '#') {
        rotate(dx, dy);
        continue;
      }
      
      x += dx;
      y += dy;
      
      state_index = (x + y * nx) * 4;
      if (dx == 1)
        state_index += 1;
      else if (dx == -1)
        state_index += 3;
      else if (dy == 1)
        state_index += 2;
      
      if (encountered_states[state_index]) {
        distinct_positions++;
        break;
      }

      encountered_states[state_index] = true;
    }
  }

  std::cout << "Part 2\n  Distinct positions for obstacle : " << distinct_positions << std::endl;

  return 0;
}