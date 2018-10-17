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

#if _VEC_USE_SSE
#include "vecmath/inc/platform/implem_sse2.h"
namespace vecmath {
typedef SSE2VectorMath PlatformVectorMath;
}
#else  // _VEC_USE_SSE
#include "vecmath/inc/platform/implem_std.h"
namespace vecmath {
typedef StandardVectorMath PlatformVectorMath;
}
#endif  // _VEC_USE_SSE

namespace vecmath {

/// @brief Vector maths function not requiring a direct access to samples
struct CommonVectorMath {
  /// @brief "FloatVec" type size in bytes
  static constexpr unsigned int FloatVecSizeBytes = sizeof(vecmath::PlatformVectorMath::FloatVec);
  /// @brief "FloatVec" type size compared to audio samples
  static constexpr unsigned int FloatVecSize = sizeof(vecmath::PlatformVectorMath::FloatVec) / sizeof(float);

  typedef PlatformVectorMath::FloatVec FloatVec;
  typedef PlatformVectorMath::FloatVec FloatVecRead;

  /// @brief Fill a whole FloatVec with the given (scalar) generator
  ///
  /// @param[in]  generator   Generator to fill the FloatVec with
  template <typename TypeGenerator>
  static inline FloatVec FillWithFloatGenerator(TypeGenerator& generator) {
    const float a(generator());
    const float b(generator());
    const float c(generator());
    const float d(generator());
    return PlatformVectorMath::Fill(a, b, c, d);
  }

  /// @brief Fill a whole FloatVec with incremental values as follows:
  ///
  /// First value:  base
  /// ...
  /// Last value:  base + FloatVecSize * increment
  ///
  /// @param[in]  base    Base value to fill the first element of the FloatVec with
  /// @param[in]  increment    Value to add at each FloatVec element
  static inline FloatVec FillIncremental(const float base,
                                         const float increment) {
    return PlatformVectorMath::Fill(base,
                                    base + increment,
                                    base + increment * 2.0f,
                                    base + increment * 3.0f);
  }

  /// @brief Fill a whole FloatVec based on its length
  ///
  /// The actual value is FloatVecSize * given value
  ///
  /// @param[in]  base    Base value to be filled with
  static inline FloatVec FillOnLength(const float base) {
    return PlatformVectorMath::Fill(base * FloatVecSize);
  }

  /// @brief Extract first element from a FloatVec
  ///
  /// @param[in]  input   FloatVec to be read
  static inline float GetFirst(FloatVecRead input) {
    return PlatformVectorMath::GetByIndex<0>(input);
  }

  /// @brief Extract last element from a FloatVec
  ///
  /// @param[in]  input   FloatVec to be read
  static inline float GetLast(FloatVecRead input) {
    return PlatformVectorMath::GetByIndex<3>(input);
  }

  /// @brief Helper function: limit input into [min ; max]
  static inline FloatVec Clamp(FloatVecRead input,
                               const FloatVecRead min,
                               const FloatVecRead max) {
    return PlatformVectorMath::Min(PlatformVectorMath::Max(input, min), max);
  }

  /// @brief Multiply a FloatVec by a scalar constant
  ///
  /// @param[in]  constant   Scalar constant to multiply the FloatVec by
  /// @param[in]  input   FloatVec to be multiplied
  static inline FloatVec MulConst(const float constant, FloatVecRead input) {
    return PlatformVectorMath::Mul(PlatformVectorMath::Fill(constant), input);
  }

  /// @brief Normalize the input based on actual FloatVec length
  ///
  /// @param[in]  input    Value to be normalized
  static inline FloatVec Normalize(FloatVecRead input) {
    // Note: division deliberately avoided
    return MulConst(0.25f, input);
  }

  /// @brief Return the absolute value of each element of the FloatVec
  static inline FloatVec Abs(FloatVecRead input) {
    return PlatformVectorMath::Max(
      PlatformVectorMath::Sub(PlatformVectorMath::Fill(0.0f), input),
      input);
  }

  static inline bool Equal(FloatVecRead threshold, FloatVecRead input) {
    const FloatVec test_result(PlatformVectorMath::Equal(threshold, input));
    return PlatformVectorMath::IsMaskFull(test_result);
  }

  static inline bool Equal(float threshold, FloatVecRead input) {
    return Equal(PlatformVectorMath::Fill(threshold), input);
  }

  /// @brief Helper binary function:
  /// true if both input are closer than the given threshold
  ///
  /// @param[in]  left   First FloatVec
  /// @param[in]  right   Second FloatVec
  /// @param[in]  threshold   Threshold below which samples are considered close
  static inline bool IsNear(FloatVecRead left,
                            FloatVecRead right,
                            const float threshold) {
    const FloatVec abs_diff(Abs(PlatformVectorMath::Sub(left, right)));
    return PlatformVectorMath::GreaterEqual(threshold, abs_diff);
  }

  /// @brief Helper binary function:
  /// true if any input elements are closer than the given threshold
  ///
  /// @param[in]  left   First FloatVec
  /// @param[in]  right   Second FloatVec
  /// @param[in]  threshold   Threshold below which samples are considered close
  static inline bool IsAnyNear(FloatVecRead left,
                               FloatVecRead right,
                               const float threshold) {
    const FloatVec abs_diff(Abs(PlatformVectorMath::Sub(left, right)));
    return PlatformVectorMath::GreaterEqualAny(threshold, abs_diff);
  }
};

}  // namespace vecmath

#endif  // VECMATH_INC_MATHS_H_
