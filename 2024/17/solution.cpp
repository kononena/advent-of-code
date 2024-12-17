#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

template <typename T>
bool read_next_integer(std::ifstream& file, T& val) {
  char c = file.get();
  while (c < '0' || c > '9') {
    if (c == -1)
      return false;
    c = file.get();
  }

  val = (T)0;
  while (c >= '0' && c <= '9') {
    val = val * (T)10 + (c - '0');
    c = file.get();
  }
  return true;
}

long combo_operand(long operand, long RA, long RB, long RC) {
  if (operand <= 3)
    return operand;
  else if (operand == 4)
    return RA;
  else if (operand == 5)
    return RB;
  else if (operand == 6)
    return RC;
  std::cerr << "Invalid combo operand : " << operand << std::endl;
  return 0L; // Invalid
}

void execute_program(std::vector<long>& output, std::vector<long>& opcodes, std::vector<long>& operands, long RA, long RB, long RC) {
  long opcode, operand, out;
  int pointer = 0;
  while (pointer < opcodes.size()) {
    opcode = opcodes[pointer];
    operand = operands[pointer];
    switch (opcode) {
      case 0L:
        // Divide by 2 ^ (combo_operand)
        RA >>= combo_operand(operand, RA, RB, RC);
        break;
      case 1L:
        RB ^= operand;
        break;
      case 2L:
        RB = (combo_operand(operand, RA, RB, RC)) & 7L;
        break;
      case 3L:
        if (RA != 0L) {
          pointer = operand;
          pointer--; // account for pointer++ at the end
        }
        break;
      case 4L:
        RB = RB ^ RC;
        break;
      case 5L:
        output.push_back(combo_operand(operand, RA, RB, RC) & 7L);
        break;
      case 6L:
        RB = RA >> combo_operand(operand, RA, RB, RC);
        break;
      case 7L:
        RC = RA >> combo_operand(operand, RA, RB, RC);
        break;
      default:
        std::cerr << "Invalid opcode : " << opcode << std::endl;
    }

    pointer++;
  }
}

bool output_matches(std::vector<long>& output, std::vector<long>& opcodes, std::vector<long>& operands, long RA, long RB, long RC, int index) {
  output.clear();
  execute_program(output, opcodes, operands, RA, RB, RC);
  if (output.size() != 2 * opcodes.size() - index)
    return false;
  
  for (int i = index; i < 2 * opcodes.size(); i++) {
    if (i & 1) {
      if (operands[i / 2] != output[i - index])
        return false;
    } else {
      if (opcodes[i / 2] != output[i - index])
        return false;
    }
  }

  return true;
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
  
  long RA, RB, RC;
  read_next_integer(file, RA);
  read_next_integer(file, RB);
  read_next_integer(file, RC);

  long opcode, operand;
  std::vector<long> opcodes;
  std::vector<long> operands;
  while (read_next_integer(file, opcode)) {
    read_next_integer(file, operand);
    opcodes.push_back(opcode);
    operands.push_back(operand);
  }

  std::vector<long> output;
  execute_program(output, opcodes, operands, RA, RB, RC);

  std::cout << "Part 1\n  Printed output : ";
  for (int i = 0; i < output.size() - 1; i++)
    std::cout << output[i] << ",";
  std::cout << output.back() << std::endl;

  // // By analysis, output length is equal to log10(RA) / 3
  // // So RA must be at least 2 * opcodes.size() * 3 bits long
  // // !NB! Each set of 3 bits is (almost) completely independent!
  // long min_RA = INT64_MAX;
  // for (long init = 0L; init < (1L << 9); init++) {
  //   RA = init;
  //   long locked = (1L << 9) - 1L;
  //   int pointer = opcodes.size() - 1;
  //   while (pointer >= 0) {
  //     if (pointer != opcodes.size() - 1) {
  //       RA <<= 3;
  //       locked <<= 3;
  //     }
  //     int target = operands[pointer];

  //     for (int B = 0; B < 8; B++) {
  //       int C = target ^ ((B ^ 1) ^ 5);
  //       int A = C << (B ^ 1);
  //       // check possible overlap between A that yields C and A that yields B
  //       // if any overlap is invalid, skip this B
  //       int overlap = 7 & (7 << (B ^ 1));
  //       if ((A & overlap) != (B & overlap))
  //         continue;
        
  //       A |= B; // this A yields the target output

  //       // check if any bits in RA are already locked
  //       // if none are locked, this lock this A and break
  //       // if some are locked, check validity
  //       // if invalid, skip this B

  //       long check_locked = 7 | (7 << (B ^ 1));
  //       long locked_overlap = locked & check_locked;
  //       if (locked_overlap != 0 && (RA & locked_overlap) != (A & locked_overlap))
  //         continue;
        
  //       RA |= A;
  //       locked |= check_locked;
  //       break;
  //     }


  //     RA <<= 3;
  //     locked <<= 3;
  //     target = opcodes[pointer];

  //     for (int B = 0; B < 8; B++) {
  //       int C = target ^ ((B ^ 1) ^ 5);
  //       int A = C << (B ^ 1);
  //       // check possible overlap between A that yields C and A that yields B
  //       // if any overlap is invalid, skip this B
  //       int overlap = 7 & (7 << (B ^ 1));
  //       if ((A & overlap) != (B & overlap))
  //         continue;
        
  //       A |= B; // this A yields the target output

  //       // check if any bits in RA are already locked
  //       // if none are locked, this lock this A and break
  //       // if some are locked, check validity
  //       // if invalid, skip this B

  //       long check_locked = 7 | (7 << (B ^ 1));
  //       long locked_overlap = locked & check_locked;
  //       if (locked_overlap != 0 && (RA & locked_overlap) != (A & locked_overlap))
  //         continue;
        
  //       RA |= A;
  //       locked |= check_locked;
  //       break;
  //     }

  //     pointer--;
  //   }

  //   if (RA < 175647654951750L && RA > 164278742384972L)
  //     std::cout << "RA : " << RA << "\n";
  //   min_RA = std::min(min_RA, RA);
  // }

  // Semi-brute-force solution
  std::queue<long> partial_A;
  std::queue<long> partial_B;
  for (long i = 0L; i < (1L << 9); i++)
  if (output_matches(output, opcodes, operands, i, RB, RC, 2 * opcodes.size() - 1))
    partial_A.push(i);
  
  int index = 2 * opcodes.size() - 2;
  while (index >= 0) {
    long nA = 0L;
    while (!partial_A.empty()) {
      long A = partial_A.front();
      nA++;
      partial_A.pop();

      for (int i = 0; i < 8; i++)
      if (output_matches(output, opcodes, operands, (A << 3) | i, RB, RC, index))
        partial_B.push((A << 3) | i);
    }

    partial_A = partial_B;
    index--;
  }

  long min_RA = INT64_MAX;
  while (!partial_A.empty()) {
    long A = partial_A.front();
    partial_A.pop();

    output.clear();
    execute_program(output, opcodes, operands, A, RB, RC);
    bool valid = output.size() == 2 * opcodes.size();
    if (!valid)
      continue;
    
    for (index = 0; index < 2 * opcodes.size() - 2; index++) {
      if (index & 1) {
        if (operands[index / 2] != output[index])
          valid = false;
      } else {
        if (opcodes[index / 2] != output[index])
          valid = false;
      }
    }
    
    if (valid)
      min_RA = std::min(min_RA, A);
  }
  std::cout << "Part 2\n  Lowest positive self-copying A : " << min_RA << std::endl;

  return 0;
}