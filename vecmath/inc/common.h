/// @file common.h
/// @brief Common utilities header
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

#ifndef VECMATH_INC_COMMON_H_
#define VECMATH_INC_COMMON_H_

#include <cassert>

#include "vecmath/inc/configuration.h"

#if (_VEC_VEC_USESSE)
extern "C" {
  // @todo(gm) move that include
  #include <emmintrin.h>
  #include <mmintrin.h>
}
#endif  // (_VEC_VEC_USESSE)

namespace vecmath {

/// @brief Ignore unused variables
template<typename Type> void IGNORE(const Type&) {}

/// @brief Assume that the following condition is always true
/// (on some compilers, allows optimization)
#if(_VEC_COMPILER_MSVC)
static inline void ASSUME(const bool condition) { __assume(condition); }
#elif(_VEC_COMPILER_GCC)
static inline void ASSUME(const bool condition) { if (!(condition)) __builtin_unreachable(); }
#else
#define ASSUME(_condition_)
#endif  // _VEC_COMPILER_ ?

/// @brief Asserts condition == true
#if(_BUILD_CONFIGURATION_DEBUG)
#define VECMATH_ASSERT(_condition_) (assert((_condition_)))
#else
// Maps to "assume" in release configuration for better optimization
#define VECMATH_ASSERT(_condition_)// {::ASSUME((_condition_));}
#endif

/// @brief Indicates that the decorated reference is not aliased
#ifndef VECMATH_RESTRICT
#if(_COMPILER_MSVC)
#define VECMATH_RESTRICT __restrict
#elif(_COMPILER_GCC)
#define VECMATH_RESTRICT __restrict__
#else
#define VECMATH_RESTRICT
#endif  // _COMPILER_ ?
#endif  // VECMATH_RESTRICT ?

/// @brief Type for block input parameter
typedef const float* VECMATH_RESTRICT const BlockIn;

/// @brief Type for block output parameter
typedef float* VECMATH_RESTRICT const BlockOut;

}  // namespace vecmath

#endif  // VECMATH_INC_COMMON_H_
