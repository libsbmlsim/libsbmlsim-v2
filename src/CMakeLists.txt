cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

file(GLOB_RECURSE LIBSBMLSIM_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# headers
include_directories(${LIBSBMLSIM_INCLUDE_DIR})
include_directories(${LIBSBML_INCLUDE_DIR})

# static library
add_library(sbmlsim-static STATIC ${LIBSBMLSIM_SOURCES})
target_link_libraries(sbmlsim-static ${LIBSBML_LIBRARIES})
#target_link_libraries(sbmlsim-static lsoda)

# shared library
add_library(sbmlsim SHARED ${LIBSBMLSIM_SOURCES})
target_link_libraries(sbmlsim ${LIBSBML_LIBRARIES})
#target_link_libraries(sbmlsim lsoda)
set_target_properties(sbmlsim PROPERTIES VERSION "${PACKAGE_VERSION}" SOVERSION "${PACKAGE_COMPAT_VERSION}")

# installation: libs
install(TARGETS sbmlsim-static sbmlsim
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )

