#include "reflective_loader.h"

// TO OBFUSCATE

DWORD64 rva_to_offset(DWORD64 rva, DWORD64 base_address) {
    PIMAGE_NT_HEADERS64 nt_headers_address = (PIMAGE_NT_HEADERS64)(base_address + ((PIMAGE_DOS_HEADER)base_address)->e_lfanew);

    DWORD64 section_virtual_address;
    DWORD64 section_data_address;

    PIMAGE_SECTION_HEADER section_header_address = IMAGE_FIRST_SECTION(nt_headers_address);
    if (rva < section_header_address->PointerToRawData) {
        return rva;
    }
    for (; section_header_address->VirtualAddress != (DWORD64)NULL; section_header_address++) {
        // rva = virtual_address + virtual_address_offset
        // rva + raw_address = virtual_address + virtual_address_offset + raw_address
        // rva - virtual_address + raw_address = raw_address + virtual_address_offset
        if (rva >= section_header_address->VirtualAddress && rva < (section_header_address->VirtualAddress + section_header_address->SizeOfRawData)) {
            return rva - section_header_address->VirtualAddress + section_header_address->PointerToRawData;
        }
        
    }
    return 0;
}

DWORD64 get_reflective_loader_offset(DWORD64 base_address, LPCSTR function_name) {
    DWORD64 function_rva;
    PIMAGE_NT_HEADERS64 nt_headers_address = (PIMAGE_NT_HEADERS64)(base_address + ((PIMAGE_DOS_HEADER)base_address)->e_lfanew);

    IMAGE_DATA_DIRECTORY exports_data_directory = nt_headers_address->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

    PIMAGE_EXPORT_DIRECTORY export_directory = (PIMAGE_EXPORT_DIRECTORY)(base_address + rva_to_offset(exports_data_directory.VirtualAddress, base_address));

    DWORD* export_function_rva_address = (DWORD*)(base_address + rva_to_offset(export_directory->AddressOfFunctions, base_address));
    DWORD* export_function_name_rva_address = (DWORD*)(base_address + rva_to_offset(export_directory->AddressOfNames, base_address));
    WORD* export_function_ordinal_rva_address = (WORD*)(base_address + rva_to_offset(export_directory->AddressOfNameOrdinals, base_address));

    for (DWORD n=0; n < export_directory->NumberOfNames; n++) {
        char *export_function_name = (char *)(base_address + rva_to_offset(export_function_name_rva_address[n], base_address));
        if (!strcmp(export_function_name, function_name)) {
            function_rva = export_function_rva_address[export_function_ordinal_rva_address[n]];
            return rva_to_offset(function_rva, base_address);
        }
    }

    return 0;
}