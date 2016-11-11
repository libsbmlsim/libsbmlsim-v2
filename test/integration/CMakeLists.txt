cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(libsbmlsim-integrationtest)

# boost
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 COMPONENTS program_options) 

# headers
include_directories(${LIBSBMLSIM_INCLUDE_DIR})
include_directories(${LIBSBML_INCLUDE_DIR})
include_directories(${gtest_SOURCE_DIR}/include)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS}) 
  add_executable(testintegrator testintegrator.cpp)
  target_link_libraries(testintegrator sbmlsim ${Boost_LIBRARIES})
endif()


# installation: example
install(TARGETS testintegrator
  RUNTIME DESTINATION bin
  )

