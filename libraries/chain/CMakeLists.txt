file(GLOB HEADERS "include/bts/chain/*.hpp")

add_library( bts_chain 
             block.cpp 
             database.cpp
           )

target_link_libraries( bts_chain fc bts_db leveldb )
target_include_directories( bts_chain
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
