cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(libsbmlsim-integrationtest)

# boost
set(BOOST_USE_STATIC_LIBS OFF) 
set(BOOST_USE_MULTITHREADED ON)  
set(BOOST_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 COMPONENTS program_options) 

# python
find_package(PythonInterp 2.6)

# headers
include_directories(${LIBSBMLSIM_INCLUDE_DIR})
include_directories(${LIBSBML_INCLUDE_DIR})
include_directories(${gtest_SOURCE_DIR}/include)

if(BOOST_FOUND)
  include_directories(${Boost_INCLUDE_DIRS}) 
  add_executable(testintegrator testintegrator.cpp)
  target_link_libraries(testintegrator sbmlsim ${Boost_LIBRARIES})
endif()

# test: testintegrator
if (PYTHONINTERP_FOUND)
  #add_test(
  #NAME TestIntegrator1
  #COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/batchTest.py 1-1
  #)
add_test(
  NAME TestIntegrator6
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/batchTest.py 6-6
  )
endif()

# installation: testintegrator
install(TARGETS testintegrator
  RUNTIME DESTINATION bin
  )

