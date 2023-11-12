// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#include "tgc/char.h"
#include "tgc/cstr.h"
#include "tgc/cstring.h"
#include "tgc/primitive.h"
#include "tgc/unit.h"

#if 0
// ---------------------------------------------------------------------
#include "cparsec4/stream/charstream.h"
#define Input Slice(char)

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
#define Input  CStr
#define STR(s) cstr(s)
#define CHR(c) cstr(c)

#endif

// ---------------------------------------------------------------------
#define Token      TOKEN(Input)
#define Chunk      CHUNK(Input)
#define Error      ERROR(Input)
#define CheckPoint CHECKPOINT(Input)

#define E          StreamError(Token, Chunk, Error)

#define IMPLEMENT
#include "tgc/vec.h"
use_Vec(Token);

#include "cparsec4/parser.h"
use_Parser(Input, Unit);
use_Parser(Input, Token);
use_Parser(Input, Vec(Token));

#define PARSER(O)    Parser(Input, O)
#define PRESULT(O)   ParseResult(Input, O)

#define TYPES_FOR_EQ JUST(Char, CStr, CString, PRESULT(Token), Error)
#include "tgc/g_assert.h"

#define TYPES_FOR_DISPLAY JUST(PRESULT(Token), PRESULT(Vec(Token)), Error)
#include "tgc/fmt/print.h"

// ---------------------------------------------------------------------
#include "cparsec4/parser/token.h"

use_recursive_Fn(Token, Token, bool);
use_ParserToken(Input);

#include "cparsec4/parser/repeat.h"
use_ParserRepeat(Input, Token);

// ---------------------------------------------------------------------
#define PARSER_OUTPUT_TYPES(I)                                           \
  PARSER_OUTPUT_TYPES_1(I), PARSER_OUTPUT_TYPES_2(I)
#define PARSER_OUTPUT_TYPES_1(I) Unit, TOKEN(I), CHUNK(I)
#define PARSER_OUTPUT_TYPES_2(I) APPLY(Vec, PARSER_OUTPUT_TYPES_1(I))

#define PARSER_ARGS(I)           BIND(I, PARSER_OUTPUT_TYPES(I))
#define PARSER_ARGS_1(I)         BIND(I, PARSER_OUTPUT_TYPES_1(I))
#define PARSER_ARGS_2(I)         BIND(I, PARSER_OUTPUT_TYPES_2(I))

def_Vec(Unit);
// def_Parser(Input, Unit);
def_Parser(Input, Vec(Unit));
def_ParserRepeat(Input, Unit);

def_Vec(Chunk);
def_Parser(Input, Chunk);
def_Parser(Input, Vec(Chunk));
def_ParserRepeat(Input, Chunk);

// ---------------------------------------------------------------------
#define CPARSEC_INPUT_TYPE      Input

#define CPARSEC_PARSER_ARGS()   PARSER_ARGS(CPARSEC_INPUT_TYPE)
#define CPARSEC_PARSER_ARGS_1() PARSER_ARGS_1(CPARSEC_INPUT_TYPE)
#define CPARSEC_PARSER_ARGS_2() PARSER_ARGS_2(CPARSEC_INPUT_TYPE)

// ---------------------------------------------------------------------
#define GENERIC_PARSER(p)                                                \
  GENERIC((p), Parser, trait_Parser, CPARSEC_PARSER_ARGS())
#define trait_Parser(I, O)    trait(Parser(I, O))

#define g_run_parse(p, input) GENERIC_PARSER(p).run_parse((p), (input))
#define g_parse(p, input)     GENERIC_PARSER(p).parse((p), (input))

// ---------------------------------------------------------------------
#define PARSER_TOKEN()       trait_ParserToken(CPARSEC_INPUT_TYPE)
#define trait_ParserToken(I) trait(ParserToken(I))

#define satisfy(predicate)   PARSER_TOKEN().Satisfy(predicate)
#define any()                PARSER_TOKEN().Any()
#define token(c)             PARSER_TOKEN().Token(c)
#define eof()                PARSER_TOKEN().Eof()

// ---------------------------------------------------------------------
#define PARSER_REPEAT(p)                                                 \
  GENERIC((p), Parser, trait_ParserRepeat, CPARSEC_PARSER_ARGS_1())
#define trait_ParserRepeat(I, O) trait(ParserRepeat(I, O))

// ---------------------------------------------------------------------
#define many(p)  PARSER_REPEAT(p).Many(p)
#define many1(p) PARSER_REPEAT(p).Many1(p)

// ---------------------------------------------------------------------
static void test_parser0(void) {
  Input input = STR("Hello / こんにちは");
  PARSER(Unit) eof = eof();
  PARSER(Vec(Token)) ps = many(any());
  {
    PRESULT(Vec(Token)) r = g_parse(ps, input);
    PRESULT(Unit) r2 = g_parse(eof, r.input);
    println(r);
    trait(Drop(PRESULT(Unit))).drop(&r2);
    trait(Drop(PRESULT(Vec(Token)))).drop(&r);
  }
  trait(Drop(PARSER(Vec(Token)))).drop(&ps);
  trait(Drop(PARSER(Unit))).drop(&eof);
}

static void test_parser1(void) {
  Input input = STR("Hello / こんにちは");
  PARSER(Token) p = any();
  PRESULT(Token) r = g_parse(p, input);
  while (!r.is_err) {
    print("'", r.ok, "', ");
    r = g_parse(p, r.input);
  }
  println(r.err);
  trait(Drop(PRESULT(Token))).drop(&r);
  trait(Drop(PARSER(Token))).drop(&p);
}

static void test_parser2(void) {
  Scoped(PARSER(Token)) p = token(CHR("a"));
  {
    Scoped(PRESULT(Token)) expect =
      trait(PRESULT(Token)).Ok(STR("abc"), CHR("a"));
    Scoped(PRESULT(Token)) actual = g_parse(p, STR("aabc"));
    assert_eq(expect, actual);
  }
  {
    Scoped(PRESULT(Token)) expect =
      trait(PRESULT(Token)).Ok(STR("bc"), CHR("a"));
    Scoped(PRESULT(Token)) actual = g_parse(p, STR("abc"));
    assert_eq(expect, actual);
  }
  {
    Scoped(PRESULT(Token)) expect =
      trait(PRESULT(Token))
        .Err(STR("bc"), trait(E).unexpected_token(CHR("b")));
    Scoped(PRESULT(Token)) actual = g_parse(p, STR("bc"));
    assert_eq(expect, actual);
  }
  {
    Scoped(PRESULT(Token)) expect =
      trait(PRESULT(Token))
        .Err(STR("c"), trait(E).unexpected_token(CHR("c")));
    Scoped(PRESULT(Token)) actual = g_parse(p, STR("c"));
    assert_eq(expect, actual);
  }
  {
    Scoped(PRESULT(Token)) expect =
      trait(PRESULT(Token)).Err(STR(""), trait(E).end_of_input());
    Scoped(PRESULT(Token)) actual = g_parse(p, STR(""));
    assert_eq(expect, actual);
  }
}

int main(void) {
  test_parser0();
  test_parser1();
  test_parser2();
  return 0;
}
