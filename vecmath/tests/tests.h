/// @file tests.h
/// @brief Tests common include file
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

#ifndef VECMATH_TESTS_TESTS_H_
#define VECMATH_TESTS_TESTS_H_

// std::generate
#include <algorithm>
#include <random>
// std::memcmp
#include <string>

#include "gtest/gtest.h"

#include "vecmath/inc/common.h"

#include "vecmath/inc/platform/implem_std.h"
#include "vecmath/inc/platform/implem_sse2.h"

using vecmath::StandardVectorMath;
using vecmath::SSE2VectorMath;

typedef vecmath::StandardVectorMath::Sample StdSample;
typedef vecmath::SSE2VectorMath::Sample SSE2Sample;

// Common base random distributions
static std::uniform_real_distribution<float> kNormDistribution(-1.0f, 1.0f);
static std::uniform_real_distribution<float> kNormPosDistribution(0.0f, 1.0f);
static std::bernoulli_distribution kBoolDistribution;

static std::default_random_engine kRandomGenerator;

// For tests only
void EXPECT_EQ_SAMPLES(const StdSample & lhs, const SSE2Sample & rhs) {
  EXPECT_EQ(vecmath::StandardVectorMath::GetByIndex<0>(lhs), vecmath::SSE2VectorMath::GetByIndex<0>(rhs));
  EXPECT_EQ(vecmath::StandardVectorMath::GetByIndex<1>(lhs), vecmath::SSE2VectorMath::GetByIndex<1>(rhs));
  EXPECT_EQ(vecmath::StandardVectorMath::GetByIndex<2>(lhs), vecmath::SSE2VectorMath::GetByIndex<2>(rhs));
  EXPECT_EQ(vecmath::StandardVectorMath::GetByIndex<3>(lhs), vecmath::SSE2VectorMath::GetByIndex<3>(rhs));
}

void EXPECT_EQ_SAMPLES(const SSE2Sample & lhs, const StdSample & rhs) {
  EXPECT_EQ(vecmath::SSE2VectorMath::GetByIndex<0>(lhs), vecmath::StandardVectorMath::GetByIndex<0>(rhs));
  EXPECT_EQ(vecmath::SSE2VectorMath::GetByIndex<1>(lhs), vecmath::StandardVectorMath::GetByIndex<1>(rhs));
  EXPECT_EQ(vecmath::SSE2VectorMath::GetByIndex<2>(lhs), vecmath::StandardVectorMath::GetByIndex<2>(rhs));
  EXPECT_EQ(vecmath::SSE2VectorMath::GetByIndex<3>(lhs), vecmath::StandardVectorMath::GetByIndex<3>(rhs));
}

#endif  // VECMATH_TESTS_TESTS_H_
