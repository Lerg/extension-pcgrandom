#include "extension.h"
#include "pcg_wrapper.h"

using namespace pcg_wrapper;

// Check if the first argument to a Lua function is the PCG RNG Wrapper lightuserdata variable.
static PCGWrapper *get_rng_lightuserda(lua_State *L) {
	if (lua_islightuserdata(L, 1)) {
		return (PCGWrapper *)lua_touserdata(L, 1);
	} else {
		luaL_error(L, "Expected lightuserdata, got %s. Wrong type for the rng variable: '%s'.", luaL_typename(L, 1), lua_tostring(L, 1));
	}
	return nullptr;
}

// Check if the argument to a Lua function is a number variable.
static uint32_t get_uint32(lua_State *L, int index, const char *variable_name, bool *success) {
	if (lua_isnumber(L, index)) {
		*success = true;
		return lua_tointeger(L, index);
	} else {
		*success = false;
		luaL_error(L, "Expected number, got %s. Wrong type for the '%s' variable: '%s'.", luaL_typename(L, index), variable_name, lua_tostring(L, index));
	}
	return 0;
}

// Check if the argument to a Lua function is a number variable.
static uint64_t get_uint64(lua_State *L, int index, const char *variable_name, bool *success) {
	if (lua_isnumber(L, index)) {
		*success = true;
		return lua_tointeger(L, index);
	} else {
		*success = false;
		luaL_error(L, "Expected number, got %s. Wrong type for the '%s' variable: '%s'.", luaL_typename(L, index), variable_name, lua_tostring(L, index));
	}
	return 0;
}

// Check if the argument to a Lua function is a table variable.
static bool is_table(lua_State *L, int index, const char *variable_name) {
	if (!lua_istable(L, index)) {
		luaL_error(L, "Expected table, got %s. Wrong type for the '%s' variable: '%s'.", luaL_typename(L, index), variable_name, lua_tostring(L, index));
		return false;
	}
	return true;
}

static int lua_new_rng(lua_State *L) {
	bool pcg_type_success, seed_success;
	uint32_t pcg_type_int = get_uint32(L, 1, "pcg_type", &pcg_type_success);
	uint64_t seed = get_uint64(L, 2, "seed", &seed_success);

	bool stream_success = false;
	uint64_t stream;
	if (lua_isnumber(L, 3)) {
		stream = get_uint64(L, 3, "stream", &stream_success);
	}
	if (pcg_type_success && seed_success) {
		if (pcg_type_int >= 0 && pcg_type_int <= PCG_TYPE_MAX) {
			PCG_TYPE pcg_type = (PCG_TYPE)pcg_type_int;
			PCGWrapper *rng = nullptr;
			if (stream_success) {
				if (supports_streams(pcg_type)) {
					rng = new PCGWrapper(pcg_type, seed, stream);
				} else {
					luaL_error(L, "The selected generator does not support streams.");
				}
			} else {
				rng = new PCGWrapper(pcg_type, seed);
			}
			if (rng != nullptr) {
				lua_pushlightuserdata(L, rng);
				return 1;
			}
		} else {
			luaL_error(L, "Wrong pcg_type id, got %u.", pcg_type_int);
		}
	}
	return 0;
}

static int lua_new_rng_array(lua_State *L) {
	bool pcg_type_success, seed_success;
	uint32_t pcg_type_int = get_uint32(L, 1, "pcg_type", &pcg_type_success);
	uint64_t seed = get_uint64(L, 3, "seed", &seed_success);
	bool stream_success = false;
	uint64_t stream;
	const int stream_index = 4;
	if (lua_isnumber(L, stream_index)) {
		stream = get_uint64(L, stream_index, "stream", &stream_success);
	}
	if (pcg_type_success && seed_success) {
		if (pcg_type_int >= 0 && pcg_type_int <= PCG_TYPE_MAX) {
			PCG_TYPE pcg_type = (PCG_TYPE)pcg_type_int;
			const int array_index = 2;
			if (is_table(L, array_index, "array")) {
				size_t array_size = lua_objlen(L, array_index);
				uint32_t required_array_size = get_required_array_size(pcg_type);
				if (array_size == required_array_size && required_array_size > 0) {
					uint32_t array[array_size];
					bool success;
					for (int i = 1; i <= array_size; ++i) {
						lua_rawgeti(L, array_index, i);
						uint32_t value = get_uint32(L, -1, "array element", &success);
						lua_pop(L, 1);
						if (success) {
							array[i - 1] = value;
						} else {
							break;
						}
					}
					if (success) {
						PCGWrapper *rng = nullptr;
						if (stream_success) {
							if (supports_streams(pcg_type)) {
								rng = new PCGWrapper(pcg_type, array, seed, stream);
							} else {
								luaL_error(L, "The selected generator does not support streams.");
							}
						} else {
							rng = new PCGWrapper(pcg_type, array, seed);
						}
						if (rng != nullptr) {
							lua_pushlightuserdata(L, rng);
							return 1;
						}
					}
				} else {
					if (required_array_size == 0) {
						luaL_error(L, "The selected generator does not support array initialization.");
					} else {
						luaL_error(L, "Wrong array size, must have %d elements.", required_array_size);
					}
				}
			}
		} else {
			luaL_error(L, "Wrong pcg_type id, got %u.", pcg_type_int);
		}
	}
	return 0;
}

static int lua_next(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->next());
		return 1;
	}
	return 0;
}

static int lua_bound(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		bool success;
		uint64_t upper_bound = get_uint64(L, 2, "upper_bound", &success);
		if (success) {
			lua_pushnumber(L, rng->bound(upper_bound));
			return 1;
		}
	}
	return 0;
}

static int lua_range(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		bool lower_success, upper_success;
		uint64_t lower_bound = get_uint64(L, 2, "lower_bound", &lower_success);
		uint64_t upper_bound = get_uint64(L, 3, "upper_bound", &upper_success);
		if (lower_success && upper_success) {
			if (upper_bound > lower_bound) {
				lua_pushnumber(L, lower_bound + rng->bound(upper_bound - lower_bound));
				return 1;
			} else {
				luaL_error(L, "upper_bound must be larger than lower_bound, got from %" PRIu64 " to %" PRIu64 ".", lower_bound, upper_bound);
			}
		}
	}
	return 0;
}

static int lua_advance(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		bool success;
		uint64_t delta = get_uint64(L, 2, "delta", &success);
		if (success) {
			rng->advance(delta);
		}
	}
	return 0;
}

static int lua_backstep(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		bool success;
		uint64_t delta = get_uint64(L, 2, "delta", &success);
		if (success) {
			rng->backstep(delta);
		}
	}
	return 0;
}

static int lua_max(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->max());
		return 1;
	}
	return 0;
}

static int lua_period_pow2(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->period_pow2());
		return 1;
	}
	return 0;
}

static int lua_streams_pow2(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->streams_pow2());
		return 1;
	}
	return 0;
}

static int lua_wrapped(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushboolean(L, rng->wrapped());
		return 1;
	}
	return 0;
}

static int lua_next_uint64(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		uint64_t value = rng->next();
		uint8_t buffer[8];
		for (int i = 0; i < 8; ++i) {
			buffer[i] = (value >> (i * 8)) & 0xff;
		}
		lua_pushlstring(L, (const char *)buffer, 8);
		return 1;
	}
	return 0;
}

static int lua_next_real1(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, (double)rng->next() / (double)rng->max());
		return 1;
	}
	return 0;
}

static int lua_next_real2(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		double value = (double)rng->next() / (double)rng->max();
		lua_pushnumber(L, value == 1.0 ? value - __DBL_EPSILON__ : value);
		return 1;
	}
	return 0;
}

static int lua_next_res53(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->next_res53());
		return 1;
	}
	return 0;
}

static int lua_d4(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->bound(4) + 1);
		return 1;
	}
	return 0;
}

static int lua_d6(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->bound(6) + 1);
		return 1;
	}
	return 0;
}

static int lua_d8(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->bound(8) + 1);
		return 1;
	}
	return 0;
}

static int lua_d10(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->bound(10) + 1);
		return 1;
	}
	return 0;
}

static int lua_d12(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->bound(12) + 1);
		return 1;
	}
	return 0;
}

static int lua_d20(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->bound(20) + 1);
		return 1;
	}
	return 0;
}

static int lua_toss(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->bound(2));
		return 1;
	}
	return 0;
}

static int lua_card(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->bound(52) + 1);
		return 2;
	}
	return 0;
}

static int lua_card2(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		lua_pushnumber(L, rng->bound(4) + 1);
		lua_pushnumber(L, rng->bound(13) + 1);
		return 2;
	}
	return 0;
}

static int lua_shuffle(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		const int array_index = 2;
		if (is_table(L, array_index, "array")) {
			size_t array_size = lua_objlen(L, array_index);
			if (array_size > 0) {
				for (size_t i = array_size; i > 1; --i) {
					uint64_t j = 1 + rng->bound(i);

					lua_rawgeti(L, array_index, i);
					int value_i = luaL_ref(L, LUA_REGISTRYINDEX);
					lua_rawgeti(L, array_index, j);

					lua_rawseti(L, array_index, i);
					lua_getref(L, value_i);
					lua_rawseti(L, array_index, j);

					luaL_unref(L, LUA_REGISTRYINDEX, value_i);
				}
			}
		}
	}
	return 0;
}

// Delete the random generator and free the alocated memory for it.
static int lua_delete(lua_State *L) {
	PCGWrapper *rng = get_rng_lightuserda(L);
	if (rng != nullptr) {
		delete rng;
	}
	return 0;
}

static const luaL_reg lua_functions[] = {
	{"new_rng", lua_new_rng},
	{"new_rng_array", lua_new_rng_array},
	{"next", lua_next},
	{"bound", lua_bound},
	{"range", lua_range},
	{"advance", lua_advance},
	{"backstep", lua_backstep},
	{"max", lua_max},
	{"period_pow2", lua_period_pow2},
	{"streams_pow2", lua_streams_pow2},
	{"wrapped", lua_wrapped},
	{"next_uint64", lua_next_uint64},
	{"next_real1", lua_next_real1},
	{"next_real2", lua_next_real2},
	{"next_res53", lua_next_res53},
	{"d4", lua_d4},
	{"d6", lua_d6},
	{"d8", lua_d8},
	{"d10", lua_d10},
	{"d12", lua_d12},
	{"d20", lua_d20},
	{"toss", lua_toss},
	{"card", lua_card},
	{"card2", lua_card2},
	{"shuffle", lua_shuffle},
	{"delete", lua_delete},
	{0, 0}
};

dmExtension::Result APP_INITIALIZE(dmExtension::AppParams *params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result APP_FINALIZE(dmExtension::AppParams *params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result INITIALIZE(dmExtension::Params *params) {
	luaL_register(params->m_L, EXTENSION_NAME_STRING, lua_functions);

	#define SET_FIELD(name) lua_pushnumber(params->m_L, name); lua_setfield(params->m_L, -2, #name);

	SET_FIELD(PCG32)
	SET_FIELD(PCG32_ONESEQ)
	SET_FIELD(PCG32_UNIQUE)
	SET_FIELD(PCG32_FAST)
	SET_FIELD(PCG64)
	SET_FIELD(PCG64_ONESEQ)
	SET_FIELD(PCG64_UNIQUE)
	SET_FIELD(PCG64_FAST)
	SET_FIELD(PCG8_ONCE_INSECURE)
	SET_FIELD(PCG16_ONCE_INSECURE)
	SET_FIELD(PCG32_ONCE_INSECURE)
	SET_FIELD(PCG64_ONCE_INSECURE)
	//SET_FIELD(PCG128_ONCE_INSECURE)
	SET_FIELD(PCG8_ONESEQ_ONCE_INSECURE)
	SET_FIELD(PCG16_ONESEQ_ONCE_INSECURE)
	SET_FIELD(PCG32_ONESEQ_ONCE_INSECURE)
	SET_FIELD(PCG64_ONESEQ_ONCE_INSECURE)
	//SET_FIELD(PCG128_ONESEQ_ONCE_INSECURE)
	SET_FIELD(PCG32_K2)
	SET_FIELD(PCG32_K2_FAST)
	SET_FIELD(PCG32_K64)
	SET_FIELD(PCG32_K64_ONESEQ)
	SET_FIELD(PCG32_K64_FAST)
	SET_FIELD(PCG32_C64)
	SET_FIELD(PCG32_C64_ONESEQ)
	SET_FIELD(PCG32_C64_FAST)
	SET_FIELD(PCG32_K1024)
	SET_FIELD(PCG32_K1024_FAST)
	SET_FIELD(PCG32_C1024)
	SET_FIELD(PCG32_C1024_FAST)
	SET_FIELD(PCG32_K16384)
	SET_FIELD(PCG32_K16384_FAST)
	SET_FIELD(PCG64_K32)
	SET_FIELD(PCG64_K32_ONESEQ)
	SET_FIELD(PCG64_K32_FAST)
	SET_FIELD(PCG64_C32)
	SET_FIELD(PCG64_C32_ONESEQ)
	SET_FIELD(PCG64_C32_FAST)
	SET_FIELD(PCG64_K1024)
	SET_FIELD(PCG64_K1024_FAST)
	SET_FIELD(PCG64_C1024)
	SET_FIELD(PCG64_C1024_FAST)

	#undef SET_FIELD

	lua_pop(params->m_L, 1);
	return dmExtension::RESULT_OK;
}

dmExtension::Result UPDATE(dmExtension::Params *params) {
	return dmExtension::RESULT_OK;
}

void EXTENSION_ON_EVENT(dmExtension::Params *params, const dmExtension::Event *event) {}

dmExtension::Result FINALIZE(dmExtension::Params *params) {
	return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, EXTENSION_NAME_STRING, APP_INITIALIZE, APP_FINALIZE, INITIALIZE, UPDATE, EXTENSION_ON_EVENT, FINALIZE)
