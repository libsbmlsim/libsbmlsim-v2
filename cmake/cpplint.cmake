cmake_minimum_required(VERSION 2.8)

add_custom_target(cpplint
  COMMAND cpplint ${PROJECT_SOURCE_DIR}/src/**/*.cpp ${PROJECT_SOURCE_DIR}/include/**/*.h )