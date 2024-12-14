#include <iostream>
#include <fstream>
#include <vector>

template <typename T>
void read_integer(std::string line, int& start, T& output) {
  char c = line[start];
  while (c < '0' || c > '9')
    c = line[++start];
  
  bool is_negative = line[start - 1] == '-';

  while (c >= '0' && c <= '9') {
    output = output * (T)10 + (c - '0');
    c = line[++start];
  }

  if (is_negative)
    output *= -1;
}

template <typename T>
void read_next_pair(std::string line, int& start, std::pair<T, T>& pair) {
  read_integer(line, start, pair.first);
  read_integer(line, start, pair.second);
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
  
  int nx = 11;
  int ny = 7;
  if (filename == "input.txt") {
    nx = 101;
    ny = 103;
  }

  long quadrants[4] = { 0L, 0L, 0L, 0L };
  long safety_factor = 1L;
  for (std::string line; std::getline(file, line); ) {
    std::pair<int, int> p;
    std::pair<int, int> v;
    int start = 0;
    read_next_pair(line, start, p);
    read_next_pair(line, start, v);

    int fx = (p.first + v.first * 100 + nx * 100) % nx;
    int fy = (p.second + v.second * 100 + ny * 100) % ny;
    if (fx == nx / 2 || fy == ny / 2)
      continue;
    
    int q = (fx > nx / 2 ? 1 : 0) + (fy > ny / 2 ? 2 : 0);
    quadrants[q]++;
  }

  for (int i = 0; i < 4; i++)
    safety_factor *= quadrants[i];
  
  std::cout << "Part 1\n  Safety factor after 100s : " << safety_factor << std::endl;

  long max_duration = 8000;
  std::vector<char> images(nx*ny*(max_duration+1), ' ');
  file.clear();
  file.seekg(0);
  for (std::string line; std::getline(file, line); ) {
    std::pair<long, long> p;
    std::pair<long, long> v;
    int start = 0;
    read_next_pair(line, start, p);
    read_next_pair(line, start, v);

    for (int n = 0; n <= max_duration; n++) {
      long fx = (p.first + v.first * n + nx * n) % nx;
      long fy = (p.second + v.second * n + ny * n) % ny;
      images[fx + fy * nx + n * nx * ny] = '*';
    }
  }

  std::vector<bool> candidate(max_duration+1, false);
  for (int n = 0; n <= max_duration; n++)
  for (int y = 0; y < ny; y++) {
    int consecutive = 0;
    for (int x = 0; x < nx; x++) {
      if (images[x + y * nx + n * nx * ny] == '*')
        consecutive++;
      else
        consecutive = 0;
      
      if (consecutive >= 20) {
        candidate[n] = true;
        break;
      }
    }
  }

  for (int n = 0; n <= max_duration; n++) {
    if (!candidate[n])
      continue;
    
    std::cout << "Part 2 : Image #" << n << "\n";
    for (int y = 0; y < ny; y++) {
      for (int x = 0; x < nx; x++)
        std::cout << images[x + y * nx + n * nx * ny];
      std::cout << "\n";
    }
    std::cout << "-----------------------------------------------------------------------------------------------------" << std::endl;
  }

  return 0;
}