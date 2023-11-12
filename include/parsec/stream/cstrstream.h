// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef PARSEC_STREAM_CSTRSTREAM_H_
#define PARSEC_STREAM_CSTRSTREAM_H_

#include "parsec/error/unexpectedparse.h"
#include "parsec/stream.h"
#include "parsec/streamerror.h"

// ---------------------------------------------------------------------
#include "cstr.h"
#include "utf8.h"

#define TOKEN_CStr      JUST(Char)
#define CHUNK_CStr      JUST(CStr)
#define ERROR_CStr      JUST(UnexpectedParse)
#define CHECKPOINT_CStr JUST(CStr)

def_StreamErrorFor(CStr);
def_Stream(CStr);

// ---------------------------------------------------------------------
#if defined(IMPLEMENT)
#endif

#endif // PARSEC_STREAM_CSTRSTREAM_H_
