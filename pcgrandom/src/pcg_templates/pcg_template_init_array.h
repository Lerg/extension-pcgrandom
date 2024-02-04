#define PCG_CASE(T) pcg = (void*)new T PCG_OPERATION; break;
#include "pcg_template_array.h"
#undef PCG_CASE
