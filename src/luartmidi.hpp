#pragma once

#include <lua.hpp>
#include "config.hpp"

extern "C" {
extern LUA_RTMIDI_API int luaopen_luartmidi(lua_State *L);
}
