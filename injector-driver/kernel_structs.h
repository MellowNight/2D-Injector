#pragma once
#include "kernel_exports.h"
#include "pe_header.h"

enum OFFSET
{
    ProcessLinksOffset = 0x448,
};

//0x58 bytes (sizeof)
typedef struct _PEB_LDR_DATA
{
    uint32_t Length;                                                           //0x0
    UCHAR Initialized;                                                      //0x4
    VOID* SsHandle;                                                         //0x8
    struct _LIST_ENTRY InLoadOrderModuleList;                               //0x10
    struct _LIST_ENTRY InMemoryOrderModuleList;                             //0x20
    struct _LIST_ENTRY InInitializationOrderModuleList;                     //0x30
    VOID* EntryInProgress;                                                  //0x40
    UCHAR ShutdownInProgress;                                               //0x48
    VOID* ShutdownThreadId;                                                 //0x50
}PEB_LDR_DATA, * PPEB_LDR_DATA;

//0xa0 bytes (sizeof)
#pragma pack(push, 8)

struct _KLDR_DATA_TABLE_ENTRY
{
    struct _LIST_ENTRY InLoadOrderLinks;                                    //0x0
    VOID* ExceptionTable;                                                   //0x10
    uint32_t ExceptionTableSize;                                               //0x18
    VOID* GpValue;                                                          //0x20
    struct _NON_PAGED_DEBUG_INFO* NonPagedDebugInfo;                        //0x28
    VOID* DllBase;                                                          //0x30
    VOID* EntryPoint;                                                       //0x38
    uint32_t SizeOfImage;                                                      //0x40
    struct _UNICODE_STRING FullDllName;                                     //0x48
    struct _UNICODE_STRING BaseDllName;                                     //0x58
    uint32_t Flags;                                                            //0x68
    USHORT LoadCount;                                                       //0x6c
    union
    {
        USHORT SignatureLevel : 4;                                            //0x6e
        USHORT SignatureType : 3;                                             //0x6e
        USHORT Frozen : 2;                                                    //0x6e
        USHORT HotPatch : 1;                                                  //0x6e
        USHORT Unused : 6;                                                    //0x6e
        USHORT EntireField;                                                 //0x6e
    } u1;                                                                   //0x6e
    VOID* SectionPointer;                                                   //0x70
    uint32_t CheckSum;                                                         //0x78
    uint32_t CoverageSectionSize;                                              //0x7c
    VOID* CoverageSection;                                                  //0x80
    VOID* LoadedImports;                                                    //0x88
    union
    {
        VOID* Spare;                                                        //0x90
        struct _KLDR_DATA_TABLE_ENTRY* NtDataTableEntry;                    //0x90
    };
    uint32_t SizeOfImageNotRounded;                                            //0x98
    uint32_t TimeDateStamp;                                                    //0x9c
};
#pragma pack(pop)


//0x7c8 bytes (sizeof)
struct _PEB
{
    UCHAR InheritedAddressSpace;                                            //0x0
    UCHAR ReadImageFileExecOptions;                                         //0x1
    UCHAR BeingDebugged;                                                    //0x2
    union
    {
        UCHAR BitField;                                                     //0x3
        struct
        {
            UCHAR ImageUsesLargePages : 1;                                    //0x3
            UCHAR IsProtectedProcess : 1;                                     //0x3
            UCHAR IsImageDynamicallyRelocated : 1;                            //0x3
            UCHAR SkipPatchingUser32Forwarders : 1;                           //0x3
            UCHAR IsPackagedProcess : 1;                                      //0x3
            UCHAR IsAppContainer : 1;                                         //0x3
            UCHAR IsProtectedProcessLight : 1;                                //0x3
            UCHAR IsLongPathAwareProcess : 1;                                 //0x3
        };
    };
    UCHAR Padding0[4];                                                      //0x4
    VOID* Mutant;                                                           //0x8
    VOID* ImageBaseAddress;                                                 //0x10
    struct _PEB_LDR_DATA* Ldr;                                              //0x18
    struct _RTL_USER_PROCESS_PARAMETERS* ProcessParameters;                 //0x20
    VOID* SubSystemData;                                                    //0x28
    VOID* ProcessHeap;                                                      //0x30
    struct _RTL_CRITICAL_SECTION* FastPebLock;                              //0x38
    union _SLIST_HEADER* volatile AtlThunkSListPtr;                         //0x40
    VOID* IFEOKey;                                                          //0x48
    union
    {
        uint32_t CrossProcessFlags;                                            //0x50
        struct
        {
            uint32_t ProcessInJob : 1;                                           //0x50
            uint32_t ProcessInitializing : 1;                                    //0x50
            uint32_t ProcessUsingVEH : 1;                                        //0x50
            uint32_t ProcessUsingVCH : 1;                                        //0x50
            uint32_t ProcessUsingFTH : 1;                                        //0x50
            uint32_t ProcessPreviouslyThrottled : 1;                             //0x50
            uint32_t ProcessCurrentlyThrottled : 1;                              //0x50
            uint32_t ProcessImagesHotPatched : 1;                                //0x50
            uint32_t ReservedBits0 : 24;                                         //0x50
        };
    };
    UCHAR Padding1[4];                                                      //0x54
    union
    {
        VOID* KernelCallbackTable;                                          //0x58
        VOID* UserSharedInfoPtr;                                            //0x58
    };
    uint32_t SystemReserved;                                                   //0x60
    uint32_t AtlThunkSListPtr32;                                               //0x64
    VOID* ApiSetMap;                                                        //0x68
    uint32_t TlsExpansionCounter;                                              //0x70
    UCHAR Padding2[4];                                                      //0x74
    VOID* TlsBitmap;                                                        //0x78
    uint32_t TlsBitmapBits[2];                                                 //0x80
    VOID* ReadOnlySharedMemoryBase;                                         //0x88
    VOID* SharedData;                                                       //0x90
    VOID** ReadOnlyStaticServerData;                                        //0x98
    VOID* AnsiCodePageData;                                                 //0xa0
    VOID* OemCodePageData;                                                  //0xa8
    VOID* UnicodeCaseTableData;                                             //0xb0
    uint32_t NumberOfProcessors;                                               //0xb8
    uint32_t NtGlobalFlag;                                                     //0xbc
    union _LARGE_INTEGER CriticalSectionTimeout;                            //0xc0
    ULONGLONG HeapSegmentReserve;                                           //0xc8
    ULONGLONG HeapSegmentCommit;                                            //0xd0
    ULONGLONG HeapDeCommitTotalFreeThreshold;                               //0xd8
    ULONGLONG HeapDeCommitFreeBlockThreshold;                               //0xe0
    uint32_t NumberOfHeaps;                                                    //0xe8
    uint32_t MaximumNumberOfHeaps;                                             //0xec
    VOID** ProcessHeaps;                                                    //0xf0
    VOID* GdiSharedHandleTable;                                             //0xf8
    VOID* ProcessStarterHelper;                                             //0x100
    uint32_t GdiDCAttributeList;                                               //0x108
    UCHAR Padding3[4];                                                      //0x10c
    struct _RTL_CRITICAL_SECTION* LoaderLock;                               //0x110
    uint32_t OSMajorVersion;                                                   //0x118
    uint32_t OSMinorVersion;                                                   //0x11c
    USHORT OSBuildNumber;                                                   //0x120
    USHORT OSCSDVersion;                                                    //0x122
    uint32_t OSPlatformId;                                                     //0x124
    uint32_t ImageSubsystem;                                                   //0x128
    uint32_t ImageSubsystemMajorVersion;                                       //0x12c
    uint32_t ImageSubsystemMinorVersion;                                       //0x130
    UCHAR Padding4[4];                                                      //0x134
    ULONGLONG ActiveProcessAffinityMask;                                    //0x138
    uint32_t GdiHandleBuffer[60];                                              //0x140
    VOID(*PostProcessInitRoutine)();                                       //0x230
    VOID* TlsExpansionBitmap;                                               //0x238
    uint32_t TlsExpansionBitmapBits[32];                                       //0x240
    uint32_t SessionId;                                                        //0x2c0
    UCHAR Padding5[4];                                                      //0x2c4
    union _ULARGE_INTEGER AppCompatFlags;                                   //0x2c8
    union _ULARGE_INTEGER AppCompatFlagsUser;                               //0x2d0
    VOID* pShimData;                                                        //0x2d8
    VOID* AppCompatInfo;                                                    //0x2e0
    struct _UNICODE_STRING CSDVersion;                                      //0x2e8
    struct _ACTIVATION_CONTEXT_DATA* ActivationContextData;                 //0x2f8
    struct _ASSEMBLY_STORAGE_MAP* ProcessAssemblyStorageMap;                //0x300
    struct _ACTIVATION_CONTEXT_DATA* SystemDefaultActivationContextData;    //0x308
    struct _ASSEMBLY_STORAGE_MAP* SystemAssemblyStorageMap;                 //0x310
    ULONGLONG MinimumStackCommit;                                           //0x318
    VOID* SparePointers[4];                                                 //0x320
    uint32_t SpareUlongs[5];                                                   //0x340
    VOID* WerRegistrationData;                                              //0x358
    VOID* WerShipAssertPtr;                                                 //0x360
    VOID* pUnused;                                                          //0x368
    VOID* pImageHeaderHash;                                                 //0x370
    union
    {
        uint32_t TracingFlags;                                                 //0x378
        struct
        {
            uint32_t HeapTracingEnabled : 1;                                     //0x378
            uint32_t CritSecTracingEnabled : 1;                                  //0x378
            uint32_t LibLoaderTracingEnabled : 1;                                //0x378
            uint32_t SpareTracingBits : 29;                                      //0x378
        };
    };
    UCHAR Padding6[4];                                                      //0x37c
    ULONGLONG CsrServerReadOnlySharedMemoryBase;                            //0x380
    ULONGLONG TppWorkerpListLock;                                           //0x388
    struct _LIST_ENTRY TppWorkerpList;                                      //0x390
    VOID* WaitOnAddressHashTable[128];                                      //0x3a0
    VOID* TelemetryCoverageHeader;                                          //0x7a0
    uint32_t CloudFileFlags;                                                   //0x7a8
    uint32_t CloudFileDiagFlags;                                               //0x7ac
    CHAR PlaceholderCompatibilityMode;                                      //0x7b0
    CHAR PlaceholderCompatibilityModeReserved[7];                           //0x7b1
    struct _LEAP_SECOND_DATA* LeapSecondData;                               //0x7b8
    union
    {
        uint32_t LeapSecondFlags;                                              //0x7c0
        struct
        {
            uint32_t SixtySecondEnabled : 1;                                     //0x7c0
            uint32_t Reserved : 31;                                              //0x7c0
        };
    };
    uint32_t NtGlobalFlag2;                                                    //0x7c4
};

//0x120 bytes (sizeof)
struct LDR_DATA_TABLE_ENTRY
{
    struct _LIST_ENTRY InLoadOrderLinks;                                    //0x0
    struct _LIST_ENTRY InMemoryOrderLinks;                                  //0x10
    struct _LIST_ENTRY InInitializationOrderLinks;                          //0x20
    VOID* DllBase;                                                          //0x30
    VOID* EntryPoint;                                                       //0x38
    uint32_t SizeOfImage;                                                      //0x40
    struct _UNICODE_STRING FullDllName;                                     //0x48
    struct _UNICODE_STRING BaseDllName;                                     //0x58
    union
    {
        UCHAR FlagGroup[4];                                                 //0x68
        uint32_t Flags;                                                        //0x68
        struct
        {
            uint32_t PackagedBinary : 1;                                         //0x68
            uint32_t MarkedForRemoval : 1;                                       //0x68
            uint32_t ImageDll : 1;                                               //0x68
            uint32_t LoadNotificationsSent : 1;                                  //0x68
            uint32_t TelemetryEntryProcessed : 1;                                //0x68
            uint32_t ProcessStaticImport : 1;                                    //0x68
            uint32_t InLegacyLists : 1;                                          //0x68
            uint32_t InIndexes : 1;                                              //0x68
            uint32_t ShimDll : 1;                                                //0x68
            uint32_t InExceptionTable : 1;                                       //0x68
            uint32_t ReservedFlags1 : 2;                                         //0x68
            uint32_t LoadInProgress : 1;                                         //0x68
            uint32_t LoadConfigProcessed : 1;                                    //0x68
            uint32_t EntryProcessed : 1;                                         //0x68
            uint32_t ProtectDelayLoad : 1;                                       //0x68
            uint32_t ReservedFlags3 : 2;                                         //0x68
            uint32_t DontCallForThreads : 1;                                     //0x68
            uint32_t ProcessAttachCalled : 1;                                    //0x68
            uint32_t ProcessAttachFailed : 1;                                    //0x68
            uint32_t CorDeferredValidate : 1;                                    //0x68
            uint32_t CorImage : 1;                                               //0x68
            uint32_t DontRelocate : 1;                                           //0x68
            uint32_t CorILOnly : 1;                                              //0x68
            uint32_t ChpeImage : 1;                                              //0x68
            uint32_t ReservedFlags5 : 2;                                         //0x68
            uint32_t Redirected : 1;                                             //0x68
            uint32_t ReservedFlags6 : 2;                                         //0x68
            uint32_t CompatDatabaseProcessed : 1;                                //0x68
        };
    };
    USHORT ObsoleteLoadCount;                                               //0x6c
    USHORT TlsIndex;                                                        //0x6e
    struct _LIST_ENTRY HashLinks;                                           //0x70
    uint32_t TimeDateStamp;                                                    //0x80
    struct _ACTIVATION_CONTEXT* EntryPointActivationContext;                //0x88
    VOID* Lock;                                                             //0x90
    struct _LDR_DDAG_NODE* DdagNode;                                        //0x98
    struct _LIST_ENTRY NodeModuleLink;                                      //0xa0
    struct _LDRP_LOAD_CONTEXT* LoadContext;                                 //0xb0
    VOID* ParentDllBase;                                                    //0xb8
    VOID* SwitchBackContext;                                                //0xc0
    struct _RTL_BALANCED_NODE BaseAddressIndexNode;                         //0xc8
    struct _RTL_BALANCED_NODE MappingInfoIndexNode;                         //0xe0
    ULONGLONG OriginalBase;                                                 //0xf8
    union _LARGE_INTEGER LoadTime;                                          //0x100
    uint32_t BaseNameHashValue;                                                //0x108
    enum _LDR_DLL_LOAD_REASON LoadReason;                                   //0x10c
    uint32_t ImplicitPathOptions;                                              //0x110
    uint32_t ReferenceCount;                                                   //0x114
    uint32_t DependentLoadFlags;                                               //0x118
    UCHAR SigningLevel;                                                     //0x11c
};