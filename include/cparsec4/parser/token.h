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

#define ParserToken(I)  TYPE_NAME(ParserToken, I)

#define Match(T, U)     Fn(TYPEOF(T), TYPEOF(U), bool)
#define Predicate(T)    Fn(TYPEOF(T), bool)

#define P_SATISFY(I)    FUNC_NAME(Satisfy, ParserToken(I))
#define P_ANY(I)        FUNC_NAME(Any, ParserToken(I))
#define P_TOKEN(I)      FUNC_NAME(Token, ParserToken(I))

#define P_EOF(I)        FUNC_NAME(Eof, ParserToken(I))

#define P_TOKENS(I)     FUNC_NAME(Tokens, ParserToken(I))
#define P_TOKENS_CMP(I) FUNC_NAME(Tokens_cmp, ParserToken(I))

#define def_ParserToken(I)                                               \
  require_trait(Parser(I, TOKEN(I)));                                    \
  require_trait(Parser(I, Unit));                                        \
                                                                         \
  def_recursive_Fn(TOKEN(I), TOKEN(I), bool);                            \
  Parser(I, TOKEN(I)) P_SATISFY(I)(Predicate(TOKEN(I)) predicate);       \
  Parser(I, TOKEN(I)) P_ANY(I)(void);                                    \
  Parser(I, TOKEN(I)) P_TOKEN(I)(TOKEN(I) c);                            \
                                                                         \
  Parser(I, Unit) P_EOF(I)(void);                                        \
                                                                         \
  Parser(I, CHUNK(I)) P_TOKENS(I)(CHUNK(I) s);                           \
  Parser(I, CHUNK(I))                                                    \
    P_TOKENS_CMP(I)(CHUNK(I) s, Match(TOKEN(I), TOKEN(I)) cmp);          \
                                                                         \
  END_OF_STATEMENT

#define impl_Satisfy(I)                                                  \
  impl_recursive_Fn(TOKEN(I), TOKEN(I), bool);                           \
  parser(I, TOKEN(I), P_SATISFY(I), Predicate(TOKEN(I))) {               \
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

#define impl_Any(I)                                                      \
  parser(I, TOKEN(I), P_ANY(I)) {                                        \
    StreamResult(TOKEN(I), ERROR(I)) r =                                 \
      trait(Stream(I)).take_one(&INPUT);                                 \
    if (r.is_err) {                                                      \
      EMPTY_ERR(I, TOKEN(I), r.err);                                     \
    }                                                                    \
    CONSUMED_OK(I, TOKEN(I), r.ok);                                      \
  }                                                                      \
  END_OF_STATEMENT

#define impl_Token(I)                                                    \
  Parser(I, TOKEN(I)) P_TOKEN(I)(TOKEN(I) c) {                           \
    Match(TOKEN(I), TOKEN(I)) f =                                        \
      trait(Match(TOKEN(I), TOKEN(I))).from(trait(Eq(TOKEN(I))).eq);     \
    Predicate(TOKEN(I)) predicate = fn_apply(f, c);                      \
    predicate.drop = f.drop; /* move ownership */                        \
    return P_SATISFY(I)(predicate);                                      \
  }                                                                      \
  END_OF_STATEMENT

#define impl_Eof(I)                                                      \
  parser(I, Unit, P_EOF(I)) {                                            \
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

#define impl_Tokens(I)                                                   \
  Parser(I, CHUNK(I)) P_TOKENS(I)(CHUNK(I) s) {                          \
    Match(TOKEN(I), TOKEN(I)) cmp =                                      \
      trait(Match(TOKEN(I), TOKEN(I))).from(trait(Eq(TOKEN(I))).eq);     \
    return P_TOKENS_CMP(I)(s, cmp);                                      \
  }                                                                      \
  END_OF_STATEMENT

#define impl_Tokens_cmp(I)                                               \
  parser(I, CHUNK(I), P_TOKENS_CMP(I), CHUNK(I),                         \
         Match(TOKEN(I), TOKEN(I))) {                                    \
    CHUNK(I) s = PARAM._0;                                               \
    bool consumed = false;                                               \
    for (;;) {                                                           \
      StreamResult(TOKEN(CHUNK(I)), ERROR(CHUNK(I))) r1 =                \
        trait(Stream(CHUNK(I))).take_one(&s);                            \
      if (r1.is_err) {                                                   \
        /* assert_eq(r1.err, end_of_input()); */                         \
        break;                                                           \
      }                                                                  \
      CHECKPOINT(I) cp = trait(Stream(I)).checkpoint(&INPUT);            \
      StreamResult(TOKEN(I), ERROR(I)) r2 =                              \
        trait(Stream(I)).take_one(&INPUT);                               \
      if (r2.is_err) {                                                   \
        trait(Stream(I)).reset(&INPUT, cp);                              \
        if (consumed) {                                                  \
          CONSUMED_ERR(I, CHUNK(I), r2.err);                             \
        } else {                                                         \
          EMPTY_ERR(I, CHUNK(I), r2.err);                                \
        }                                                                \
      }                                                                  \
      if (!fn_apply(PARAM._1, r1.ok, r2.ok)) {                           \
        trait(Stream(I)).reset(&INPUT, cp);                              \
        ERROR(I) err = trait(StreamErrorFor(I)).unexpected_token(r2.ok); \
        if (consumed) {                                                  \
          CONSUMED_ERR(I, CHUNK(I), err);                                \
        } else {                                                         \
          EMPTY_ERR(I, CHUNK(I), err);                                   \
        }                                                                \
      }                                                                  \
      consumed = true;                                                   \
    }                                                                    \
    if (consumed) {                                                      \
      CONSUMED_OK(I, CHUNK(I), PARAM._0);                                \
    } else {                                                             \
      EMPTY_OK(I, CHUNK(I), PARAM._0);                                   \
    }                                                                    \
  }                                                                      \
  END_OF_STATEMENT

#endif // CPARSEC4_PARSER_TOKEN_H_
