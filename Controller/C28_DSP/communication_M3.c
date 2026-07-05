
#include "communication_M3.h"

short WriteTo_CTOM_MSGRAM_Float(unsigned short offset, float value) {
	// COPY_PASTE

	float *XMEM_pw;
	if (offset > 0x3FF) {
		return -1;
	}
	else {
		XMEM_pw = (float *) (0x0003F800 + offset);
		*XMEM_pw = value;
		return 0;
	}
}

short WriteTo_CTOM_MSGRAM(unsigned short offset, short value) {
	// COPY_PASTE

	short *XMEM_pw;
	if (offset > 0x3FF) {
		return -1;
	}
	else {
		XMEM_pw = (short *) (0x0003F800 + offset);
		*XMEM_pw = value;
		return 0;
	}
}

short ReadFrom_MTOC_MSGRAM(short offset) {
	// COPY_PASTE

	short *XMEM_pw;
	short result;
	if (offset > 0x3FF) {
		return 0;
	}
	else {
		XMEM_pw = (short *) (0x0003FC00 + offset);
		result = *XMEM_pw;
		return result;
	}
}

float ReadFrom_MTOC_MSGRAM_Float(short offset) {
	// COPY_PASTE

	float *XMEM_pw;
	float result;
	if (offset > 0x3FF) {
		return 0;
	}
	else {
		XMEM_pw = (float *) (0x0003FC00 + offset);
		result = *XMEM_pw;
		return result;
	}
}
