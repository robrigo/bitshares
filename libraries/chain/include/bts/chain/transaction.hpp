#pragma once
#include <bts/chain/types.hpp>
#include <bts/chain/operations.hpp>

namespace bts { namespace chain {

   struct transaction 
   {
      fc::time_point_sec expiration;
      vector<operation>  operations;
   };

   struct signed_transaction : public transaction
   {
      vector<signature_type> signatures;
   };


} }

FC_REFLECT( bts::chain::transaction, (expiration)(operations) )
FC_REFLECT_DERIVED( bts::chain::signed_transaction, (bts::chain::transaction), (signatures) )
