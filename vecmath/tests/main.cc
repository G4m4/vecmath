/// @file tests/main.cc
/// @brief Tests main entry point
/// @author gm
/// @copyright gm 2017
///
/// This file is part of Vecmath
///
///            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
///                    Version 2, December 2004
///
/// Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
///
/// Everyone is permitted to copy and distribute verbatim or modified
/// copies of this license document, and changing it is allowed as long
/// as the name is changed.
///
///            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
///   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
///
///  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "gtest/gtest.h"

/// @brief Main function, of course.
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);

  const int kRetValue(RUN_ALL_TESTS());

  return kRetValue;
}
