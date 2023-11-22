// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_PRELUDE_TG_PARSER_H_
#define CPARSEC4_PRELUDE_TG_PARSER_H_

#include "cparsec4/parser.h"
#include "cparsec4/prelude/config.h"

#define GENERIC_PARSER(p)                                                \
  GENERIC((p), Parser, trait_Parser, CPARSEC4_INOUT_TYPES())
#define trait_Parser(I, O)    trait(Parser(I, O))

#define g_run_parse(p, input) GENERIC_PARSER(p).run_parse((p), (input))
#define g_parse(p, input)     GENERIC_PARSER(p).parse((p), (input))

FOREACH(def_Parser, CPARSEC4_INOUT_TYPES());

// FOREACH(impl_Parser, CPARSEC4_INOUT_TYPES());

#endif // CPARSEC4_PRELUDE_TG_PARSER_H_
