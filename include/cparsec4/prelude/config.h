// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_PRELUDE_CONFIG_H_
#define CPARSEC4_PRELUDE_CONFIG_H_

// ---------------------------------------------------------------------
// #define CPARSEC4_INPUT_TYPE CStr

#if !defined(CPARSEC4_INPUT_TYPE)
#warning "CPARSEC4_INPUT_TYPE is not defined. (CStr is assumed)"
#define CPARSEC4_INPUT_TYPE CStr
#endif

#include "tgc/prelude.h"

#include "cparsec4/stream/charstream.h"
#include "cparsec4/stream/cstrstream.h"

// ---------------------------------------------------------------------
// #define CPARSEC4_CONFIG_MINIMUM 1

#if (CPARSEC4_CONFIG_NOTHING)
#define CPARSEC4_BUILTIN_OUTPUT_TYPES()
#elif (CPARSEC4_CONFIG_MINIMUM)
#define CPARSEC4_BUILTIN_OUTPUT_TYPES() CPARSEC4_CORE_OUTPUT_TYPES()
#else
#define CPARSEC4_BUILTIN_OUTPUT_TYPES() CPARSEC4_STD_OUTPUT_TYPES()
#endif

#define CPARSEC4_CORE_OUTPUT_TYPES() Unit, Char, CStr, char, Slice(char)

#define CPARSEC4_STD_OUTPUT_TYPES()                                      \
  CPARSEC4_CORE_OUTPUT_TYPES(), BOOLEAN_TYPES, INTEGER_TYPES,            \
    REAL_FLOATING_TYPES

// ---------------------------------------------------------------------
// #define CPARSEC4_CONFIG_USER_OUTPUT_TYPES Foo, Bar

#if defined(CPARSEC4_CONFIG_USER_OUTPUT_TYPES)
#define CPARSEC4_USER_OUTPUT_TYPES() CPARSEC4_CONFIG_USER_OUTPUT_TYPES
#else
#define CPARSEC4_USER_OUTPUT_TYPES()
#endif

// ---------------------------------------------------------------------
#define CPARSEC4_OUTPUT_TYPES()                                          \
  CPARSEC4_OUTPUT_TYPES_1(), CPARSEC4_OUTPUT_TYPES_2()
#define CPARSEC4_OUTPUT_TYPES_1()                                        \
  SQUASH(CPARSEC4_USER_OUTPUT_TYPES(), CPARSEC4_BUILTIN_OUTPUT_TYPES())
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
#define CPARSEC4_VALUE_TYPES                                             \
  CPARSEC4_RESULT_TYPES(), CPARSEC4_ERROR_TYPES(), CPARSEC4_OUTPUT_TYPES()

#define CPARSEC4_TYPES_FOR_DROP                                          \
  CPARSEC4_PARSER_TYPES(), CPARSEC4_RESULT_TYPES(),                      \
    CPARSEC4_ERROR_TYPES(), CPARSEC4_OUTPUT_TYPES_2()

// ---------------------------------------------------------------------
#if !defined(TYPES_FOR_EQ)
#define TYPES_FOR_EQ JUST(CPARSEC4_VALUE_TYPES)
#endif
#include "tgc/g_assert.h"

#if !defined(TYPES_FOR_DISPLAY)
#define TYPES_FOR_DISPLAY JUST(CPARSEC4_VALUE_TYPES)
#endif
#include "tgc/fmt/print.h"

#if !defined(TYPES_FOR_DROP)
#define TYPES_FOR_DROP JUST(CPARSEC4_TYPES_FOR_DROP)
#endif
#include "tgc/drop.h"

#endif // CPARSEC4_PRELUDE_CONFIG_H_
