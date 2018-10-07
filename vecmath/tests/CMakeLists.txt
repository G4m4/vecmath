# @brief Build Vecmath tests executable

# preventing warnings from external source files
include_directories(
  SYSTEM
  ${GTEST_INCLUDE_DIRS}
)

include_directories(
  ${VECMATH_INCLUDE_DIR}
)

# Retrieve all vecmath headers
file(GLOB_RECURSE
    VECMATH_HDR
    ${VECMATH_INCLUDE_DIR}/vecmath/inc/*.h
)

set(VECMATH_TESTS_HDR
    tests.h
)

set(VECMATH_TESTS_SRC
    main.cc
    basics.cc
    ${VECMATH_HDR} # So it does appear in generated files
)

# Target
add_executable(vecmath_tests
  ${VECMATH_TESTS_HDR}
  ${VECMATH_TESTS_SRC}
)

set_target_mt(vecmath_tests)

# Force SSE version
add_definitions(-D SET_SSE_VERSION=3)
if(COMPILER_IS_GCC OR COMPILER_IS_CLANG)
  add_compiler_flags(vecmath_tests "-msse3")
  add_compiler_flags(vecmath_tests "-std=c++11")
else()
  # On Windows SSE3 will simply be a project flag as there is no way to force the compiler
endif()

target_link_libraries(vecmath_tests
  gtest_main
)
