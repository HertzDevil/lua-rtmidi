#pragma once

#include <lua.hpp>
#include <RtMidi.h>
#include "config.hpp"

constexpr const char MT_RTMIDIERROR[] = "luartmidi.rtmidierror";
constexpr const char MT_RTMIDIERROR_CLASS[] = "luartmidi.rtmidierror.__class";

extern LUA_RTMIDI_LOCAL RtMidiError &getRtMidiError(lua_State *L, int index);
extern LUA_RTMIDI_LOCAL int RtMidiError_register(lua_State *L);
extern "C" {
extern LUA_RTMIDI_API int luaopen_luartmidi_rtmidierror(lua_State *L);
}
