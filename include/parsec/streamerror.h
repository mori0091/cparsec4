#ifndef PARSEC_STREAMERROR_H_
#define PARSEC_STREAMERROR_H_

#include "cstring.h"

#define StreamError(Token, Chunk, Error)                                 \
  TYPE_NAME(StreamError, TYPEOF(Token), TYPEOF(Chunk), TYPEOF(Error))

#define def_StreamError(Token, Chunk, Error)                             \
  def_trait(StreamError(Token, Chunk, Error)) {                          \
    Error (*unexpected_token)(Token token);                              \
    Error (*unexpected_range)(Chunk chunk);                              \
    Error (*unexpected_message)(CString msg);                            \
    Error (*unexpected_static_message)(const char* const msg);           \
                                                                         \
    Error (*expected_token)(Token token);                                \
    Error (*expected_range)(Chunk chunk);                                \
    Error (*expected_message)(CString msg);                              \
    Error (*expected_static_message)(const char* const msg);             \
                                                                         \
    Error (*message_token)(Token token);                                 \
    Error (*message_range)(Chunk chunk);                                 \
    Error (*message_message)(CString msg);                               \
    Error (*message_static_message)(const char* const msg);              \
                                                                         \
    Error (*end_of_input)(void);                                         \
  }

#include "stream.h"

#define StreamErrorFor(I) StreamError(TOKEN(I), CHUNK(I), ERROR(I))

#define def_StreamErrorFor(I)                                            \
  def_StreamError(TOKEN(I), CHUNK(I), ERROR(I))

#endif // PARSEC_STREAMERROR_H_
