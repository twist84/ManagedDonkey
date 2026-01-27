project(remote)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "source/*.cpp" "source/*.h" "source/*.hpp" "source/*.inl")

set(REMOTE_SOURCES ${SOURCE_FILES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "Source" FILES ${SOURCE_FILES})

add_executable(remote_console ${REMOTE_SOURCES})

target_include_directories(remote_console PRIVATE source)

target_link_libraries(remote_console PRIVATE Ws2_32.lib)

set_target_properties(remote_console PROPERTIES
    OUTPUT_NAME "remote_console"
)
