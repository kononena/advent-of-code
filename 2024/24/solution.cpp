#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "utils.hpp"

struct wire {
  std::string name;
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

std::string wire_name_suffix(int i, int j) {
  if (j < 0) {
    i--;
    j = 9;
  }
  return std::to_string(i) + std::to_string(j);
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
    wires[wire_index] = { wire_index_to_name(wire_index), nullptr, nullptr, true, 0, line[5] == '1' };
  }

  std::string a, op, b, c;
  for (std::string line; std::getline(file, line); ) {
    std::istringstream iss(line);
    iss >> a >> op >> b >> c >> c;

    int ai = wire_name_to_index(a);
    int bi = wire_name_to_index(b);
    int ci = wire_name_to_index(c);

    if (wires.find(ai) == wires.end())
      wires[ai] = { a, nullptr, nullptr, false, 0, false };
    if (wires.find(bi) == wires.end())
      wires[bi] = { b, nullptr, nullptr, false, 0, false };
    if (wires.find(ci) == wires.end())
      wires[ci] = { c, nullptr, nullptr, false, 0, false };
    
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
  // i = j + 1
  // zi = (xi XOR yi) XOR ci
  // ci = (xj AND yj) OR (cj AND (xj XOR yj))

  // Swaps executed:
  wire *wa, *wb, *temp;
  int itemp;
  // z07, swt
  wa = &wires[wire_name_to_index("z07")];
  wb = &wires[wire_name_to_index("swt")];
  temp = wa->left;  wa->left  = wb->left;  wb->left  = temp;
  temp = wa->right; wa->right = wb->right; wb->right = temp;
  itemp = wa->type; wa->type  = wb->type;  wb->type = itemp;
  // z13, pqc
  wa = &wires[wire_name_to_index("z13")];
  wb = &wires[wire_name_to_index("pqc")];
  temp = wa->left;  wa->left  = wb->left;  wb->left  = temp;
  temp = wa->right; wa->right = wb->right; wb->right = temp;
  itemp = wa->type; wa->type  = wb->type;  wb->type = itemp;
  // z31, bgs
  wa = &wires[wire_name_to_index("z31")];
  wb = &wires[wire_name_to_index("bgs")];
  temp = wa->left;  wa->left  = wb->left;  wb->left  = temp;
  temp = wa->right; wa->right = wb->right; wb->right = temp;
  itemp = wa->type; wa->type  = wb->type;  wb->type = itemp;
  // rjm, wsv
  wa = &wires[wire_name_to_index("rjm")];
  wb = &wires[wire_name_to_index("wsv")];
  temp = wa->left;  wa->left  = wb->left;  wb->left  = temp;
  temp = wa->right; wa->right = wb->right; wb->right = temp;
  itemp = wa->type; wa->type  = wb->type;  wb->type = itemp;

  for (int i = 0; i < 9; i++)
  for (int j = 0; j < 9; j++) {
    std::string suffix_cur = wire_name_suffix(i, j);
    std::string suffix_prev = wire_name_suffix(i, j-1);
    std::string zname = "z" + suffix_cur;
    int wire_index = wire_name_to_index(zname);

    if (wires.find(wire_index) == wires.end() || zname == "z00" || zname == "z01" || zname == "z45")
      continue;
    
    // Wire types: AND 0, OR 1, XOR 2
    wire &zi = wires[wire_index];
    wire *xi_XOR_yi, *ci;

    // If this gate is not XOR, this gate is incorrect
    if (zi.type != 2) {
      std::cout << zname << " : self\n";
      continue;
    }

    // If neither parent is XOR, this gate is incorrect
    if (zi.left->type != 2 && zi.right->type != 2) {
      std::cout << zname << " : neither parent is XOR\n";
      continue;
    }

    // Otherwise, check parent gates
    if (zi.left->type == 2) {
      xi_XOR_yi = zi.left;
      ci = zi.right;
    } else {
      xi_XOR_yi = zi.right;
      ci = zi.left;
    }

    // Check validity of xi XOR yi
    if ((xi_XOR_yi->left->name != "x" + suffix_cur && xi_XOR_yi->right->name != "x" + suffix_cur)) {
      std::cout << zname << " : parent " << xi_XOR_yi->name << " is not xi XOR yi\n";
      continue;
    }

    // Check validity of ci = (xj AND yj) OR (cj AND (xj XOR yj))
    if (ci->type != 1) {
      std::cout << zname << " : parent " << ci->name << " (ci) is not _ OR _\n";
      continue;
    }

    wire *xj_AND_yj, *cj_AND_xj_XOR_yj;
    if (ci->left->type != 0 || ci->right->type != 0) {
      std::cout << zname << " : parent " << ci->name << " (ci) is not (_ AND _) OR (_ AND _)\n";
      continue;
    }

    if (ci->left->left->name == "x" + suffix_prev || ci->left->right->name == "x" + suffix_prev) {
      xj_AND_yj = ci->left;
      cj_AND_xj_XOR_yj = ci->right;
    } else {
      xj_AND_yj = ci->right;
      cj_AND_xj_XOR_yj = ci->left;
    }

    if (cj_AND_xj_XOR_yj->left->type != 2 && cj_AND_xj_XOR_yj->right->type != 2) {
      std::cout << zname << " : parent " << ci->name << " (ci) parent " << cj_AND_xj_XOR_yj << " (cj AND (xj XOR yj)) is not (_ AND (_ XOR _))\n";
      continue;
    }
  }

  // Based on output of above from initial state:
  // z07, z13, z31 are not XOR
  // z24 is missing parent-XOR
  // z08, z14, z25, z32 have incorrect carry bits

  // zi = (xi XOR yi) XOR ci
  // ci = (xj AND yj) OR (cj AND (xj XOR yj))

  // Deducing:
  /*
    z07:
      target (x07 XOR y07) XOR c07
      real   x07 AND y07
      x07 XOR y07 -> kpv
    c07:
      x06 AND y06 -> mwg
      x06 XOR y06 -> qgc
      qgc AND kfh -> cfw
      mwg OR qgc -> rvc
    => c07 = rvc
    z07: kpv XOR rvc -> z07
    => swap z07, swt
  */
  // Fixed z07, z08

  // Deducing:
  /*
    z13:
      target (x13 XOR y13) XOR c13
      real   skt OR wpp
      x13 XOR y13 -> dwq
    c13:
      x12 AND y12 -> pmr
      x12 XOR y12 -> bqw
      bqw AND mnh -> dsw
      pmr OR dsw -> pgq
    => c13 = pgq
    z13: dwq XOR pgq -> pqc
    => swap z13, pqc
  */
  // Fixed z13, z14

  // Deducing:
  /*
    z31:
      target (x31 XOR y31) XOR c31
      real   kqk AND djr
      x31 XOR y31 -> kqk
    c31:
      x30 AND y30 -> hcg
      x30 XOR y30 -> fdm
      fdm AND msm -> cwb
      hcg OR cwb -> djr
    => c31 = djr
    z31: djr XOR kqk -> bgs
    => swap z31, bgs
  */
  // Fixed z31, z32
  
  // zi = (xi XOR yi) XOR ci
  // ci = (xj AND yj) OR (cj AND (xj XOR yj))

  // Deducing:
  /* 
    z24:
      target (x24 XOR y24) XOR c24
      real   ktp XOR rjm
      cms OR qdw -> ktp
      x24 AND y24 -> rjm
      x24 XOR y24 -> wsv
    => swap rjm, wsv
  */
  // Fixed z24, z25

  std::vector<std::string> swaps { "z07", "swt", "z13", "pqc", "z31", "bgs", "rjm", "wsv" };
  std::sort(swaps.begin(), swaps.end());
  std::string output = "";
  for (int i = 0; i < swaps.size(); i++)
    output += swaps[i] + ((i == swaps.size() - 1) ? "" : ",");
  
  std::cout << "Part 2\n  Swapped wires : " << output << std::endl;

  return 0;
}