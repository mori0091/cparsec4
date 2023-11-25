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

#define many(p)  P_GENERIC_1(P_MANY, p)(p)
#define many1(p) P_GENERIC_1(P_MANY1, p)(p)

FOREACH(def_ParserRepeat, CPARSEC4_INOUT_TYPES_1());

#endif // CPARSEC4_PRELUDE_TG_REPEAT_H_
