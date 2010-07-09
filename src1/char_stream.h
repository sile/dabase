#ifndef CHAR_STREAM_H
#define CHAR_STREAM_H

class CharStream {
public:
  CharStream(const char* str) : cur(str) {}
  unsigned char read() { return *cur++; }     
  unsigned char prev() const { return cur[-1]; }
  unsigned char peek() const { return *cur; } 
  const char*   rest() const { return cur; }

private:
  const char* cur;
};

#endif

