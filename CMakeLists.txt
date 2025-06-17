cmake_minimum_required(VERSION 3.16)
project(chess VERSION 1.0.0)

# Enable testing support
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# request use of C++20 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# warning
string(APPEND CMAKE_CXX_FLAGS
      " -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion"
      " -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast"
      " -fno-omit-frame-pointer")

# standard library assertions
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_ASSERTIONS")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  string(APPEND CMAKE_CXX_FLAGS " -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE")
endif()

# address sanitizer and undefined-behaviour sanitizer in Debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined")
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_SANITIZE_STD_ALLOCATOR")
endif()
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined")

# Find SFML and TGUI
find_package(SFML 2.6 COMPONENTS graphics REQUIRED)
find_package(TGUI 1 REQUIRED)

# Source files
file(GLOB SOURCES "source/*.cpp")

# Executable
add_executable(chess ${SOURCES})

# Link SFML and TGUI
target_link_libraries(chess sfml-graphics sfml-window sfml-system TGUI::TGUI)

# Link INCLUDE
target_include_directories(chess PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Optionally enable tests
# option(BUILD_TESTS "Build the tests executables" OFF)
# if(BUILD_TESTS)
#     add_subdirectory(testing)
# endif()