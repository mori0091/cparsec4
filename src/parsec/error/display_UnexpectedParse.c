#include "parsec/error/unexpectedparse.h"

impl_Display_for(UnexpectedParse) {
  const char* s =
    (self == Eoi ? "<end of input>"
                 : (self == Unexpected ? "<unexpected>" : "<?>"));
  fmt_write(out, s);
}
