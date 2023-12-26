cmake_minimum_required(VERSION 3.15)
project(pale-noel)
set(CMAKE_CXX_STANDARD 14)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

set(CMAKE_TOOLCHAIN_FILE "C:/dev/scripts/buildsystems/vcpkg.cmake")

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES ./pale-noel.cc ./src)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(pale-noel ${SOURCE_FILES})
