// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_PRELUDE_TG_CHOICE_H_
#define CPARSEC4_PRELUDE_TG_CHOICE_H_

#include "cparsec4/parser/choice.h"
#include "cparsec4/prelude/tg_parser.h"

#define choice(p, ...) FOLDR(either, p, __VA_ARGS__)
#define either(p1, p2) P_GENERIC(P_EITHER, p1)(p1, p2)

FOREACH(def_ParserChoice, CPARSEC4_INOUT_TYPES());

#endif // CPARSEC4_PRELUDE_TG_CHOICE_H_
