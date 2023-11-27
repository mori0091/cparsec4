// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_PRELUDE_TG_COMBINATOR_H_
#define CPARSEC4_PRELUDE_TG_COMBINATOR_H_

#include "cparsec4/parser/combinator.h"
#include "cparsec4/prelude/tg_parser.h"

#define attempt(p) P_GENERIC(P_ATTEMPT, p)(p)

FOREACH(def_ParserCombinator, CPARSEC4_INOUT_TYPES());

#endif // CPARSEC4_PRELUDE_TG_COMBINATOR_H_
