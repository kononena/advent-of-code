#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "utils.hpp"

struct state128 {
  uint32_t a;
  uint32_t b;
  uint32_t c;
  uint32_t d;
};

uint32_t md5_f(state128 state) {
  return (state.b & state.c) | ((~state.b) & state.d);
}

uint32_t md5_g(state128 state) {
  return (state.b & state.d) | (state.c & (~state.d));
}

uint32_t md5_h(state128 state) {
  return state.b ^ state.c ^ state.d;
}

uint32_t md5_i(state128 state) {
  return state.c ^ (state.b | (~state.d));
}

uint32_t leftrotate(uint32_t F, uint32_t s) {
  return (F << s) | (F >> (32 - s));
}

void print_uint32_t_as_hex(uint32_t value) {
  for (int j = 7; j >= 0; j--) {
    char val = (value >> (j * 4)) & 0xf;
    if (val >= 10)
      std::cout << (char)(val - 10 + 'a');
    else
      std::cout << (char)(val + '0');
  }
}

uint32_t reverse_endianness(uint32_t value) {
  return (value >> 24)
       | ((value & 0x00ff0000) >> 8)
       | ((value & 0x0000ff00) << 8)
       | (value << 24);
}

void compute_md5_hash(std::vector<char> input, state128& state_out) {
  // Setup helpers s, K
  uint32_t s[64], K[64];

  for (int i = 0; i < 16; i++)
    s[i] = 7 + 5 * (i % 4); // 7, 12, 17, 22
  for (int i = 0; i < 16; i++) {
    s[i+16] = 0;
    switch (i % 4) {
      case 3:
        s[i+16] += 6; // 20
      case 2:
        s[i+16] += 5; // 14
      case 1:
        s[i+16] += 4; // 9
      case 0:
        s[i+16] += 5; // 5
    }
  }
  for (int i = 0; i < 16; i++) {
    s[i+32] = 0;
    switch (i % 4) {
      case 3:
        s[i+32] += 7; // 23
      case 2:
        s[i+32] += 5; // 16
      case 1:
        s[i+32] += 7; // 11
      case 0:
        s[i+32] += 4; // 4
    }
  }
  for (int i = 0; i < 16; i++) {
    s[i+48] = 0;
    switch (i % 4) {
      case 3:
        s[i+48] += 6; // 21
      case 2:
        s[i+48] += 5; // 15
      case 1:
        s[i+48] += 4; // 10
      case 0:
        s[i+48] += 6; // 6
    }
  }
  
  for (int i = 0; i < 64; i++)
    K[i] = std::abs(std::sin(i + 1)) * (double)0x100000000;

  // Initial state
  state128 state0 = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };

  // Append 1's, 0's and size to input
  uint64_t original_length = input.size() * 8;
  input.push_back(0x80);
  long size = input.size();
  while ((size % 64) != 56) {
    input.push_back(0);
    size++;
  }
  
  // for (int i = 0; i < 8; i++)
  //   input.push_back(original_length >> (56 - 8 * i));
  for (int i = 4; i < 8; i++)
    input.push_back(original_length >> (56 - 8 * i));
  for (int i = 0; i < 4; i++)
    input.push_back(original_length >> (56 - 8 * i));
  size += 8;

  for (int m = 0; m < size; m += 64) {
    uint32_t M[16];
    for (int j = 0; j < 16; j++)
      M[j] = (input[    j * 4 + m] << 24)
           | (input[1 + j * 4 + m] << 16)
           | (input[2 + j * 4 + m] <<  8)
           | (input[3 + j * 4 + m] <<  0);
    for (int j = 0; j < 16; j++)
      M[j] = reverse_endianness(M[j]);
    
    state128 state = state0;
    for (uint32_t i = 0; i < 64; i++) {
      uint32_t F, g;
      if (i < 16) {
        F = md5_f(state);
        g = i;
      } else if (i < 32) {
        F = md5_g(state);
        g = (5 * i + 1) % 16;
      } else if (i < 48) {
        F = md5_h(state);
        g = (3 * i + 5) % 16;
      } else {
        F = md5_i(state);
        g = (7 * i) % 16;
      }

      F += state.a + K[i] + M[g];
      state.a = state.d;
      state.d = state.c;
      state.c = state.b;
      state.b = state.b + leftrotate(F, s[i]);
    }

    state0.a += state.a;
    state0.b += state.b;
    state0.c += state.c;
    state0.d += state.d;
  }

  state_out = state0;
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
  
  /* Part 1 */
  std::vector<char> input_key;
  char c = file.get();
  while (c != -1) {
    input_key.push_back(c);
    c = file.get();
  }

  state128 state;
  compute_md5_hash(input_key, state);
  state.a = reverse_endianness(state.a);
  state.b = reverse_endianness(state.b);
  state.c = reverse_endianness(state.c);
  state.d = reverse_endianness(state.d);
  print_uint32_t_as_hex(state.a);
  print_uint32_t_as_hex(state.b);
  print_uint32_t_as_hex(state.c);
  print_uint32_t_as_hex(state.d);
  std::cout << std::endl;

  std::cout << "Part 1\n  Solution : " << std::endl;

  /* Part 2 */

  std::cout << "Part 2\n  Solution : " << std::endl;

  return 0;
}