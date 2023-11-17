// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_PRELUDE_H_
#define CPARSEC4_PRELUDE_H_

#if !defined(CPARSEC4_INPUT_TYPE)
#warning "CPARSEC4_INPUT_TYPE is not defined. (CStr is assumed)"
#define CPARSEC4_INPUT_TYPE CStr
#endif

#include "tgc/prelude.h"

#include "cparsec4/stream/charstream.h"
#include "cparsec4/stream/cstrstream.h"

// ---------------------------------------------------------------------
#define CPARSEC4_INPUT_TYPES() CStr, Slice(char)

#define CPARSEC4_OUTPUT_TYPES()                                          \
  CPARSEC4_OUTPUT_TYPES_1(), CPARSEC4_OUTPUT_TYPES_2()
#define CPARSEC4_OUTPUT_TYPES_1()                                        \
  Unit, Char, CStr, PRIMITIVE_TYPES, Slice(char)
#define CPARSEC4_OUTPUT_TYPES_2() APPLY(Vec, CPARSEC4_OUTPUT_TYPES_1())

#define CPARSEC4_INOUT_TYPES()    PARSER_INOUT_TYPES(CPARSEC4_INPUT_TYPE)
#define CPARSEC4_INOUT_TYPES_1()  PARSER_INOUT_TYPES_1(CPARSEC4_INPUT_TYPE)
#define CPARSEC4_INOUT_TYPES_2()  PARSER_INOUT_TYPES_2(CPARSEC4_INPUT_TYPE)

#define PARSER_INOUT_TYPES(I)     BIND(I, CPARSEC4_OUTPUT_TYPES())
#define PARSER_INOUT_TYPES_1(I)   BIND(I, CPARSEC4_OUTPUT_TYPES_1())
#define PARSER_INOUT_TYPES_2(I)   BIND(I, CPARSEC4_OUTPUT_TYPES_2())

// ---------------------------------------------------------------------
#define CPARSEC4_PARSER_TYPES() APPLY(Parser, CPARSEC4_INOUT_TYPES())
#define CPARSEC4_REPLY_TYPES()  APPLY(ParseReply, CPARSEC4_INOUT_TYPES())
#define CPARSEC4_RESULT_TYPES() APPLY(ParseResult, CPARSEC4_INOUT_TYPES())
#define CPARSEC4_ERROR_TYPES()  UnexpectedParse

// ---------------------------------------------------------------------
#define CPARSEC4_ALL_INOUT_TYPES()                                       \
  SEP_BY(COMMA, PARSER_INOUT_TYPES, CPARSEC4_INPUT_TYPES())
#define CPARSEC4_ALL_INOUT_TYPES_1()                                     \
  SEP_BY(COMMA, PARSER_INOUT_TYPES_1, CPARSEC4_INPUT_TYPES())
#define CPARSEC4_ALL_INOUT_TYPES_2()                                     \
  SEP_BY(COMMA, PARSER_INOUT_TYPES_2, CPARSEC4_INPUT_TYPES())

#define CPARSEC4_ALL_PARSER_TYPES()                                      \
  APPLY(Parser, CPARSEC4_ALL_INOUT_TYPES())
#define CPARSEC4_ALL_REPLY_TYPES()                                       \
  APPLY(ParseReply, CPARSEC4_ALL_INOUT_TYPES())
#define CPARSEC4_ALL_RESULT_TYPES()                                      \
  APPLY(ParseResult, CPARSEC4_ALL_INOUT_TYPES())

// ---------------------------------------------------------------------
#define USING_TYPES                                                      \
  CPARSEC4_RESULT_TYPES(), CPARSEC4_ERROR_TYPES(), CPARSEC4_OUTPUT_TYPES()

#define USING_TYPES_FOR_DROP                                             \
  CPARSEC4_PARSER_TYPES(), CPARSEC4_RESULT_TYPES(),                      \
    CPARSEC4_ERROR_TYPES(), CPARSEC4_OUTPUT_TYPES_2()

#define TYPES_FOR_EQ JUST(USING_TYPES)
#include "tgc/g_assert.h"

#define TYPES_FOR_DISPLAY JUST(USING_TYPES)
#include "tgc/fmt/print.h"

#define TYPES_FOR_DROP JUST(USING_TYPES_FOR_DROP)
#include "tgc/drop.h"

// ---------------------------------------------------------------------
#include "cparsec4/parser.h"

#define GENERIC_PARSER(p)                                                \
  GENERIC((p), Parser, trait_Parser, CPARSEC4_INOUT_TYPES())
#define trait_Parser(I, O)    trait(Parser(I, O))

#define g_run_parse(p, input) GENERIC_PARSER(p).run_parse((p), (input))
#define g_parse(p, input)     GENERIC_PARSER(p).parse((p), (input))

FOREACH(def_Parser, CPARSEC4_INOUT_TYPES());

// FOREACH(impl_Parser, CPARSEC4_INOUT_TYPES());

// ---------------------------------------------------------------------
#include "cparsec4/parser/token.h"

#define PARSER_TOKEN()       trait_ParserToken(CPARSEC4_INPUT_TYPE)
#define trait_ParserToken(I) trait(ParserToken(I))

#define satisfy(predicate)   PARSER_TOKEN().Satisfy(predicate)
#define any()                PARSER_TOKEN().Any()
#define token(c)             PARSER_TOKEN().Token(c)
#define eof()                PARSER_TOKEN().Eof()

def_ParserToken(CPARSEC4_INPUT_TYPE);

// impl_ParserToken(CPARSEC4_INPUT_TYPE);

// ---------------------------------------------------------------------
#include "cparsec4/parser/repeat.h"

#define PARSER_REPEAT(p)                                                 \
  GENERIC((p), Parser, trait_ParserRepeat, CPARSEC4_INOUT_TYPES_1())
#define trait_ParserRepeat(I, O) trait(ParserRepeat(I, O))

#define many(p)                  PARSER_REPEAT(p).Many(p)
#define many1(p)                 PARSER_REPEAT(p).Many1(p)

FOREACH(def_ParserRepeat, CPARSEC4_INOUT_TYPES_1());

// FOREACH(impl_ParserRepeat, CPARSEC4_INOUT_TYPES_1());

#endif // CPARSEC4_PRELUDE_H_
