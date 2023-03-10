cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 23)

find_package(nlohmann_json 3.1.0 REQUIRED)
find_package(Boost 1.60 COMPONENTS program_options REQUIRED )
find_package(Curses REQUIRED)

include_directories(${Boost_INCLUDE_DIR} )
include_directories(${CURSES_INCLUDE_DIR})

add_subdirectory(src/test)

add_library(
  splendor_lib
  src/ui.h
  src/ui.cpp
  src/splendor.h
  src/splendor.cpp
)

add_library(
  petri_lib
  src/petri.h
  src/petri.cpp
)

FILE(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/assets/config.json
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_executable(
  splendor
  main.cpp
)
target_link_libraries(
  splendor
  splendor_lib
  nlohmann_json::nlohmann_json
  ${Boost_LIBRARIES}
  ${CURSES_LIBRARIES}
)