#pragma once
#include <bts/chain/operations.hpp>

namespace bts { namespace chain { 

   struct create_user_asset_operation
   {
       static const operation_type type;
       string symbol;

       void evaluate( transaction_evaluation_state& eval_state );
   };
} }

FC_REFLECT( bts::chain::create_user_asset_operation, (symbol) )
