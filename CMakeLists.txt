cmake_minimum_required(VERSION 3.7.2)
project(TestPlusPlus LANGUAGES CXX)

if(NOT "${CMAKE_CXX_STANDARD}")
  set(CMAKE_CXX_STANDARD 11)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
set(CMAKE_BUILD_TYPE staged)
set(CMAKE_CXX_FLAGS_STAGED "-Wall -Wextra -Wpedantic -Werror -Wnon-virtual-dtor -Wno-unused-function -Wno-unknown-pragmas -O0 -g --coverage")

file(GLOB_RECURSE sources test/*.cpp)

add_executable(test_seq ${sources})
target_include_directories(test_seq PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(test_seq PUBLIC gcov)

add_executable(test_par ${sources})
target_compile_options(test_par PUBLIC -fopenmp)
target_include_directories(test_par PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(test_par PUBLIC gcov gomp)

add_executable(rel_test_seq ${sources})
target_include_directories(rel_test_seq PUBLIC ${PROJECT_SOURCE_DIR}/release)
target_link_libraries(rel_test_seq PUBLIC gcov)

add_executable(rel_test_par ${sources})
target_compile_options(rel_test_par PUBLIC -fopenmp)
target_include_directories(rel_test_par PUBLIC ${PROJECT_SOURCE_DIR}/release)
target_link_libraries(rel_test_par PUBLIC gcov gomp)

if("${GENERATE_COMPILEDB}")
  if(NOT "${COMPILEDB_TARGET}")
    set(COMPILEDB_TARGET test_par)
  endif()
  execute_process(COMMAND compiledb --command-style -n make -C ${CMAKE_BINARY_DIR} ${COMPILEDB_TARGET})
endif()
