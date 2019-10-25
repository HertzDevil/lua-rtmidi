#pragma once

#include <lua.hpp>
#include <RtMidi.h>
#include "config.hpp"

constexpr const char MT_RTMIDIIN[] = "luartmidi.rtmidiin";
constexpr const char MT_RTMIDIIN_CLASS[] = "luartmidi.rtmidiin.__class";

extern LUA_RTMIDI_LOCAL RtMidiIn &getRtMidiIn(lua_State *L, int index);
extern LUA_RTMIDI_LOCAL int RtMidiIn_register(lua_State *L);
extern "C" {
extern LUA_RTMIDI_API int luaopen_luartmidi_rtmidiin(lua_State *L);
}
