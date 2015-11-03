#include <iostream>
#include <vector>
#include <cerrno>
#include <cstring>
#include "BitOutputStream.hpp"
#include "HCTree.hpp"

using namespace std;

int main( int argc, char* argv[] )
{
   if( argc != 3 )
   {
      cerr << "Usage: " << argv[0] << " inputfile outputfile."  << endl;
      return -1;
   }

   errno = 0;
   ifstream in;
   in.open( argv[1], ios::binary );
   if( errno )
   {
      cerr << "Error in " << argv[1] << ": " << strerror( errno ) << endl;
      return -1;
   }

   errno = 0;
   ofstream out;
   out.open( argv[2], ios::binary );
   if( errno )
   {
      cerr << "Error in " << argv[2] << ": " << strerror( errno ) << endl;
      return -1;
   }

   int msgCnt = 0;
   int symCnt = 0;
   vector<int>* freqs = new vector<int>( 256, 0 );
   while( 1 )
   {
      unsigned char c = in.get();
      if(! in.good() )
      {
         break;
      }

      if( freqs->at( c ) == 0 )
      {
         symCnt++;
      }
      freqs->at( c )  += 1;
      msgCnt++;
   }
   if(! in.eof() )
   {
      cerr << "There was a problem, sorry." << endl;
      delete freqs;
      return -1;
   }
   in.close();

   HCTree* encoderTree = new HCTree();
   encoderTree->build( *freqs );
   BitOutputStream* output = new BitOutputStream( out );
   output->writeInt( msgCnt );
   output->writeInt( symCnt );
   for( int i = 0; i < 256; i++ )
   {
      if( freqs->at( i ) )
      {
         output->writeByte( i );
         output->writeInt( freqs->at( i ) );
      }
   }

   ifstream in2;
   in2.open( argv[1], ios::binary );
   while( 1 )
   {
      unsigned char c = in2.get();
      if( ! in2.good() )
      {
         break;
      }
      encoderTree->encode( c, *output );
   }
   if( ! in2.eof() )
   {
      delete output;
      delete encoderTree;
      delete freqs;
      cerr << "Sorry, but there was a problem." << endl;
      return -1;
   }
   output->flush();

   in2.close();
   out.close();

   delete output;
   delete encoderTree;
   delete freqs;
   return 0;
}
