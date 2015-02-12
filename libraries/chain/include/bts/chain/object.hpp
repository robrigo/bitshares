#pragma once
#include <bts/chain/types.hpp>
#include <fc/io/raw.hpp>

namespace bts { namespace chain {

   enum object_type
   {
      null_object_type      = 0,
      base_object_type      = 1,
      account_object_type   = 2,
      asset_object_type     = 3,
      delegate_object_type  = 4
   };

   struct packed_object
   {
      enum_type<uint16_t,object_type>  type = null_object_type;
      vector<char>                     data;
      
      packed_object(){}

      template<typename T>
      packed_object( const T& o )
      {
         type = o.type;
         data = fc::raw::pack( o );
      }

      template<typename T>
      void unpack( T& o )const
      { 
         FC_ASSERT( o.type == type ); 
         fc::raw::unpack( data, o ); 
      }
   };

   class object : public std::enable_shared_from_this<object>
   {
      public:
         object( object_type t = base_object_type ):type(t){};
         virtual ~object(){};

         virtual packed_object pack()const;
         virtual void          unpack( const packed_object& obj );

         enum_type<uint16_t,object_type> type;
         object_id_type                 id = 0;
   };

} }

FC_REFLECT_ENUM( bts::chain::object_type,
                 (null_object_type)
                 (base_object_type)
                 (account_object_type) 
                 (asset_object_type)
                 (delegate_object_type)
               )

FC_REFLECT( bts::chain::object, (type)(id) )
