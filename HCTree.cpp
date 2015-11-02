#include <iostream>
#include <queue>
#include <vector>
#include "HCNode.hpp"
#include "HCTree.hpp"
using namespace std;

void HCTree::build( const vector<int>& freqs )
{
   priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp>* huffmanForest
     = new priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp>();
   for( int i = 0; i < 256; i++ )
   {
      if ( freqs.at( i ) )
      {
         HCNode* newNode = new HCNode( freqs.at( i ), char( i ) );
         huffmanForest->push( newNode );
         leaves.at( i ) = newNode;
      }
   }

   while( huffmanForest->size() > 1 )
   {
      HCNode* node1 = huffmanForest->top();
      huffmanForest->pop();
      HCNode* node0 = huffmanForest->top();
      huffmanForest->pop();

      HCNode* intNode = new HCNode( node1->count + node0->count,
                                    node1->symbol );

      intNode->c0 = node0;
      intNode->c1 = node1;
      
      node0->p = intNode;
      node1->p = intNode;
      
      huffmanForest->push( intNode );
   }

   root = huffmanForest->top();
   huffmanForest->pop();

   delete huffmanForest;
}

void HCTree::encodeByte( HCNode* node, BitOutputStream& out ) const
{
   if( node == root )
   {
      return;
   }

   encodeByte( node->p, out );
   if( node == node->p->c0 )
   {
      out.writeBit( 0 );
   }
   else
   {
      out.writeBit( 1 );
   }
}

void HCTree::encode( byte symbol, BitOutputStream& out ) const
{
   encodeByte( leaves.at( symbol ), out );
}

int HCTree::decode( BitInputStream& in ) const
{
   HCNode* curr = root;
   while( curr->c0 && curr->c1 )
   {
      int bit = in.readBit();
      if( bit )
      {
         curr = curr->c1;
      }
      else
      {
         curr = curr->c0;
      }
   }

   return curr->symbol;
}

void HCTree::deleteAll( HCNode* node )
{
   if( !node )
   {
      return;
   }

   deleteAll( node->c1 );
   deleteAll( node->c0 );
   delete node;
   node = 0;
}

HCTree::~HCTree()
{
   deleteAll( root );
   root = 0;
}
