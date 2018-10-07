/// @file maths.h
/// @brief Vecmath common header
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

#ifndef VECMATH_INC_MATHS_H_
#define VECMATH_INC_MATHS_H_

#include "vecmath/inc/common.h"

#if _USE_SSE
#include "vecmath/inc/platform/implem_sse2.h"
namespace vecmath {
typedef SSE2VectorMath PlatformVectorMath;
}
#else  // _USE_SSE
#include "vecmath/inc/platform/implem_std.h"
namespace vecmath {
typedef StandardVectorMath PlatformVectorMath;
}
#endif  // _USE_SSE

namespace vecmath {

/// @brief Vector maths function not requiring a direct access to samples
struct CommonVectorMath {
  /// @brief "Sample" type size in bytes
  static constexpr unsigned int SampleSizeBytes = sizeof(vecmath::PlatformVectorMath::Sample);
  /// @brief "Sample" type size compared to audio samples
  static constexpr unsigned int SampleSize = sizeof(vecmath::PlatformVectorMath::Sample) / sizeof(float);

  typedef PlatformVectorMath::Sample Sample;

  /// @brief Fill a whole Sample with the given (scalar) generator
  ///
  /// @param[in]  generator   Generator to fill the Sample with
  template <typename TypeGenerator>
  static inline Sample FillWithFloatGenerator(TypeGenerator& generator) {
    const float a(generator());
    const float b(generator());
    const float c(generator());
    const float d(generator());
    return PlatformVectorMath::Fill(a, b, c, d);
  }

  /// @brief Fill a whole Sample with incremental values as follows:
  ///
  /// First value:  base
  /// ...
  /// Last value:  base + SampleSize * increment
  ///
  /// @param[in]  base    Base value to fill the first element of the Sample with
  /// @param[in]  increment    Value to add at each Sample element
  static inline Sample FillIncremental(const float base,
                                       const float increment) {
    return PlatformVectorMath::Fill(base,
                                    base + increment,
                                    base + increment * 2.0f,
                                    base + increment * 3.0f);
  }

  /// @brief Fill a whole Sample based on its length
  ///
  /// The actual value is SampleSize * given value
  ///
  /// @param[in]  base    Base value to be filled with
  static inline Sample FillOnLength(const float base) {
    return PlatformVectorMath::Fill(base * vecmath::SampleSize);
  }

  /// @brief Extract first element from a Sample
  ///
  /// @param[in]  input   Sample to be read
  static inline float GetFirst(SampleRead input) {
    return PlatformVectorMath::GetByIndex<0>(input);
  }

  /// @brief Extract last element from a Sample
  ///
  /// @param[in]  input   Sample to be read
  static inline float GetLast(SampleRead input) {
    return PlatformVectorMath::GetByIndex<3>(input);
  }

  /// @brief Helper function: limit input into [min ; max]
  static inline Sample Clamp(SampleRead input,
                             const SampleRead min,
                             const SampleRead max) {
    return PlatformVectorMath::Min(PlatformVectorMath::Max(input, min), max);
  }

  /// @brief Multiply a Sample by a scalar constant
  ///
  /// @param[in]  constant   Scalar constant to multiply the Sample by
  /// @param[in]  input   Sample to be multiplied
  static inline Sample MulConst(const float constant, SampleRead input) {
    return PlatformVectorMath::Mul(PlatformVectorMath::Fill(constant), input);
  }

  /// @brief Normalize the input based on actual Sample length
  ///
  /// @param[in]  input    Value to be normalized
  static inline Sample Normalize(SampleRead input) {
    // Note: division deliberately avoided
    return MulConst(0.25f, input);
  }

  /// @brief Return the absolute value of each element of the Sample
  static inline Sample Abs(SampleRead input) {
    return PlatformVectorMath::Max(
      PlatformVectorMath::Sub(PlatformVectorMath::Fill(0.0f), input),
      input);
  }

  static inline bool Equal(SampleRead threshold, SampleRead input) {
    const Sample test_result(PlatformVectorMath::Equal(threshold, input));
    return PlatformVectorMath::IsMaskFull(test_result);
  }

  static inline bool Equal(float threshold, SampleRead input) {
    return Equal(PlatformVectorMath::Fill(threshold), input);
  }

  /// @brief Helper binary function:
  /// true if both input are closer than the given threshold
  ///
  /// @param[in]  left   First Sample
  /// @param[in]  right   Second Sample
  /// @param[in]  threshold   Threshold below which samples are considered close
  static inline bool IsNear(SampleRead left,
                            SampleRead right,
                            const float threshold) {
    const Sample abs_diff(Abs(PlatformVectorMath::Sub(left, right)));
    return PlatformVectorMath::GreaterEqual(threshold, abs_diff);
  }

  /// @brief Helper binary function:
  /// true if any input elements are closer than the given threshold
  ///
  /// @param[in]  left   First Sample
  /// @param[in]  right   Second Sample
  /// @param[in]  threshold   Threshold below which samples are considered close
  static inline bool IsAnyNear(SampleRead left,
                               SampleRead right,
                               const float threshold) {
    const Sample abs_diff(Abs(PlatformVectorMath::Sub(left, right)));
    return PlatformVectorMath::GreaterEqualAny(threshold, abs_diff);
  }
};

}  // namespace vecmath

#endif  // VECMATH_INC_MATHS_H_
