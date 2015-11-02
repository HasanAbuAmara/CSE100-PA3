#include <iostream>
#include <string>
#include "BitOutputStream.hpp"
using namespace std;

void BitOutputStream::flush()
{
   out.put( buf );
   out.flush();
   buf = buf_index = 0;
}

char setBit( char b, int bit, int n )
{
   char c;

   if( bit == 1 )
   {
      c = b | ( bit << ( n - 1 ) );
   }
   else
   {
      c = b & ~( 1 << ( n - 1 ) );
   }

   return c;
}

void BitOutputStream::writeBit( int i )
{
   if( buf_index == 8 )
   {
      flush();
   }

   buf = setBit( buf, i, buf_index + 1 );

   buf_index++;
}

void BitOutputStream::writeByte( int i )
{
   out.put( char( i ) );
   out.flush();
} 

void BitOutputStream::writeInt( int i )
{
   out.write( reinterpret_cast<const char*>( &i ), sizeof( i ) );
   out.flush();
}

