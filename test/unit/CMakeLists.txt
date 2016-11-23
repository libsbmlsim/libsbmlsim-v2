cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(libsbmlsim-unittest)

# headers
include_directories(${LIBSBMLSIM_INCLUDE_DIR})
include_directories(${LIBSBML_INCLUDE_DIR})
include_directories(${gtest_SOURCE_DIR}/include)

# test: SBMLSim
add_executable(SBMLSimTest SBMLSimTest.cpp)
target_link_libraries(SBMLSimTest gtest_main sbmlsim)
add_test(
  NAME SBMLSimTest
  COMMAND $<TARGET_FILE:SBMLSimTest>
  )

# test: MathUtil
add_executable(MathUtilTest MathUtilTest.cpp)
target_link_libraries(MathUtilTest gtest_main sbmlsim)
add_test(
        NAME MathUtilTest
        COMMAND $<TARGET_FILE:MathUtilTest>
)

