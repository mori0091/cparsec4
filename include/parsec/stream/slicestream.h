// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef PARSEC_STREAM_SLICESTREAM_H_
#define PARSEC_STREAM_SLICESTREAM_H_

#include "parsec/stream.h"
#include "parsec/streamerror.h"

// ---------------------------------------------------------------------
#include "slice.h"

/**
 * Implement `trait(Stream(Slice(T)))` to use `Slice(T)` as input stream.
 *
 * - `Slice(T)` as type of input stream.
 * - `T` as type of token of the input stream.
 * - `Slice(T)` as type of chunk of tokens.
 * - `E` as type of stream error.
 * - `Slice(T)` as type of check-point.
 *
 * \param T Typename of token of the input stream type.
 * \param E Typename of stream error.
 */
#define impl_Stream_for_Slice(T, E)                                      \
  static StreamResult(T, E)                                              \
    FUNC_NAME(take_one, Stream(Slice(T)))(Slice(T) * input) {            \
    if (!input->len) {                                                   \
      return (StreamResult(T, E)){                                       \
        .err = trait(StreamError(T, Slice(T), E)).end_of_input(),        \
        .is_err = true};                                                 \
    }                                                                    \
    input->len--;                                                        \
    return (StreamResult(T, E)){.ok = *input->ptr++};                    \
  }                                                                      \
  static StreamResult(Slice(T), E)                                       \
    FUNC_NAME(take_some, Stream(Slice(T)))(Slice(T) * input, size_t n) { \
    if (input->len < n) {                                                \
      return (StreamResult(Slice(T), E)){                                \
        .err = trait(StreamError(T, Slice(T), E)).end_of_input(),        \
        .is_err = true};                                                 \
    }                                                                    \
    input->len -= n;                                                     \
    input->ptr += n;                                                     \
    return (StreamResult(Slice(T), E)){                                  \
      .ok = {.ptr = input->ptr - n, .len = n}};                          \
  }                                                                      \
  static Slice(T)                                                        \
    FUNC_NAME(checkpoint, Stream(Slice(T)))(Slice(T) * input) {          \
    return *input;                                                       \
  }                                                                      \
  static bool FUNC_NAME(reset, Stream(Slice(T)))(Slice(T) * input,       \
                                                 Slice(T) cp) {          \
    *input = cp;                                                         \
    return true;                                                         \
  }                                                                      \
  impl_trait(Stream(Slice(T))) {                                         \
    .take_one = FUNC_NAME(take_one, Stream(Slice(T))),                   \
    .take_some = FUNC_NAME(take_some, Stream(Slice(T))),                 \
    .checkpoint = FUNC_NAME(checkpoint, Stream(Slice(T))),               \
    .reset = FUNC_NAME(reset, Stream(Slice(T))),                         \
  }

#endif // PARSEC_STREAM_SLICESTREAM_H_
