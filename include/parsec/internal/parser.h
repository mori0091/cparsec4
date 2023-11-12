// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef PARSEC_INTERNAL_PARSER_H_
#define PARSEC_INTERNAL_PARSER_H_

#include "copy.h"
#include "core.h"
#include "eq.h"
#include "fmt/debug.h"
#include "fmt/display.h"
#include "fmt/print.h"
#include "function.h"
#include "unit.h"

#undef ParseResult
#define ParseResult(I, O) TYPE_NAME(ParseResult, TYPEOF(I), TYPEOF(O))

#define def_ParseResult(I, O)                                            \
  enum {                                                                 \
    METADATA(ParseResult(I, O)) =                                        \
      E_DROP & (METADATA(O) | METADATA(ERROR(I)))                        \
  };                                                                     \
  typedef struct {                                                       \
    I input;                                                             \
    union {                                                              \
      O ok;                                                              \
      ERROR(I) err;                                                      \
    };                                                                   \
    bool is_err;                                                         \
  } ParseResult(I, O);                                                   \
                                                                         \
  require_trait(Debug(I), Debug(O), Debug(ERROR(I)));                    \
  def_Debug(ParseResult(I, O));                                          \
  def_Display(ParseResult(I, O));                                        \
                                                                         \
  require_trait(Drop(O), Drop(ERROR(I)));                                \
  def_Drop(ParseResult(I, O));                                           \
                                                                         \
  require_trait(Eq(I), Eq(O), Eq(ERROR(I)));                             \
  def_Eq(ParseResult(I, O));                                             \
                                                                         \
  def_trait(ParseResult(I, O)) {                                         \
    ParseResult(I, O) (*Ok)(I input, O ok);                              \
    ParseResult(I, O) (*Err)(I input, ERROR(I) err);                     \
  }

#define impl_ParseResult(I, O)                                           \
  impl_Debug_for(ParseResult(I, O)) {                                    \
    fmt_write(out, "ParseResult { input: ");                             \
    trait(Debug(I)).fmt(self.input, out);                                \
    if (self.is_err) {                                                   \
      fmt_write(out, ", err: ");                                         \
      trait(Debug(ERROR(I))).fmt(self.err, out);                         \
    } else {                                                             \
      fmt_write(out, ", ok: ");                                          \
      trait(Debug(O)).fmt(self.ok, out);                                 \
    }                                                                    \
    fmt_write(out, " }");                                                \
  }                                                                      \
  impl_Display_for(ParseResult(I, O)) {                                  \
    trait(Debug(ParseResult(I, O))).fmt(self, out);                      \
  }                                                                      \
                                                                         \
  /* trait Drop(ParseResult(I, O)) */                                    \
  impl_Drop_for(ParseResult(I, O)) {                                     \
    if (self) {                                                          \
      if (IS_DROP(ParseResult(I, O))) {                                  \
        if (self->is_err) {                                              \
          if (IS_DROP(ERROR(I)))                                         \
            trait(Drop(ERROR(I))).drop(&self->err);                      \
        } else {                                                         \
          if (IS_DROP(O))                                                \
            trait(Drop(O)).drop(&self->ok);                              \
        }                                                                \
      }                                                                  \
      *self = (ParseResult(I, O)){0};                                    \
    }                                                                    \
  }                                                                      \
  /* trait Eq ParseResult(I, O) */                                       \
  impl_Eq_for(ParseResult(I, O)) {                                       \
    return (trait(Eq(I)).eq(a.input, b.input) && a.is_err == b.is_err && \
            (a.is_err ? trait(Eq(ERROR(I))).eq(a.err, b.err)             \
                      : trait(Eq(O)).eq(a.ok, b.ok)));                   \
  }                                                                      \
                                                                         \
  static ParseResult(I, O)                                               \
    FUNC_NAME(Ok, ParseResult(I, O))(I input, O ok) {                    \
    return (ParseResult(I, O)){.input = input, .ok = ok};                \
  }                                                                      \
  static ParseResult(I, O)                                               \
    FUNC_NAME(Err, ParseResult(I, O))(I input, ERROR(I) err) {           \
    return (ParseResult(I, O)){                                          \
      .input = input, .err = err, .is_err = true};                       \
  }                                                                      \
  impl_trait(ParseResult(I, O)){                                         \
    .Ok = FUNC_NAME(Ok, ParseResult(I, O)),                              \
    .Err = FUNC_NAME(Err, ParseResult(I, O)),                            \
  };                                                                     \
  END_OF_STATEMENT

#undef ParseReply
#define ParseReply(I, O) TYPE_NAME(ParseReply, TYPEOF(I), TYPEOF(O))

#define def_ParseReply(I, O)                                             \
  enum { METADATA(ParseReply(I, O)) = METADATA(ParseResult(I, O)) };     \
  typedef struct {                                                       \
    bool consumed;                                                       \
    ParseResult(I, O) result;                                            \
  } ParseReply(I, O);                                                    \
                                                                         \
  def_trait(ParseReply(I, O)) {                                          \
    ParseReply(I, O) (*Ok)(bool consumed, I input, O ok);                \
    ParseReply(I, O) (*Err)(bool consumed, I input, ERROR(I) err);       \
  };                                                                     \
                                                                         \
  require_trait(Drop(ParseResult(I, O)));                                \
  def_Drop(ParseReply(I, O));                                            \
                                                                         \
  require_trait(Eq(ParseResult(I, O)));                                  \
  def_Eq(ParseReply(I, O))

#define impl_ParseReply(I, O)                                            \
  static ParseReply(I, O)                                                \
    FUNC_NAME(Ok, ParseReply(I, O))(bool consumed, I input, O ok) {      \
    return (ParseReply(I, O)){                                           \
      .consumed = consumed, .result.input = input, .result.ok = ok};     \
  }                                                                      \
  static ParseReply(I, O) FUNC_NAME(Err, ParseReply(I, O))(              \
    bool consumed, I input, ERROR(I) err) {                              \
    return (ParseReply(I, O)){                                           \
      .consumed = consumed,                                              \
      .result.input = input,                                             \
      .result.err = err,                                                 \
      .result.is_err = true,                                             \
    };                                                                   \
  }                                                                      \
  impl_trait(ParseReply(I, O)){                                          \
    .Ok = FUNC_NAME(Ok, ParseReply(I, O)),                               \
    .Err = FUNC_NAME(Err, ParseReply(I, O)),                             \
  };                                                                     \
                                                                         \
  impl_Drop_for(ParseReply(I, O)) {                                      \
    if (self) {                                                          \
      if (IS_DROP(ParseResult(I, O))) {                                  \
        trait(Drop(ParseResult(I, O))).drop(&self->result);              \
      }                                                                  \
      *self = (ParseReply(I, O)){0};                                     \
    }                                                                    \
  }                                                                      \
  impl_Eq_for(ParseReply(I, O)) {                                        \
    return (a.consumed == b.consumed &&                                  \
            trait(Eq(ParseResult(I, O))).eq(a.result, b.result));        \
  }                                                                      \
  END_OF_STATEMENT

#undef Parser
#define Parser(I, O)       TYPE_NAME(Parser, TYPEOF(I), TYPEOF(O))

#define UnParser(I, O)     Fn(UnParserArgs(I, O))
#define UnParserArgs(I, O) I, ParseReply(I, O)

#define def_Parser(I, O)                                                 \
  def_ParseResult(I, O);                                                 \
  def_ParseReply(I, O);                                                  \
                                                                         \
  def_recursive_Fn(UnParserArgs(I, O));                                  \
                                                                         \
  enum { METADATA(Parser(I, O)) = E_DROP };                              \
  typedef struct {                                                       \
    UnParser(I, O) unParser;                                             \
  } Parser(I, O);                                                        \
                                                                         \
  def_Drop(Parser(I, O));                                                \
                                                                         \
  def_trait(Parser(I, O)) {                                              \
    ParseReply(I, O) (*run_parser)(Parser(I, O) p, I input);             \
    ParseResult(I, O) (*parse)(Parser(I, O) p, I input);                 \
  }

#define impl_Parser(I, O)                                                \
  impl_ParseResult(I, O);                                                \
  impl_ParseReply(I, O);                                                 \
                                                                         \
  impl_recursive_Fn(UnParserArgs(I, O));                                 \
                                                                         \
  /* trait Drop(Parser(I, O)) */                                         \
  impl_Drop_for(Parser(I, O)) {                                          \
    if (self && self->unParser.args) {                                   \
      trait(Drop(UnParser(I, O))).drop(&self->unParser);                 \
      *self = (Parser(I, O)){0};                                         \
    }                                                                    \
  }                                                                      \
  /* trait Parser(I, O) */                                               \
  static ParseReply(I, O)                                                \
    FUNC_NAME(run_parser, Parser(I, O))(Parser(I, O) p, I input) {       \
    ParseReply(I, O) r = fn_apply(p.unParser, input);                    \
    return r;                                                            \
  }                                                                      \
  static ParseResult(I, O)                                               \
    FUNC_NAME(parse, Parser(I, O))(Parser(I, O) p, I input) {            \
    return FUNC_NAME(run_parser, Parser(I, O))(p, input).result;         \
  }                                                                      \
  impl_trait(Parser(I, O)) {                                             \
    .run_parser = FUNC_NAME(run_parser, Parser(I, O)),                   \
    .parse = FUNC_NAME(parse, Parser(I, O)),                             \
  }

#undef impl_parser
#define impl_parser(...) impl_parser_(__VA_ARGS__)
#define impl_parser_(I, O, ...)                                          \
  CAT(impl_parser, VARIADIC_SIZE(__VA_ARGS__))(I, O, __VA_ARGS__)

#define UnParserFnEnv(I, O, name) Tuple(ParserParam_##name, I)

#define impl_parser1(I, O, name)                                         \
  typedef Unit ParserParam_##name;                                       \
  enum { METADATA(ParserParam_##name) = 0 };                             \
  def_Drop(ParserParam_##name);                                          \
  def_Fn(ParserParam_##name, UnParserArgs(I, O));                        \
  Fn(ParserParam_##name, UnParserArgs(I, O)) name##_ctor_(void);         \
  static void name##_drop(void* p) {                                     \
    UnParser(I, O)* self = p;                                            \
    free(self->args);                                                    \
    *self = (UnParser(I, O)){0};                                         \
  }                                                                      \
  Parser(I, O) name(void) {                                              \
    Fn(ParserParam_##name, UnParserArgs(I, O)) f = name##_ctor_();       \
    UnParser(I, O) unParser = fn_apply(f, UNIT);                         \
    unParser.drop = name##_drop;                                         \
    return (Parser(I, O)){unParser};                                     \
  }                                                                      \
  fn(name##_ctor_, ParserParam_##name, UnParserArgs(I, O)) /* {...} */

#define impl_parser2(I, O, name, T1)                                     \
  typedef Tuple(T1) ParserParam_##name;                                  \
  enum { METADATA(ParserParam_##name) = E_DROP & METADATA(T1) };         \
  def_Drop(ParserParam_##name);                                          \
  impl_Drop_for(ParserParam_##name) {                                    \
    if (IS_DROP(T1))                                                     \
      trait(Drop(T1)).drop(&self->_0);                                   \
  }                                                                      \
  def_Fn(ParserParam_##name, UnParserArgs(I, O));                        \
  Fn(ParserParam_##name, UnParserArgs(I, O)) name##_ctor_(void);         \
  static void name##_drop(void* p) {                                     \
    UnParser(I, O)* self = p;                                            \
    UnParserFnEnv(I, O, name)* args = self->args;                        \
    if (IS_DROP(ParserParam_##name)) {                                   \
      ParserParam_##name* env = &args->_0;                               \
      trait(Drop(ParserParam_##name)).drop(env);                         \
    }                                                                    \
    free(args);                                                          \
    *self = (UnParser(I, O)){0};                                         \
  }                                                                      \
  Parser(I, O) name(T1 x1) {                                             \
    Fn(ParserParam_##name, UnParserArgs(I, O)) f = name##_ctor_();       \
    ParserParam_##name env = {x1};                                       \
    UnParser(I, O) unParser = fn_apply(f, env);                          \
    unParser.drop = name##_drop;                                         \
    return (Parser(I, O)){unParser};                                     \
  }                                                                      \
  fn(name##_ctor_, ParserParam_##name, UnParserArgs(I, O)) /* {...} */

#define impl_parser3(I, O, name, T1, T2)                                 \
  typedef Tuple(T1, T2) ParserParam_##name;                              \
  enum {                                                                 \
    METADATA(ParserParam_##name) =                                       \
      E_DROP & (METADATA(T1) | METADATA(T2))                             \
  };                                                                     \
  def_Drop(ParserParam_##name);                                          \
  impl_Drop_for(ParserParam_##name) {                                    \
    if (IS_DROP(T1))                                                     \
      trait(Drop(T1)).drop(&self->_0);                                   \
    if (IS_DROP(T2))                                                     \
      trait(Drop(T2)).drop(&self->_1);                                   \
  }                                                                      \
  def_Fn(ParserParam_##name, UnParserArgs(I, O));                        \
  Fn(ParserParam_##name, UnParserArgs(I, O)) name##_ctor_(void);         \
  static void name##_drop(void* p) {                                     \
    UnParser(I, O)* self = p;                                            \
    UnParserFnEnv(I, O, name)* args = self->args;                        \
    if (IS_DROP(ParserParam_##name)) {                                   \
      ParserParam_##name* env = &args->_0;                               \
      trait(Drop(ParserParam_##name)).drop(env);                         \
    }                                                                    \
    free(args);                                                          \
    *self = (UnParser(I, O)){0};                                         \
  }                                                                      \
  Parser(I, O) name(T1 x1, T2 x2) {                                      \
    Fn(ParserParam_##name, UnParserArgs(I, O)) f = name##_ctor_();       \
    ParserParam_##name env = {x1, x2};                                   \
    UnParser(I, O) unParser = fn_apply(f, env);                          \
    unParser.drop = name##_drop;                                         \
    return (Parser(I, O)){unParser};                                     \
  }                                                                      \
  fn(name##_ctor_, ParserParam_##name, UnParserArgs(I, O)) /* {...} */

#define impl_parser4(I, O, name, T1, T2, T3)                             \
  typedef Tuple(T1, T2, T3) ParserParam_##name;                          \
  enum {                                                                 \
    METADATA(ParserParam_##name) =                                       \
      E_DROP & (METADATA(T1) | METADATA(T2) | METADATA(T3))              \
  };                                                                     \
  def_Drop(ParserParam_##name);                                          \
  impl_Drop_for(ParserParam_##name) {                                    \
    if (IS_DROP(T1))                                                     \
      trait(Drop(T1)).drop(&self->_0);                                   \
    if (IS_DROP(T2))                                                     \
      trait(Drop(T2)).drop(&self->_1);                                   \
    if (IS_DROP(T3))                                                     \
      trait(Drop(T3)).drop(&self->_2);                                   \
  }                                                                      \
  def_Fn(ParserParam_##name, UnParserArgs(I, O));                        \
  Fn(ParserParam_##name, UnParserArgs(I, O)) name##_ctor_(void);         \
  static void name##_drop(void* p) {                                     \
    UnParser(I, O)* self = p;                                            \
    UnParserFnEnv(I, O, name)* args = self->args;                        \
    if (IS_DROP(ParserParam_##name)) {                                   \
      ParserParam_##name* env = &args->_0;                               \
      trait(Drop(ParserParam_##name)).drop(env);                         \
    }                                                                    \
    free(args);                                                          \
    *self = (UnParser(I, O)){0};                                         \
  }                                                                      \
  Parser(I, O) name(T1 x1, T2 x2, T3, x3) {                              \
    Fn(ParserParam_##name, UnParserArgs(I, O)) f = name##_ctor_();       \
    ParserParam_##name env = {x1, x2, x3};                               \
    UnParser(I, O) unParser = fn_apply(f, env);                          \
    unParser.drop = name##_drop;                                         \
    return (Parser(I, O)){unParser};                                     \
  }                                                                      \
  fn(name##_ctor_, ParserParam_##name, UnParserArgs(I, O)) /* {...} */

#endif // PARSEC_INTERNAL_PARSER_H_
