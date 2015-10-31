#include <iostream>
#include <string>
#include "BitInputStream.hpp"

using namespace std;

void BitInputStream::fill()
{
   buf = in.get();
   buf_index = 0;
}

char bitVal( char b, int n )
{
   char c;
   c = b>>(n-1) & 1;
   return c;
}

int BitInputStream::readBit()
{
   if( buf_index == 8 )
   {
      fill();
   }

   int v = bitVal( buf, buf_index + 1 );

   buf_index++;
   return v;
}

int BitInputStream::readByte()
{
   return in.get();
}

int BitInputStream::readInt()
{
   char* buffer = new char[ sizeof(int) + 1 ];
   buffer[ sizeof(int) ] = '\0';
   in.read( buffer, sizeof(int) );
   int num = stoi( string( buffer ) );
   delete[] buffer;
   return num;
}
