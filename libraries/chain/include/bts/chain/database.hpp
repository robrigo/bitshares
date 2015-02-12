#pragma once
#include <bts/chain/object.hpp>
#include <bts/chain/block.hpp>
#include <bts/chain/account.hpp>
#include <map>

namespace bts { namespace chain {

   template<typename T>
   class object_pointer
   {
      public:
         object_pointer(){}
         object_pointer( const std::shared_ptr<T>& ptr )
         { if( ptr ) id = ptr->id; }
         object_pointer& operator = ( const std::shared_ptr<T>& ptr )
         { if( ptr ) id = ptr->id; return *this; }
         object_pointer& operator = ( const std::shared_ptr<const T>& ptr )
         { if( ptr ) id = ptr->id; return *this; }

         explicit operator bool()const { return id != 0; }

         std::shared_ptr<T>       get_mutable();
         std::shared_ptr<const T> get()const;

         object_id_type       id = 0;
   };

   /**
    *  Undo State saves off the initial values associated 
    *  with each object prior to making changes.
    */
   struct undo_state
   {
       object_id_type                         old_next_object_id;
       map<object_id_type, packed_object>     old_values;
       map<string,object_id_type >            old_account_index;
   };

   class database
   {
      public:
         database();
         ~database();

         void push_undo_state();
         void pop_undo_state();
         void undo();

         void push_block( const block& b );
         void index_account( const object_pointer<account>& a );

         template<typename T>
         object_pointer<T> create()
         {
            ++_next_object_id;

            auto obj = std::make_shared<T>();
            obj->id  = _next_object_id;
            object_pointer<T> ptr(obj);

            _loaded_objects[obj->id] = obj;
            return ptr;
         }
         
         template<typename T>
         std::shared_ptr<const T> get( object_id_type id )
         {
            auto itr = _loaded_objects.find( id );
            if( itr != _loaded_objects.end() )
               return std::dynamic_pointer_cast<T>(itr->second);
            FC_ASSERT( !"Invalid Object ID" );
            return std::shared_ptr<T>();
         }

         template<typename T>
         std::shared_ptr<T> get_mutable( object_id_type id )
         {
            auto itr = _loaded_objects.find( id );
            if( itr != _loaded_objects.end() )
            {
               auto result   = std::dynamic_pointer_cast<T>(itr->second);
               if( result ) save_undo( itr->second );
               return result;
            }
            FC_ASSERT( !"Invalid Object ID" );
            return std::shared_ptr<T>();
         }

         static database& instance();

      private:
         void save_undo( const shared_ptr<object>& obj );
         void apply_transaction( const signed_transaction& trx );

         object_id_type                           _next_object_id;
         deque<undo_state>                        _undo_state;
         map<object_id_type, shared_ptr<object> > _loaded_objects;
         map<string, object_pointer<account> >    _account_index;
   };

   template<typename T>
   std::shared_ptr<T>       object_pointer<T>::get_mutable()
   {
      return database::instance().get_mutable<T>( id );
   }

   template<typename T>
   std::shared_ptr<const T> object_pointer<T>::get()const
   {
      return database::instance().get<T>( id );
   }



} }

FC_REFLECT( bts::chain::packed_object, (type)(data) )

FC_REFLECT( bts::chain::undo_state, 
            (old_next_object_id)
            (old_values)
            (old_account_index) )

FC_REFLECT_TEMPLATE((typename T), bts::chain::object_pointer<T>, (id) )



