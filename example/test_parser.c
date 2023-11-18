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

#define E          StreamErrorFor(Input)

#define PARSER(O)  Parser(Input, O)
#define PRESULT(O) ParseResult(Input, O)

// ---------------------------------------------------------------------
#include "cparsec4/prelude.h"

// ---------------------------------------------------------------------
static void test_parser0(void) {
  Input input = STR("Hello / こんにちは");
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
  Input input = STR("Hello / こんにちは");
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
  Scoped(PARSER(Tok)) p = token(CHR("a"));
  {
    Scoped(PRESULT(Tok)) expect =
      trait(PRESULT(Tok)).Ok(STR("abc"), CHR("a"));
    Scoped(PRESULT(Tok)) actual = g_parse(p, STR("aabc"));
    assert_eq(expect, actual);
  }
  {
    Scoped(PRESULT(Tok)) expect =
      trait(PRESULT(Tok)).Ok(STR("bc"), CHR("a"));
    Scoped(PRESULT(Tok)) actual = g_parse(p, STR("abc"));
    assert_eq(expect, actual);
  }
  {
    Scoped(PRESULT(Tok)) expect =
      trait(PRESULT(Tok))
        .Err(STR("bc"), trait(E).unexpected_token(CHR("b")));
    Scoped(PRESULT(Tok)) actual = g_parse(p, STR("bc"));
    assert_eq(expect, actual);
  }
  {
    Scoped(PRESULT(Tok)) expect =
      trait(PRESULT(Tok))
        .Err(STR("c"), trait(E).unexpected_token(CHR("c")));
    Scoped(PRESULT(Tok)) actual = g_parse(p, STR("c"));
    assert_eq(expect, actual);
  }
  {
    Scoped(PRESULT(Tok)) expect =
      trait(PRESULT(Tok)).Err(STR(""), trait(E).end_of_input());
    Scoped(PRESULT(Tok)) actual = g_parse(p, STR(""));
    assert_eq(expect, actual);
  }
}

static void test_many(void) {
  PARSER(Vec(Tok)) ps = many(token(CHR("a")));
  {
    PRESULT(Vec(Tok)) actual = g_parse(ps, STR("aaa"));
    PRESULT(Vec(Tok))
    expect = trait(PRESULT(Vec(Tok)))
               .Ok(STR(""), vecof(Tok, CHR("a"), CHR("a"), CHR("a")));
    assert_eq(expect, actual);
    g_drop(expect);
    g_drop(actual);
  }
  {
    PRESULT(Vec(Tok)) actual = g_parse(ps, STR("aab"));
    PRESULT(Vec(Tok))
    expect = trait(PRESULT(Vec(Tok)))
               .Ok(STR("b"), vecof(Tok, CHR("a"), CHR("a")));
    assert_eq(expect, actual);
    g_drop(expect);
    g_drop(actual);
  }
  {
    PRESULT(Vec(Tok)) actual = g_parse(ps, STR("b"));
    PRESULT(Vec(Tok))
    expect = trait(PRESULT(Vec(Tok))).Ok(STR("b"), (Vec(Tok)){0});
    assert_eq(expect, actual);
    g_drop(expect);
    g_drop(actual);
  }
  {
    PRESULT(Vec(Tok)) actual = g_parse(ps, STR(""));
    PRESULT(Vec(Tok))
    expect = trait(PRESULT(Vec(Tok))).Ok(STR(""), (Vec(Tok)){0});
    assert_eq(expect, actual);
    g_drop(expect);
    g_drop(actual);
  }
  g_drop(ps);
}

static void test_many1(void) {
  PARSER(Vec(Tok)) ps = many1(token(CHR("a")));
  {
    PRESULT(Vec(Tok)) actual = g_parse(ps, STR("aaa"));
    PRESULT(Vec(Tok))
    expect = trait(PRESULT(Vec(Tok)))
               .Ok(STR(""), vecof(Tok, CHR("a"), CHR("a"), CHR("a")));
    assert_eq(expect, actual);
    g_drop(expect);
    g_drop(actual);
  }
  {
    PRESULT(Vec(Tok)) actual = g_parse(ps, STR("aab"));
    PRESULT(Vec(Tok))
    expect = trait(PRESULT(Vec(Tok)))
               .Ok(STR("b"), vecof(Tok, CHR("a"), CHR("a")));
    assert_eq(expect, actual);
    g_drop(expect);
    g_drop(actual);
  }
  {
    PRESULT(Vec(Tok)) actual = g_parse(ps, STR("b"));
    PRESULT(Vec(Tok))
    expect = trait(PRESULT(Vec(Tok)))
               .Err(STR("b"), trait(E).unexpected_token(CHR("b")));
    assert_eq(expect, actual);
    g_drop(expect);
    g_drop(actual);
  }
  {
    PRESULT(Vec(Tok)) actual = g_parse(ps, STR(""));
    PRESULT(Vec(Tok))
    expect =
      trait(PRESULT(Vec(Tok))).Err(STR(""), trait(E).end_of_input());
    assert_eq(expect, actual);
    g_drop(expect);
    g_drop(actual);
  }
  g_drop(ps);
}

#include <locale.h>
#include <stdlib.h>

int main(void) {
  setlocale(LC_ALL, getenv("LANG"));

  test_parser0();
  test_parser1();
  test_parser2();

  test_many();
  test_many1();
  return 0;
}
