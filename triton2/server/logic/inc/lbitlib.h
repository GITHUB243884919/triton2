#ifndef _LBITLIB_H_
#define _LBITLIB_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#define TOBIT(L, n, res)    ((void)(res), luaL_checkinteger((L), (n)))

#define MONADIC(name, op)                                       \
  static int bit_ ## name(lua_State *L) {                       \
    lua_Number f;                                               \
    lua_pushinteger(L, op TOBIT(L, 1, f));        \
    return 1;                                                   \
  }

#define VARIADIC(name, op)                      \
  static int bit_ ## name(lua_State *L) {       \
    lua_Number f;                               \
    int n = lua_gettop(L), i = 0;               \
    lua_Integer w = TOBIT(L, 1, f);             \
    for (i = 2; i <= n; i++)                    \
      w op TOBIT(L, i, f);                      \
    lua_pushinteger(L, w);        \
    return 1;                                   \
  }

#define LOGICAL_SHIFT(name, op)                                         \
  static int bit_ ## name(lua_State *L) {                               \
    lua_Number f;                                                       \
    lua_pushinteger(L, (lua_Integer)TOBIT(L, 1, f) op (unsigned)luaL_checknumber(L, 2)); \
    return 1;                                                           \
  }

#define ARITHMETIC_SHIFT(name, op)                                      \
  static int bit_ ## name(lua_State *L) {                               \
    lua_Number f;                                                       \
    lua_pushinteger(L, (lua_Integer)TOBIT(L, 1, f) op (unsigned)luaL_checknumber(L, 2)); \
    return 1;                                                           \
  }

MONADIC(cast,  +)
MONADIC(bnot,  ~)
VARIADIC(band, &=)
VARIADIC(bor,  |=)
VARIADIC(bxor, ^=)
ARITHMETIC_SHIFT(lshift,  <<)
LOGICAL_SHIFT(rshift,     >>)
ARITHMETIC_SHIFT(arshift, >>)

#endif 
