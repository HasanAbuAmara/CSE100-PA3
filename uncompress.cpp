#include <iostream>
#include <vector>
#include <cerrno>
#include <cstring>
#include "HCTree.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

using namespace std;

int main( int argc, char* argv[] )
{
   if( argc != 3 )
   {
      cerr << "Usage: " << argv[0] << " infile outfile." << endl;
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

   ofstream out;
   out.open( argv[2], ios::binary );
   if( errno )
   {
      cerr << "Error in " << argv[2] << ": " << strerror( errno ) << endl;
      in.close();
      return -1;
   }

   vector<int>* freqs = new vector<int>( 256, 0 );
   BitInputStream* input = new BitInputStream( in );
   BitOutputStream* output = new BitOutputStream( out );
   
   int msgCnt = input->readInt();
   int symCnt = input->readInt();
   for( int i = 1; i <= symCnt; i++ )
   {
      int c = input->readByte();
      int cCnt = input->readInt();
      freqs->at( c ) = cCnt;
   }

   HCTree* decodeTree = new HCTree();
   decodeTree->build( *freqs );
   input->fill();
   for( int i = 0; i < msgCnt; i++ )
   {
      output->writeByte( decodeTree->decode( *input ) );
   }
   in.close();
   out.close();

   delete output;
   delete decodeTree;
   delete input;
   delete freqs;
   return 0;
}
