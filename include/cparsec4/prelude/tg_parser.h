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

/**
 * Expands to `f(I, O)` depending on type of parser `p`.
 *
 * Expands to `f(I, O)` if
 * - `p` is a `Parser(I, O)`, and
 * - `(I, O)` is a member of `CPARSEC4_INOUT_TYPES()`.
 */
#define P_GENERIC(f, p) GENERIC((p), Parser, f, CPARSEC4_INOUT_TYPES())

/**
 * Expands to `f(I, O)` depending on type of parser `p`.
 *
 * Expands to `f(I, O)` if
 * - `p` is a `Parser(I, O)`, and
 * - `(I, O)` is a member of `CPARSEC4_INOUT_TYPES_1()`.
 */
#define P_GENERIC_1(f, p)                                                \
  GENERIC((p), Parser, f, CPARSEC4_INOUT_TYPES_1())

/**
 * Expands to `trait(Parser(I, O))` depending on type of parser `p`.
 *
 * Expands to `trait(Parser(I, O))` if
 * - `p` is a `Parser(I, O)`, and
 * - `(I, O)` is a member of `CPARSEC4_INOUT_TYPES()`.
 */
#define GENERIC_PARSER(p)     P_GENERIC(trait_Parser, p)
#define trait_Parser(I, O)    trait(Parser(I, O))

#define g_run_parse(p, input) GENERIC_PARSER(p).run_parse((p), (input))
#define g_parse(p, input)     GENERIC_PARSER(p).parse((p), (input))

FOREACH(def_Parser, CPARSEC4_INOUT_TYPES());

// FOREACH(impl_Parser, CPARSEC4_INOUT_TYPES());

#endif // CPARSEC4_PRELUDE_TG_PARSER_H_
