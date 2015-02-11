#include <fc/reflect/reflect.hpp>
#include <fc/io/enum_type.hpp>
#include <fc/variant.hpp>
#include <map>

namespace bts { namespace chain {

   using fc::variant_object;
   using fc::enum_type;
   using std::map;

   typedef uint32_t object_id_type;

   enum object_type
   {
      null_object_type    = 0,
      base_object_type    = 1,
      account_object_type = 2
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
      void unpack( T& o ) 
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

      // exposed for reflection purposes only, do not access directly
         enum_type<uint16_t,object_type> type;
         object_id_type                  id = 0;

         void set_database( database* db ) { _db = db; }
      private:
         database*                      _db = nullptr;
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

   template<typename T>
   class object_pointer
   {
      public:
         object_pointer(){}
         object_pointer( std::shared_ptr<T> ptr )
         :_ptr(ptr){ if( ptr ) id = ptr->id(); }

         explicit operator bool()const { return id != 0; }
         std::shared_ptr<T> get( database* db )
         {
            auto shared = _ptr.lock();

         }

         object_id_type       id = 0;
      private:
         std::weak_ptr<T> _ptr;
   };

   class database
   {
      public:
         void push_undo_state();
         void pop_undo_state();
         void undo();

         template<typename T>
         object_pointer<T> create()
         {
            ++_next_object_id;
            _undo_state.new_ids.push_back( _next_object_id );

            auto obj = std::make_shared<T>();
            obj->id  = _next_object_id;
            obj->set_database( this );

            object_pointer<T> ptr(obj);

            _loaded_objects[obj->id] = obj;
            return ptr;
         }

         void push_block( const block& b );
         void index_account( const object_pointer<account>& a );
         
         template<typename T>
         std::shared_ptr<const T> get( object_id_type id )
         {
            auto itr = _loaded_objects.find( id );
            if( itr != _loaded_objects.end() )
               return std::dynamic_pointer_cast<T>(itr->second);
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
            return std::shared_ptr<T>();
         }

      private:
         void apply_transaction( const signed_transaction& trx );

         deque<undo_state>                        _undo_state;
         map<object_id_type, shared_ptr<object> > _loaded_objects;
         map<string, object_pointer<account> >    _account_index;
   };

} }

FC_REFLECT_ENUM( bts::chain::object_type,
                 (null_object_type)
                 (base_object_type)
                 (account_object_type) 
               )
FC_REFLECT( bts::chain::packed_object, (type)(data) )
FC_REFLECT( bts::chain::object, (id) )

FC_REFLECT( bts::chain::undo_state, 
            (old_next_object_id)
            (old_values)
            (old_account_index) )

FC_REFLECT_TEMPLATE((typename T), bts::chain::object_pointer, (id) )



