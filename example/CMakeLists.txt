cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# headers
include_directories(${LIBSBMLSIM_INCLUDE_DIR})
include_directories(${LIBSBML_INCLUDE_DIR})

add_executable(sbmlsim-example example.cpp)
target_link_libraries(sbmlsim-example sbmlsim)

add_executable(sbmlsim-testsuite-runner testsuite-runner.cpp)
target_link_libraries(sbmlsim-testsuite-runner sbmlsim)

# installation: example
install(TARGETS sbmlsim-example
  RUNTIME DESTINATION bin
  )

# installation: testsuite-runner
install(TARGETS sbmlsim-testsuite-runner
  RUNTIME DESTINATION bin
  )
