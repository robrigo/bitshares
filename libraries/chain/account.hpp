#pragma once
#include <bts/chain/database.hpp>

namespace bts { namespace chain {
   class account : public object
   {
      public:
         account():object( account_object_type ){};

         virtual packed_object pack()const override { return packed_object( *this ); }
         virtual void          unpack( const packed_object& obj ) override { obj.unpack(*this); }

         string                             name;
         authority                          owner;
         authority                          active;
         map<asset_pointer,balance_pointer> balances;
   };
}} 
FC_REFLECT_DERIVED( bts::chain::account, (bts::chain::object), (name) )
