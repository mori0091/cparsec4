// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_PRELUDE_TG_REPEAT_H_
#define CPARSEC4_PRELUDE_TG_REPEAT_H_

#include "cparsec4/parser/repeat.h"
#include "cparsec4/prelude/tg_parser.h"

#define PARSER_REPEAT(p)                                                 \
  GENERIC((p), Parser, trait_ParserRepeat, CPARSEC4_INOUT_TYPES_1())
#define trait_ParserRepeat(I, O) trait(ParserRepeat(I, O))

#define many(p)                  PARSER_REPEAT(p).Many(p)
#define many1(p)                 PARSER_REPEAT(p).Many1(p)

FOREACH(def_ParserRepeat, CPARSEC4_INOUT_TYPES_1());

// FOREACH(impl_ParserRepeat, CPARSEC4_INOUT_TYPES_1());

#endif // CPARSEC4_PRELUDE_TG_REPEAT_H_
