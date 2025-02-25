cmake_minimum_required(VERSION 3.16)
project(progettopf VERSION 0.1.0)

# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# richiedi l'uso di C++17, senza estensioni non-standard offerte dal compilatore usato 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS
      " -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion"
      " -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast")

# abilita asserzioni di debug (in gcc), l'address sanitizer e l'undefined-behaviour sanitizer in debug mode
# string(APPEND CMAKE_CXX_FLAGS_DEBUG " -D_GLIBCXX_ASSERTIONS -fsanitize=address,undefined -fno-omit-frame-pointer")
# string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)
find_package(TGUI 1 REQUIRED)

add_executable(chess game.cpp piece.cpp board.cpp move.cpp main.cpp)

target_link_libraries(chess sfml-graphics TGUI::TGUI)
