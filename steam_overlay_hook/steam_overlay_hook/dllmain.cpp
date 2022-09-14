#include "traumHook.h"

DWORD WINAPI attach()
{
	if (!traumHook::InitializeSteamHook(MODE_TEXT))
	{
		//HOOK FAILED
		//MessageBox(0, L"hook failed", L"ERROR", 0);
		return NULL;
	}

	return NULL;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason_for_call, LPVOID lpResreved)
{
	switch (reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		attach();
		break;
	}
	return TRUE;
}

