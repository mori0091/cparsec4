// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_STREAM_H_
#define CPARSEC4_STREAM_H_

#include "tgc/result.h"

#define Stream(I)          TYPE_NAME(Stream, TYPEOF(I))
#define StreamResult(T, E) TYPE_NAME(StreamResult, TYPEOF(T), TYPEOF(E))

/**
 * Define trait Stream for an input stream type.
 *
 * To define `StreamError(T, C, E)` and `Stream(I)` traits, do as follows
 * ~~~c
 * // Ensure that `TOKEN(I)` is expanded to `T`.
 * #define Token_I      JUST(T)
 * // Ensure that `CHUNK(I)` is expanded to `C`.
 * #define Chunk_I      JUST(C)
 * // Ensure that `ERROR(I)` is expanded to `E`.
 * #define Error_I      JUST(E)
 * // Ensure that `CHECKPOINT(I)` is expanded to `CP`.
 * #define CheckPoint_I JUST(CP)
 *
 * def_StreamError(I);
 * def_Stream(I);
 * ~~~
 *
 * where
 * - `I` is the typename of input stream,
 * - `T` is the typename of token type of `I`,
 * - `C` is the typename of chunk type of `I`,
 * - `E` is the typename of error type of `I`,
 * - `CP` is the typename of check-point type of `I`.
 *
 *
 * \param Input  Typename of input stream.
 *
 * \note
 * Input, Token, Chunk, Error, and CheckPoint may all be of the same type.
 * However, the type names of Token and Chunk must be different.
 */
#define def_Stream(Input)                                                \
  def_Stream_(Input, TOKEN(Input), CHUNK(Input), ERROR(Input),           \
              CHECKPOINT(Input))

#define def_Stream_(Input, Token, Chunk, Error, CheckPoint)              \
  typedef Result(Token, Error) StreamResult(Token, Error);               \
  typedef Result(Chunk, Error) StreamResult(Chunk, Error);               \
  def_trait(Stream(Input)) {                                             \
    StreamResult(Token, Error) (*take_one)(Input * input);               \
    StreamResult(Chunk, Error) (*take_some)(Input * input, size_t n);    \
    CheckPoint (*checkpoint)(Input * input);                             \
    bool (*reset)(Input * input, CheckPoint cp);                         \
  }

/**
 * Expands to the typename of token type of the input stream.
 *
 * \param Input  Typename of input stream type.
 * \sa def_Stream()
 */
#define TOKEN(Input) EXTRACT_OR_DEFAULT(CAT(TOKEN_, Input), UNDEFINED)

/**
 * Expands to the typename of chunk type of the input stream.
 *
 * \param Input  Typename of input stream type.
 * \sa def_Stream()
 */
#define CHUNK(Input) EXTRACT_OR_DEFAULT(CAT(CHUNK_, Input), UNDEFINED)

/**
 * Expands to the typename of error type of the input stream.
 *
 * \param Input  Typename of input stream type.
 * \sa def_Stream()
 */
#define ERROR(Input) EXTRACT_OR_DEFAULT(CAT(ERROR_, Input), UNDEFINED)

/**
 * Expands to the typename of check-point type of the input stream.
 *
 * \param Input  Typename of input stream type.
 * \sa def_Stream()
 */
#define CHECKPOINT(Input)                                                \
  EXTRACT_OR_DEFAULT(CAT(CHECKPOINT_, Input), UNDEFINED)

#endif // CPARSEC4_STREAM_H_
