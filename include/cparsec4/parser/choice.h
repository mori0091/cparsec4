// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_PARSER_CHOICE_H_
#define CPARSEC4_PARSER_CHOICE_H_

#include "cparsec4/parser.h"
#include "cparsec4/stream.h"
#include "tgc/vec.h"

#define ParserChoice(I, O) TYPE_NAME(ParserChoice, I, O)

#define P_EITHER(I, O)     FUNC_NAME(Either, ParserChoice(I, O))

#define def_ParserChoice(I, O)                                           \
  require_trait(Parser(I, O));                                           \
                                                                         \
  Parser(I, O) P_EITHER(I, O)(Parser(I, O) p1, Parser(I, O) p2);         \
                                                                         \
  END_OF_STATEMENT

#define impl_Either(I, O)                                                \
  parser(I, O, P_EITHER(I, O), Parser(I, O), Parser(I, O)) {             \
    ParseReply(I, O) r1 =                                                \
      trait(Parser(I, O)).run_parser(PARAM._0, INPUT);                   \
    if (r1.consumed || !r1.result.is_err) {                              \
      return r1;                                                         \
    }                                                                    \
    ParseReply(I, O) r2 =                                                \
      trait(Parser(I, O)).run_parser(PARAM._1, INPUT);                   \
    if (r2.result.is_err) {                                              \
      /* \todo merge r1.result.err and r2.result.err */                  \
    }                                                                    \
    return r2;                                                           \
  }                                                                      \
  END_OF_STATEMENT

#endif // CPARSEC4_PARSER_CHOICE_H_
