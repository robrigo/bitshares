#include <bts/chain/database.hpp>
#include <fc/io/raw.hpp>

namespace bts { namespace chain {

void database::save_undo( const shared_ptr<object>& obj )
{
   FC_ASSERT( obj );
   auto id = obj->id;
   auto current_undo = _undo_state.back().old_values.find(id);
   if( current_undo == _undo_state.back().old_values.end() )
   {
      _undo_state.back().old_values[id] = obj->pack();
   }
}
void database::pop_undo_state()
{
   _undo_state.pop_back();
}
void database::push_undo_state()
{
   _undo_state.push_back( undo_state() );
   _undo_state.back().old_next_object_id = _next_object_id;
}
void database::undo()
{
   for( auto item : _undo_state.back().old_values )
   {
      if( item.second.type == null_object_type )
      {
         _loaded_objects.erase( item.first );
      }
      else
      {
         auto obj_itr = _loaded_objects.find( item.first );
         FC_ASSERT( obj_itr != _loaded_objects.end() );
         obj_itr->second->unpack( item.second );
      }
   }
   for( auto item : _undo_state.back().old_account_index )
   {
      if( item.second != 0 ) 
      { 
         _account_index[item.first] = get<account>(item.second); 
      }
      else
      {
         _account_index.erase( item.first );
      }
   }
   _next_object_id = _undo_state.back().old_next_object_id;
   _undo_state.pop_back();
}

void database::index_account( const object_pointer<account>& a )
{
   auto name    = a.get()->name;
   auto cur_itr = _account_index.find(name);
   if( cur_itr != _account_index.end() )
   {
      _undo_state.back().old_account_index[name] = cur_itr->second.id;
   }
   else
   {
      _undo_state.back().old_account_index[name] = 0;
   }
   _account_index[name] = a;
}

void database::push_block( const block& new_block )
{ try {
   push_undo_state();
   optional<fc::exception> except;
   try {
      for( const auto& trx : new_block.transactions )
         apply_transaction( trx );
   } 
   catch ( const fc::exception& e ) { except = e; }
   if( except ) 
   {
      undo();
      throw *except;
   }
} FC_CAPTURE_AND_RETHROW( (new_block) ) }


void database::apply_transaction( const signed_transaction& trx )
{ try {
   push_undo_state();
   optional<fc::exception> except;
   try {
      
   } 
   catch ( const fc::exception& e ) { except = e; }
   if( except ) 
   {
      undo();
      throw *except;
   }
} FC_CAPTURE_AND_RETHROW( (trx) ) }

} } // namespace bts::chain
