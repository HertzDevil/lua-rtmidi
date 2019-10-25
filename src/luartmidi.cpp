#include "luartmidi.hpp"
#include "rtmidi.hpp"
#include "rtmidiin.hpp"
#include "rtmidiout.hpp"
#include "rtmidierror.hpp"
#include <thread>
#include <chrono>

__declspec(dllexport) int luaopen_luartmidi(lua_State *L) {
	lua_createtable(L, 0, 4);

	lua_pushliteral(L, "sleep");
	lua_pushcfunction(L, [] (lua_State *L) {
		auto s = std::chrono::duration<lua_Number>(luaL_checknumber(L, 1));
		std::this_thread::sleep_for(s);
		return 0;
	});
	lua_rawset(L, -3);

	lua_pushliteral(L, "RtMidi");
	luaopen_luartmidi_rtmidi(L);
	lua_rawset(L, -3);

	lua_pushliteral(L, "RtMidiIn");
	luaopen_luartmidi_rtmidiin(L);
	lua_rawset(L, -3);

	lua_pushliteral(L, "RtMidiOut");
	luaopen_luartmidi_rtmidiout(L);
	lua_rawset(L, -3);

	lua_pushliteral(L, "RtMidiError");
	luaopen_luartmidi_rtmidierror(L);
	lua_rawset(L, -3);

	return 1;
}
