cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

file(GLOB_RECURSE LIBSBMLSIM_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# variables
set(LIBLSODA_INCLUDE_DIR ${liblsoda_SOURCE_DIR}/src)

# headers
include_directories(${LIBSBMLSIM_INCLUDE_DIR})
include_directories(${LIBSBML_INCLUDE_DIR})
include_directories(${LIBLSODA_INCLUDE_DIR})

# static library
if(NOT without-static)
  add_library(sbmlsim-static STATIC ${LIBSBMLSIM_SOURCES})
  target_link_libraries(sbmlsim-static ${LIBSBML_LIBRARIES})
  install(TARGETS sbmlsim-static
    ARCHIVE DESTINATION lib
    )
endif()

# shared library
if(NOT without-shared)
  add_library(sbmlsim SHARED ${LIBSBMLSIM_SOURCES} $<TARGET_OBJECTS:lsoda-pic-object>)
  target_link_libraries(sbmlsim ${LIBSBML_LIBRARIES})
  set_target_properties(sbmlsim PROPERTIES VERSION "${PACKAGE_VERSION}" SOVERSION "${PACKAGE_COMPAT_VERSION}")
  install(TARGETS sbmlsim
    LIBRARY DESTINATION lib
    )
endif()

