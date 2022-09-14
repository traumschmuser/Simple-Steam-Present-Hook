#pragma once
#include "tool.h"
namespace traumHook 
{
	//offsets
	uintptr_t gameOverlayRenderer64;
	uintptr_t steamCreateHook_offset;
	uintptr_t steamPresentHook_offset;

	//steam Present Hook
	__int64(*Present)(void*, __int64, __int64);
	__int64 main_hook(void* swapchain, __int64 interval, __int64 flags)
	{
		//calls your "main" function
		tool::main();
		
		//returns original Present
		return Present(swapchain, interval, flags);
	}

	//steam create hook function
	__int64 SteamCreateHook(uintptr_t pTarget, uintptr_t pDetour, __int64* ppOriginal, int flag)
	{
		return ((__int64 (*)(uintptr_t, uintptr_t, __int64*, int))(gameOverlayRenderer64 + steamCreateHook_offset))(pTarget, pDetour, ppOriginal, flag);
	}

	bool InitializeSteamHook()
	{
		//get module base address
		gameOverlayRenderer64 = (uintptr_t)GetModuleHandle(L"GameOverlayRenderer64.dll");
		if (gameOverlayRenderer64) {
			//pattern scans
			steamCreateHook_offset = memory::PatternScan("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x30\x33\xC0", "xxxxxxxxxxxx", gameOverlayRenderer64, 0xFFFFF);
			if (steamCreateHook_offset) steamCreateHook_offset -= gameOverlayRenderer64;
			else return false;

			steamPresentHook_offset = memory::PatternScan("\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x41\x56\x48\x83\xEC\x20\x41\x8B\xE8\x8B\xF2", "xxxxxxxxxxxxxxxxxxxxx", gameOverlayRenderer64, 0xFFFFF);
			if (steamPresentHook_offset) steamPresentHook_offset -= gameOverlayRenderer64;
			else return false;
			
			//create hook
			if (SteamCreateHook(gameOverlayRenderer64 + steamPresentHook_offset, (__int64)main_hook, (__int64*)&Present, 1) != 1) return false;
			else return true;
		}
		return false;
	}
}