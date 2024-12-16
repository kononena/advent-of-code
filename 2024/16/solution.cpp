#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>

int pack_dir(int dx, int dy) {
  if (dx == 1)
    return 1;
  else if (dy == 1)
    return 2;
  else if (dx == -1)
    return 3;
  else
    return 0;
}

void unpack_dir(int dir, int& dx, int& dy) {
  dir &= 3;
  dx = 0;
  dy = 0;
  if (dir == 1)
    dx = 1;
  else if (dir == 2)
    dy = 1;
  else if (dir == 3)
    dx = -1;
  else
    dy = -1;
}

long pack_state(int nx, int x, int y, int dx, int dy) {
  long pos = x + y * nx;
  long dir = pack_dir(dx, dy);
  return (pos << 2) + dir;
}

void unpack_state(long state, int nx, int& x, int& y, int& dx, int& dy) {
  long pos = state >> 2;
  x = pos % nx;
  y = pos / nx;
  unpack_dir(state, dx, dy);
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
  
  int nx = 15;
  int ny = 15;
  if (is_input) {
    nx = 141;
    ny = 141;
  }

  int sx = 0, sy = 0; // start x, y
  int gx = 0, gy = 0; // goal x, y
  char c;
  std::vector<char> map(nx*ny, '.');
  for (int y = 0; y < ny; y++)
  for (int x = 0; x < nx; x++) {
    file >> c;
    if (c == 'S') {
      sx = x; sy = y;
    } else if (c == 'E') {
      gx = x; gy = y;
    } else {
      map[x + y * nx] = c;
    }
  }

  std::queue<long> states;
  std::unordered_map<long, long> state_scores;

  int x, y, dx, dy, temp;
  long state, score;
  long state_new;
  long goal_state = pack_state(nx, gx, gy, 0, 0);
  long max_score = INT64_MAX;
  
  state = pack_state(nx, sx, sy, 1, 0);
  states.push(state);
  state_scores[state] = 0L;

  while (!states.empty()) {
    state = states.front();
    states.pop();

    unpack_state(state, nx, x, y, dx, dy);
    score = state_scores[state];

    for (int i = 0; i < 3; i++) {
      // Score increases by 1 when traveling forward
      long increase = 1L;
      if (i == 1) {
        // Score increases by 1000 when rotating
        // Rotate by 90 degrees in some direction
        temp = dx; dx = dy; dy = temp;
        increase = 1000L;
      } else if (i == 2) {
        // Rotate by 180 degrees from previous rotated direction
        dx = -dx; dy = -dy;
        increase = 1000L;
      }

      if (map[(x + dx) + (y + dy) * nx] == '#')
        continue;

      if (score + increase > max_score)
        continue;
      
      // Only travel if not rotating
      if (i == 0)
        state_new = pack_state(nx, x+dx, y+dy, dx, dy);
      else
        state_new = pack_state(nx, x, y, dx, dy);
      
      if (state_scores.find(state_new) == state_scores.end() || state_scores[state_new] > score + increase) {
        states.push(state_new);
        state_scores[state_new] = score + increase;
      }
    }

    for (int i = 0; i < 4; i++)
    if (state_scores.find(goal_state + i) != state_scores.end())
      max_score = std::min(max_score, state_scores[goal_state + i]);
  }

  std::cout << "Part 1\n  Minimum path score : " << max_score << std::endl;

  std::vector<bool> on_path(nx*ny, false);
  for (int i = 0; i < 4; i++)
  if (state_scores.find(goal_state + i) != state_scores.end() && state_scores[goal_state + i] == max_score)
    states.push(goal_state + i);
  
  while (!states.empty()) {
    state = states.front();
    states.pop();
    
    unpack_state(state, nx, x, y, dx, dy);
    score = state_scores[state];
    on_path[x + y * nx] = true;

    for (int i = 0; i < 3; i++) {
      long increase = 1L;
      if (i == 1) {
        temp = dx; dx = dy; dy = temp;
        increase = 1000L;
      } else if (i == 2) {
        dx = -dx; dy = -dy;
        increase = 1000L;
      }

      // Travel backwards
      // If best score at that state == score - increase, then it is on the best path
      if (map[(x - dx) + (y - dy) * nx] == '#')
        continue;
      
      if (i == 0)
        state_new = pack_state(nx, x-dx, y-dy, dx, dy);
      else
        state_new = pack_state(nx, x, y, dx, dy);
      
      if (state_scores.find(state_new) != state_scores.end() && state_scores[state_new] == score - increase)
        states.push(state_new);
    }
  }

  long on_best_paths = 0L;
  for (int y = 0; y < ny; y++)
  for (int x = 0; x < nx; x++)
  if (on_path[x + y * nx])
    on_best_paths++;

  std::cout << "Part 2\n  Tiles on best paths : " << on_best_paths << std::endl;

  return 0;
}