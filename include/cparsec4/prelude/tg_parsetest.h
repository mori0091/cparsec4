// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_PRELUDE_TG_PARSETEST_H_
#define CPARSEC4_PRELUDE_TG_PARSETEST_H_

#include "cparsec4/prelude/tg_parser.h"
#include "cparsec4/test/parsetest.h"

#define PARSE_TEST(p)         P_GENERIC(trait_ParseTest, p)
#define trait_ParseTest(I, O) trait(ParseTest(I, O))

#define g_test_parse_ok(p_, input_, ok_, rest_)                          \
  PARSE_TEST(p_).test_parse_ok(__FILE__, __LINE__, (p_), (input_),       \
                               (ok_), (rest_))

#define g_test_parse_err(p_, input_, err_, rest_)                        \
  PARSE_TEST(p_).test_parse_err(__FILE__, __LINE__, (p_), (input_),      \
                                (err_), (rest_))

FOREACH(def_ParseTest, CPARSEC4_INOUT_TYPES());

#endif // CPARSEC4_PRELUDE_TG_PARSETEST_H_
