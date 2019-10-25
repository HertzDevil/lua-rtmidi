#pragma once

#include <lua.hpp>
#include <RtMidi.h>

constexpr const char MT_RTMIDIERROR[] = "luartmidi.rtmidierror";
constexpr const char MT_RTMIDIERROR_CLASS[] = "luartmidi.rtmidierror.__class";

extern RtMidiError &getRtMidiError(lua_State *L, int index);
extern int RtMidiError_register(lua_State *L);
extern "C" {
__declspec(dllexport) extern int luaopen_luartmidi_rtmidierror(lua_State *L);
}
