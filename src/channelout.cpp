#include "channelout.hpp"
#include "util.hpp"

namespace {

int ChannelOut_number(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	lua_pushinteger(L, self.index);
	return 1;
}

int ChannelOut_noteoff(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	lua_Integer pitch = luaL_checkinteger(L, 2);
	lua_Integer velocity = luaL_optinteger(L, 3, 0);

	luaL_argcheck(L, pitch >= 0 && pitch <= 0x7F, 2, "Pitch must be between 0 and 127");
	luaL_argcheck(L, velocity >= 0 && velocity <= 0x7F, 3, "Velocity must be between 0 and 127");

	self.send(0x80 + self.index, pitch, velocity);
	lua_settop(L, 1);
	return 1;
}

int ChannelOut_noteon(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	lua_Integer pitch = luaL_checkinteger(L, 2);
	lua_Integer velocity = luaL_checkinteger(L, 3);

	luaL_argcheck(L, pitch >= 0 && pitch <= 0x7F, 2, "Pitch must be between 0 and 127");
	luaL_argcheck(L, velocity >= 0 && velocity <= 0x7F, 3, "Velocity must be between 0 and 127");

	self.send(0x90 + self.index, pitch, velocity);
	lua_settop(L, 1);
	return 1;
}

int ChannelOut_aftertouch(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	lua_Integer pressure = luaL_checkinteger(L, 2);

	luaL_argcheck(L, pressure >= 0 && pressure <= 0x7F, 2, "Aftertouch pressure must be between 0 and 127");

	if (!lua_isnoneornil(L, 3)) {
		lua_Integer pitch = luaL_checkinteger(L, 3);
		luaL_argcheck(L, pitch >= 0 && pitch <= 0x7F, 3, "Pitch must be between 0 and 127");
		self.send(0xA0 + self.index, pitch, pressure);
	}
	else
		self.send(0xD0 + self.index, pressure);

	lua_settop(L, 1);
	return 1;
}

int ChannelOut_controlchange(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	lua_Integer cc = luaL_checkinteger(L, 2);

	if (lua_isboolean(L, 3)) {
		luaL_argcheck(L, cc >= 0 && cc <= 0x77, 2, "Controller number must be between 0 and 119");
		self.send(0xB0 + self.index, cc, lua_toboolean(L, 3) ? 0x7F : 0x00);
	}
	else {
		lua_Integer value = luaL_checkinteger(L, 3);
		if (lua_isnumber(L, 4)) {
			lua_Integer lsb = luaL_checkinteger(L, 4);
			luaL_argcheck(L, cc >= 0 && cc <= 0x1F, 2, "Controller number must be between 0 and 31");
			luaL_argcheck(L, lsb >= 0 && lsb <= 0x7F, 4, "Controller value must be between 0 and 127");
			self.send(0xB0 + self.index, cc, value);
			self.send(0xB0 + self.index, cc + 0x20, lsb);
		}
		else if (lua_isboolean(L, 4) && lua_toboolean(L, 4)) {
			luaL_argcheck(L, cc >= 0 && cc <= 0x1F, 2, "Controller number must be between 0 and 31");
			luaL_argcheck(L, value >= 0 && value <= 0x3FFF, 3, "Controller value must be between 0 and 16383");
			self.send(0xB0 + self.index, cc, (value >> 7) & 0x7Fu);
			self.send(0xB0 + self.index, cc + 0x20, value & 0x7Fu);
		}
		else {
			luaL_argcheck(L, cc >= 0 && cc <= 0x77, 2, "Controller number must be between 0 and 119");
			luaL_argcheck(L, value >= 0 && value <= 0x7F, 3, "Controller value must be between 0 and 127");
			self.send(0xB0 + self.index, cc, value);
		}
	}

	lua_settop(L, 1);
	return 1;
}

int ChannelOut_programchange(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	lua_Integer value = luaL_checkinteger(L, 2);

	luaL_argcheck(L, value >= 0 && value <= 0x7F, 2, "MIDI program number must be between 0 and 127");

	self.send(0xC0 + self.index, value);
	lua_settop(L, 1);
	return 1;
}

int ChannelOut_pitchbend(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	lua_Integer value = luaL_checkinteger(L, 2);

	luaL_argcheck(L, value >= 0 && value <= 0x3FFF, 2, "Pitch bend position must be between 0 and 16383");

	self.send(0xE0 + self.index, value & 0x7Fu, (value >> 7) & 0x7Fu);
	lua_settop(L, 1);
	return 1;
}

int ChannelOut_allsoundoff(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	self.send(0xB0 + self.index, 0x78, 0x00);
	lua_settop(L, 1);
	return 1;
}

int ChannelOut_resetallcontrollers(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	self.send(0xB0 + self.index, 0x79, 0x00);
	lua_settop(L, 1);
	return 1;
}

int ChannelOut_localcontrol(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	self.send(0xB0 + self.index, 0x7A, lua_toboolean(L, 2) ? 0x7F : 0x00);
	lua_settop(L, 1);
	return 1;
}

int ChannelOut_allnotesoff(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	self.send(0xB0 + self.index, 0x7B, 0x00);
	lua_settop(L, 1);
	return 1;
}

int ChannelOut_omni(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	self.send(0xB0 + self.index, lua_toboolean(L, 2) ? 0x7D : 0x7C, 0x00);
	lua_settop(L, 1);
	return 1;
}

int ChannelOut_mono(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	lua_Integer m = luaL_optinteger(L, 2, 0);
	luaL_argcheck(L, m >= 0 && m <= 16, 2, "Channel count must be between 0 and 16");
	self.send(0xB0 + self.index, 0x7E, m);
	lua_settop(L, 1);
	return 1;
}

int ChannelOut_poly(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	self.send(0xB0 + self.index, 0x7F, 0x00);
	lua_settop(L, 1);
	return 1;
}

int ChannelOut_rpn(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	lua_Integer number = luaL_checkinteger(L, 2);
	lua_Integer value = luaL_checkinteger(L, 3);

	luaL_argcheck(L, number >= 0 && number <= 0x3FFF, 2, "Parameter number must be between 0 and 16383");
	luaL_argcheck(L, value >= 0 && value <= 0x3FFF, 3, "Parameter value must be between 0 and 16383");

	self.send(0xB0 + self.index, 0x64, number & 0x7Fu);
	self.send(0xB0 + self.index, 0x65, (number >> 7) & 0x7Fu);
	self.send(0xB0 + self.index, 0x06, (value >> 7) & 0x7Fu);
	self.send(0xB0 + self.index, 0x26, value & 0x7Fu);
	lua_settop(L, 1);
	return 1;
}

int ChannelOut_nrpn(lua_State *L) {
	auto &self = getChannelOut(L, 1);
	lua_Integer number = luaL_checkinteger(L, 2);
	lua_Integer value = luaL_checkinteger(L, 3);

	luaL_argcheck(L, number >= 0 && number <= 0x3FFF, 2, "Parameter number must be between 0 and 16383");
	luaL_argcheck(L, value >= 0 && value <= 0x3FFF, 3, "Parameter value must be between 0 and 16383");

	self.send(0xB0 + self.index, 0x62, number & 0x7Fu);
	self.send(0xB0 + self.index, 0x63, (number >> 7) & 0x7Fu);
	self.send(0xB0 + self.index, 0x06, (value >> 7) & 0x7Fu);
	self.send(0xB0 + self.index, 0x26, value & 0x7Fu);
	lua_settop(L, 1);
	return 1;
}

#define METHOD(X) {#X, ChannelOut_##X}
const luaL_Reg ChannelOut_mt_index[] = {
	// accessors
	METHOD(number),

	// channel voice messages
	METHOD(noteoff),
	METHOD(noteon),
	METHOD(aftertouch),
	METHOD(controlchange),
	METHOD(programchange),
	METHOD(pitchbend),

	// channel mode messages
	METHOD(allsoundoff),
	METHOD(resetallcontrollers),
	METHOD(localcontrol),
	METHOD(allnotesoff),
	METHOD(omni),
	METHOD(mono),
	METHOD(poly),

	// helper methods
	METHOD(rpn),
	METHOD(nrpn),

	{nullptr, nullptr},
};
#undef METHOD

} // namespace

ChannelOut &getChannelOut(lua_State *L, int index) {
	return *reinterpret_cast<ChannelOut *>(
	 luaL_checkudata(L, index, MT_CHANNELOUT));
}

int ChannelOut_register(lua_State *L) {
	if (luaL_newmetatable(L, MT_CHANNELOUT)) {
		lua_pushliteral(L, "__index");
		luaNewLib(L, ChannelOut_mt_index);
		lua_rawset(L, -3);

		lua_pushliteral(L, "__gc");
		lua_pushcfunction(L, [] (lua_State *L) -> int {
			auto channel = reinterpret_cast<ChannelOut *>(lua_touserdata(L, 1));
			if (channel)
				channel->~ChannelOut();
			return 0;
		});
		lua_rawset(L, -3);
	}
	lua_pop(L, 1);
	return 0;
}
