#include "memory_helper.h"

uintptr_t memory::PatternScan(const char* pattern, std::string mask, uintptr_t begin, uintptr_t size)
{
    size_t pattern_length = mask.length();
    uintptr_t pattern_pos = 0;

    for (uintptr_t i = 0; i < size - pattern_length; ++i) 
    {
        while (pattern_pos < pattern_length)
        {
            if (*(char*)(begin + i + pattern_pos) == pattern[pattern_pos] || mask[pattern_pos] == '?') { if (pattern_pos == pattern_length - 1) return begin + i; }
            else { pattern_pos = 0; break; }
            pattern_pos++;
        }
    }
    return NULL;
}
