// -*- coding: utf-8-unix -*-

// Copyright (c) 2023 Daishi Mori (mori0091)
//
// This software is released under the MIT License.
// See https://github.com/mori0091/cparsec4/blob/main/LICENSE
//
// GitHub cparsec4 project
// https://github.com/mori0091/cparsec4

#ifndef CPARSEC4_STREAM_CSTRSTREAM_H_
#define CPARSEC4_STREAM_CSTRSTREAM_H_

#include "cparsec4/error/unexpectedparse.h"
#include "cparsec4/stream.h"
#include "cparsec4/streamerror.h"

// ---------------------------------------------------------------------
#include "tgc/cstr.h"
#include "tgc/utf8.h"

#define TOKEN_CStr      JUST(Char)
#define CHUNK_CStr      JUST(CStr)
#define ERROR_CStr      JUST(UnexpectedParse)
#define CHECKPOINT_CStr JUST(CStr)

def_StreamErrorFor(CStr);
def_Stream(CStr);

#endif // CPARSEC4_STREAM_CSTRSTREAM_H_
