#pragma once
#include <bts/chain/operations.hpp>
#include <bts/chain/address.hpp>

namespace bts { namespace chain { 
   /**
    *  Place holder for state tracked while processing a
    *  transaction.  
    */
   class transaction_evaluation_state
   {
      public:
         set<address> signed_by;
   };
} } // namespace bts::chain 
