// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef PARSEC_STREAM_CHARSTREAM_H_
#define PARSEC_STREAM_CHARSTREAM_H_

#include "parsec/error/unexpectedparse.h"
#include "parsec/stream/slicestream.h"
#include "primitive.h"

require_type(Slice(char));

// \note `Slice(T)` is expanded to `pSlice_T`
#define TOKEN_pSlice_char      JUST(char)
#define CHUNK_pSlice_char      JUST(pSlice_char)
#define ERROR_pSlice_char      JUST(UnexpectedParse)
#define CHECKPOINT_pSlice_char JUST(pSlice_char)

def_StreamErrorFor(Slice(char));
def_Stream(Slice(char));

#endif // PARSEC_STREAM_CHARSTREAM_H_
