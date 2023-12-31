// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_PARSER_REPEAT_H_
#define CPARSEC4_PARSER_REPEAT_H_

#include "cparsec4/parser.h"
#include "cparsec4/stream.h"
#include "tgc/vec.h"

#define ParserRepeat(I, O) TYPE_NAME(ParserRepeat, I, O)

#define P_MANY(I, O)       FUNC_NAME(Many, ParserRepeat(I, O))
#define P_MANY1(I, O)      FUNC_NAME(Many1, ParserRepeat(I, O))

#define def_ParserRepeat(I, O)                                           \
  require_trait(Parser(I, O));                                           \
  require_trait(Parser(I, Vec(O)));                                      \
                                                                         \
  Parser(I, Vec(O)) P_MANY(I, O)(Parser(I, O) p);                        \
  Parser(I, Vec(O)) P_MANY1(I, O)(Parser(I, O) p);                       \
                                                                         \
  END_OF_STATEMENT

#define impl_Many(I, O)                                                  \
  parser(I, Vec(O), P_MANY(I, O), Parser(I, O)) {                        \
    Vec(O) v = {0};                                                      \
    bool consumed = false;                                               \
    for (;;) {                                                           \
      ParseReply(I, O) r =                                               \
        trait(Parser(I, O)).run_parser(PARAM._0, INPUT);                 \
      INPUT = r.result.input;                                            \
      consumed |= r.consumed;                                            \
      if (r.result.is_err) {                                             \
        if (r.consumed) {                                                \
          trait(Drop(Vec(O))).drop(&v);                                  \
          CONSUMED_ERR(I, Vec(O), r.result.err);                         \
        }                                                                \
        if (IS_DROP(ParseReply(I, O))) {                                 \
          trait(Drop(ParseReply(I, O))).drop(&r);                        \
        }                                                                \
        break;                                                           \
      }                                                                  \
      trait(Vec(O)).push(&v, r.result.ok);                               \
    }                                                                    \
    if (consumed) {                                                      \
      CONSUMED_OK(I, Vec(O), v);                                         \
    } else {                                                             \
      EMPTY_OK(I, Vec(O), v);                                            \
    }                                                                    \
  }                                                                      \
  END_OF_STATEMENT

#define impl_Many1(I, O)                                                 \
  parser(I, Vec(O), P_MANY1(I, O), Parser(I, O)) {                       \
    Vec(O) v = {0};                                                      \
    bool consumed = false;                                               \
    for (;;) {                                                           \
      ParseReply(I, O) r =                                               \
        trait(Parser(I, O)).run_parser(PARAM._0, INPUT);                 \
      INPUT = r.result.input;                                            \
      consumed |= r.consumed;                                            \
      if (r.result.is_err) {                                             \
        if (r.consumed) {                                                \
          trait(Drop(Vec(O))).drop(&v);                                  \
          CONSUMED_ERR(I, Vec(O), r.result.err);                         \
        }                                                                \
        if (!consumed) {                                                 \
          EMPTY_ERR(I, Vec(O), r.result.err);                            \
        }                                                                \
        if (IS_DROP(ParseReply(I, O))) {                                 \
          trait(Drop(ParseReply(I, O))).drop(&r);                        \
        }                                                                \
        break;                                                           \
      }                                                                  \
      trait(Vec(O)).push(&v, r.result.ok);                               \
    }                                                                    \
    assert(consumed);                                                    \
    CONSUMED_OK(I, Vec(O), v);                                           \
  }                                                                      \
  END_OF_STATEMENT

#endif // CPARSEC4_PARSER_REPEAT_H_
