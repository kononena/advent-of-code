#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <chrono>

int main() {
  std::string filename = "input.txt";

  std::ifstream file;
  file.open(filename, std::ios::in);
  if (!file.is_open()) {
    std::cerr << "Error opening file\n";
    return 1;
  }
  std::cout << "Using file " << filename << "\n";
  
  // Example input is 10x10 plus padding
  int nx = 12;
  int ny = 12;
  if (filename == "input.txt") {
    // Actual input is 140x140, pad from each end for simplicity
    nx = 142;
    ny = 142;
  }

  std::vector<char> garden(nx*ny, '-');
  std::vector<bool> visited(nx*ny, false);

  // Read input
  for (int x = 1; x < nx-1; x++)
  for (int y = 1; y < ny-1; y++)
    file >> garden[x + y * nx];
  
  // Mark padded edges as visited
  for (int x = 0; x < nx; x++) {
    visited[x            ] = true;
    visited[x + (ny-1)*nx] = true;
  }
  for (int y = 0; y < ny; y++) {
    visited[       y * nx] = true;
    visited[nx-1 + y * nx] = true;
  }

  // Check each cell if not yet visited, process the attached region
  std::queue<int> q;
  long total_price = 0;

  auto t1 = std::chrono::high_resolution_clock::now();

  for (int x = 0; x < nx; x++)
  for (int y = 0; y < ny; y++) {
    int s = x + y * nx;
    if (visited[s])
      continue;
    
    q.push(s);
    visited[s] = true;
    char plant = garden[s];

    int area = 0;
    int perimeter = 0;

    while (!q.empty()) {
      int k = q.front();
      q.pop();
      area++;

      int neighbors[4] = { k - 1, k + 1, k - nx, k + nx };
      for (int i : neighbors) {
        if (garden[i] == plant) {
          if (!visited[i]) {
            visited[i] = true;
            q.push(i);
          }
        }
        else
          perimeter++;
        
      }
    }

    total_price += area * perimeter;
  }

  auto t2 = std::chrono::high_resolution_clock::now();
  double timing_duration = (t2 - t1).count();

  std::cout << "Part 1\n  Total price : " << total_price << "\n";
  std::cout << "  Elapsed time : " << timing_duration / 1e6 << " ms" << std::endl;
  

  // Reset traversal
  for (int x = 1; x < nx-1; x++)
  for (int y = 1; y < ny-1; y++)
    visited[x + y * nx] = false;
  long discounted_price = 0;

  t1 = std::chrono::high_resolution_clock::now();

  for (int x = 0; x < nx; x++)
  for (int y = 0; y < ny; y++) {
    int s = x + y * nx;
    if (visited[s])
      continue;
    
    q.push(s);
    visited[s] = true;
    char plant = garden[s];

    int area = 0;

    std::vector<bool> region(nx*ny, false);
    int xmin = nx, xmax = 0;
    int ymin = ny, ymax = 0;

    // First, traverse garden to obtain region and area
    while (!q.empty()) {
      int k = q.front();
      q.pop();
      area++;

      // Update bounding box of this region
      region[k] = true;
      int kx = k % nx;
      int ky = k / nx;
      xmin = std::min(xmin, kx);
      xmax = std::max(xmax, kx);
      ymin = std::min(ymin, ky);
      ymax = std::max(ymax, ky);

      int neighbors[4] = { k - 1, k + 1, k - nx, k + nx };
      for (int i : neighbors)
      if (garden[i] == plant && !visited[i]) {
        visited[i] = true;
        q.push(i);
      }
    }
    xmin--; xmax++;
    ymin--; ymax++;

    int perimeter = 0;
    // Then, process region to obtain perimeter
    // First slide in y direction with a window of height 2
    for (int sx = xmin; sx < xmax; sx++) {
      bool building_edge = false, top = false, bot = false;

      for (int sy = ymin; sy <= ymax; sy++) {
        bool new_top = region[sx   + sy * nx];
        bool new_bot = region[sx+1 + sy * nx];

        if (building_edge)
        if (top != new_top || bot != new_bot) {
          perimeter++;
          building_edge = false;
        }

        if (new_top != new_bot)
          building_edge = true;

        top = new_top; bot = new_bot;
      }
    }

    // Then slide in x direction equivalently
    for (int sy = ymin; sy < ymax; sy++) {
      bool building_edge = false, top = false, bot = false;

      for (int sx = xmin; sx <= xmax; sx++) {
        bool new_top = region[sx +  sy      * nx];
        bool new_bot = region[sx + (sy + 1) * nx];

        if (building_edge)
        if (top != new_top || bot != new_bot) {
          perimeter++;
          building_edge = false;
        }

        if (new_top != new_bot)
          building_edge = true;

        top = new_top; bot = new_bot;
      }
    }

    // std::cout << "Region '" << plant << "' has area " << area << " and perimeter " << perimeter << "\n";
    discounted_price += area * perimeter;
  }

  t2 = std::chrono::high_resolution_clock::now();
  timing_duration = (t2 - t1).count();

  std::cout << "Part 2\n  Discounted price : " << discounted_price << "\n";
  std::cout << "  Elapsed time : " << timing_duration / 1e6 << " ms" << std::endl;

  return 0;
}