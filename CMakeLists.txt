# cmake version
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
message(STATUS "Running cmake version ${CMAKE_VERSION}")

# project information
project(libsbmlsim CXX)
set(PACKAGE_VERSION "2.0.0")
set(PACKAGE_COMPAT_VERSION "2.0")

# cmake modules
include(CMakeCXXInformation)
include(CheckCXXCompilerFlag)
include(cmake/find_libsbml.cmake)
include(cmake/cpplint.cmake)

# check C++11 availability
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()

# build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# variables
set(LIBSBMLSIM_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(LIBSBMLSIM_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIBSBMLSIM_THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty)
set(LIBSBMLSIM_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
set(LIBSBMLSIM_EXAMPLE_DIR ${PROJECT_SOURCE_DIR}/example)
set(LIBSBMLSIM_DOC_DIR ${PROJECT_SOURCE_DIR}/doc)

# headers
add_subdirectory(${LIBSBMLSIM_INCLUDE_DIR})

# sources
add_subdirectory(${LIBSBMLSIM_SOURCE_DIR})

# third-party libraries
add_subdirectory(${LIBSBMLSIM_THIRDPARTY_DIR})

# tests
if(NOT without-test)
  enable_testing()
  add_subdirectory(${LIBSBMLSIM_TEST_DIR})
endif()

# examples
if(NOT without-example)
  add_subdirectory(${LIBSBMLSIM_EXAMPLE_DIR})
endif()

# docs
if(NOT without-docs)
  add_subdirectory(${LIBSBMLSIM_DOC_DIR})
endif()

message(STATUS "Project name: ${CMAKE_PROJECT_NAME}")
