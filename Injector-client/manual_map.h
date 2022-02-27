#pragma once
#include "includes.h"

#define PAGE_SIZE 0x1000
#define PeHeader(image) ((IMAGE_NT_HEADERS64*)((uint64_t)image + ((IMAGE_DOS_HEADER*)image)->e_lfanew))

namespace PE
{
    int64_t	RvaToOffset(
        IMAGE_DOS_HEADER* base,
        uintptr_t rva
    );

    void CopyHeaders(
        IMAGE_DOS_HEADER* src,
        char* dest
    );

    bool ResolveImports(
        char* base
    );

    size_t RemapImage(
        uint8_t* unmapped_pe,
        uint8_t** out_buffer,
        int32_t target_pid,
        uintptr_t load_destination
    );

    void ResolveRelocations(
        char* mapped_base
    );

    void* GetExport(
        uintptr_t base,
        const char* export_name
    );

    template<typename T>
    T	GetExportAddr(uint8_t* base, const char* entry_name)
    {
        auto entry = (char*)GetExport((uintptr_t)base, entry_name);
        return reinterpret_cast<T>(entry + (uintptr_t)base);
    }

    void ForEachSection(
        uint8_t* image_base,
        void(*SectionCallback)(IMAGE_SECTION_HEADER*, uintptr_t base, void* callback_data),
        void* callback_data
    );

    void ForEachImport(
        uint8_t* image_base,
        void(*IATCallback)(
            char* module_name,
            const char* import_name,
            uintptr_t function_addr,
            void* callback_data
            ),
        void* callback_data
    );
};