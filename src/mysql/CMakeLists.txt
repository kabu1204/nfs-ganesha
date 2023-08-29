set(MYSQL_APPLIER_SRC
    handle_to_name.c
    )

add_library(mysql_applier STATIC ${MYSQL_APPLIER_SRC})

target_link_libraries(mysql_applier hashtable)

set_target_properties(mysql_applier PROPERTIES COMPILE_FLAGS "-fPIC")