/* This module provides an assert() replacement that prints some contextual
 * informations before terminating because of a failed assert().
 */

/* There is intentionally no #include guard here. */

#if defined(NDEBUG)

#define assert(expr)

#elif defined(AUXILIARY)

/* no use to write auxiliary information on a failed assert() in auxiliary
 * programs */

#include <assert.h>

#else

void assert_impl(char const *assertion, char const *file, int line);

#define assert(expr) ((expr) ? (void)0 : assert_impl(#expr,__FILE__,__LINE__))

#endif
