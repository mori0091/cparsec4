#include "parsec/error/unexpectedparse.h"

impl_Debug_for(UnexpectedParse) {
  const char* s =
    (self == Eoi ? "UnexpectedParse::Eoi"
                 : (self == Unexpected ? "UnexpectedParse::Unexpected"
                                       : "UnexpectedParse::<?>"));
  fmt_write(out, s);
}
