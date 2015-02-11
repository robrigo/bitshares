#include <bts/chain/account_opperations.hpp>

namespace bts { namespace chain {
   void create_account::evaluate( transaction_evaluation_state& eval_state )
   {
       object_pointer<account> current_account = eval_state.db->get_account( this->name );
       FC_ASSERT( !current_account );

       object_pointer<account> new_account = eval_state.db->create<account>();
       new_account->name = this->name;
       eval_state.db->index_account( new_account );
   }
} }
