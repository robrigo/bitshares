#pragma once
#include <fc/io/varint.hpp>
#include <fc/io/raw_fwd.hpp>
#include <fc/io/enum_type.hpp>
#include <fc/crypto/ripemd160.hpp>
#include <fc/crypto/elliptic.hpp>
#include <fc/reflect/reflect.hpp>
#include <fc/reflect/variant.hpp>
#include <fc/optional.hpp>
#include <memory>
#include <vector>
#include <deque>

namespace bts { namespace chain {
   using                               std::map;
   using                               std::vector;
   using                               std::unordered_map;
   using                               std::string;
   using                               std::deque;
   using                               std::shared_ptr;
   using                               std::set;

   using                               fc::variant_object;
   using                               fc::enum_type;
   using                               fc::optional;

   //typedef fc::unsigned_int            object_id_type;
   typedef uint32_t                    object_id_type;
   typedef object_id_type              account_id_type;
   typedef object_id_type              asset_id_type;
   typedef object_id_type              delegate_id_type;
   typedef fc::ripemd160               block_id_type;
   typedef fc::sha256                  digest_type;
   typedef fc::ecc::compact_signature  signature_type;
} }  // bts::chain
