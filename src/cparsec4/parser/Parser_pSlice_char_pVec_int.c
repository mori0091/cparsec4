// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#define CPARSEC4_CONFIG_NOTHING           (1)
#define CPARSEC4_CONFIG_USER_OUTPUT_TYPES int
#define CPARSEC4_INPUT_TYPE               Slice(char)
#include "cparsec4/prelude/tg_parser.h"

#if defined(NEED_INT)
impl_Parser(Slice(char), Vec(int));
#endif
