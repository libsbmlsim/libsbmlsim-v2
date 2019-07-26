cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(libsbmlsim-integrationtest)

# boost
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 COMPONENTS program_options) 

# python
find_package(PythonInterp 2.6)

# headers
include_directories(${LIBSBMLSIM_INCLUDE_DIR})
include_directories(${LIBSBML_INCLUDE_DIR})
include_directories(${gtest_SOURCE_DIR}/include)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS}) 
  add_executable(testintegrator testintegrator.cpp)
  target_link_libraries(testintegrator sbmlsim ${Boost_LIBRARIES})
endif()

# test: testintegrator
if (PYTHONINTERP_FOUND)
  add_test(
    NAME IntegrationTest
    COMMAND ${PYTHON_EXECUTABLE} $<TARGET_FILE_DIR:testintegrator>/batchTest.py 1-25
    )

  if(FALSE)
  foreach(case_id RANGE 1 25)
    add_test(
      NAME IntegrationTestCase${case_id}
      COMMAND ${PYTHON_EXECUTABLE} $<TARGET_FILE_DIR:testintegrator>/batchTest.py ${case_id}-${case_id}
      )
  endforeach()
  endif()
endif()

# installation: testintegrator
install(TARGETS testintegrator
  RUNTIME DESTINATION bin
  )

# copy the files related to integration-test and build testsuite
# before the build of testintegrator
add_custom_command(TARGET testintegrator PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_CURRENT_SOURCE_DIR}/testsuite $<TARGET_FILE_DIR:testintegrator>/testsuite
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/batchTest.py $<TARGET_FILE_DIR:testintegrator>
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/wrapper.py $<TARGET_FILE_DIR:testintegrator>
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/batchTestPrintFailed.sh $<TARGET_FILE_DIR:testintegrator>
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/mytable.css $<TARGET_FILE_DIR:testintegrator>
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/parseModel.pl $<TARGET_FILE_DIR:testintegrator>
  COMMAND cd $<TARGET_FILE_DIR:testintegrator>/testsuite/src/test-runner/testsuite-core && ant jar
  COMMENT "Building testsuite"
  )
