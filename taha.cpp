// dllmain.cpp — Combined DLL Loader for Sekiro
#include <windows.h>
#include <string>
#include <shlwapi.h>  // PathRemoveFileSpecA
#pragma comment(lib, "Shlwapi.lib")

static HMODULE hModA = NULL;
static HMODULE hModB = NULL;

// Thread to load the two original DLLs
DWORD WINAPI LoadMyDlls(LPVOID lpParam) {
    char path[MAX_PATH];
    if (GetModuleFileNameA((HMODULE)lpParam, path, MAX_PATH)) {
        PathRemoveFileSpecA(path);

        char dllA[MAX_PATH], dllB[MAX_PATH];
        // These must match your DLL filenames
        sprintf_s(dllA, "%s\\dinput8.dll", path);
        sprintf_s(dllB, "%s\\dinput8_2.dll", path);

        hModA = LoadLibraryA(dllA);
        if (hModA) OutputDebugStringA("Loaded first DLL.\n");
        else       OutputDebugStringA("Failed to load first DLL.\n");

        Sleep(200); // wait in case dependencies exist

        hModB = LoadLibraryA(dllB);
        if (hModB) OutputDebugStringA("Loaded second DLL.\n");
        else       OutputDebugStringA("Failed to load second DLL.\n");
    }
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hinstDLL);
        CreateThread(NULL, 0, LoadMyDlls, (LPVOID)hinstDLL, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        if (hModB) FreeLibrary(hModB);
        if (hModA) FreeLibrary(hModA);
        break;
    }
    return TRUE;
}
