cmake_minimum_required (VERSION 3.0)
project (MiniDbg)

add_compile_options(-std=c++14)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(deps/libelfin deps/linenoise include)
add_executable(minidbg src/minidbg.cpp deps/linenoise/linenoise.c)

add_executable(hello examples/hello.cpp)
set_target_properties(hello
                      PROPERTIES COMPILE_FLAGS "-g -O0")

add_executable(variable examples/variable.cpp)
set_target_properties(variable
                      PROPERTIES COMPILE_FLAGS "-gdwarf-2 -O0")

add_executable(unwinding examples/stack_unwinding.cpp)
set_target_properties(unwinding
                      PROPERTIES COMPILE_FLAGS "-g -O0")


add_custom_target(
   libelfin
   COMMAND make
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/libelfin
)
target_link_libraries(minidbg
                      ${PROJECT_SOURCE_DIR}/deps/libelfin/dwarf/libdwarf++.so
                      ${PROJECT_SOURCE_DIR}/deps/libelfin/elf/libelf++.so)
add_dependencies(minidbg libelfin)
