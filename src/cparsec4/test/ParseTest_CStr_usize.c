// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#define CPARSEC4_CONFIG_NOTHING           (1)
#define CPARSEC4_CONFIG_USER_OUTPUT_TYPES usize
#define CPARSEC4_INPUT_TYPE               CStr
#include "cparsec4/prelude/tg_parsetest.h"

#if defined(NEED_SIZE_T)
impl_ParseTest(CStr, usize);
#endif
