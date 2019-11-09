#include "luartmidi.hpp"
#include "rtmidi.hpp"
#include "rtmidiin.hpp"
#include "rtmidiout.hpp"
#include "rtmidierror.hpp"
#include <chrono>

/* Currently mingw-std-threads' sleep routines have significantly less overhead than winpthreads' */
#ifdef USE_MINGW_THREADS
	#include <mingw.thread.h>
	namespace _this_thread = mingw_stdthread::this_thread;
#else
	#include <thread>
	namespace _this_thread = std::this_thread;
#endif

extern "C" LUA_RTMIDI_API int luaopen_luartmidi(lua_State *L) {
	lua_createtable(L, 0, 4);

	lua_pushliteral(L, "sleep");
	lua_pushcfunction(L, [] (lua_State *L) {
		auto s = std::chrono::duration<lua_Number>(luaL_checknumber(L, 1));
		_this_thread::sleep_for(s);
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
