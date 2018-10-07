Vecmath - Basic vector math library
==================================

[![Build Status](https://travis-ci.org/G4m4/vecmath.svg?branch=master)](https://travis-ci.org/G4m4/vecmath)

Vecmath is a collection of wrapper math functions in C to easily handle vectorised (SIMD) instructions. Nothing fancy, not many features, but hopefully as portable and low tech as possible.

The entire code is headers only, with basically one file per implementation and glue code in between those.

Building Vecmath library
-------------------------

There is nothing to build but a basic tests library used for testing purpose. It relies upon Cmake and Google tests and is built like this:

    git submodule init
    git submodule update
    mkdir build && cd build
    cmake -DVECMATH_HAS_GTEST=ON ..
    cmake --build .

License
==================================
Vecmath is under a very permissive license.

See the COPYING file for the full license text.
