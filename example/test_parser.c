// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

// ---------------------------------------------------------------------
#if 0
#include "cparsec4/stream/charstream.h"
#define CPARSEC4_INPUT_TYPE Slice(char)

#include <string.h>
#define STR(s) (trait(Slice(char)).from(strlen(s), (s)))
#define CHR(c)                                                           \
  (_Generic((c)                                                         \
            , char : (c)                                                \
            , int : (c)                                                 \
            , char * : (c)[0]                                           \
            , const char * : (c)[0]                                     \
            ))

#else

#include "cparsec4/stream/cstrstream.h"
#define CPARSEC4_INPUT_TYPE CStr
#define STR(s)              cstr(s)
#define CHR(c)              cstr(c)

#endif

// ---------------------------------------------------------------------
#define Input      CPARSEC4_INPUT_TYPE
#define Tok        TOKEN(Input)
#define Chunk      CHUNK(Input)
#define Error      ERROR(Input)
#define CheckPoint CHECKPOINT(Input)

#define E          trait(StreamErrorFor(Input))

#define PARSER(O)  Parser(Input, O)
#define PRESULT(O) ParseResult(Input, O)

// ---------------------------------------------------------------------
#include "cparsec4/prelude.h"

// ---------------------------------------------------------------------
static void test_parser0(void) {
  Input input = STR(u8"Hello / こんにちは");
  PARSER(Unit) eof = eof();
  PARSER(Vec(Tok)) ps = many(any());
  {
    PRESULT(Vec(Tok)) r = g_parse(ps, input);
    PRESULT(Unit) r2 = g_parse(eof, r.input);
    println(r);
    g_drop(r2);
    g_drop(r);
  }
  g_drop(ps);
  g_drop(eof);
}

static void test_parser1(void) {
  Input input = STR(u8"Hello / こんにちは");
  PARSER(Tok) p = any();
  PRESULT(Tok) r = g_parse(p, input);
  while (!r.is_err) {
    print("'", r.ok, "', ");
    r = g_parse(p, r.input);
  }
  println(r.err);
  g_drop(r);
  g_drop(p);
}

static void test_parser2(void) {
  PARSER(Tok) p = token(CHR("a"));
  // clang-format off
  g_test_parse_ok (p, STR("aabc"), CHR("a"), STR("abc"));
  g_test_parse_ok (p, STR("abc") , CHR("a"), STR("bc"));
  g_test_parse_err(p, STR("bc")  , E.unexpected_token(CHR("b")), STR("bc"));
  g_test_parse_err(p, STR("c")   , E.unexpected_token(CHR("c")), STR("c"));
  g_test_parse_err(p, STR("")    , E.end_of_input(), STR(""));
  // clang-format on
  g_drop(p);
}

static void test_many(void) {
  PARSER(Vec(Tok)) p = many(token(CHR("a")));
  // clang-format off
  g_test_parse_ok(p, STR("aaa"), vecof(Tok, CHR("a"), CHR("a"), CHR("a")), STR(""));
  g_test_parse_ok(p, STR("aab"), vecof(Tok, CHR("a"), CHR("a")), STR("b"));
  g_test_parse_ok(p, STR("b")  , (Vec(Tok)){0}, STR("b"));
  g_test_parse_ok(p, STR("")   , (Vec(Tok)){0}, STR(""));
  // clang-format on
  g_drop(p);
}

static void test_many1(void) {
  PARSER(Vec(Tok)) p = many1(token(CHR("a")));
  // clang-format off
  g_test_parse_ok (p, STR("aaa"), vecof(Tok, CHR("a"), CHR("a"), CHR("a")), STR(""));
  g_test_parse_ok (p, STR("aab"), vecof(Tok, CHR("a"), CHR("a")), STR("b"));
  g_test_parse_err(p, STR("b")  , E.unexpected_token(CHR("b")), STR("b"));
  g_test_parse_err(p, STR("")   , E.end_of_input(), STR(""));
  // clang-format on
  g_drop(p);
}

static void test_choice(void) {
  PARSER(Tok) p = choice(token(CHR("a")), token(CHR("b")));
  // clang-format off
  g_test_parse_ok (p, STR("abc"), CHR("a"), STR("bc"));
  g_test_parse_ok (p, STR("bc") , CHR("b"), STR("c"));
  g_test_parse_err(p, STR("c")  , E.unexpected_token(CHR("c")), STR("c"));
  g_test_parse_err(p, STR("")   , E.end_of_input(), STR(""));
  // clang-format on
  g_drop(p);
}

static void test_choice2(void) {
  PARSER(Chunk) p = choice(tokens(STR("one")), tokens(STR("two")), tokens(STR("three")));
  // clang-format off
  g_test_parse_ok (p, STR("one"), STR("one"), STR(""));
  g_test_parse_ok (p, STR("two"), STR("two"), STR(""));
  g_test_parse_err(p, STR("three"), E.unexpected_token(CHR("h")), STR("hree"));
  // clang-format on
  g_drop(p);
}

static void test_attempt(void) {
  PARSER(Chunk) p = choice(attempt(tokens(STR("one"))), attempt(tokens(STR("two"))), attempt(tokens(STR("three"))));
  // clang-format off
  g_test_parse_ok(p, STR("one")  , STR("one")  , STR(""));
  g_test_parse_ok(p, STR("two")  , STR("two")  , STR(""));
  g_test_parse_ok(p, STR("three"), STR("three"), STR(""));
  // clang-format on
  g_drop(p);
}

#include <locale.h>
#include <stdlib.h>

char * set_character_encoding_to_utf8(void) {
  char * lc = setlocale(LC_ALL, ".UTF-8");
  if (!lc) {
    lc = setlocale(LC_ALL, "C.UTF-8");
  }
  if (!lc) {
    // fall back to the default locale.
    lc = setlocale(LC_ALL, "");
  }
  return lc;
}

int main(void) {
  println("The locale is set to ", set_character_encoding_to_utf8());

  test_parser0();
  test_parser1();
  test_parser2();

  test_many();
  test_many1();

  test_choice();
  test_choice2();
  test_attempt();

  return 0;
}
