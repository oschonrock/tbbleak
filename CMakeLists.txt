cmake_minimum_required(VERSION 3.15)
project(tbbleak)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
  set(PROJECT_COMPILE_OPTIONS /W3)
  string(APPEND CMAKE_CXX_FLAGS_DEBUG " /fsanitize=address")
else() # unix'ish
  set(PROJECT_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Wshadow -Wextra-semi
    -Wmissing-noreturn -Wconversion -Wsign-conversion -Wno-ignored-attributes)
    if (NOT MINGW) # sanitizers are not working under mingw
      string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined,leak")
    endif()
endif()

add_executable(sort_leak app/sort_leak.cpp)
target_compile_features(sort_leak PRIVATE cxx_std_17)
target_compile_options(sort_leak PRIVATE ${PROJECT_COMPILE_OPTIONS})
target_link_libraries(sort_leak PRIVATE tbb)

