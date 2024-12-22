#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "utils.hpp"

void update_secret(uint32_t &secret) {
  secret = ((secret <<  6) ^ secret) % 16777216u;
  secret = ((secret >>  5) ^ secret) % 16777216u;
  secret = ((secret << 11) ^ secret) % 16777216u;
}

void add_or_init(std::unordered_map<int, long> &prices, std::unordered_set<int> &sold, int key, long value) {
  if (sold.find(key) != sold.end())
    return;
  
  sold.insert(key);
  if (prices.find(key) == prices.end())
    prices[key] = value;
  else
    prices[key] += value;
}

int price_change_index(int price_changes[4]) {
  // Possible values are -9,...,9, i.e. 19 values
  int index = 0;
  for (int i = 0; i < 4; i++)
    index = index * 19 + (price_changes[i] + 9);
  return index;
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

  int N = is_input ? 1729 : 4;
  std::vector<uint32_t> secrets(N, 0);
  for (int i = 0; i < N; i++)
    file >> secrets[i];
  
  /* Part 1 */
  timer.tic();
  for (int iteration = 0; iteration < 2000; iteration++)
  for (int i = 0; i < N; i++)
    update_secret(secrets[i]);
  timer.toc();
  
  long secret_sum = 0L;
  for (int i = 0; i < N; i++)
    secret_sum += secrets[i];

  std::cout << "Part 1\n  Sum of 2000th secrets : " << secret_sum << std::endl;
  timer.print();

  /* Part 2 */
  file.clear();
  file.seekg(0);
  for (int i = 0; i < N; i++)
    file >> secrets[i];
  
  std::unordered_map<int, long> prices;
  std::unordered_set<int> sold;
  int changes[4], index;
  // For each monkey
  timer.tic();
  for (int m = 0; m < N; m++) {
    // Sold stores which price change sequences have been processed
    sold.clear();

    uint32_t secret1 = secrets[m];
    uint32_t secret0 = secret1;
    update_secret(secret1);
    for (int i = 1; i < 4; i++) {
      changes[i] = (int)(secret1 % 10) - (int)(secret0 % 10);
      secret0 = secret1;
      update_secret(secret1);
    }

    // For the remainder of the 2000 price changes
    for (int iter = 4; iter < 2000; iter++) {
      // Update backlog of price changes
      for (int i = 0; i < 3; i++)
        changes[i] = changes[i+1];
      // Add new price change
      changes[3] = (int)(secret1 % 10) - (int)(secret0 % 10) ;
      index = price_change_index(changes);

      add_or_init(prices, sold, index, (secret1 % 10));

      secret0 = secret1;
      update_secret(secret1);
    }
  }
  timer.toc();

  long max_price = 0L;
  for (auto kv : prices)
    max_price = std::max(max_price, kv.second);
  
  std::cout << "Part 2\n  Maximum number of bananas : " << max_price << std::endl;
  timer.print();

  return 0;
}