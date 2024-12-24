#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "utils.hpp"

struct wire {
  wire* left;
  wire* right;
  bool ready;
  int type;
  bool output;
};

void evaluate_wire(wire& w) {
  if (w.ready || (w.left == nullptr && w.right == nullptr))
    return;

  evaluate_wire(*w.left);
  evaluate_wire(*w.right);
  bool l = w.left->output;
  bool r = w.right->output;

  switch (w.type) {
    case 0:
      w.output = l & r;
      break;
    case 1:
      w.output = l | r;
      break;
    case 2:
      w.output = l ^ r;
      break;
    default:
      std::cerr << "invalid wire type " << w.type << std::endl;
      w.output = false;
      break;
  }

  w.ready = true;
}

int wire_name_to_index(const std::string& name) {
  return (name[0] - '0') * 10000
       + (name[1] - '0') * 100
       + (name[2] - '0');
}

std::string wire_index_to_name(int index) {
  std::string name = "...";
  name[0] = (index / 10000) + '0';
  name[1] = ((index / 100) % 100) + '0';
  name[2] = (index % 100) + '0';
  return name;
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
  std::unordered_map<int, wire> wires;
  for (std::string line; std::getline(file, line); ) {
    if (line.empty())
      break;
    
    // Lines are of the form:
    // x00: 1
    // 012..5
    int wire_index = wire_name_to_index(line);
    wires[wire_index] = { nullptr, nullptr, true, 0, line[5] == '1'};
  }

  std::string a, op, b, c;
  for (std::string line; std::getline(file, line); ) {
    std::istringstream iss(line);
    iss >> a >> op >> b >> c >> c;

    int ai = wire_name_to_index(a);
    int bi = wire_name_to_index(b);
    int ci = wire_name_to_index(c);

    if (wires.find(ai) == wires.end())
      wires[ai] = { nullptr, nullptr, false, 0, false };
    if (wires.find(bi) == wires.end())
      wires[bi] = { nullptr, nullptr, false, 0, false };
    if (wires.find(ci) == wires.end())
      wires[ci] = { nullptr, nullptr, false, 0, false };
    
    wires[ci].left  = &wires[ai];
    wires[ci].right = &wires[bi];
    int type = 0;
    if (op == "OR")
      type = 1;
    else if (op == "XOR")
      type = 2;
    wires[ci].type = type;
  }

  long z_decimal = 0L;
  for (int i = 9; i >= 0; i--)
  for (int j = 9; j >= 0; j--) {
    std::string wire_name = "z" + std::to_string(i) + std::to_string(j);
    int wire_index = wire_name_to_index(wire_name);

    if (wires.find(wire_index) == wires.end())
      continue;
    
    evaluate_wire(wires[wire_index]);

    z_decimal <<= 1;
    if (wires[wire_index].output)
      z_decimal |= 1;
  }

  std::cout << "Part 1\n  Decimal number of z-wires : " << z_decimal << std::endl;

  /* Part 2 */

  std::cout << "Part 2\n  Solution : " << std::endl;

  return 0;
}