// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_TEST_PARSETEST_H_
#define CPARSEC4_TEST_PARSETEST_H_

#include "cparsec4/parser.h"
#include "tgc/g_assert.h"

#define ParseTest(I, O) TYPE_NAME(ParseTest, TYPEOF(I), TYPEOF(O))

#define def_ParseTest(I, O)                                              \
  require_trait(Parser(I, O));                                           \
  def_trait(ParseTest(I, O)) {                                           \
    void (*test_parse_ok)(Parser(I, O) p, I input, O ok, I rest);        \
    void (*test_parse_err)(Parser(I, O) p, I input, ERROR(I) err,        \
                           I rest);                                      \
  }

#define impl_ParseTest(I, O)                                             \
  static void FUNC_NAME(test_parse_ok, ParseTest(I, O))(                 \
    Parser(I, O) p, I input, O ok, I rest) {                             \
    ParseResult(I, O) actual = trait(Parser(I, O)).parse(p, input);      \
    ParseResult(I, O) expect = trait(ParseResult(I, O)).Ok(rest, ok);    \
    assert_eq(expect, actual);                                           \
    g_drop(expect);                                                      \
    g_drop(actual);                                                      \
  }                                                                      \
  static void FUNC_NAME(test_parse_err, ParseTest(I, O))(                \
    Parser(I, O) p, I input, ERROR(I) err, I rest) {                     \
    ParseResult(I, O) actual = trait(Parser(I, O)).parse(p, input);      \
    ParseResult(I, O) expect = trait(ParseResult(I, O)).Err(rest, err);  \
    assert_eq(expect, actual);                                           \
    g_drop(expect);                                                      \
    g_drop(actual);                                                      \
  }                                                                      \
  impl_trait(ParseTest(I, O)) {                                          \
    .test_parse_ok = FUNC_NAME(test_parse_ok, ParseTest(I, O)),          \
    .test_parse_err = FUNC_NAME(test_parse_err, ParseTest(I, O)),        \
  }

#endif // CPARSEC4_TEST_PARSETEST_H_
