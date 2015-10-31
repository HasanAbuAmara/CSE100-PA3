#include "HCNode.hpp"

bool HCNode::operator<( HCNode const& other ) const
{
   if ( count != other.count )
   {
      return count > other.count;
   }
   else
   {
      return symbol < other.symbol;
   }
}

bool comp( HCNode* one, HCNode* other )
{
   return *one < *other;
}
