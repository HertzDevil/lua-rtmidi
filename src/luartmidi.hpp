#pragma once

#include <lua.hpp>

extern "C" {
__declspec(dllexport) extern int luaopen_luartmidi(lua_State *L);
}
