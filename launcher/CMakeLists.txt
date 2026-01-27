project(launcher)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "source/*.cpp" "source/*.h" "source/*.hpp" "source/*.inl")
file(GLOB_RECURSE RESOURCE_FILES CONFIGURE_DEPENDS "resource/*.rc")

set(LAUNCHER_SOURCES ${SOURCE_FILES} ${RESOURCE_FILES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "Source" FILES ${SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/resource" PREFIX "Resources" FILES ${RESOURCE_FILES})

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
