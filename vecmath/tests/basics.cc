/// @file tests/basics.cc
/// @brief Vecmath tests
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

#include "vecmath/tests/tests.h"

TEST(Parity, FillOne) {
  const float random_scalar = kNormDistribution(kRandomGenerator);
  const StdFloatVec std_fill = StandardVectorMath::Fill(random_scalar);
  const SSE2FloatVec sse2_fill = SSE2VectorMath::Fill(random_scalar);
  EXPECT_EQ_SAMPLES(std_fill, sse2_fill);
}

TEST(Parity, Fill) {
  const float random_scalar_0 = kNormDistribution(kRandomGenerator);
  const float random_scalar_1 = kNormDistribution(kRandomGenerator);
  const float random_scalar_2 = kNormDistribution(kRandomGenerator);
  const float random_scalar_3 = kNormDistribution(kRandomGenerator);
  const StdFloatVec std_fill = StandardVectorMath::Fill(random_scalar_0,
                                                      random_scalar_1,
                                                      random_scalar_2,
                                                      random_scalar_3);
  const SSE2FloatVec sse2_fill = SSE2VectorMath::Fill(random_scalar_0,
                                                    random_scalar_1,
                                                    random_scalar_2,
                                                    random_scalar_3);
  EXPECT_EQ_SAMPLES(std_fill, sse2_fill);
}
