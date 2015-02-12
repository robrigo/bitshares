#pragma once

namespace bts { namespace chain {
   struct create_account_operation
   {
       static const operation_type type;

       string          name;
       account_id_type paying_account;
       share_type      registration_fee;
       asset_id_type   registration_fee_type;

       authority       owner;
       authority       active;

       void evaluate( transaction_evaluation_state& eval_state );
   };

} } // bts::chain
