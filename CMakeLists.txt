cmake_minimum_required(VERSION 3.20)
project(SmartLibraryManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(smart_library
    src/main.cpp
    src/Book.cpp
    src/User.cpp
    src/BookHashTable.cpp
    src/BookInventory.cpp
    src/UserLinkedList.cpp
    src/LibrarySystem.cpp
)

target_include_directories(smart_library PRIVATE src)

if(MSVC)
    target_compile_options(smart_library PRIVATE /W4 /permissive-)
else()
    target_compile_options(smart_library PRIVATE -Wall -Wextra -Wpedantic)
endif()
