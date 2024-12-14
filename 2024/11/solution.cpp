#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <chrono>

template <typename T>
void insert_or_add(std::unordered_map<T, T>& map, T key, T value) {
  if (map.find(key) == map.end())
    map[key] = value;
  else
    map[key] += value;
}

void update_map(std::unordered_map<long, long>& map, long stone, long qty) {
  long digits = stone == 0L ? 1 : std::log10(stone) + 1;

  if (stone == 0L)
    insert_or_add(map, 1L, qty);
  else if (digits & 1L) // Odd
    insert_or_add(map, stone * 2024L, qty);
  else {
    digits /= 2L;
    long separator = 1;
    for (int i = 0; i < digits; i++)
      separator *= 10;
    
    insert_or_add(map, stone % separator, qty);
    insert_or_add(map, stone / separator, qty);
  }
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

  int n_initial = 2;
  if (filename == "input.txt")
    n_initial = 8;
  
  std::unordered_map<long, long> stones_cur;
  std::unordered_map<long, long> stones_new;

  for (int i = 0; i < n_initial; i++) {
    long val;
    file >> val;
    insert_or_add(stones_cur, val, 1L);
  }

  auto t1 = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 25; i++) {
    for (auto kv : stones_cur)
      update_map(stones_new, kv.first, kv.second);
    
    stones_cur = stones_new;
    stones_new.clear();
  }

  auto t2 = std::chrono::high_resolution_clock::now();
  double timing_duration = (t2 - t1).count() / 1e6;

  long stone_count = 0L;
  for (auto kv : stones_cur)
    stone_count += kv.second;

  std::cout << "Part 1\n  Stones after 25 blinks : " << stone_count << "\n";
  std::cout << "  Finished in " << timing_duration << " ms" << std::endl;
  


  t1 = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 50; i++) {
    for (auto kv : stones_cur)
      update_map(stones_new, kv.first, kv.second);
    
    stones_cur = stones_new;
    stones_new.clear();
  }

  t2 = std::chrono::high_resolution_clock::now();
  timing_duration = (t2 - t1).count() / 1e6;

  stone_count = 0L;
  for (auto kv : stones_cur)
    stone_count += kv.second;

  std::cout << "Part 2\n  Stones after 75 blinks : " << stone_count << "\n";
  std::cout << "  Finished in " << timing_duration << " ms" << std::endl;

  return 0;
}