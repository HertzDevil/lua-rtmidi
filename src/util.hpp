#pragma once

#include <cstddef>
#include <lua.hpp>

// std::size
template <typename T, std::size_t N>
constexpr std::size_t ARRAY_SIZE(const T (&)[N]) noexcept {
	return N;
}

// [-1, +0, e]
template <typename T>
T &luaGetUserdata(lua_State *L, int index, const char *tname) {
	luaL_checktype(L, index, LUA_TUSERDATA);
	if (!lua_getmetatable(L, index))
		luaL_typerror(L, index, tname);

	while (!lua_rawequal(L, -1, -2)) {
		if (!lua_istable(L, -1))
			luaL_typerror(L, index, tname);
		else {
			// [T mt] [v.mt]
			lua_pushliteral(L, "__index");
			// [T mt] [v.mt] "__index"
			lua_rawget(L, -2);
			// [T mt] [v.mt] [v.mt].__index
			lua_replace(L, -2);
			// [T mt] [v.mt].__index
			if (!lua_getmetatable(L, -1))
				luaL_typerror(L, index, tname);
			// [T mt] [v.mt].__index getmetatable([v.mt].__index)
			lua_replace(L, -2);
			// [T mt] getmetatable([v.mt].__index)
		}
	}

	lua_pop(L, 2);
	return *reinterpret_cast<T *>(lua_touserdata(L, index));
}

// [-0, +1, m]
template <std::size_t N>
void luaNewLib(lua_State *L, const luaL_Reg (&funcs)[N]) {
#if LUA_VERSION_NUM >= 503
	luaL_newlib(L, funcs);
#else
	lua_createtable(L, 0, N - 1);
	luaL_register(L, nullptr, funcs);
#endif
}

#if LUA_VERSION_NUM >= 502
inline int luaL_typerror(lua_State *L, int narg, const char *tname) {
	const char *msg = lua_pushfstring(L, "%s expected, got %s", tname, luaL_typename(L, narg));
	return luaL_argerror(L, narg, msg);
}
#endif
