#ifndef DOUBLE_ARRAY_H
#define DOUBLE_ARRAY_H

#include <cstdio>
#include "char_stream.h"

class DoubleArray {
public:
  DoubleArray(const char* filepath) {
    FILE* f = fopen(filepath, "rb");
    
    unsigned node_size;
    fread(&node_size, sizeof(unsigned), 1, f);
    base = new int [node_size];
    chck = new int [node_size];
    
    fread(base, sizeof(int), node_size, f);
    fread(chck, sizeof(int), node_size, f);
    fclose(f);
  }
  ~DoubleArray() {
    delete [] base;
    delete [] chck;
  }

  int search(const char* key) const {
    int node_index=0;
    CharStream in(key);
    for(;;) {
      int next_index = base[node_index] + in.read();
      int node       = base[next_index];
      if(chck[next_index] == node_index)
        if(node < 0) return -node;
        else         node_index = next_index;
      else
        return -1;  // キーが存在しない場合は-1を返す
    }
  }

  template<class Callback>
  void each_common_prefix(const char* key, Callback& fn) const {
    int node_index=0;
    CharStream in(key);
    for(unsigned offset=0;; offset++) {
      int terminal_index = base[node_index] + '\0';      // 途中一致があるかどうかを判定
      if(chck[terminal_index] == node_index) {
        fn(key, offset, -base[terminal_index]);          // コールバック呼び出し
        if(in.peek()=='\0')
          break;
      }

      int next_index = base[node_index] + in.read();
      if(chck[next_index] == node_index) {
        // assert(node >= 0);   上で終端チェックは行っているので、nodeがマイナスになることはないはず
        node_index = next_index;
      } else
        break;
    }    
  }

private:
  int *base;
  int *chck;
};

#endif

