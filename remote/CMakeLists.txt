project(remote)

set(REMOTE_SOURCES
    source/main.cpp
)

add_executable(remote_console ${REMOTE_SOURCES})

target_include_directories(remote_console PRIVATE source)

target_link_libraries(remote_console PRIVATE Ws2_32.lib)

set_target_properties(remote_console PROPERTIES
    OUTPUT_NAME "remote_console"
)
