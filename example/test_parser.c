// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#include "tgc/prelude.h"

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

#define E          StreamError(Tok, Chunk, Error)

// ---------------------------------------------------------------------
#define PARSER_OUTPUT_TYPES(I)                                           \
  PARSER_OUTPUT_TYPES_1(I), PARSER_OUTPUT_TYPES_2(I)
#define PARSER_OUTPUT_TYPES_1(I) Unit, TOKEN(I), CHUNK(I)
#define PARSER_OUTPUT_TYPES_2(I) APPLY(Vec, PARSER_OUTPUT_TYPES_1(I))

#define PARSER_INOUT_TYPES(I)    BIND(I, PARSER_OUTPUT_TYPES(I))
#define PARSER_INOUT_TYPES_1(I)  BIND(I, PARSER_OUTPUT_TYPES_1(I))
#define PARSER_INOUT_TYPES_2(I)  BIND(I, PARSER_OUTPUT_TYPES_2(I))

// ---------------------------------------------------------------------
#define CPARSEC4_PARSER_OUTPUT_TYPES()                                   \
  PARSER_OUTPUT_TYPES(CPARSEC4_INPUT_TYPE)
#define CPARSEC4_PARSER_OUTPUT_TYPES_1()                                 \
  PARSER_OUTPUT_TYPES_1(CPARSEC4_INPUT_TYPE)
#define CPARSEC4_PARSER_OUTPUT_TYPES_2()                                 \
  PARSER_OUTPUT_TYPES_2(CPARSEC4_INPUT_TYPE)

#define CPARSEC4_PARSER_INOUT_TYPES()                                    \
  PARSER_INOUT_TYPES(CPARSEC4_INPUT_TYPE)
#define CPARSEC4_PARSER_INOUT_TYPES_1()                                  \
  PARSER_INOUT_TYPES_1(CPARSEC4_INPUT_TYPE)
#define CPARSEC4_PARSER_INOUT_TYPES_2()                                  \
  PARSER_INOUT_TYPES_2(CPARSEC4_INPUT_TYPE)

// ---------------------------------------------------------------------
#include "cparsec4/parser.h"

#define GENERIC_PARSER(p)                                                \
  GENERIC((p), Parser, trait_Parser, CPARSEC4_PARSER_INOUT_TYPES())
#define trait_Parser(I, O)    trait(Parser(I, O))

#define g_run_parse(p, input) GENERIC_PARSER(p).run_parse((p), (input))
#define g_parse(p, input)     GENERIC_PARSER(p).parse((p), (input))

FOREACH(def_Parser, CPARSEC4_PARSER_INOUT_TYPES());

FOREACH(impl_Parser, CPARSEC4_PARSER_INOUT_TYPES());

// ---------------------------------------------------------------------
#include "cparsec4/parser/token.h"

#define PARSER_TOKEN()       trait_ParserToken(CPARSEC4_INPUT_TYPE)
#define trait_ParserToken(I) trait(ParserToken(I))

#define satisfy(predicate)   PARSER_TOKEN().Satisfy(predicate)
#define any()                PARSER_TOKEN().Any()
#define token(c)             PARSER_TOKEN().Token(c)
#define eof()                PARSER_TOKEN().Eof()

def_recursive_Fn(Tok, Tok, bool);
def_ParserToken(CPARSEC4_INPUT_TYPE);

impl_recursive_Fn(Tok, Tok, bool);
impl_ParserToken(CPARSEC4_INPUT_TYPE);

// ---------------------------------------------------------------------
#include "cparsec4/parser/repeat.h"

#define PARSER_REPEAT(p)                                                 \
  GENERIC((p), Parser, trait_ParserRepeat,                               \
          CPARSEC4_PARSER_INOUT_TYPES_1())
#define trait_ParserRepeat(I, O) trait(ParserRepeat(I, O))

#define many(p)                  PARSER_REPEAT(p).Many(p)
#define many1(p)                 PARSER_REPEAT(p).Many1(p)

FOREACH(def_ParserRepeat, CPARSEC4_PARSER_INOUT_TYPES_1());

FOREACH(impl_ParserRepeat, CPARSEC4_PARSER_INOUT_TYPES_1());

// ---------------------------------------------------------------------
#define PARSER(O)  Parser(Input, O)
#define PRESULT(O) ParseResult(Input, O)

// #define IMPLEMENT

#define TYPES_FOR_EQ JUST(Char, CStr, CString, PRESULT(Tok), Error)
#include "tgc/g_assert.h"

#define TYPES_FOR_DISPLAY JUST(PRESULT(Tok), PRESULT(Vec(Tok)), Error)
#include "tgc/fmt/print.h"

// ---------------------------------------------------------------------
static void test_parser0(void) {
  Input input = STR("Hello / こんにちは");
  PARSER(Unit) eof = eof();
  PARSER(Vec(Tok)) ps = many(any());
  {
    PRESULT(Vec(Tok)) r = g_parse(ps, input);
    PRESULT(Unit) r2 = g_parse(eof, r.input);
    println(r);
    trait(Drop(PRESULT(Unit))).drop(&r2);
    trait(Drop(PRESULT(Vec(Tok)))).drop(&r);
  }
  trait(Drop(PARSER(Vec(Tok)))).drop(&ps);
  trait(Drop(PARSER(Unit))).drop(&eof);
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
  trait(Drop(PRESULT(Tok))).drop(&r);
  trait(Drop(PARSER(Tok))).drop(&p);
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

#include <stdlib.h>
#include <locale.h>

int main(void) {
  setlocale(LC_ALL, getenv("LANG"));

  test_parser0();
  test_parser1();
  test_parser2();
  return 0;
}
