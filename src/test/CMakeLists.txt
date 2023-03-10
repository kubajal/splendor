
include_directories(..)

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

add_executable(
  tests
  tests.cpp
)

target_link_libraries(
  tests
  splendor_lib
  petri_lib
  nlohmann_json::nlohmann_json
  ${Boost_LIBRARIES}
  ${CURSES_LIBRARIES}
)

FILE(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/../../assets/
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)
gtest_discover_tests(tests)

target_link_libraries(
  tests
  GTest::gtest_main
  gmock
)

# copy pretty-printers
FILE(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/../../tools
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

# copy .gdbinit
FILE(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/../../.gdbinit
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)
