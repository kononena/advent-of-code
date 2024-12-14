#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

int main() {
  int N = 1000;
  std::string filename = "input.txt";
  std::ifstream file;
  file.open(filename, std::ios::in);

  if (!file.is_open()) {
    std::cout << "Error opening file\n";
    return 1;
  }

  std::vector<int> left(N, 0);
  std::vector<int> right(N, 0);
  for (int i = 0; i < N; i++)
    file >> left[i] >> right[i];
  file.close();

  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());

  long total_distance = 0;
  for (int i = 0; i < N; i++)
    total_distance += abs(left[i] - right[i]);
  
  std::cout << "Part 1\n  Total distance : " << total_distance << std::endl;

  std::vector<int> counts(left[N-1] - left[0] + 1, 0);
  int offset = left[0];
  int num_elems = counts.size();

  for (int i = 0; i < N; i++) {
    int elem = right[i];
    int index = elem - offset;

    if (index < 0 || index >= num_elems)
      continue;
    
    counts[index] += elem;
  }

  long similarity_score = 0;
  for (int i = 0; i < N; i++)
    similarity_score += counts[left[i] - offset];
  
  std::cout << "Part 2\n  Similarity score : " << similarity_score << std::endl;

  return 0;
}