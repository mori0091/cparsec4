// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_PRELUDE_TG_TOKEN_H_
#define CPARSEC4_PRELUDE_TG_TOKEN_H_

#include "cparsec4/parser/token.h"
#include "cparsec4/prelude/tg_parser.h"

#define satisfy(predicate) P_SATISFY(CPARSEC4_INPUT_TYPE)(predicate)
#define any()              P_ANY(CPARSEC4_INPUT_TYPE)()
#define token(c)           P_TOKEN(CPARSEC4_INPUT_TYPE)(c)
#define eof()              P_EOF(CPARSEC4_INPUT_TYPE)()

def_ParserToken(CPARSEC4_INPUT_TYPE);

// impl_ParserToken(CPARSEC4_INPUT_TYPE);

#endif // CPARSEC4_PRELUDE_TG_TOKEN_H_
