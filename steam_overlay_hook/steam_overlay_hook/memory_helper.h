#pragma once
#include <Windows.h>
#include <string>

namespace memory
{
	uintptr_t PatternScan(const char* pattern, std::string mask, uintptr_t begin, uintptr_t size);
}
