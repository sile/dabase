#ifndef BUILDER_H
#define BUILDER_H

#include <cstdio>
#include "node_allocator.h"
#include "char_stream.h"
#include "char_stream_vector.h"

class Builder {
public:
  Builder(const char* filepath) 
    : csv(filepath), id(0), node_size(csv.size()*15) { // XXX: ノード用の配列のサイズはテキトウに決め打ち (危険!!!)
    base = new int[node_size];
    chck = new int[node_size];
    for(int i=0; i < node_size; i++)
      chck[i] = -1;
  }

  ~Builder() {
    delete [] base;
    delete [] chck;
  }
  
  Builder& build() {
    build_impl(0, csv.size(), 0);
    return *this;
  }

  void save(const char* filepath) {
    FILE* f = fopen(filepath, "wb");
    
    if(node_size > 0xFF)
      while(chck[node_size-0xFF]==-1)
        node_size--;
    
    fwrite(&node_size, sizeof(unsigned), 1, f);
    fwrite(base, sizeof(int), node_size, f);
    fwrite(chck, sizeof(int), node_size, f);
    fclose(f);
  }

private:
  void build_impl(std::size_t beg, std::size_t end, int root_node) {
    if(end-beg == 1) {
      for(; csv[beg].prev() != '\0'; csv[beg].read())
        root_node = set_node(root_node, alloc.allocate(csv[beg].peek()), csv[beg].peek());
      base[root_node] = --id;  
      return;
    }

    std::vector<unsigned char> children;
    std::vector<std::size_t>   ranges;
    do {
      children.push_back(csv[beg].peek());
      ranges.push_back(beg);
      beg = end_of_same_node(csv, beg, end);
    } while (beg != end);
    ranges.push_back(end);

    int base_node = alloc.allocate(children);

    for(std::size_t i=0; i < children.size(); i++)
      build_impl(ranges[i], ranges[i+1], set_node(root_node, base_node, children[i]));
  }

  int set_node(int node, int base_node, unsigned char child) {
    int next   = base_node + child;
    base[node] = base_node;
    chck[next] = node;
    return next;
  }

  unsigned end_of_same_node(CharStreamVector& csv, std::size_t beg, std::size_t end) {
    unsigned char ch = csv[beg].read();
    std::size_t cur  = beg+1;
    for(; cur < end && ch == csv[cur].peek(); cur++)
      csv[cur].read();
    return cur;
  }

private:
  CharStreamVector csv;
  int id;
  int node_size;
  int* base;
  int* chck;
  NodeAllocator alloc;
};

#endif

