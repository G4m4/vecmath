/// @file implem_std.h
/// @brief VecMath maths header - standard implementation
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

#ifndef VECMATH_INC_PLATFORM_IMPLEM_STD_H_
#define VECMATH_INC_PLATFORM_IMPLEM_STD_H_

#include <cmath>
#include <cstring>

#include "vecmath/inc/common.h"

namespace vecmath {

struct StandardVectorMath {

  /// @brief "FloatVec" type - actually, this is the data computed at each "tick";
  /// If using vectorization it will be longer than 1 audio sample
  struct FloatVec { float data_[4]; };
  struct IntVec { int data_[4]; };

  /// @brief Type for FloatVec parameter "read only":
  /// It should be passed by value since it allows to keep it into a register,
  /// instead of passing its address and loading it.
  typedef const FloatVec FloatVecRead;

  /// @brief "FloatVec" type size in bytes
  static constexpr unsigned int FloatVecSizeBytes = sizeof(FloatVec);
  /// @brief "FloatVec" type size compared to audio samples
  static constexpr unsigned int FloatVecSize = sizeof(FloatVec) / sizeof(float);

  /// @brief Fill a whole FloatVec with all given scalars
  ///
  /// @param[in]  a   Last value
  /// @param[in]  b   Second to last value
  /// @param[in]  c   Second value
  /// @param[in]  d   First value
  static inline FloatVec Fill(float a,
                            float b,
                            float c,
                            float d) {
    return {{ a, b, c, d }};
  }

  /// @brief Fill a whole int vector with all given integers
  static inline IntVec Fill(int a,
                            int b,
                            int c,
                            int d) {
    return {{ a, b, c, d }};
  }

  /// @brief Fill a whole FloatVec with the given value
  ///
  /// @param[in]  value   Value to be copied through the whole FloatVec
  static inline FloatVec Fill(float value) {
    return Fill( value, value, value, value );
  }

  /// @brief Fill a whole FloatVec with the given float array
  /// beware of the order: SSE is "little-endian" (sort of)
  ///
  /// @param[in]  value   Pointer to the float array to be used:
  ///                     must be FloatVecSizeBytes long
  static inline FloatVec Fill(BlockIn value) {
    return Fill( value[0], value[1], value[2], value[3] );
  }

  /// @brief Extract one element from a FloatVec (compile-time version)
  ///
  /// @param[in]  input   FloatVec to be read
  // TODO(gm): faster _mm_store_ss specialization
  template<unsigned i>
  static float GetByIndex(FloatVecRead input) {
    return input.data_[i];
  }

  /// @brief Integer version of the above
  template<unsigned i>
  static int GetByIndex(IntVec input) {
    return input.data_[i];
  }

  /// @brief Extract one element from a FloatVec (runtime version, in loops)
  ///
  /// @param[in]  input   FloatVec to be read
  /// @param[in]  i   Index of the element to retrieve
  static inline float GetByIndex(FloatVecRead input, const unsigned i) {
    VECMATH_ASSERT(i < FloatVecSize);
    return input.data_[i];
  }

  /// @brief Add "left" to "right"
  static inline FloatVec Add(FloatVecRead left, FloatVecRead right) {
    return Fill(
      left.data_[0] + right.data_[0],
      left.data_[1] + right.data_[1],
      left.data_[2] + right.data_[2],
      left.data_[3] + right.data_[3] );
  }

  /// @brief Sum all elements of a FloatVec
  static inline float AddHorizontal(FloatVecRead input) {
    return input.data_[0]
      + input.data_[1]
      + input.data_[2]
      + input.data_[3];
  }

  /// @brief Substract "right" from "left"
  static inline FloatVec Sub(FloatVecRead left, FloatVecRead right) {
    return Fill(
      left.data_[0] - right.data_[0],
      left.data_[1] - right.data_[1],
      left.data_[2] - right.data_[2],
      left.data_[3] - right.data_[3] );
  }

  /// @brief Element-wise multiplication
  static inline FloatVec Mul(FloatVecRead left, FloatVecRead right) {
    return Fill(
      left.data_[0] * right.data_[0],
      left.data_[1] * right.data_[1],
      left.data_[2] * right.data_[2],
      left.data_[3] * right.data_[3] );
  }

  /// @brief Shift to right all elements of the input by 1,
  /// and shift in the given value
  ///
  /// E.g. given (x_{n}, x_{n + 1}, x_{n + 2}, x_{n + 3})
  /// return (value, x_{n}, x_{n + 1}, x_{n + 2})
  ///
  /// @param[in]  input   FloatVec to be shifted
  /// @param[in]  value   value to be shifted in
  static inline FloatVec RotateOnRight(FloatVecRead input, float value) {
    return Fill(
      value,
      input.data_[0],
      input.data_[1],
      input.data_[2]);
  }

  /// @brief Shift to left all elements of the input by 1,
  /// and shift in the given value
  ///
  /// E.g. given (x_{n}, x_{n + 1}, x_{n + 2}, x_{n + 3})
  /// return (x_{n + 1}, x_{n + 2}, x_{n + 3}, value)
  ///
  /// @param[in]  input   FloatVec to be shifted
  /// @param[in]  value   value to be shifted in
  static inline FloatVec RotateOnLeft(FloatVecRead input, float value) {
    return Fill(
      input.data_[1],
      input.data_[2],
      input.data_[3],
      value);
  }

  /// @brief Return the sign of each element of the FloatVec
  ///
  /// Sgn(0.0) return 0.0
  static inline FloatVec Sgn(FloatVecRead input) {
    return Fill(
      static_cast<float>(input.data_[0] > 0.0f) - static_cast<float>(input.data_[0] < 0.0f),
      static_cast<float>(input.data_[1] > 0.0f) - static_cast<float>(input.data_[1] < 0.0f),
      static_cast<float>(input.data_[2] > 0.0f) - static_cast<float>(input.data_[2] < 0.0f),
      static_cast<float>(input.data_[3] > 0.0f) - static_cast<float>(input.data_[3] < 0.0f) );
  }

  /// @brief Return the sign of each element of the FloatVec, no zero version
  ///
  /// Sgn(0.0) return 1.0f
  static inline FloatVec SgnNoZero(FloatVecRead input) {
    return Fill(
      input.data_[0] >= 0.0f ? 1.0f : -1.0f,
      input.data_[1] >= 0.0f ? 1.0f : -1.0f,
      input.data_[2] >= 0.0f ? 1.0f : -1.0f,
      input.data_[3] >= 0.0f ? 1.0f : -1.0f );
  }

  /// @brief Store the given FloatVec into memory
  ///
  /// @param[in]  buffer   Memory to be filled with the input
  /// @param[in]  input   FloatVec to be stored
  static inline void Store(float* const buffer, FloatVecRead input) {
    std::memcpy(buffer, &input.data_[0], sizeof(input));
  }

  static inline void StoreUnaligned(float* const buffer, FloatVecRead input) {
    Store(buffer, input);
  }

  /// @brief Get each right half of the two given vectors
  ///
  /// Given left = (x0, x1, x2, x3) and right = (y0, y1, y2, y3)
  /// it will return (x2, x3, y2, y3)
  static inline FloatVec TakeEachRightHalf(FloatVecRead left,
                                         FloatVecRead right) {
    return Fill( left.data_[2], left.data_[3],
             right.data_[2], right.data_[3] );
  }

  /// @brief Revert the given vector values order
  ///
  /// Given value = (x0, x1, x2, x3)
  /// it will return (x3, x2, x1, x0)
  static inline FloatVec Revert(FloatVecRead input) {
    return Fill(
      input.data_[3],
      input.data_[2],
      input.data_[1],
      input.data_[0] );
  }

  /// @brief Return each min element of both inputs
  static inline FloatVec Min(FloatVecRead left, FloatVecRead right) {
    return Fill(
      left.data_[0] < right.data_[0] ? left.data_[0] : right.data_[0],
      left.data_[1] < right.data_[1] ? left.data_[1] : right.data_[1],
      left.data_[2] < right.data_[2] ? left.data_[2] : right.data_[2],
      left.data_[3] < right.data_[3] ? left.data_[3] : right.data_[3] );
  }

  /// @brief Return each max element of both inputs
  static inline FloatVec Max(FloatVecRead left, FloatVecRead right) {
    return Fill(
      left.data_[0] > right.data_[0] ? left.data_[0] : right.data_[0],
      left.data_[1] > right.data_[1] ? left.data_[1] : right.data_[1],
      left.data_[2] > right.data_[2] ? left.data_[2] : right.data_[2],
      left.data_[3] > right.data_[3] ? left.data_[3] : right.data_[3] );
  }

  /// @brief Round each FloatVec element to the nearest integer
  static inline FloatVec Round(FloatVecRead input) {
    return Fill(
      input.data_[0] > 0.0f ? input.data_[0] + 0.5f : input.data_[0] - 0.5f,
      input.data_[1] > 0.0f ? input.data_[1] + 0.5f : input.data_[1] - 0.5f,
      input.data_[2] > 0.0f ? input.data_[2] + 0.5f : input.data_[2] - 0.5f,
      input.data_[3] > 0.0f ? input.data_[3] + 0.5f : input.data_[3] - 0.5f );
  }
  /// @brief Helper function: increment the input and wraps it into [-1.0 ; 1.0[
  ///
  /// @param[in]  input         Input to be wrapped - supposed not to be < 1.0
  /// @param[in]  increment     Increment to add to the input
  /// @return the incremented output in [-1.0 ; 1.0[
  static inline FloatVec IncrementAndWrap(FloatVecRead input, FloatVecRead increment) {
    const FloatVec output(Add(input, increment));
    const FloatVec additional_increment(Fill(
      output.data_[0] > 1.0f ? -2.0f : 0.0f,
      output.data_[1] > 1.0f ? -2.0f : 0.0f,
      output.data_[2] > 1.0f ? -2.0f : 0.0f,
      output.data_[3] > 1.0f ? -2.0f : 0.0f ));

    return Add(output, additional_increment);
  }

  /// @brief Helper binary function: return true if all input elements are true
  ///
  /// @param[in]  input   Input to be checked
  static inline bool IsMaskFull(FloatVecRead input) {
    return input.data_[0] > 0.0f
        && input.data_[1] > 0.0f
        && input.data_[2] > 0.0f
        && input.data_[3] > 0.0f;
  }

  /// @brief Helper binary function: return true if all input elements are null
  ///
  /// @param[in]  input   Input to be checked
  static inline bool IsMaskNull(FloatVecRead input) {
    return input.data_[0] == 0.0f
      && input.data_[1] == 0.0f
      && input.data_[2] == 0.0f
      && input.data_[3] == 0.0f;
  }

  /// @brief Helper binary function:
  /// true if each threshold element is >= than the matching input element
  static inline FloatVec GreaterEqual(FloatVecRead threshold, FloatVecRead input) {
    return Fill(
      threshold.data_[0] >= input.data_[0] ? 0xffffffff : 0.0f,
      threshold.data_[1] >= input.data_[1] ? 0xffffffff : 0.0f,
      threshold.data_[2] >= input.data_[2] ? 0xffffffff : 0.0f,
      threshold.data_[3] >= input.data_[3] ? 0xffffffff : 0.0f );
  }

  static inline FloatVec GreaterThan(FloatVecRead threshold, FloatVecRead input) {
    return Fill(
      threshold.data_[0] > input.data_[0] ? 0xffffffff : 0.0f,
      threshold.data_[1] > input.data_[1] ? 0xffffffff : 0.0f,
      threshold.data_[2] > input.data_[2] ? 0xffffffff : 0.0f,
      threshold.data_[3] > input.data_[3] ? 0xffffffff : 0.0f );
  }

  /// @brief Helper binary function:
  /// true if any input element is >= than any of the threshold element
  static inline bool GreaterEqual(float threshold, FloatVecRead input) {
    return threshold >= input.data_[0]
      && threshold >= input.data_[1]
      && threshold >= input.data_[2]
      && threshold >= input.data_[3];
  }

  static inline bool GreaterEqualAny(float threshold, FloatVecRead input) {
    return threshold >= input.data_[0]
      || threshold >= input.data_[1]
      || threshold >= input.data_[2]
      || threshold >= input.data_[3];
  }

  static inline bool GreaterThan(float threshold, FloatVecRead input) {
    return threshold > input.data_[0]
      && threshold > input.data_[1]
      && threshold > input.data_[2]
      && threshold > input.data_[3];
  }

  /// @brief Helper binary function:
  /// true if each threshold element is <= than the input element
  static inline FloatVec LessEqual(FloatVecRead threshold, FloatVecRead input) {
    return Fill(
      threshold.data_[0] <= input.data_[0] ? 0xffffffff : 0.0f,
      threshold.data_[1] <= input.data_[1] ? 0xffffffff : 0.0f,
      threshold.data_[2] <= input.data_[2] ? 0xffffffff : 0.0f,
      threshold.data_[3] <= input.data_[3] ? 0xffffffff : 0.0f );
  }

  static inline FloatVec LessThan(FloatVecRead threshold, FloatVecRead input) {
    return Fill(
      threshold.data_[0] < input.data_[0] ? 0xffffffff : 0.0f,
      threshold.data_[1] < input.data_[1] ? 0xffffffff : 0.0f,
      threshold.data_[2] < input.data_[2] ? 0xffffffff : 0.0f,
      threshold.data_[3] < input.data_[3] ? 0xffffffff : 0.0f );
  }

  static inline bool LessEqual(float threshold, FloatVecRead input) {
    return threshold <= input.data_[0]
      && threshold <= input.data_[1]
      && threshold <= input.data_[2]
      && threshold <= input.data_[3];
  }

  static inline bool LessThan(float threshold, FloatVecRead input) {
    return threshold < input.data_[0]
      && threshold < input.data_[1]
      && threshold < input.data_[2]
      && threshold < input.data_[3];
  }

  static inline FloatVec Equal(FloatVecRead threshold, FloatVecRead input) {
    return Fill(
      threshold.data_[0] == input.data_[0] ? 0xffffffff : 0.0f,
      threshold.data_[1] == input.data_[1] ? 0xffffffff : 0.0f,
      threshold.data_[2] == input.data_[2] ? 0xffffffff : 0.0f,
      threshold.data_[3] == input.data_[3] ? 0xffffffff : 0.0f );
  }

  static inline bool Equal(float threshold, FloatVecRead input) {
    return threshold == input.data_[0]
      && threshold == input.data_[1]
      && threshold == input.data_[2]
      && threshold == input.data_[3];
  }

  /// @brief Beware, not an actual bitwise AND! More like a "float select"
  static inline FloatVec ExtractValueFromMask(FloatVecRead value, FloatVecRead mask) {
    return Fill(
      mask.data_[0] == 0xffffffff ? value.data_[0] : 0.0f,
      mask.data_[1] == 0xffffffff ? value.data_[1] : 0.0f,
      mask.data_[2] == 0xffffffff ? value.data_[2] : 0.0f,
      mask.data_[3] == 0xffffffff ? value.data_[3] : 0.0f);
  }

  static inline IntVec TruncToInt(FloatVecRead float_value) {
    return Fill(
      static_cast<int>(float_value.data_[0]),
      static_cast<int>(float_value.data_[1]),
      static_cast<int>(float_value.data_[2]),
      static_cast<int>(float_value.data_[3]));
  }
};

}  // namespace vecmath

#endif  // VECMATH_INC_IMPLEM_STD_H_
