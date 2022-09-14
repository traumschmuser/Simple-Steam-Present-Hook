#pragma once
#include <Windows.h>

namespace memory
{
	bool MemCompare(const char* bData, const char* bSig, const char* szMask);
	uintptr_t PatternScan(const char* pattern, const char* mask, uintptr_t begin, unsigned int size);
}