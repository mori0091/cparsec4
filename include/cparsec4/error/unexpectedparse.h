// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_ERROR_UNEXPECTEDPARSE_H_
#define CPARSEC4_ERROR_UNEXPECTEDPARSE_H_

#include "cparsec4/streamerror.h"

typedef enum UnexpectedParse {
  Eoi,
  Unexpected,
} UnexpectedParse;

#include "copy.h"
def_Copy(UnexpectedParse);

#include "eq.h"
def_Eq(UnexpectedParse);

#include "fmt/debug.h"
def_Debug(UnexpectedParse);

#include "fmt/display.h"
def_Display(UnexpectedParse);

// ---------------------------------------------------------------------
/**
 * Implement `trait(StreamError(Token, Chunk, UnexpectedParse))`.
 *
 * `impl_StreamError_for_UnexpectedParse(Token, Chunk)` means that:
 * - implements `trait(StreamError(Token, Chunk, UnexpectedParse))`
 * - for `UnexpectedParse`.
 *
 * \param Token  Typename of the token type.
 * \param Chunk  Typename of the chunk type.
 */
#define impl_StreamError_for_UnexpectedParse(Token, Chunk)               \
  static UnexpectedParse FUNC_NAME(                                      \
    token, StreamError(Token, Chunk, UnexpectedParse))(Token _) {        \
    ((void)_);                                                           \
    return Unexpected;                                                   \
  }                                                                      \
  static UnexpectedParse FUNC_NAME(                                      \
    range, StreamError(Token, Chunk, UnexpectedParse))(Chunk _) {        \
    ((void)_);                                                           \
    return Unexpected;                                                   \
  }                                                                      \
  static UnexpectedParse FUNC_NAME(                                      \
    message, StreamError(Token, Chunk, UnexpectedParse))(CString s) {    \
    trait(Drop(CString)).drop(&s);                                       \
    return Unexpected;                                                   \
  }                                                                      \
  static UnexpectedParse FUNC_NAME(                                      \
    static_message,                                                      \
    StreamError(Token, Chunk, UnexpectedParse))(const char* const _) {   \
    ((void)_);                                                           \
    return Unexpected;                                                   \
  }                                                                      \
  static UnexpectedParse FUNC_NAME(                                      \
    end_of_input, StreamError(Token, Chunk, UnexpectedParse))(void) {    \
    return Eoi;                                                          \
  }                                                                      \
  impl_trait(StreamError(Token, Chunk, UnexpectedParse)) {               \
    .unexpected_token =                                                  \
      FUNC_NAME(token, StreamError(Token, Chunk, UnexpectedParse)),      \
    .unexpected_range =                                                  \
      FUNC_NAME(range, StreamError(Token, Chunk, UnexpectedParse)),      \
    .unexpected_message =                                                \
      FUNC_NAME(message, StreamError(Token, Chunk, UnexpectedParse)),    \
    .unexpected_static_message = FUNC_NAME(                              \
      static_message, StreamError(Token, Chunk, UnexpectedParse)),       \
                                                                         \
    .expected_token =                                                    \
      FUNC_NAME(token, StreamError(Token, Chunk, UnexpectedParse)),      \
    .expected_range =                                                    \
      FUNC_NAME(range, StreamError(Token, Chunk, UnexpectedParse)),      \
    .expected_message =                                                  \
      FUNC_NAME(message, StreamError(Token, Chunk, UnexpectedParse)),    \
    .expected_static_message = FUNC_NAME(                                \
      static_message, StreamError(Token, Chunk, UnexpectedParse)),       \
                                                                         \
    .message_token =                                                     \
      FUNC_NAME(token, StreamError(Token, Chunk, UnexpectedParse)),      \
    .message_range =                                                     \
      FUNC_NAME(range, StreamError(Token, Chunk, UnexpectedParse)),      \
    .message_message =                                                   \
      FUNC_NAME(message, StreamError(Token, Chunk, UnexpectedParse)),    \
    .message_static_message = FUNC_NAME(                                 \
      static_message, StreamError(Token, Chunk, UnexpectedParse)),       \
                                                                         \
    .end_of_input = FUNC_NAME(                                           \
      end_of_input, StreamError(Token, Chunk, UnexpectedParse)),         \
  }

#endif // CPARSEC4_ERROR_UNEXPECTEDPARSE_H_
