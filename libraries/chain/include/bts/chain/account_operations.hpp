#pragma once
#include <bts/chain/authority.hpp>
#include <bts/chain/transaction_evaluation_state.hpp>
#include <bts/chain/operations.hpp>

namespace bts { namespace chain {
   struct create_account_operation
   {
       static const operation_type type;

       string          name;
       account_id_type paying_account;
       share_type      registration_fee = 0;
       asset_id_type   registration_fee_type = 0;

       authority       owner;
       authority       active;

       void evaluate( transaction_evaluation_state& eval_state );
   };

} } // bts::chain

FC_REFLECT( bts::chain::create_account_operation, 
            (name)
            (paying_account)
            (registration_fee)
            (registration_fee_type)
            (owner)
            (active) )
