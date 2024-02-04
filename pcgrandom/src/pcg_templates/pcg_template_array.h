switch (pcg_type) {
	case PCG32_K2:     PCG_CASE(pcg32_k2)
	case PCG32_K64:    PCG_CASE(pcg32_k64)
	case PCG32_C64:    PCG_CASE(pcg32_c64)
	case PCG32_K1024:  PCG_CASE(pcg32_k1024)
	case PCG32_C1024:  PCG_CASE(pcg32_c1024)
	case PCG32_K16384: PCG_CASE(pcg32_k16384)
	//case PCG64_K32:    PCG_CASE(pcg64_k32)
	//case PCG64_C32:    PCG_CASE(pcg64_c32)
	//case PCG64_K1024:  PCG_CASE(pcg64_k1024)
	//case PCG64_C1024:  PCG_CASE(pcg64_c1024)
}
