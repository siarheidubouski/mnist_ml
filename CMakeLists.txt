
cmake_minimum_required(VERSION 3.15)

project(mnist_ml VERSION 1.0.1 DESCRIPTION "Machine learning library")
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(mnist_ml SHARED src/data_handler.cc)
#add_library(project_options INTERFACE)
#target_compile_features(project_options INTERFACE cxx_std_17)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    add_compile_definitions(project_options INTERFACE -ftime-trace)
  endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" ON)
option(ENABLE_TESTING "Enable Test Builds" OFF)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if(ENABLE_PCH)
  # This sets a global PCH parameter, each project will build its own PCH, which is a good idea if any #define's change
  #
  # consider breaking this out per project as necessary
  target_precompile_headers(
    project_options
    INTERFACE
    <vector>
    <string>
    <map>
    <utility>)
endif()

# Set up some extra Conan dependencies based on our needs before loading Conan
set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")

include(cmake/Conan.cmake)
run_conan()

if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

#add_subdirectory(include)
#add_subdirectory(src)

#option(ENABLE_UNITY "Enable Unity builds of projects" OFF)
#if(ENABLE_UNITY)
  # Add for any project you want to apply unity builds for
  #  set_target_properties(intro PROPERTIES UNITY_BUILD ON)
  #endif()
