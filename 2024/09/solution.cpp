#include <iostream>
#include <fstream>
#include <vector>

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
  
  std::string disk;
  file >> disk;

  long Oi = 0L;
  long Li = 0L;
  long Ri = disk.length() - 1;
  if (Ri & 1L)
    Ri--;
  
  int Lval = disk[Li] - '0';
  int Rval = disk[Ri] - '0';

  long checksum = 0L;
  while (Li < Ri) {
    if (Li & 1L) { // Right
      checksum += (Ri >> 1) * (Oi++);
      Rval--;
    }
    else // Left
      checksum += (Li >> 1) * (Oi++);
    Lval--;
    
    while (Lval <= 0) {
      Li++;
      Lval = disk[Li] - '0';
    }
    while (Rval <= 0) {
      Ri -= 2;
      Rval = disk[Ri] - '0';
    }
  }
  
  if (Li == Ri)
  while (Rval-- > 0)
    checksum += (Ri / 2) * (Oi++);

  std::cout << "Part 1\n  Checksum : " << checksum << std::endl;

  std::vector<std::pair<long, long>> output;
  for (long i = 0; i < disk.length(); i++) {
    int val = disk[i] - '0';
    output.push_back(std::make_pair(val, (i & 1L) ? (-1L) : (i / 2)));
  }

  int i = output.size() - 1;
  while (i >= 0) {

    auto lv = output[i--];
    long length = lv.first;
    long value = lv.second;
    if (value == -1L)
      continue;
    
    for (int j = 0; j <= i; j++) {
      auto jlv = output[j];
      long jlength = jlv.first;
      long jvalue = jlv.second;
      if (jvalue != -1 || jlength < length)
        continue;
      
      output[i+1] = std::make_pair(length, -1L);
      if (jlength != length) {
        output.insert(output.begin() + j + 1, std::make_pair(jlength - length, -1L));
        i++;
      }
      output[j] = std::make_pair(length, value);
      break;
    }
  }

  checksum = 0L;
  Oi = 0L;
  for (int k = 0; k < output.size(); k++) {
    long length = output[k].first;
    long value = output[k].second;
    for (long j = 0; j < length; j++) {
      if (value != -1)
        checksum += value * Oi;
      Oi++;
    }
  }

  std::cout << "Part 2\n  Checksum : " << checksum << std::endl;

  return 0;
}