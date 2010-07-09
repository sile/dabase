#include <iostream>
#include <string>
#include "double_array.h"

struct Callback {
  void operator()(const char* key, unsigned offset, int id) const {
    std::cout << " => " << id << "#" << std::string(key,offset) << std::endl;
  }
};

int main(int argc, char** argv) {
  if(argc != 2) {
    std::cerr << "Usage: da <index>" << std::endl;
    return 1;
  }
  
  DoubleArray da(argv[1]);
  std::string word;
  Callback fn;
  
  while(getline(std::cin, word)) 
    da.each_common_prefix(word.c_str(), fn);

  return 0;
}

