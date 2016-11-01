cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# headers
include_directories(${LIBSBMLSIM_INCLUDE_DIR})
include_directories(${LIBSBML_INCLUDE_DIR})

add_executable(sbmlsim-example example.cpp)
target_link_libraries(sbmlsim-example sbmlsim)

# installation: example
install(TARGETS sbmlsim-example
  RUNTIME DESTINATION bin
  )

# installation: example models
file(GLOB_RECURSE example_models "${LIBSBMLSIM_EXAMPLE_DIR}/*")
install(FILES ${example_models} DESTINATION share/sbmlsim/example/models)

