#include <iostream>
#include "builder.h"

int main(int argc, char** argv) {
  if(argc != 3) {
    std::cerr << "Usage: mkda <index> <wordlist>" << std::endl;
    return 1;
  }

  Builder(argv[2]).build().save(argv[1]);
  return 0;
}

