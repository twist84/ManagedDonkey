project(game)

# 1. Version Generation
execute_process(
    COMMAND git rev-parse --short=7 HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE GIT_RESULT
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT GIT_RESULT EQUAL 0)
    set(GIT_COMMIT_HASH "unknown")
endif()

# 2. Source Files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "source/*.cpp" "source/*.h" "source/*.hpp" "source/*.inl")
file(GLOB_RECURSE COMMON_FILES CONFIGURE_DEPENDS "common/*.cpp" "common/*.h" "common/*.hpp" "common/*.inl")
file(GLOB_RECURSE RESOURCE_FILES CONFIGURE_DEPENDS "resource/*.rc")

set(GAME_SOURCES ${SOURCE_FILES} ${COMMON_FILES} ${RESOURCE_FILES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "Source" FILES ${SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/common" PREFIX "Common" FILES ${COMMON_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/resource" PREFIX "Resources" FILES ${RESOURCE_FILES})

# 3. Target
add_library(game SHARED ${GAME_SOURCES})

target_compile_definitions(game PRIVATE VERSION_GIT_COMMIT="${GIT_COMMIT_HASH}")

# 4. Include Directories
target_include_directories(game PRIVATE
    source
    resource
    common/lib_3dpart
    common/incl.sys
    common/havok
    ${CMAKE_SOURCE_DIR}/third_party/detours/include
    ${CMAKE_SOURCE_DIR}/third_party/directx/include
    ${CMAKE_SOURCE_DIR}/third_party/directxtex/include
)

# 5. Forced Includes
if(MSVC)
    target_compile_options(game PRIVATE
        /FI"_force_includes/_force_included_compile.hpp"
        /FI"_force_includes/_force_included_types.hpp"
        /FI"_force_includes/_force_included_enums.hpp"
        /FI"_force_includes/_force_included_macros.hpp"
        /WX # Treat warnings as error
        /MP # Multi-processor compilation
    )
endif()

# 6. Linking
target_link_directories(game PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/detours/lib.X86
    ${CMAKE_SOURCE_DIR}/third_party/directx/lib.X86
    ${CMAKE_SOURCE_DIR}/third_party/directxtex/lib.X86/$<IF:$<OR:$<CONFIG:0_Tags>,$<CONFIG:1_Debug>>,Debug,Release>
)

target_link_libraries(game PRIVATE
    dbghelp.lib
    detours.lib
    Ws2_32.lib
    Winmm.lib
    d3d9.lib
    d3dx9.lib
    DirectXTex.lib
    user32.lib
    gdi32.lib
    shell32.lib
    ole32.lib
)

set_target_properties(game PROPERTIES
    OUTPUT_NAME "game"
)

# Post-build copy to root bin (as done in Makefile)
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:game> ${CMAKE_SOURCE_DIR}/bin/game.dll
)
