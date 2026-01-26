project(launcher)

set(LAUNCHER_SOURCES
    source/main.cpp
    resource/launcher_resource.rc
)

add_executable(launcher ${LAUNCHER_SOURCES})

target_include_directories(launcher PRIVATE 
    source 
    resource
    ${CMAKE_SOURCE_DIR}/third_party/detours/include
)

target_link_directories(launcher PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/detours/lib.X86
)

target_link_libraries(launcher PRIVATE 
    detours.lib 
    shlwapi.lib
)

# Disable specific warnings 4099
if(MSVC)
    target_compile_options(launcher PRIVATE /wd4099)
endif()
