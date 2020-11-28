#ifndef LIFESENSOR_COMMON_MAP_H
#define LIFESENSOR_COMMON_MAP_H

/**
 * macro _MAP_EVAL*(...)
 * these macros let the preprocessor expand the inital arguments multiple times,
 * which results in multiple evaluation runs of the initial arguments.
 */
#define _MAP_EVAL1(...) __VA_ARGS__
#define _MAP_EVAL2(...) _MAP_EVAL1(_MAP_EVAL1(__VA_ARGS__))
#define _MAP_EVAL4(...) _MAP_EVAL2(_MAP_EVAL2(__VA_ARGS__))
#define _MAP_EVAL16(...) _MAP_EVAL4(_MAP_EVAL4(__VA_ARGS__))
#define _MAP_EVAL256(...) _MAP_EVAL16(_MAP_EVAL16(__VA_ARGS__))

/**
 * macro _MAP_NOP
 * this macro expands to nothing and can be used to break up another macro from
 * its arguments to stop the preprocessor from expanding the macro.
 */
#define _MAP_NOP /*empty*/

/**
 * macros _MAP_POP{0,1}(F, X, ...)
 * these macros are called as _MAP_POP{0,1}(F, ...) and take the first argument
 * of the variadic arguments and apply the function F to it.
 * If any variadic arguments remain they call their sibling macro which does
 * the same. The use of the sibling and _MAP_NOP macro ensures that the
 * preprocessor does not detect a recursion call and stops processing.
 */
#define _MAP_POP0(F, X, ...) \
    F(X)                     \
    __VA_OPT__(, _MAP_POP1 _MAP_NOP(F, __VA_ARGS__))
#define _MAP_POP1(F, X, ...) \
    F(X)                     \
    __VA_OPT__(, _MAP_POP0 _MAP_NOP(F, __VA_ARGS__))

/**
 * macro MAP(F, ...)
 * Maps the function F to each of the variadic arguments and uses a comma as
 * delimiter.
 * I.e. MAP(F,1,2,3) results in F(1), F(2), F(3)
 */
#define MAP(F, ...) __VA_OPT__(_MAP_EVAL256(_MAP_POP0(F, __VA_ARGS__)))

/**
 * see _MAP_POP{0,1}
 */
#define _MAP1_POP0(F, P0, X, ...) \
    F(P0, X)                      \
    __VA_OPT__(, _MAP1_POP1 _MAP_NOP(F, P0, __VA_ARGS__))
#define _MAP1_POP1(F, P0, X, ...) \
    F(P0, X)                      \
    __VA_OPT__(, _MAP1_POP0 _MAP_NOP(F, P0, __VA_ARGS__))

/**
 * macro MAP1(F, P0, ...)
 * Maps the parameterized function F to each of the variadic arguments
 * and uses a comma as delimiter.
 * I.e. MAP1(F,A,1,2,3) results in F(A,1), F(A,2), F(A,3)
 */
#define MAP1(F, P0, ...) \
    __VA_OPT__(_MAP_EVAL256(_MAP1_POP0(F, P0, __VA_ARGS__)))

/**
 * see _MAP_POP{0,1}
 */
#define _MAP2_POP0(F, P0, P1, X, ...) \
    F(P0, P1, X)                      \
    __VA_OPT__(, _MAP2_POP1 _MAP_NOP(F, P0, P1, __VA_ARGS__))
#define _MAP2_POP1(F, P0, P1, X, ...) \
    F(P0, P1, X)                      \
    __VA_OPT__(, _MAP2_POP0 _MAP_NOP(F, P0, P1, __VA_ARGS__))

/**
 * macro MAP2(F, P0, P1, ...)
 * Maps the parameterized function F to each of the variadic arguments
 * and uses a comma as delimiter.
 * I.e. MAP2(F,A,B,1,2,3) results in F(A,B,1), F(A,B,2), F(A,B,3)
 */
#define MAP2(F, P0, P1, ...) \
    __VA_OPT__(_MAP_EVAL256(_MAP2_POP0(F, P0, P1, __VA_ARGS__)))

/**
 * see _MAP_POP{0,1}
 */
#define _MAP3_POP0(F, P0, P1, P2, X, ...) \
    F(P0, P1, P2, X)                      \
    __VA_OPT__(, _MAP3_POP1 _MAP_NOP(F, P0, P1, P2, __VA_ARGS__))
#define _MAP3_POP1(F, P0, P1, P2, X, ...) \
    F(P0, P1, P2, X)                      \
    __VA_OPT__(, _MAP3_POP0 _MAP_NOP(F, P0, P1, P2, __VA_ARGS__))

/**
 * macro MAP3(F, P0, P1, P2, ...)
 * Maps the parameterized function F to each of the variadic arguments
 * and uses a comma as delimiter.
 * I.e. MAP3(F,A,B,C,1,2,3) results in F(A,B,C,1), F(A,B,C,2), F(A,B,C,3)
 */
#define MAP3(F, P0, P1, P2, ...) \
    __VA_OPT__(_MAP_EVAL256(_MAP3_POP0(F, P0, P1, P2, __VA_ARGS__)))

/**
 * see _MAP_POP{0,1}
 */
#define _MAPD_POP0(D, F, X, ...) \
    F(X)                         \
    __VA_OPT__(D _MAPD_POP1 _MAP_NOP(D, F, __VA_ARGS__))
#define _MAPD_POP1(D, F, X, ...) \
    F(X)                         \
    __VA_OPT__(D _MAPD_POP0 _MAP_NOP(D, F, __VA_ARGS__))

/**
 * macro MAP(D, F, ...)
 * Maps the function F to each of the variadic arguments and uses D as
 * delimiter.
 * I.e. MAP(+,F,1,2,3) results in F(1) + F(2) + F(3)
 */
#define MAPD(D, F, ...) __VA_OPT__(_MAP_EVAL256(_MAPD_POP0(D, F, __VA_ARGS__)))

/**
 * see _MAP_POP{0,1}
 */
#define _MAPD1_POP0(D, F, P0, X, ...) \
    F(P0, X)                          \
    __VA_OPT__(D _MAPD1_POP1 _MAP_NOP(D, F, P0, __VA_ARGS__))
#define _MAPD1_POP1(D, F, P0, X, ...) \
    F(P0, X)                          \
    __VA_OPT__(D _MAPD1_POP0 _MAP_NOP(D, F, P0, __VA_ARGS__))

/**
 * macro MAPD1(D, F, P0, ...)
 * Maps the parameterized function F to each of the variadic arguments
 * and uses D as delimiter.
 * I.e. MAPD1(+,F,A,1,2,3) results in F(A,1) + F(A,2) + F(A,3)
 */
#define MAPD1(D, F, P0, ...) \
    __VA_OPT__(_MAP_EVAL256(_MAPD1_POP0(D, F, P0, __VA_ARGS__)))

/**
 * see _MAP_POP{0,1}
 */
#define _MAPD2_POP0(D, F, P0, P1, X, ...) \
    F(P0, P1, X)                          \
    __VA_OPT__(D _MAPD2_POP1 _MAP_NOP(D, F, P0, P1, __VA_ARGS__))
#define _MAPD2_POP1(D, F, P0, P1, X, ...) \
    F(P0, P1, X)                          \
    __VA_OPT__(D _MAPD2_POP0 _MAP_NOP(D, F, P0, P1, __VA_ARGS__))

/**
 * macro MAPD2(D, F, P0, P1, ...)
 * Maps the parameterized function F to each of the variadic arguments
 * and uses D as delimiter.
 * I.e. MAPD2(+,F,A,B,1,2,3) results in F(A,B,1) + F(A,B,2) + F(A,B,3)
 */
#define MAPD2(D, F, P0, P1, ...) \
    __VA_OPT__(_MAP_EVAL256(_MAPD2_POP0(D, F, P0, P1, __VA_ARGS__)))

/**
 * see _MAP_POP{0,1}
 */
#define _MAPD3_POP0(D, F, P0, P1, P2, X, ...) \
    F(P0, P1, P2, X)                          \
    __VA_OPT__(D _MAPD3_POP1 _MAP_NOP(D, F, P0, P1, P2, __VA_ARGS__))
#define _MAPD3_POP1(D, F, P0, P1, P2, X, ...) \
    F(P0, P1, P2, X)                          \
    __VA_OPT__(D _MAPD3_POP0 _MAP_NOP(D, F, P0, P1, P2, __VA_ARGS__))

/**
 * macro MAPD3(D, F, P0, P1, P2, ...)
 * Maps the parameterized function F to each of the variadic arguments
 * and uses D as delimiter.
 * I.e. MAPD3(+,F,A,B,C,1,2,3) results in F(A,B,C,1) + F(A,B,C,2) + F(A,B,C,3)
 */
#define MAPD3(D, F, P0, P1, P2, ...) \
    __VA_OPT__(_MAP_EVAL256(_MAPD3_POP0(D, F, P0, P1, P2, __VA_ARGS__)))

#endif