#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "utils.hpp"

/// @brief Wire types:
///
/// 0 : direct assign from LEFT
///
/// 1 : NOT of LEFT
///
/// 2 : AND
///
/// 3 : OR
///
/// 4 : LSHIFT
///
/// 5 : RSHIFT
struct wire {
  wire* LEFT;
  wire* RIGHT;
  bool ready;
  int type;
  uint16_t output;
};

void evaluate_wire(wire* w) {
  if (w->ready || (w->LEFT == nullptr && w->RIGHT == nullptr))
    return;
  
  uint16_t l, r;
  if (w->LEFT != nullptr) {
    evaluate_wire(w->LEFT);
    l = w->LEFT->output;
  }
  if (w->RIGHT != nullptr) {
    evaluate_wire(w->RIGHT);
    r = w->RIGHT->output;
  }

  switch (w->type) {
    case 0:
      w->output = l;
      break;
    case 1:
      w->output = ~l;
      break;
    case 2:
      w->output = l & r;
      break;
    case 3:
      w->output = l | r;
      break;
    case 4:
      w->output = l << r;
      break;
    case 5:
      w->output = l >> r;
      break;
    default:
      std::cerr << "invalid type : " << w->type << std::endl;
      w->output = 0;
      break;
  }

  w->ready = true;
}

wire* init_or_get(std::unordered_map<std::string, wire>& wires, std::string name) {
  if (wires.find(name) == wires.end()) {
    wire w;
    if (name[0] >= '0' && name[0] <= '9')
      w = { nullptr, nullptr, true, -1, (uint16_t)std::stoul(name)};
    else
      w = { nullptr, nullptr, false, -1, (uint16_t)0u };
    wires[name] = w;
  }

  return &wires[name];
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
  std::unordered_map<std::string, wire> wires;
  std::string a, b, c, d, e;
  for (std::string line; std::getline(file, line); ) {
    std::istringstream iss(line);
    iss >> a >> b >> c >> d >> e;
    // std::cout << a << " " << b << " " << c << " " << d << " " << e << std::endl;

    wire *left, *right, *output;
    if (b == "->") {
      left = init_or_get(wires, a);
      output = init_or_get(wires, c);
      output->LEFT = left;
      output->type = 0;
    } else if (c == "->") {
      left = init_or_get(wires, b);
      output = init_or_get(wires, d);
      output->LEFT = left;
      output->type = 1;
    } else {
      left = init_or_get(wires, a);
      right = init_or_get(wires, c);
      output = init_or_get(wires, e);
      output->LEFT = left;
      output->RIGHT = right;
      output->type = 2; // AND
      switch (b[0]) {
        case 'R': // RSHIFT
          output->type++;
        case 'L': // LSHIFT
          output->type++;
        case 'O': // OR
          output->type++;
      }
    }
  }

  int wire_value = 0;
  std::string wire_name = is_input ? "a" : "f";
  evaluate_wire(&wires[wire_name]);
  wire_value = wires[wire_name].output;

  std::cout << "Part 1\n  Signal provided to " << wire_name << " : " << wire_value << std::endl;

  /* Part 2 */
  std::string wire_target = is_input ? "b" : "x";
  for (auto kv : wires)
    wires[kv.first].ready = false;
  wires[wire_target].output = wires[wire_name].output;
  wires[wire_target].ready = true;
  evaluate_wire(&wires[wire_name]);
  wire_value = wires[wire_name].output;

  std::cout << "Part 2\n  Overridden signal provided to " << wire_name << " : " << wire_value << std::endl;

  return 0;
}