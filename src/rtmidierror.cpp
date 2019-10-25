#include "rtmidierror.hpp"
#include "util.hpp"

namespace {

int RtMidiError_ctor(lua_State *L) {
	std::size_t sz;
	const char *message = luaL_checklstring(L, 2, &sz);
	lua_Integer typ = luaL_optinteger(L, 3, RtMidiError::Type::UNSPECIFIED);

	auto midierror = reinterpret_cast<RtMidiError *>(
	 lua_newuserdata(L, sizeof(RtMidiError)));
	new (midierror) RtMidiError {message, static_cast<RtMidiError::Type>(typ)};
	luaL_getmetatable(L, MT_RTMIDIERROR);
	lua_setmetatable(L, -2);
	return 1;
}

int RtMidiError_dtor(lua_State *L) {
	auto midierror = reinterpret_cast<RtMidiError *>(lua_touserdata(L, 1));
	if (midierror)
		midierror->~RtMidiError();
	return 0;
}

int RtMidiError_printmessage(lua_State *L) {
	auto &self = getRtMidiError(L, 1);
	self.printMessage();
	return 0;
}

int RtMidiError_gettype(lua_State *L) {
	auto &self = getRtMidiError(L, 1);
	lua_pushinteger(L, self.getType());
	return 1;
}

int RtMidiError_getmessage(lua_State *L) {
	auto &self = getRtMidiError(L, 1);
	auto msg = self.getMessage();
	lua_pushlstring(L, msg.c_str(), msg.size());
	return 1;
}

#define METHOD(X) {#X, RtMidiError_##X}
const luaL_Reg RtMidiError_mt_index[] = {
	// finalizer
	{"_finalize", RtMidiError_dtor},

	// method wrappers
	METHOD(printmessage),
	METHOD(gettype),
	METHOD(getmessage),
	{"what", RtMidiError_getmessage},

	{nullptr, nullptr},
};
#undef METHOD

#define ENUM(X) {RtMidiError::Type::X, #X}
const std::pair<RtMidiError::Type, const char *> RTMIDIERROR_TYPE_VALUES[] = {
	ENUM(WARNING),
	ENUM(DEBUG_WARNING),
	ENUM(UNSPECIFIED),
	ENUM(NO_DEVICES_FOUND),
	ENUM(INVALID_DEVICE),
	ENUM(MEMORY_ERROR),
	ENUM(INVALID_PARAMETER),
	ENUM(INVALID_USE),
	ENUM(DRIVER_ERROR),
	ENUM(SYSTEM_ERROR),
	ENUM(THREAD_ERROR),
};
#undef ENUM

} // namespace

RtMidiError &getRtMidiError(lua_State *L, int index) {
	luaL_getmetatable(L, MT_RTMIDIERROR);
	return luaGetUserdata<RtMidiError>(L, index, "RtMidiError");
}

int RtMidiError_register(lua_State *L) {
	// instance methods
	if (luaL_newmetatable(L, MT_RTMIDIERROR)) {
		lua_pushliteral(L, "__index");
		luaNewLib(L, RtMidiError_mt_index);
		lua_rawset(L, -3);

		lua_pushliteral(L, "__tostring");
		lua_pushcfunction(L, RtMidiError_getmessage);
		lua_rawset(L, -3);

		lua_pushliteral(L, "__gc");
		lua_pushcfunction(L, RtMidiError_dtor);
		lua_rawset(L, -3);
	}
	lua_pop(L, 1);

	// constructor
	lua_newtable(L);
	if (luaL_newmetatable(L, MT_RTMIDIERROR_CLASS)) {
		lua_pushliteral(L, "__call");
		lua_pushcfunction(L, RtMidiError_ctor);
		lua_rawset(L, -3);
	}
	lua_setmetatable(L, -2);

	lua_pushliteral(L, "Type");
	lua_createtable(L, ARRAY_SIZE(RTMIDIERROR_TYPE_VALUES), ARRAY_SIZE(RTMIDIERROR_TYPE_VALUES));
	for (const auto &p : RTMIDIERROR_TYPE_VALUES) {
		lua_pushstring(L, p.second);
		lua_rawseti(L, -2, p.first);
		lua_pushstring(L, p.second);
		lua_pushinteger(L, p.first);
		lua_rawset(L, -3);
	}
	lua_rawset(L, -3);

	return 1;
}

extern "C" {

__declspec(dllexport) int luaopen_luartmidi_rtmidierror(lua_State *L) {
	return RtMidiError_register(L);
}

}
