#include "pcg/pcg_random.hpp"

namespace pcg_wrapper {
	enum PCG_TYPE {
		// 32-Bit Generators with 64-Bit State
		PCG32,
		PCG32_ONESEQ,
		PCG32_UNIQUE,
		PCG32_FAST,

		// 64-Bit Generators with 128-Bit State
		PCG64,
		PCG64_ONESEQ,
		PCG64_UNIQUE,
		PCG64_FAST,

		// Insecure
		PCG8_ONCE_INSECURE,
		PCG16_ONCE_INSECURE,
		PCG32_ONCE_INSECURE,
		PCG64_ONCE_INSECURE,
		//PCG128_ONCE_INSECURE,
		PCG8_ONESEQ_ONCE_INSECURE,
		PCG16_ONESEQ_ONCE_INSECURE,
		PCG32_ONESEQ_ONCE_INSECURE,
		PCG64_ONESEQ_ONCE_INSECURE,
		//PCG128_ONESEQ_ONCE_INSECURE,

		// Extended 32-Bit Generators
		PCG32_K2,
		PCG32_K2_FAST,
		PCG32_K64,
		PCG32_K64_ONESEQ,
		PCG32_K64_FAST,
		PCG32_C64,
		PCG32_C64_ONESEQ,
		PCG32_C64_FAST,
		PCG32_K1024,
		PCG32_K1024_FAST,
		PCG32_C1024,
		PCG32_C1024_FAST,
		PCG32_K16384,
		PCG32_K16384_FAST,

		// Extended 64-Bit Generators
		PCG64_K32,
		PCG64_K32_ONESEQ,
		PCG64_K32_FAST,
		PCG64_C32,
		PCG64_C32_ONESEQ,
		PCG64_C32_FAST,
		PCG64_K1024,
		PCG64_K1024_FAST,
		PCG64_C1024,
		PCG64_C1024_FAST
	};
	const uint32_t PCG_TYPE_MAX = PCG64_C1024_FAST;
	const uint64_t RES53_INT = 0x20000000000000;

	static bool supports_streams(PCG_TYPE pcg_type) {
		switch (pcg_type) {
			case PCG32:
			case PCG64:
			case PCG8_ONCE_INSECURE:
			case PCG16_ONCE_INSECURE:
			case PCG32_ONCE_INSECURE:
			case PCG64_ONCE_INSECURE:
			case PCG32_K2:
			case PCG32_K64:
			case PCG32_C64:
			case PCG32_K1024:
			case PCG32_C1024:
			case PCG32_K16384:
			case PCG64_K32:
			case PCG64_C32:
			case PCG64_K1024:
			case PCG64_C1024:
				return true;
			default:
				return false;
		}
	}

	static uint32_t get_required_array_size(PCG_TYPE pcg_type) {
		switch (pcg_type) {
			case PCG32_K2:
				return 2;
			//case PCG64_K32:
			//case PCG64_C32:
			//	return 32;
			case PCG32_K64:
			case PCG32_C64:
				return 64;
			case PCG32_K1024:
			case PCG32_C1024:
			//case PCG64_K1024:
			//case PCG64_C1024:
				return 1024;
			case PCG32_K16384:
				return 16384;
			default:
				return 0;
		}
	}

	struct PCGWrapper {
		PCG_TYPE pcg_type;
		void *pcg;

		PCGWrapper(PCG_TYPE pcg_type, uint64_t seed) : pcg_type(pcg_type), pcg(nullptr) {
			#define PCG_OPERATION (seed)
			#include "pcg_templates/pcg_template_init.h"
			#undef PCG_OPERATION
		}

		PCGWrapper(PCG_TYPE pcg_type, uint64_t seed, uint64_t stream) : pcg_type(pcg_type), pcg(nullptr) {
			#define PCG_OPERATION (seed, stream)
			#include "pcg_templates/pcg_template_init_stream.h"
			#undef PCG_OPERATION
		}

		PCGWrapper(PCG_TYPE pcg_type, uint32_t *array, uint64_t seed) : pcg_type(pcg_type), pcg(nullptr) {
			#define PCG_OPERATION (array, seed)
			#include "pcg_templates/pcg_template_init_array.h"
			#undef PCG_OPERATION
		}

		PCGWrapper(PCG_TYPE pcg_type, uint32_t *array, uint64_t seed, uint64_t stream) : pcg_type(pcg_type), pcg(nullptr) {
			#define PCG_OPERATION (array, seed, stream)
			#include "pcg_templates/pcg_template_init_array.h"
			#undef PCG_OPERATION
		}

		//PCGWrapper(PCG_TYPE pcg_type, uint64_t *array, uint64_t seed) : pcg_type(pcg_type), pcg(nullptr) {
		//	#define PCG_OPERATION (array, seed)
		//	#include "pcg_templates/pcg_template_init_array.h"
		//	#undef PCG_OPERATION
		//}

		//PCGWrapper(PCG_TYPE pcg_type, uint64_t *array, uint64_t seed, uint64_t stream) : pcg_type(pcg_type), pcg(nullptr) {
		//	#define PCG_OPERATION (array, seed, stream)
		//	#include "pcg_templates/pcg_template_init_array.h"
		//	#undef PCG_OPERATION
		//}

		~PCGWrapper() {
			if (pcg != nullptr) {
				#include "pcg_templates/pcg_template_delete.h"
			}
		}

		uint64_t next() {
			#define PCG_OPERATION ()
			#include "pcg_templates/pcg_template_deref.h"
			#undef PCG_OPERATION
		}

		uint64_t next_res53() {
			#define PCG_OPERATION (RES53_INT)
			#include "pcg_templates/pcg_template_deref.h"
			#undef PCG_OPERATION
		}

		uint64_t bound(uint64_t upper_bound) {
			#define PCG_OPERATION (upper_bound)
			#include "pcg_templates/pcg_template_deref.h"
			#undef PCG_OPERATION
		}

		void advance(uint64_t delta) {
			#define PCG_OPERATION advance(delta)
			#include "pcg_templates/pcg_template_ref_nonkdd.h"
			#undef PCG_OPERATION
		}

		void backstep(uint64_t delta) {
			#define PCG_OPERATION backstep(delta)
			#include "pcg_templates/pcg_template_ref_nonkdd.h"
			#undef PCG_OPERATION
		}

		uint64_t max() {
			#define PCG_OPERATION max()
			#include "pcg_templates/pcg_template_ref.h"
			#undef PCG_OPERATION
		}

		uint64_t period_pow2() {
			#define PCG_OPERATION period_pow2()
			#include "pcg_templates/pcg_template_ref.h"
			#undef PCG_OPERATION
		}

		uint64_t streams_pow2() {
			#define PCG_OPERATION streams_pow2()
			#include "pcg_templates/pcg_template_ref.h"
			#undef PCG_OPERATION
		}

		bool wrapped() {
			#define PCG_OPERATION wrapped()
			#include "pcg_templates/pcg_template_ref.h"
			#undef PCG_OPERATION
		}
	};
}
