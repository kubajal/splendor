cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include(GoogleTest)

find_package(nlohmann_json 3.1.0 REQUIRED)
find_package(Boost 1.60 COMPONENTS program_options REQUIRED )
find_package(Curses REQUIRED)

include_directories(${Boost_INCLUDE_DIR} )
include_directories(${CURSES_INCLUDE_DIR})

add_library(
  splendor_lib
  ui.h
  ui.cpp
  splendor.h
  splendor.cpp
)

add_executable(
  tests
  tests.cpp
  petri.h
  petri.cpp
)
target_link_libraries(
  tests
  splendor_lib
  nlohmann_json::nlohmann_json
  ${Boost_LIBRARIES}
  ${CURSES_LIBRARIES}
)

FILE(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/assets/config.json
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)
gtest_discover_tests(tests)

target_link_libraries(
  tests
  GTest::gtest_main
  gmock
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