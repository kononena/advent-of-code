#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#include "utils.hpp"

struct path_state {
  int x, y;
  std::string path;
};

void numpad_pos(char c, int &x, int &y) {
  if (c == 'A') {
    x = 2; y = 3;
    return;
  } else if (c == '0') {
    x = 1; y = 3;
    return;
  }

  int n = c - '1';
  x = (n % 3);
  y = 2 - (n / 3);
}

void dirpad_pos(char c, int &x, int &y) {
  y = 1;
  if (c == '^' || c == 'A')
    y = 0;
  
  x = 2;
  if (c == '<')
    x = 0;
  else if (c == '^' || c == 'v')
    x = 1;
}

void add_paths(const std::string pad, int ex, int ey, std::queue<path_state> &q, std::queue<std::string> &paths, std::string prepend) {
  while (!q.empty()) {
    path_state p = q.front();
    q.pop();

    if (pad[p.x + p.y * 3] == '#')
      continue;

    if (p.x > ex)
      q.push({ p.x-1, p.y, p.path + '<' });
    else if (p.x < ex)
      q.push({ p.x+1, p.y, p.path + '>' });
    
    if (p.y > ey)
      q.push({ p.x, p.y-1, p.path + '^' });
    else if (p.y < ey)
      q.push({ p.x, p.y+1, p.path + 'v' });
    
    if (p.x == ex && p.y == ey)
      paths.push(prepend + p.path);
  }
}

void numpad_paths(char start, char end, std::queue<std::string> &paths, std::string prepend) {
  int sx, sy, ex, ey;
  numpad_pos(start, sx, sy);
  numpad_pos(end, ex, ey);

  const std::string numpad = "789456123#0A";
  std::queue<path_state> q;
  q.push({ sx, sy, "" });

  add_paths(numpad, ex, ey, q, paths, prepend);
}

void dirpad_paths(char start, char end, std::queue<std::string> &paths, std::string prepend) {
  int sx, sy, ex, ey;
  dirpad_pos(start, sx, sy);
  dirpad_pos(end, ex, ey);

  const std::string dirpad = "#^A<v>";
  std::queue<path_state> q;
  q.push({ sx, sy, "" });

  add_paths(dirpad, ex, ey, q, paths, prepend);
}

void process_code(std::string code, std::queue<std::string> &output, bool is_numpad) {
  std::queue<std::string> paths;
  std::queue<std::string> paths_new;
  paths.push("");

  char ca = 'A';

  for (int i = 0; i < code.length(); i++) {
    char c = code[i];
    while (!paths.empty()) {
      std::string path = paths.front();
      paths.pop();
      if (is_numpad)
        numpad_paths(ca, c, paths_new, path);
      else
        dirpad_paths(ca, c, paths_new, path);
    }
    ca = c;

    while (!paths_new.empty()) {
      paths.push(paths_new.front() + 'A');
      paths_new.pop();
    }
  }

  while (!paths.empty()) {
    output.push(paths.front());
    paths.pop();
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
  
  std::cout << "Using file " << filename << std::endl;
  std::ifstream file;
  file.open(filename, std::ios::in);
  if (!file.is_open()) {
    std::cerr << "Error opening file" << std::endl;
    return 1;
  }

  bool is_input = filename == "input.txt";
  utils::Timer timer;

  int code_length = 4;
  
  /* Part 1 */
  long complexity = 0L;
  for (std::string line; std::getline(file, line); ) {
    std::queue<std::string> paths;
    std::queue<std::string> paths_new;

    process_code(line, paths, true);
    // Now, paths contains all possible shortest paths on numpad
    while (!paths.empty()) {
      process_code(paths.front(), paths_new, false);
      paths.pop();
    }

    // Now, paths_new contains all possible shortest paths on dirpad
    while (!paths_new.empty()) {
      process_code(paths_new.front(), paths, false);
      paths_new.pop();
    }

    // Now, paths contains all inputs!
    long min_length = INT64_MAX;
    while (!paths.empty()) {
      min_length = std::min(min_length, (long)paths.front().length());
      paths.pop();
    }

    long code = 0L;
    int i = 0;
    char c = line[i];
    while (c < '0' || c > '9')
      c = line[++i];
    
    while (c >= '0' && c <= '9') {
      code = code * 10L + (c - '0');
      c = line[++i];
    }

    complexity += code * min_length;
  }

  std::cout << "Part 1\n  Complexity : " << complexity << std::endl;

  /* Part 2 */

  std::cout << "Part 2\n  Solution : " << std::endl;

  return 0;
}