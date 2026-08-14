find_package(GTest REQUIRED)
include(GoogleTest)

# Add each test file here as you write it.
set(NANOBOOK_TEST_SOURCES
  # test_enter.cpp
)

if(NANOBOOK_TEST_SOURCES)
  add_executable(nanobook_tests ${NANOBOOK_TEST_SOURCES})

  # Add nanobook_reference here once the engine library is uncommented in the
  # top-level CMakeLists.txt.
  target_link_libraries(nanobook_tests PRIVATE
    nanobook_core
    GTest::gtest
    GTest::gtest_main)

  # The engine builds with -Werror and the full conversion set. Test code keeps
  # the error discipline but drops the conversion warnings, which GoogleTest's
  # macros trip on their own.
  target_compile_options(nanobook_tests PRIVATE -Wall -Wextra -Werror)

  gtest_discover_tests(nanobook_tests)
else()
  message(STATUS "nanobook: no test sources yet, skipping the test target")
endif()
