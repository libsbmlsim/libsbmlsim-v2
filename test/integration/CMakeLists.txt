cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(libsbmlsim-integrationtest)

# headers
include_directories(${LIBSBMLSIM_INCLUDE_DIR})
include_directories(${LIBSBML_INCLUDE_DIR})
include_directories(${gtest_SOURCE_DIR}/include)

