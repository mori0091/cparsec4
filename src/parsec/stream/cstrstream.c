#include "parsec/stream/cstrstream.h"

impl_StreamError_for_UnexpectedParse(TOKEN(CStr), CHUNK(CStr));

static size_t utf8_char_len(const CStr s) {
  if (is_utf8_1((unsigned char*)s.ptr))
    return 1;
  if (is_utf8_2((unsigned char*)s.ptr))
    return 2;
  if (is_utf8_3((unsigned char*)s.ptr))
    return 3;
  if (is_utf8_4((unsigned char*)s.ptr))
    return 4;
  return 0;
}

static StreamResult(CHUNK(CStr), ERROR(CStr))
  FUNC_NAME(take_some, Stream(CStr))(CStr* input, size_t n) {
  CHUNK(CStr) c = {.ptr = input->ptr, .len = 0};
  while (n) {
    size_t m;
    if (!input->len || !(m = utf8_char_len(*input))) {
      return (StreamResult(CHUNK(CStr), ERROR(CStr))){
        .err = trait(StreamError(TOKEN(CStr), CHUNK(CStr), ERROR(CStr)))
                 .end_of_input(),
        .is_err = true};
    }
    input->ptr += m;
    input->len -= m;
    c.len += m;
    n--;
  }
  return (StreamResult(CHUNK(CStr), ERROR(CStr))){.ok = c};
}

static StreamResult(TOKEN(CStr), ERROR(CStr))
  FUNC_NAME(take_one, Stream(CStr))(CStr* input) {
  StreamResult(CHUNK(CStr), ERROR(CStr)) r =
    FUNC_NAME(take_some, Stream(CStr))(input, 1);
  return *((StreamResult(TOKEN(CStr), ERROR(CStr))*)&r);
}

static CHECKPOINT(CStr) FUNC_NAME(checkpoint, Stream(CStr))(CStr* input) {
  return *input;
}

static bool FUNC_NAME(reset, Stream(CStr))(CStr* input,
                                           CHECKPOINT(CStr) cp) {
  *input = cp;
  return true;
}

impl_trait(Stream(CStr)){
  .take_one = FUNC_NAME(take_one, Stream(CStr)),
  .take_some = FUNC_NAME(take_some, Stream(CStr)),
  .checkpoint = FUNC_NAME(checkpoint, Stream(CStr)),
  .reset = FUNC_NAME(reset, Stream(CStr)),
};
