#include <bts/chain/account_operations.hpp>
#include <bts/chain/database.hpp>

namespace bts { namespace chain {
   void create_account_operation::evaluate( transaction_evaluation_state& eval_state )
   {
       database& db = database::instance();

       object_pointer<account> current_account = db.lookup_account( this->name );
       FC_ASSERT( !current_account );

       std::shared_ptr<account> new_account = db.create<account>();
       new_account->name = this->name;
       db.index_account( new_account );
   }
} }
