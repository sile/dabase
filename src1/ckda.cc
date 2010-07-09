#include <iostream>
#include <sys/time.h>
#include "double_array.h"
#include "char_stream_vector.h"

inline double gettime(){
  timeval tv;
  gettimeofday(&tv,NULL);
  return static_cast<double>(tv.tv_sec)+static_cast<double>(tv.tv_usec)/1000000.0;
}

int main(int argc, char** argv) {
  if(argc != 3) {
    std::cerr << "Usage: ckda <index> <wordlist>" << std::endl;
    return 1;
  }
  
  DoubleArray da(argv[1]);
  CharStreamVector csv(argv[2]);

  double beg_t = gettime();
  unsigned err = 0;
  
  for(unsigned i=0; i < csv.size(); i++)
    if(da.search(csv[i].rest()) == -1)
      err++;

  std::cerr << "Not Found: " << err << "/" << csv.size() << std::endl;
  std::cerr << "Elapsed: " << gettime()-beg_t << " sec" << std::endl << std::endl;

  return 0;
}

