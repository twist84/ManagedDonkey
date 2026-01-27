cmake_minimum_required(VERSION 4.2.0)

project(ManagedDonkey LANGUAGES C CXX RC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Disable C++20 module scanning (reduces build overhead and log redundancy)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# Configuration types
if(MSVC)
  # Prefixing with numbers and underscores to force Visual Studio UI order
  set(CMAKE_CONFIGURATION_TYPES "0_Tags;1_Debug;2_Test;3_Play;4_Profile;5_Release" CACHE STRING "" FORCE)
  
  # Flags for custom configurations (Inherit from Debug or Release)
  set(CMAKE_CXX_FLAGS_0_TAGS "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS_0_TAGS "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "" FORCE)
  
  set(CMAKE_CXX_FLAGS_1_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS_1_DEBUG "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "" FORCE)

  set(CMAKE_CXX_FLAGS_2_TEST "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS_2_TEST "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "" FORCE)
  
  set(CMAKE_CXX_FLAGS_3_PLAY "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS_3_PLAY "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "" FORCE)
  
  set(CMAKE_CXX_FLAGS_4_PROFILE "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS_4_PROFILE "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "" FORCE)

  set(CMAKE_CXX_FLAGS_5_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS_5_RELEASE "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "" FORCE)

  # Enable debug symbols (Compiler)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Zi" CACHE STRING "" FORCE)

  # Enable debug symbols and map files (Linker)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG" CACHE STRING "" FORCE)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /DEBUG" CACHE STRING "" FORCE)
endif()

# Global definitions
add_compile_definitions(WIN32 _CONSOLE _UNICODE UNICODE)

# Map configurations to NETWORK_EXECUTABLE_TYPE
# 0_Tags=0, 1_Debug=1, 2_Test=2, 3_Play=3, 4_Profile=4, 5_Release=5
add_compile_definitions(NETWORK_EXECUTABLE_TYPE=$<IF:$<CONFIG:0_Tags>,0,$<IF:$<CONFIG:1_Debug>,1,$<IF:$<CONFIG:2_Test>,2,$<IF:$<CONFIG:3_Play>,3,$<IF:$<CONFIG:4_Profile>,4,$<IF:$<CONFIG:5_Release>,5,1>>>>>>>)

# Global output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/arch/$<CONFIG>)

# Global definitions
add_definitions(-DWIN32 -D_CONSOLE -D_UNICODE -DUNICODE)

# Subdirectories
add_subdirectory(remote)
add_subdirectory(launcher)
add_subdirectory(game)
