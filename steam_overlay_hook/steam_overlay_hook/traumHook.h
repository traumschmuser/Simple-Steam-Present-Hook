#pragma once
#include "tool.h"

constexpr int MODE_TEXT = 1; //calling CreateHook	-> .text patching
constexpr int MODE_DATA = 2; //pointer replacement	-> .data patching

namespace traumHook 
{
	//offsets
	uintptr_t gameOverlayRenderer64; //GameOverlayRenderer64.dll base address

	uintptr_t steamCreateHook_offset; //Steam CreateHook() offset				[.text]
	uintptr_t steamPresentHook_offset; //SteamPresentHook() offset				[.text]

	uintptr_t steamPresentPtr_address; //Steam return Present Pointer			[.data]
	uintptr_t origPresentPtr_content; //Steam original Present return address	[.data]

	//hook mode
	int mode;

	//steam Present Hook
	__int64(*Present)(void*, __int64, __int64);
	__int64 main_hook(void* swapchain, __int64 interval, __int64 flags)
	{
		//calls your "main" function
		tool::main(mode);

		//returns original Present
		if (mode == MODE_TEXT) return Present(swapchain, interval, flags); //.text
		else if (mode == MODE_DATA) return ((__int64 (*)(void*, __int64, __int64))(origPresentPtr_content))(swapchain, interval, flags); //.data
	}

	//steam create hook function
	__int64 SteamCreateHook(uintptr_t pTarget, uintptr_t pDetour, __int64* ppOriginal, int flag)
	{
		return ((__int64 (*)(uintptr_t, uintptr_t, __int64*, int))(gameOverlayRenderer64 + steamCreateHook_offset))(pTarget, pDetour, ppOriginal, flag);
	}

	bool InitializeSteamHook(int hook_mode)
	{
		//get module base address
		gameOverlayRenderer64 = (uintptr_t)GetModuleHandle(L"GameOverlayRenderer64.dll");
		if (!gameOverlayRenderer64) return false;

		mode = hook_mode;

		if (hook_mode == MODE_TEXT) {
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
		else if(hook_mode == MODE_DATA)
		{
			//get present pointer address
			steamPresentPtr_address = memory::PatternScan("\x48\xFF\x25\x00\x00\x00\x00\x48\x89\x5C\x24\x30", "xxx????xxxxx", gameOverlayRenderer64, 0xFFFFF);
			if (!steamPresentPtr_address) return false;
			steamPresentPtr_address = *(unsigned int*)(steamPresentPtr_address + 0x3) + steamPresentPtr_address + 0x7;

			//create hook
			origPresentPtr_content = *(uintptr_t*)(steamPresentPtr_address);
			*(uintptr_t*)(steamPresentPtr_address) = (uintptr_t)&main_hook;

			return true;
		}

		return false;
	}
}