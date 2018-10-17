/// @file implem_sse2.h
/// @brief Maths header - SSE2 implementation
/// @author gm
/// @copyright gm 2017
///
/// This file is part of VecMath
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

#ifndef VECMATH_INC_PLATFORM_IMPLEM_SSE2_H_
#define VECMATH_INC_PLATFORM_IMPLEM_SSE2_H_

#include <math.h>

#include "vecmath/inc/common.h"

#if _VEC_USE_SSE

extern "C" {
#include <emmintrin.h>
#include <mmintrin.h>
}

namespace vecmath {

struct SSE2VectorMath {
  /// @brief "FloatVec" type - actually, this is the data computed at each "tick";
  /// If using vectorization it will be longer than 1 audio sample
  typedef __m128 FloatVec;
  typedef __m128i IntVec;

  /// @brief Type for FloatVec parameter "read only":
  /// It should be passed by value since it allows to keep it into a register,
  /// instead of passing its address and loading it.
  typedef const FloatVec FloatVecRead;

  /// @brief "FloatVec" type size in bytes
  static constexpr unsigned int FloatVecSizeBytes = sizeof(FloatVec);
  /// @brief "FloatVec" type size compared to audio samples
  static constexpr unsigned int FloatVecSize = sizeof(FloatVec) / sizeof(float);

  /// @brief Fill a whole FloatVec with the given value
  ///
  /// @param[in]  value   Value to be copied through the whole FloatVec
  static inline FloatVec Fill(const float value) {
    return _mm_set1_ps(value);
  }

  /// @brief Fill a whole FloatVec with the given float array
  ///
  /// @param[in]  value   Pointer to the float array to be used:
  ///                     must be FloatVecSizeBytes long
  static inline FloatVec Fill(const float* value) {
    return _mm_load_ps(value);
  }

  /// @brief Fill a whole FloatVec with all given scalars,
  /// beware of the order: SSE is "little-endian" (sort of)
  ///
  /// @param[in]  a   Last value
  /// @param[in]  b   Second to last value
  /// @param[in]  c   Second value
  /// @param[in]  d   First value
  static inline FloatVec Fill(const float a,
                            const float b,
                            const float c,
                            const float d) {
    return _mm_set_ps(d, c, b, a);
  }

  /// @brief Helper union for vectorized type to scalar array conversion
  typedef union {
    FloatVec sample_v;  ///< Vectorized type
    float sample[FloatVecSize];  ///< Array of scalars
  } ConverterFloatScalarVector;
  /// @brief Helper union for vectorized integer type to scalar array conversion
  typedef union {
    IntVec sample_v;  ///< Vectorized type
    int sample[FloatVecSize];  ///< Array of scalars
  } ConverterIntScalarVector;

  /// @brief Extract one element from a FloatVec (compile-time version)
  ///
  /// @param[in]  input   FloatVec to be read
  // TODO(gm): faster _mm_store_ss specialization
  template<unsigned i>
  static float GetByIndex(FloatVecRead input) {
    ConverterFloatScalarVector converter;
    converter.sample_v = input;
    return converter.sample[i];
  }

  /// @brief Integer version of the above
  template<unsigned i>
  static int GetByIndex(IntVec input) {
    ConverterIntScalarVector converter;
    converter.sample_v = input;
    return converter.sample[i];
  }

  /// @brief Extract one element from a FloatVec (runtime version, in loops)
  ///
  /// @param[in]  input   FloatVec to be read
  /// @param[in]  i   Index of the element to retrieve
  static inline float GetByIndex(FloatVecRead input, const unsigned i) {
    VECMATH_ASSERT(i < FloatVecSize);
    ConverterFloatScalarVector converter;
    converter.sample_v = input;
    return converter.sample[i];
  }

  /// @brief Add "left" to "right"
  static inline FloatVec Add(FloatVecRead left, FloatVecRead right) {
    return _mm_add_ps(left, right);
  }

  /// @brief Sum all elements of a FloatVec
  static inline float AddHorizontal(FloatVecRead input) {
    const FloatVec first_add(Add(input, _mm_movehl_ps(input, input)));
    const FloatVec shuffled(_mm_shuffle_ps(first_add, first_add,
                          _MM_SHUFFLE(0, 1, 0, 1)));
    return GetByIndex<0>(Add(shuffled, first_add));
  }

  /// @brief Substract "right" from "left"
  static inline FloatVec Sub(FloatVecRead left, FloatVecRead right) {
    return _mm_sub_ps(left, right);
  }

  /// @brief Element-wise multiplication
  static inline FloatVec Mul(FloatVecRead left, FloatVecRead right) {
    return _mm_mul_ps(left, right);
  }

  /// @brief Shift to right all elements of the input by 1,
  /// and shift in the given value
  ///
  /// E.g. given (x_{n}, x_{n + 1}, x_{n + 2}, x_{n + 3})
  /// return (value, x_{n}, x_{n + 1}, x_{n + 2})
  ///
  /// @param[in]  input   FloatVec to be shifted
  /// @param[in]  value   value to be shifted in
  static inline FloatVec RotateOnRight(FloatVecRead input,
                                     const float value) {
    // beware of the order: SSE is "little-endian" (sort of)
    const FloatVec rotated(_mm_castsi128_ps(
      _mm_slli_si128(_mm_castps_si128(input), 4)));
    return Add(Fill(value, 0.0f, 0.0f, 0.0f), rotated);
  }

  /// @brief Shift to left all elements of the input by 1,
  /// and shift in the given value
  ///
  /// E.g. given (x_{n}, x_{n + 1}, x_{n + 2}, x_{n + 3})
  /// return (x_{n + 1}, x_{n + 2}, x_{n + 3}, value)
  ///
  /// @param[in]  input   FloatVec to be shifted
  /// @param[in]  value   value to be shifted in
  static inline FloatVec RotateOnLeft(FloatVecRead input,
                                    const float value) {
    // beware of the order: SSE is "little-endian" (sort of)
    const FloatVec rotated(_mm_castsi128_ps(
      _mm_srli_si128(_mm_castps_si128(input), 4)));
    return Add(Fill(0.0f, 0.0f, 0.0f, value), rotated);
  }

  /// @brief Return the sign of each element of the FloatVec
  ///
  /// Sgn(0.0) return 0.0
  static inline FloatVec Sgn(FloatVecRead input) {
    const FloatVec kZero(_mm_setzero_ps());
    const FloatVec kOne(Fill(1.0f));
    const FloatVec kMinus(Fill(-1.0f));
    const FloatVec kPlusMask(_mm_and_ps(_mm_cmpgt_ps(input, kZero), kOne));
    const FloatVec kMinusMask(_mm_and_ps(_mm_cmplt_ps(input, kZero), kMinus));
    return Add(kPlusMask, kMinusMask);
  }

  /// @brief Return the sign of each element of the FloatVec, no zero version
  ///
  /// Sgn(0.0) return 1.0f
  static inline FloatVec SgnNoZero(FloatVecRead value) {
    const FloatVec kZero(_mm_setzero_ps());
    const FloatVec kOne(Fill(1.0f));
    const FloatVec kMinus(Fill(-1.0f));
    const FloatVec kPlusMask(_mm_and_ps(_mm_cmpge_ps(value, kZero), kOne));
    const FloatVec kMinusMask(_mm_and_ps(_mm_cmplt_ps(value, kZero), kMinus));
    return Add(kPlusMask, kMinusMask);
  }

  /// @brief Store the given FloatVec into memory
  ///
  /// @param[in]  buffer   Memory to be filled with the input
  /// @param[in]  input   Sampel to be stored
  static inline void Store(float* const buffer, FloatVecRead input) {
    _mm_store_ps(buffer, input);
  }

  static inline void StoreUnaligned(float* const buffer, FloatVecRead input) {
    _mm_storeu_ps(buffer, input);
  }

  /// @brief Get each right half of the two given vectors
  ///
  /// Given left = (x0, x1, x2, x3) and right = (y0, y1, y2, y3)
  /// it will return (x2, x3, y2, y3)
  static inline FloatVec TakeEachRightHalf(FloatVecRead left,
                                         FloatVecRead right) {
    // beware of the order: SSE is "little-endian" (sort of)
    return _mm_shuffle_ps(left, right, _MM_SHUFFLE(3, 2, 3, 2));
  }

  /// @brief Revert the given vector values order
  ///
  /// Given value = (x0, x1, x2, x3)
  /// it will return (x3, x2, x1, x0)
  static inline FloatVec Revert(FloatVecRead value) {
    return _mm_shuffle_ps(value, value, _MM_SHUFFLE(0, 1, 2, 3));
  }

  /// @brief Return each min element of both inputs
  static inline FloatVec Min(FloatVecRead left, FloatVecRead right) {
    return _mm_min_ps(left, right);
  }

  /// @brief Return each max element of both inputs
  static inline FloatVec Max(FloatVecRead left, FloatVecRead right) {
#if (_VEC_USE_SSE)
    return _mm_max_ps(left, right);
#else
    return std::max(left, right);
#endif  // (_VEC_USE_SSE)
  }

  /// @brief Round each FloatVec element to the nearest integer
  static inline FloatVec Round(FloatVecRead input) {
#if (_VEC_USE_SSE)
    const FloatVec kZero(_mm_setzero_ps());
    const FloatVec kPlus(Fill(0.5f));
    const FloatVec kMinus(Fill(-0.5f));
    const FloatVec kPlusMask(_mm_and_ps(_mm_cmpge_ps(input, kZero), kPlus));
    const FloatVec kMinusMask(_mm_and_ps(_mm_cmplt_ps(input, kZero), kMinus));
    const FloatVec kAddMask(Add(kPlusMask, kMinusMask));
    return Add(kAddMask, input);
#else
    return (input > 0.0f) ? (input + 0.5f) : (input - 0.5f);
#endif  // (_VEC_USE_SSE)
  }

  /// @brief Helper function: increment the input and wraps it into [-1.0 ; 1.0[
  ///
  /// @param[in]  input         Input to be wrapped - supposed not to be < 1.0
  /// @param[in]  increment     Increment to add to the input
  /// @return the incremented output in [-1.0 ; 1.0[
  static inline FloatVec IncrementAndWrap(FloatVecRead input, FloatVecRead increment) {
    // TODO(gm): check if a common code path can be found
#if (_VEC_USE_SSE)
    const FloatVec output(Add(input, increment));
    const FloatVec constant(Fill(-2.0f));
    const FloatVec threshold(Fill(1.0f));
    const FloatVec addition_mask(_mm_cmpgt_ps(output, threshold));
    const FloatVec add(_mm_and_ps(addition_mask, constant));
    return Add(output, add);
#else
    FloatVec output(Add(input, increment));
    if (output > 1.0f) {
      const float constant(-2.0f);
      output = Add(output, constant);
    }
    return output;
#endif  // (_VEC_USE_SSE)
  }

  /// @brief Helper binary function: return true if all input elements are true
  ///
  /// @param[in]  input   Input to be checked
  static inline bool IsMaskFull(FloatVecRead input) {
    return 15 == _mm_movemask_ps(input);
  }

  /// @brief Helper binary function: return true if all input elements are null
  ///
  /// @param[in]  input   Input to be checked
  static inline bool IsMaskNull(FloatVecRead input) {
    return 0 == _mm_movemask_ps(input);
  }

  static inline FloatVec GreaterEqual(FloatVecRead threshold, FloatVecRead input) {
    return _mm_cmpge_ps(threshold, input);
  }

  static inline FloatVec GreaterThan(FloatVecRead threshold, FloatVecRead input) {
    return _mm_cmpgt_ps(threshold, input);
  }

  /// @brief Helper binary function:
  /// true if each threshold element is >= than the input element
  static inline bool GreaterEqual(float threshold, FloatVecRead input) {
    const FloatVec test_result(GreaterEqual(Fill(threshold), input));
    return IsMaskFull(test_result);
  }

  static inline bool GreaterEqualAny(float threshold, FloatVecRead input) {
    const FloatVec test_result(GreaterEqual(Fill(threshold), input));
    return !IsMaskNull(test_result);
  }

  static inline bool GreaterThan(float threshold, FloatVecRead input) {
    const FloatVec test_result(GreaterThan(Fill(threshold), input));
    return IsMaskFull(test_result);
  }

  static inline FloatVec LessEqual(FloatVecRead threshold, FloatVecRead input) {
    return _mm_cmple_ps(threshold, input);
  }

  static inline FloatVec LessThan(FloatVecRead threshold, FloatVecRead input) {
    return _mm_cmplt_ps(threshold, input);
  }

  /// @brief Helper binary function:
  /// true if each threshold element is <= than the input element
  static inline bool LessEqual(float threshold, FloatVecRead input) {
    const FloatVec test_result(LessEqual(Fill(threshold), input));
    return IsMaskFull(test_result);
  }

  static inline bool LessThan(float threshold, FloatVecRead input) {
    const FloatVec test_result(LessThan(Fill(threshold), input));
    return IsMaskFull(test_result);
  }

  static inline FloatVec Equal(FloatVecRead threshold, FloatVecRead value) {
    return _mm_cmpeq_ps(threshold, value);
  }

  static inline bool Equal(float threshold, FloatVecRead input) {
    const FloatVec test_result(Equal(Fill(threshold), input));
    return IsMaskFull(test_result);
  }

  /// @brief Beware, not an actual bitwise AND! More like a "float select"
  static inline FloatVec ExtractValueFromMask(FloatVecRead value, FloatVecRead mask) {
    return _mm_and_ps(value, mask);
  }

  static inline IntVec TruncToInt(FloatVecRead float_value) {
    return _mm_cvttps_epi32(float_value);
  }
};

}  // namespace vecmath

#endif  // _VEC_USE_SSE

#endif  // VECMATH_SRC_PLATFORM_IMPLEM_SSE2_H_
