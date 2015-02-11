#pragma once
#include <bts/chain/types.hpp>

namespace bts { namespace chain {
   enum operation_type
   {
      null_op_type = 0
   };

   struct operation 
   {
      enum_type<uint16_t,operation_type> type = null_op_type;
      vector<char>                       data;

      operation(){}

      operation( operation&& o )
      :type(o.type),data(std::move(o.data)){}

      template<typename OperationType>
      operation( const OperationType& t )
      {
         type = OperationType::type;
         data = fc::raw::pack( t );
      }

      template<typename OperationType>
      OperationType as()const
      {
         FC_ASSERT( (operation_type)type == OperationType::type, 
                    "", ("type",type)("OperationType",OperationType::type) );

         return fc::raw::unpack<OperationType>(data);
      }

      operation& operator=( const operation& o )
      {
         if( this == &o ) return *this;
         type = o.type;
         data = o.data;
         return *this;
      }

      operation& operator=( operation&& o )
      {
         if( this == &o ) return *this;
         type = o.type;
         data = std::move(o.data);
         return *this;
      }
   };


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

FC_REFLECT_ENUM( bts::chain::operation_type, (null_op_type) )
FC_REFLECT( bts::chain::operation, (type)(data) )
FC_REFLECT( bts::chain::transaction, (expiration)(operations) )
FC_REFLECT_DERIVED( bts::chain::signed_transaction, (bts::chain::transaction), (signatures) )
