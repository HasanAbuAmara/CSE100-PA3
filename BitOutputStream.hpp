#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>

using namespace std;

class BitOutputStream {

private:
  ostream& out;
  char buf;
  int buf_index;
  //char setBit( char b, int bit, int n );

public:
  BitOutputStream(ostream& s) : out(s), buf(0), buf_index(0) { }

  // write a bit to the bit stream
  void writeBit(int bit);

  // write an byte to the stream (used for header)
  void writeByte(int i);

  // write an integer to the stream (used for header)
  void writeInt(int i);

  void flush();
};

#endif
