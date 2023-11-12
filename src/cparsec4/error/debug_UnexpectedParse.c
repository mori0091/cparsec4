// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#include "cparsec4/error/unexpectedparse.h"

impl_Debug_for(UnexpectedParse) {
  const char* s =
    (self == Eoi ? "UnexpectedParse::Eoi"
                 : (self == Unexpected ? "UnexpectedParse::Unexpected"
                                       : "UnexpectedParse::<?>"));
  fmt_write(out, s);
}
