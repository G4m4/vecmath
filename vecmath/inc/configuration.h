/// @file configuration.h
/// @brief VecMath configuration file
///
/// Provides basic preprocessor macros in order to detect
/// user variables/hardware/OS/compiler...
///
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

#ifndef VECMATH_INC_CONFIGURATION_H_
#define VECMATH_INC_CONFIGURATION_H_

/// @brief Compiler detection
#if(defined(__GNUC__))
  #define _COMPILER_GCC 1
#elif(defined(_MSC_VER))
  #define _COMPILER_MSVC 1
#else
  #error "Compiler could not be detected"
#endif

/// @brief Build configuration detection
/// Since there are no easy cross-platform way to do this,
/// we assume that "no asserts" means release
#if(defined(_NDEBUG) || defined(NDEBUG))
  #define _BUILD_CONFIGURATION_DEBUG 0
#else  // defined(NDEBUG) ?
  #define _BUILD_CONFIGURATION_DEBUG 1
#endif  // defined(NDEBUG) ?

/// @brief Architecture detection - compiler specific preprocessor macros
#if _COMPILER_MSVC
  #if defined(_M_IX86)
    #define _ARCH_X86 1
  #endif
  #if defined(_M_X64)
    #define _ARCH_X64 1
  #endif
#elif _COMPILER_GCC
  #if (defined(__i386__))
    #define _ARCH_X86 1
  #endif
#endif

/// @brief SIMD enabling, based on platform
#if defined(_DISABLE_SIMD)
  #define _USE_SSE 0
#else
  #if (_ARCH_X86) || (_M_X64)
    #define _USE_SSE 1
  #endif
#endif

#endif  // VECMATH_INC_CONFIGURATION_H_
