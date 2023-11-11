#include "parsec/stream/charstream.h"

impl_StreamError_for_UnexpectedParse(char, Slice(char));
impl_Stream_for_Slice(char, UnexpectedParse);
