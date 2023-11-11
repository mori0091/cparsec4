#ifndef PARSEC_PARSER_H_
#define PARSEC_PARSER_H_

/**
 * Typename of a parser.
 *
 * \param Input   Typename of input stream type of the parser.
 * \param Output  Typename of output type of the parser.
 */
#define Parser(Input, Output) internal

/**
 * Typename of result of a parser.
 *
 * ~~~c
 * typedef struct {
 *   Input input; ///< remaining input
 *   union {
 *     Output       ok;  ///< ok value
 *     ERROR(Input) err; ///< err value
 *   };
 *   bool is_err; ///< true if the result is err.
 * } ParseResult(Input, Output);
 * ~~~
 *
 * \param Input   Typename of input stream type of the parser.
 * \param Output  Typename of output type of the parser.
 */
#define ParseResult(Input, Output) internal

/**
 * Typename of reply of a parser.
 *
 * ~~~c
 * typedef struct {
 *    bool consumed;  ///< true if some tokens were consumed.
 *    ParseResult(Input, Output) result;  ///< ok or err reuslt.
 * } ParseReply(Input, Output);
 * ~~~
 *
 * \param Input   Typename of input stream type of the parser.
 * \param Output  Typename of output type of the parser.
 */
#define ParseReply(Input, Output) internal

/**
 * Define `Parser(Input, Output)`, `ParseResult(Input, Output)`, and
 * `trait(Parser(Input, Output))`.
 *
 * Also generate code of `trait(Parser(Input, Output))` if `IMPLEMENT`
 * macro was defined.
 *
 * \param Input   Typename of input stream type of the parser.
 * \param Output  Typename of output type of the parser.
 */
#if !defined(IMPLEMENT)
#define use_Parser(Input, Output) def_Parser(Input, Output)
#else
#define use_Parser(Input, Output)                                        \
  def_Parser(Input, Output);                                             \
  impl_Parser(Input, Output)
#endif

/**
 * Implement a parser.
 *
 * - `impl_parser(I, O, name) {...}`
 *   - Define constructor `Parser(I, O) name(void)`.
 * - `impl_parser(I, O, name, T1) {...}`
 *   - Define constructor `Parser(I, O) name(T1 x1)`.
 * - `impl_parser(I, O, name, T1, T2) {...}`
 *   - Define constructor `Parser(I, O) name(T1 x1, T2, x2)`.
 * - `impl_parser(I, O, name, T1, T2, T3) {...}`
 *   - Define constructor `Parser(I, O) name(T1 x1, T2 x2, T3 x3)`.
 *
 * A parser is constructed by calling to the defined constructor.
 *
 * Trailing block (`{...}`) will be executed when the parser is applied to
 * an input by `trait(Parser(I, O, E)).parse(parser, input)`.
 */
#define impl_parser(I, O, name, ...) internal

#define PARAM                        (args->_0)
#define INPUT                        (args->_1)

#define CONSUMED_OK(I, O, ok)                                            \
  return trait(ParseReply(I, O)).Ok(true, INPUT, (ok))

#define CONSUMED_ERR(I, O, err)                                          \
  return trait(ParseReply(I, O)).Err(true, INPUT, (err))

#define EMPTY_OK(I, O, ok)                                               \
  return trait(ParseReply(I, O)).Ok(false, INPUT, (ok))

#define EMPTY_ERR(I, O, err)                                             \
  return trait(ParseReply(I, O)).Err(false, INPUT, (err))

#include "./internal/parser.h"

#endif // PARSEC_PARSER_H_
