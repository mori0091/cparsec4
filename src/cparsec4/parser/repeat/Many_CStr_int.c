// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#define CPARSEC4_INPUT_TYPE CStr
#include "cparsec4/prelude.h"

#if defined(NEED_INT)
impl_Many(CStr, int);
#endif
