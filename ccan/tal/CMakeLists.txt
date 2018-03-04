

include_directories(../alignof
        ../build_assert
        ../check_type
        ../compiler
        ../container_of
        ../hash
        ../htable
        ../likely
        ../list
        ../str
        ../take
        ../typesafe_cb)
add_library(tal tal.c ../take/take.c ../str/str.c ../list/list.c ../htable/htable.c)