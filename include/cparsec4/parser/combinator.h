// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_PARSER_COMBINATOR_H_
#define CPARSEC4_PARSER_COMBINATOR_H_

#include "cparsec4/parser.h"
#include "cparsec4/stream.h"

#define ParserCombinator(I, O) TYPE_NAME(ParserCombinator, I, O)

#define P_ATTEMPT(I, O)        FUNC_NAME(Attempt, ParserCombinator(I, O))

#define def_ParserCombinator(I, O)                                       \
  require_trait(Parser(I, O));                                           \
                                                                         \
  Parser(I, O) P_ATTEMPT(I, O)(Parser(I, O) p);                          \
                                                                         \
  END_OF_STATEMENT

#define impl_Attempt(I, O)                                               \
  parser(I, O, P_ATTEMPT(I, O), Parser(I, O)) {                          \
    ParseReply(I, O) r =                                                 \
      trait(Parser(I, O)).run_parser(PARAM._0, INPUT);                   \
    if (r.consumed && r.result.is_err) {                                 \
      EMPTY_ERR(I, O, r.result.err);                                     \
    }                                                                    \
    return r;                                                            \
  }                                                                      \
  END_OF_STATEMENT

#endif // CPARSEC4_PARSER_COMBINATOR_H_
