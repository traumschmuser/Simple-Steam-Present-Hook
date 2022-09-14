#include "memory_helper.h"

//compares bytes, needed for pattern scanning
bool memory::MemCompare(const char* bData, const char* bSig, const char* szMask) {
	for (; *szMask; ++szMask, ++bData, ++bSig) {
		if (*szMask == 'x' && *bData != *bSig) {
			return false;
		}
	}
	return (*szMask == NULL);
}

//pattern scan
uintptr_t memory::PatternScan(const char* pattern, const char* mask, uintptr_t begin, unsigned int size)
{
	unsigned int patternLength = (unsigned int)strlen(pattern);

	for (unsigned int i = 0; i < size - patternLength; i++)
	{
		if (MemCompare((const char*)(begin + i), (const char*)pattern, mask)) {
			return begin + i;
		}
	}
	return (uintptr_t)nullptr;
}