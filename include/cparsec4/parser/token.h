// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_PARSER_TOKEN_H_
#define CPARSEC4_PARSER_TOKEN_H_

#include "cparsec4/parser.h"
#include "cparsec4/stream.h"
#include "tgc/unit.h"

#define ParserToken(I) TYPE_NAME(ParserToken, I)

#define Match(T, U)    Fn(TYPEOF(T), TYPEOF(U), bool)
#define Predicate(T)   Fn(TYPEOF(T), bool)

#if !defined(IMPLEMENT)
#define use_ParserToken(I) def_ParserToken(I)
#else
#define use_ParserToken(I)                                               \
  def_ParserToken(I);                                                    \
  impl_ParserToken(I)
#endif

#define def_ParserToken(I)                                               \
  def_recursive_Fn(TOKEN(I), TOKEN(I), bool);                            \
  require_trait(Parser(I, TOKEN(I)));                                    \
  require_trait(Parser(I, Unit));                                        \
                                                                         \
  def_trait(ParserToken(I)) {                                            \
    Parser(I, TOKEN(I)) (*Satisfy)(Predicate(TOKEN(I)) predicate);       \
    Parser(I, TOKEN(I)) (*Any)(void);                                    \
    Parser(I, TOKEN(I)) (*Token)(TOKEN(I) c);                            \
    Parser(I, Unit) (*Eof)(void);                                        \
  }

#define impl_ParserToken(I)                                              \
  impl_recursive_Fn(TOKEN(I), TOKEN(I), bool);                           \
  impl_ParserToken_Satisfy(I);                                           \
  impl_ParserToken_Any(I);                                               \
  impl_ParserToken_Token(I);                                             \
  impl_ParserToken_Eof(I);                                               \
                                                                         \
  impl_trait(ParserToken(I)) {                                           \
    .Satisfy = FUNC_NAME(Satisfy, ParserToken(I)),                       \
    .Any = FUNC_NAME(Any, ParserToken(I)),                               \
    .Token = FUNC_NAME(Token, ParserToken(I)),                           \
    .Eof = FUNC_NAME(Eof, ParserToken(I)),                               \
  }

#define impl_ParserToken_Satisfy(I)                                      \
  parser(I, TOKEN(I), FUNC_NAME(Satisfy, ParserToken(I)),                \
         Predicate(TOKEN(I))) {                                          \
    CHECKPOINT(I) checkpoint = trait(Stream(I)).checkpoint(&INPUT);      \
    StreamResult(TOKEN(I), ERROR(I)) r =                                 \
      trait(Stream(I)).take_one(&INPUT);                                 \
    if (r.is_err) {                                                      \
      EMPTY_ERR(I, TOKEN(I), r.err);                                     \
    }                                                                    \
    if (fn_apply(PARAM._0, r.ok)) {                                      \
      CONSUMED_OK(I, TOKEN(I), r.ok);                                    \
    }                                                                    \
    if (trait(Stream(I)).reset(&INPUT, checkpoint)) {                    \
      ERROR(I)                                                           \
      err = trait(StreamErrorFor(I)).unexpected_token(r.ok);             \
      EMPTY_ERR(I, TOKEN(I), err);                                       \
    }                                                                    \
    ERROR(I)                                                             \
    err = trait(StreamErrorFor(I))                                       \
            .unexpected_static_message("backtrack too far");             \
    CONSUMED_ERR(I, TOKEN(I), err);                                      \
  }                                                                      \
  END_OF_STATEMENT

#define impl_ParserToken_Any(I)                                          \
  parser(I, TOKEN(I), FUNC_NAME(Any, ParserToken(I))) {                  \
    StreamResult(TOKEN(I), ERROR(I)) r =                                 \
      trait(Stream(I)).take_one(&INPUT);                                 \
    if (r.is_err) {                                                      \
      EMPTY_ERR(I, TOKEN(I), r.err);                                     \
    }                                                                    \
    CONSUMED_OK(I, TOKEN(I), r.ok);                                      \
  }                                                                      \
  END_OF_STATEMENT

#define impl_ParserToken_Token(I)                                        \
  Parser(I, TOKEN(I)) FUNC_NAME(Token, ParserToken(I))(TOKEN(I) c) {     \
    Match(TOKEN(I), TOKEN(I)) f =                                        \
      trait(Match(TOKEN(I), TOKEN(I))).from(trait(Eq(TOKEN(I))).eq);     \
    Predicate(TOKEN(I)) predicate = fn_apply(f, c);                      \
    predicate.drop = f.drop; /* move ownership */                        \
    return FUNC_NAME(Satisfy, ParserToken(I))(predicate);                \
  }                                                                      \
  END_OF_STATEMENT

#define impl_ParserToken_Eof(I)                                          \
  parser(I, Unit, FUNC_NAME(Eof, ParserToken(I))) {                      \
    CHECKPOINT(I) cp = trait(Stream(I)).checkpoint(&INPUT);              \
    StreamResult(TOKEN(I), ERROR(I)) r =                                 \
      trait(Stream(I)).take_one(&INPUT);                                 \
    trait(Stream(I)).reset(&INPUT, cp);                                  \
    if (r.is_err) {                                                      \
      ERROR(I) eoi = trait(StreamErrorFor(I)).end_of_input();            \
      if (trait(Eq(ERROR(I))).eq(r.err, eoi)) {                          \
        if (IS_DROP(ERROR(I))) {                                         \
          trait(Drop(ERROR(I))).drop(&r.err);                            \
        }                                                                \
        EMPTY_OK(I, Unit, UNIT);                                         \
      }                                                                  \
      if (IS_DROP(ERROR(I))) {                                           \
        trait(Drop(ERROR(I))).drop(&r.err);                              \
      }                                                                  \
    } else {                                                             \
      if (IS_DROP(TOKEN(I))) {                                           \
        trait(Drop(TOKEN(I))).drop(&r.ok);                               \
      }                                                                  \
    }                                                                    \
    ERROR(I)                                                             \
    err =                                                                \
      trait(StreamErrorFor(I)).expected_static_message("end of input");  \
    EMPTY_ERR(I, Unit, err);                                             \
  }                                                                      \
  END_OF_STATEMENT

#endif // CPARSEC4_PARSER_TOKEN_H_
