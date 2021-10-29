/** @file */
#pragma once
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
typedef unsigned long long  UINT64;


#define PAGE_MASK           (PAGE_SIZE - 1)

typedef union
{
  struct
  {
    /**
     * @brief Protection Enable
     *
     * [Bit 0] Enables protected mode when set; enables real-address mode when clear. This flag does not enable paging
     * directly. It only enables segment-level protection. To enable paging, both the PE and PG flags must be set.
     *
     * @see Vol3A[9.9(Mode Switching)]
     */
    UINT64 ProtectionEnable                                        : 1;
#define CR0_PROTECTION_ENABLE_BIT                                    0
#define CR0_PROTECTION_ENABLE_FLAG                                   0x01
#define CR0_PROTECTION_ENABLE_MASK                                   0x01
#define CR0_PROTECTION_ENABLE(_)                                     (((_) >> 0) & 0x01)

    /**
     * @brief Monitor Coprocessor
     *
     * [Bit 1] Controls the interaction of the WAIT (or FWAIT) instruction with the TS flag (bit 3 of CR0). If the MP flag is
     * set, a WAIT instruction generates a device-not-available exception (\#NM) if the TS flag is also set. If the MP flag is
     * clear, the WAIT instruction ignores the setting of the TS flag.
     */
    UINT64 MonitorCoprocessor                                      : 1;
#define CR0_MONITOR_COPROCESSOR_BIT                                  1
#define CR0_MONITOR_COPROCESSOR_FLAG                                 0x02
#define CR0_MONITOR_COPROCESSOR_MASK                                 0x01
#define CR0_MONITOR_COPROCESSOR(_)                                   (((_) >> 1) & 0x01)

    /**
     * @brief FPU Emulation
     *
     * [Bit 2] Indicates that the processor does not have an internal or external x87 FPU when set; indicates an x87 FPU is
     * present when clear. This flag also affects the execution of MMX/SSE/SSE2/SSE3/SSSE3/SSE4 instructions.
     * When the EM flag is set, execution of an x87 FPU instruction generates a device-not-available exception (\#NM). This
     * flag must be set when the processor does not have an internal x87 FPU or is not connected to an external math
     * coprocessor. Setting this flag forces all floating-point instructions to be handled by software emulation.
     * Also, when the EM flag is set, execution of an MMX instruction causes an invalid-opcode exception (\#UD) to be
     * generated. Thus, if an IA-32 or Intel 64 processor incorporates MMX technology, the EM flag must be set to 0 to enable
     * execution of MMX instructions. Similarly for SSE/SSE2/SSE3/SSSE3/SSE4 extensions, when the EM flag is set, execution of
     * most SSE/SSE2/SSE3/SSSE3/SSE4 instructions causes an invalid opcode exception (\#UD) to be generated. If an IA-32 or
     * Intel 64 processor incorporates the SSE/SSE2/SSE3/SSSE3/SSE4 extensions, the EM flag must be set to 0 to enable
     * execution of these extensions. SSE/SSE2/SSE3/SSSE3/SSE4 instructions not affected by the EM flag include: PAUSE,
     * PREFETCHh, SFENCE, LFENCE, MFENCE, MOVNTI, CLFLUSH, CRC32, and POPCNT.
     */
    UINT64 EmulateFpu                                              : 1;
#define CR0_EMULATE_FPU_BIT                                          2
#define CR0_EMULATE_FPU_FLAG                                         0x04
#define CR0_EMULATE_FPU_MASK                                         0x01
#define CR0_EMULATE_FPU(_)                                           (((_) >> 2) & 0x01)

    /**
     * @brief Task Switched
     *
     * [Bit 3] Allows the saving of the x87 FPU/MMX/SSE/SSE2/SSE3/SSSE3/SSE4 context on a task switch to be delayed until an
     * x87 FPU/MMX/SSE/SSE2/SSE3/SSSE3/SSE4 instruction is actually executed by the new task. The processor sets this flag on
     * every task switch and tests it when executing x87 FPU/MMX/SSE/SSE2/SSE3/SSSE3/SSE4 instructions.
     * - If the TS flag is set and the EM flag (bit 2 of CR0) is clear, a device-not-available exception (\#NM) is raised prior
     * to the execution of any x87 FPU/MMX/SSE/SSE2/SSE3/SSSE3/SSE4 instruction; with the exception of PAUSE, PREFETCHh,
     * SFENCE, LFENCE, MFENCE, MOVNTI, CLFLUSH, CRC32, and POPCNT.
     * - If the TS flag is set and the MP flag (bit 1 of CR0) and EM flag are clear, an \#NM exception is not raised prior to
     * the execution of an x87 FPU WAIT/FWAIT instruction.
     * - If the EM flag is set, the setting of the TS flag has no effect on the execution of x87
     * FPU/MMX/SSE/SSE2/SSE3/SSSE3/SSE4 instructions.
     *   The processor does not automatically save the context of the x87 FPU, XMM, and MXCSR registers on a task switch.
     *   Instead, it sets the TS flag, which causes the processor to raise an \#NM exception whenever it encounters an x87
     *   FPU/MMX/SSE/SSE2/SSE3/SSSE3/SSE4 instruction in the instruction stream for the new task (with the exception of the
     *   instructions listed above).
     *   The fault handler for the \#NM exception can then be used to clear the TS flag (with the CLTS instruction) and save
     *   the context of the x87 FPU, XMM, and MXCSR registers. If the task never encounters an x87
     *   FPU/MMX/SSE/SSE2/SSE3/SSSE3/SSE4 instruction, the x87 FPU/MMX/SSE/SSE2/SSE3/SSSE3/SSE4 context is never saved.
     */
    UINT64 TaskSwitched                                            : 1;
#define CR0_TASK_SWITCHED_BIT                                        3
#define CR0_TASK_SWITCHED_FLAG                                       0x08
#define CR0_TASK_SWITCHED_MASK                                       0x01
#define CR0_TASK_SWITCHED(_)                                         (((_) >> 3) & 0x01)

    /**
     * @brief Extension Type
     *
     * [Bit 4] Reserved in the Pentium 4, Intel Xeon, P6 family, and Pentium processors. In the Pentium 4, Intel Xeon, and P6
     * family processors, this flag is hardcoded to 1. In the Intel386 and Intel486 processors, this flag indicates support of
     * Intel 387 DX math coprocessor instructions when set.
     */
    UINT64 ExtensionType                                           : 1;
#define CR0_EXTENSION_TYPE_BIT                                       4
#define CR0_EXTENSION_TYPE_FLAG                                      0x10
#define CR0_EXTENSION_TYPE_MASK                                      0x01
#define CR0_EXTENSION_TYPE(_)                                        (((_) >> 4) & 0x01)

    /**
     * @brief Numeric Error
     *
     * [Bit 5] Enables the native (internal) mechanism for reporting x87 FPU errors when set; enables the PC-style x87 FPU
     * error reporting mechanism when clear. When the NE flag is clear and the IGNNE\# input is asserted, x87 FPU errors are
     * ignored. When the NE flag is clear and the IGNNE\# input is deasserted, an unmasked x87 FPU error causes the processor
     * to assert the FERR\# pin to generate an external interrupt and to stop instruction execution immediately before
     * executing the next waiting floating-point instruction or WAIT/FWAIT instruction.
     * The FERR\# pin is intended to drive an input to an external interrupt controller (the FERR\# pin emulates the ERROR\#
     * pin of the Intel 287 and Intel 387 DX math coprocessors). The NE flag, IGNNE\# pin, and FERR\# pin are used with
     * external logic to implement PC-style error reporting. Using FERR\# and IGNNE\# to handle floating-point exceptions is
     * deprecated by modern operating systems; this non-native approach also limits newer processors to operate with one
     * logical processor active.
     *
     * @see Vol1[8.7(Handling x87 FPU Exceptions in Software)]
     * @see Vol1[A.1(APPENDIX A | EFLAGS Cross-Reference)]
     */
    UINT64 NumericError                                            : 1;
#define CR0_NUMERIC_ERROR_BIT                                        5
#define CR0_NUMERIC_ERROR_FLAG                                       0x20
#define CR0_NUMERIC_ERROR_MASK                                       0x01
#define CR0_NUMERIC_ERROR(_)                                         (((_) >> 5) & 0x01)
    UINT64 Reserved1                                               : 10;

    /**
     * @brief Write Protect
     *
     * [Bit 16] When set, inhibits supervisor-level procedures from writing into readonly pages; when clear, allows
     * supervisor-level procedures to write into read-only pages (regardless of the U/S bit setting). This flag facilitates
     * implementation of the copy-onwrite method of creating a new process (forking) used by operating systems such as UNIX.
     *
     * @see Vol3A[4.1.3(Paging-Mode Modifiers)]
     * @see Vol3A[4.6(ACCESS RIGHTS)]
     */
    UINT64 WriteProtect                                            : 1;
#define CR0_WRITE_PROTECT_BIT                                        16
#define CR0_WRITE_PROTECT_FLAG                                       0x10000
#define CR0_WRITE_PROTECT_MASK                                       0x01
#define CR0_WRITE_PROTECT(_)                                         (((_) >> 16) & 0x01)
    UINT64 Reserved2                                               : 1;

    /**
     * @brief Alignment Mask
     *
     * [Bit 18] Enables automatic alignment checking when set; disables alignment checking when clear. Alignment checking is
     * performed only when the AM flag is set, the AC flag in the EFLAGS register is set, CPL is 3, and the processor is
     * operating in either protected or virtual-8086 mode.
     */
    UINT64 AlignmentMask                                           : 1;
#define CR0_ALIGNMENT_MASK_BIT                                       18
#define CR0_ALIGNMENT_MASK_FLAG                                      0x40000
#define CR0_ALIGNMENT_MASK_MASK                                      0x01
#define CR0_ALIGNMENT_MASK(_)                                        (((_) >> 18) & 0x01)
    UINT64 Reserved3                                               : 10;

    /**
     * @brief Not Write-through
     *
     * [Bit 29] When the NW and CD flags are clear, write-back (for Pentium 4, Intel Xeon, P6 family, and Pentium processors)
     * or write-through (for Intel486 processors) is enabled for writes that hit the cache and invalidation cycles are enabled.
     */
    UINT64 NotWriteThrough                                         : 1;
#define CR0_NOT_WRITE_THROUGH_BIT                                    29
#define CR0_NOT_WRITE_THROUGH_FLAG                                   0x20000000
#define CR0_NOT_WRITE_THROUGH_MASK                                   0x01
#define CR0_NOT_WRITE_THROUGH(_)                                     (((_) >> 29) & 0x01)

    /**
     * @brief Cache Disable
     *
     * [Bit 30] When the CD and NW flags are clear, caching of memory locations for the whole of physical memory in the
     * processor's internal (and external) caches is enabled. When the CD flag is set, caching is restricted. To prevent the
     * processor from accessing and updating its caches, the CD flag must be set and the caches must be invalidated so that no
     * cache hits can occur.
     *
     * @see Vol3A[11.5.3(Preventing Caching)]
     * @see Vol3A[11.5(CACHE CONTROL)]
     */
    UINT64 CacheDisable                                            : 1;
#define CR0_CACHE_DISABLE_BIT                                        30
#define CR0_CACHE_DISABLE_FLAG                                       0x40000000
#define CR0_CACHE_DISABLE_MASK                                       0x01
#define CR0_CACHE_DISABLE(_)                                         (((_) >> 30) & 0x01)

    /**
     * @brief Paging Enable
     *
     * [Bit 31] Enables paging when set; disables paging when clear. When paging is disabled, all linear addresses are treated
     * as physical addresses. The PG flag has no effect if the PE flag (bit 0 of register CR0) is not also set; setting the PG
     * flag when the PE flag is clear causes a general-protection exception (\#GP).
     * On Intel 64 processors, enabling and disabling IA-32e mode operation also requires modifying CR0.PG.
     *
     * @see Vol3A[4(PAGING)]
     */
    UINT64 PagingEnable                                            : 1;
#define CR0_PAGING_ENABLE_BIT                                        31
#define CR0_PAGING_ENABLE_FLAG                                       0x80000000
#define CR0_PAGING_ENABLE_MASK                                       0x01
#define CR0_PAGING_ENABLE(_)                                         (((_) >> 31) & 0x01)
    UINT64 Reserved4                                               : 32;
  };

  UINT64 Flags;
} CR0;

typedef union
{
  struct
  {
    UINT64 Reserved1                                               : 3;

    /**
     * @brief Page-level Write-Through
     *
     * [Bit 3] Controls the memory type used to access the first paging structure of the current paging-structure hierarchy.
     * This bit is not used if paging is disabled, with PAE paging, or with 4-level paging if CR4.PCIDE=1.
     *
     * @see Vol3A[4.9(PAGING AND MEMORY TYPING)]
     */
    UINT64 PageLevelWriteThrough                                   : 2;
#define CR3_PAGE_LEVEL_WRITE_THROUGH_BIT                             3
#define CR3_PAGE_LEVEL_WRITE_THROUGH_FLAG                            0x08
#define CR3_PAGE_LEVEL_WRITE_THROUGH_MASK                            0x01
#define CR3_PAGE_LEVEL_WRITE_THROUGH(_)                              (((_) >> 3) & 0x01)

    /**
     * @brief Page-level Cache Disable
     *
     * [Bit 4] Controls the memory type used to access the first paging structure of the current paging-structure hierarchy.
     * This bit is not used if paging is disabled, with PAE paging, or with 4-level paging2 if CR4.PCIDE=1.
     *
     * @see Vol3A[4.9(PAGING AND MEMORY TYPING)]
     */
    UINT64 PageLevelCacheDisable                                   : 1;
#define CR3_PAGE_LEVEL_CACHE_DISABLE_BIT                             4
#define CR3_PAGE_LEVEL_CACHE_DISABLE_FLAG                            0x10
#define CR3_PAGE_LEVEL_CACHE_DISABLE_MASK                            0x01
#define CR3_PAGE_LEVEL_CACHE_DISABLE(_)                              (((_) >> 4) & 0x01)
    UINT64 Reserved2                                               : 6;

    /**
     * @brief Address of page directory
     *
     * [Bits 47:12] Physical address of the 4-KByte aligned page directory (32-bit paging) or PML4 table (64-bit paging) used
     * for linear-address translation.
     *
     * @see Vol3A[4.3(32-BIT PAGING)]
     * @see Vol3A[4.5(4-LEVEL PAGING)]
     */
    UINT64 AddressOfPageDirectory                                  : 52;
#define CR3_ADDRESS_OF_PAGE_DIRECTORY_BIT                            12
#define CR3_ADDRESS_OF_PAGE_DIRECTORY_FLAG                           0xFFFFFFFFF000
#define CR3_ADDRESS_OF_PAGE_DIRECTORY_MASK                           0xFFFFFFFFF
#define CR3_ADDRESS_OF_PAGE_DIRECTORY(_)                             (((_) >> 12) & 0xFFFFFFFFF)
  };

  UINT64 Flags;
} CR3;

typedef union
{
  struct
  {
    /**
     * @brief Virtual-8086 Mode Extensions
     *
     * [Bit 0] Enables interrupt- and exception-handling extensions in virtual-8086 mode when set; disables the extensions when
     * clear. Use of the virtual mode extensions can improve the performance of virtual-8086 applications by eliminating the
     * overhead of calling the virtual- 8086 monitor to handle interrupts and exceptions that occur while executing an 8086
     * program and, instead, redirecting the interrupts and exceptions back to the 8086 program's handlers. It also provides
     * hardware support for a virtual interrupt flag (VIF) to improve reliability of running 8086 programs in multitasking and
     * multiple-processor environments.
     *
     * @see Vol3B[20.3(INTERRUPT AND EXCEPTION HANDLING IN VIRTUAL-8086 MODE)]
     */
    UINT64 VirtualModeExtensions                                   : 1;
#define CR4_VIRTUAL_MODE_EXTENSIONS_BIT                              0
#define CR4_VIRTUAL_MODE_EXTENSIONS_FLAG                             0x01
#define CR4_VIRTUAL_MODE_EXTENSIONS_MASK                             0x01
#define CR4_VIRTUAL_MODE_EXTENSIONS(_)                               (((_) >> 0) & 0x01)

    /**
     * @brief Protected-Mode Virtual Interrupts
     *
     * [Bit 1] Enables hardware support for a virtual interrupt flag (VIF) in protected mode when set; disables the VIF flag in
     * protected mode when clear.
     *
     * @see Vol3B[20.4(PROTECTED-MODE VIRTUAL INTERRUPTS)]
     */
    UINT64 ProtectedModeVirtualInterrupts                          : 1;
#define CR4_PROTECTED_MODE_VIRTUAL_INTERRUPTS_BIT                    1
#define CR4_PROTECTED_MODE_VIRTUAL_INTERRUPTS_FLAG                   0x02
#define CR4_PROTECTED_MODE_VIRTUAL_INTERRUPTS_MASK                   0x01
#define CR4_PROTECTED_MODE_VIRTUAL_INTERRUPTS(_)                     (((_) >> 1) & 0x01)

    /**
     * @brief Time Stamp Disable
     *
     * [Bit 2] Restricts the execution of the RDTSC instruction to procedures running at privilege level 0 when set; allows
     * RDTSC instruction to be executed at any privilege level when clear. This bit also applies to the RDTSCP instruction if
     * supported (if CPUID.80000001H:EDX[27] = 1).
     */
    UINT64 TimestampDisable                                        : 1;
#define CR4_TIMESTAMP_DISABLE_BIT                                    2
#define CR4_TIMESTAMP_DISABLE_FLAG                                   0x04
#define CR4_TIMESTAMP_DISABLE_MASK                                   0x01
#define CR4_TIMESTAMP_DISABLE(_)                                     (((_) >> 2) & 0x01)

    /**
     * @brief Debugging Extensions
     *
     * [Bit 3] References to debug registers DR4 and DR5 cause an undefined opcode (\#UD) exception to be generated when set;
     * when clear, processor aliases references to registers DR4 and DR5 for compatibility with software written to run on
     * earlier IA-32 processors.
     *
     * @see Vol3B[17.2.2(Debug Registers DR4 and DR5)]
     */
    UINT64 DebuggingExtensions                                     : 1;
#define CR4_DEBUGGING_EXTENSIONS_BIT                                 3
#define CR4_DEBUGGING_EXTENSIONS_FLAG                                0x08
#define CR4_DEBUGGING_EXTENSIONS_MASK                                0x01
#define CR4_DEBUGGING_EXTENSIONS(_)                                  (((_) >> 3) & 0x01)

    /**
     * @brief Page Size Extensions
     *
     * [Bit 4] Enables 4-MByte pages with 32-bit paging when set; restricts 32-bit paging to pages of 4 KBytes when clear.
     *
     * @see Vol3A[4.3(32-BIT PAGING)]
     */
    UINT64 PageSizeExtensions                                      : 1;
#define CR4_PAGE_SIZE_EXTENSIONS_BIT                                 4
#define CR4_PAGE_SIZE_EXTENSIONS_FLAG                                0x10
#define CR4_PAGE_SIZE_EXTENSIONS_MASK                                0x01
#define CR4_PAGE_SIZE_EXTENSIONS(_)                                  (((_) >> 4) & 0x01)

    /**
     * @brief Physical Address Extension
     *
     * [Bit 5] When set, enables paging to produce physical addresses with more than 32 bits. When clear, restricts physical
     * addresses to 32 bits. PAE must be set before entering IA-32e mode.
     *
     * @see Vol3A[4(PAGING)]
     */
    UINT64 PhysicalAddressExtension                                : 1;
#define CR4_PHYSICAL_ADDRESS_EXTENSION_BIT                           5
#define CR4_PHYSICAL_ADDRESS_EXTENSION_FLAG                          0x20
#define CR4_PHYSICAL_ADDRESS_EXTENSION_MASK                          0x01
#define CR4_PHYSICAL_ADDRESS_EXTENSION(_)                            (((_) >> 5) & 0x01)

    /**
     * @brief Machine-Check Enable
     *
     * [Bit 6] Enables the machine-check exception when set; disables the machine-check exception when clear.
     *
     * @see Vol3B[15(MACHINE-CHECK ARCHITECTURE)]
     */
    UINT64 MachineCheckEnable                                      : 1;
#define CR4_MACHINE_CHECK_ENABLE_BIT                                 6
#define CR4_MACHINE_CHECK_ENABLE_FLAG                                0x40
#define CR4_MACHINE_CHECK_ENABLE_MASK                                0x01
#define CR4_MACHINE_CHECK_ENABLE(_)                                  (((_) >> 6) & 0x01)

    /**
     * @brief Page Global Enable
     *
     * [Bit 7] (Introduced in the P6 family processors.) Enables the global page feature when set; disables the global page
     * feature when clear. The global page feature allows frequently used or shared pages to be marked as global to all users
     * (done with the global flag, bit 8, in a page-directory or page-table entry). Global pages are not flushed from the
     * translation-lookaside buffer (TLB) on a task switch or a write to register CR3. When enabling the global page feature,
     * paging must be enabled (by setting the PG flag in control register CR0) before the PGE flag is set. Reversing this
     * sequence may affect program correctness, and processor performance will be impacted.
     *
     * @see Vol3A[4.10(CACHING TRANSLATION INFORMATION)]
     */
    UINT64 PageGlobalEnable                                        : 1;
#define CR4_PAGE_GLOBAL_ENABLE_BIT                                   7
#define CR4_PAGE_GLOBAL_ENABLE_FLAG                                  0x80
#define CR4_PAGE_GLOBAL_ENABLE_MASK                                  0x01
#define CR4_PAGE_GLOBAL_ENABLE(_)                                    (((_) >> 7) & 0x01)

    /**
     * @brief Performance-Monitoring Counter Enable
     *
     * [Bit 8] Enables execution of the RDPMC instruction for programs or procedures running at any protection level when set;
     * RDPMC instruction can be executed only at protection level 0 when clear.
     */
    UINT64 PerformanceMonitoringCounterEnable                      : 1;
#define CR4_PERFORMANCE_MONITORING_COUNTER_ENABLE_BIT                8
#define CR4_PERFORMANCE_MONITORING_COUNTER_ENABLE_FLAG               0x100
#define CR4_PERFORMANCE_MONITORING_COUNTER_ENABLE_MASK               0x01
#define CR4_PERFORMANCE_MONITORING_COUNTER_ENABLE(_)                 (((_) >> 8) & 0x01)

    /**
     * @brief Operating System Support for FXSAVE and FXRSTOR instructions
     *
     * [Bit 9] When set, this flag:
     * -# indicates to software that the operating system supports the use of the FXSAVE and FXRSTOR instructions,
     * -# enables the FXSAVE and FXRSTOR instructions to save and restore the contents of the XMM and MXCSR registers along
     * with the contents of the x87 FPU and MMX registers, and
     * -# enables the processor to execute SSE/SSE2/SSE3/SSSE3/SSE4 instructions, with the exception of the PAUSE, PREFETCHh,
     * SFENCE, LFENCE, MFENCE, MOVNTI, CLFLUSH, CRC32, and POPCNT.
     * If this flag is clear, the FXSAVE and FXRSTOR instructions will save and restore the contents of the x87 FPU and MMX
     * registers, but they may not save and restore the contents of the XMM and MXCSR registers. Also, the processor will
     * generate an invalid opcode exception (\#UD) if it attempts to execute any SSE/SSE2/SSE3 instruction, with the exception
     * of PAUSE, PREFETCHh, SFENCE, LFENCE, MFENCE, MOVNTI, CLFLUSH, CRC32, and POPCNT. The operating system or executive must
     * explicitly set this flag.
     *
     * @remarks CPUID feature flag FXSR indicates availability of the FXSAVE/FXRSTOR instructions. The OSFXSR bit provides
     *          operating system software with a means of enabling FXSAVE/FXRSTOR to save/restore the contents of the X87 FPU, XMM and
     *          MXCSR registers. Consequently OSFXSR bit indicates that the operating system provides context switch support for
     *          SSE/SSE2/SSE3/SSSE3/SSE4.
     */
    UINT64 OsFxsaveFxrstorSupport                                  : 1;
#define CR4_OS_FXSAVE_FXRSTOR_SUPPORT_BIT                            9
#define CR4_OS_FXSAVE_FXRSTOR_SUPPORT_FLAG                           0x200
#define CR4_OS_FXSAVE_FXRSTOR_SUPPORT_MASK                           0x01
#define CR4_OS_FXSAVE_FXRSTOR_SUPPORT(_)                             (((_) >> 9) & 0x01)

    /**
     * @brief Operating System Support for Unmasked SIMD Floating-Point Exceptions
     *
     * [Bit 10] Operating System Support for Unmasked SIMD Floating-Point Exceptions - When set, indicates that the operating
     * system supports the handling of unmasked SIMD floating-point exceptions through an exception handler that is invoked
     * when a SIMD floating-point exception (\#XM) is generated. SIMD floating-point exceptions are only generated by
     * SSE/SSE2/SSE3/SSE4.1 SIMD floatingpoint instructions.
     * The operating system or executive must explicitly set this flag. If this flag is not set, the processor will generate an
     * invalid opcode exception (\#UD) whenever it detects an unmasked SIMD floating-point exception.
     */
    UINT64 OsXmmExceptionSupport                                   : 1;
#define CR4_OS_XMM_EXCEPTION_SUPPORT_BIT                             10
#define CR4_OS_XMM_EXCEPTION_SUPPORT_FLAG                            0x400
#define CR4_OS_XMM_EXCEPTION_SUPPORT_MASK                            0x01
#define CR4_OS_XMM_EXCEPTION_SUPPORT(_)                              (((_) >> 10) & 0x01)

    /**
     * @brief User-Mode Instruction Prevention
     *
     * [Bit 11] When set, the following instructions cannot be executed if CPL > 0: SGDT, SIDT, SLDT, SMSW, and STR. An attempt
     * at such execution causes a generalprotection exception (\#GP).
     */
    UINT64 UsermodeInstructionPrevention                           : 1;
#define CR4_USERMODE_INSTRUCTION_PREVENTION_BIT                      11
#define CR4_USERMODE_INSTRUCTION_PREVENTION_FLAG                     0x800
#define CR4_USERMODE_INSTRUCTION_PREVENTION_MASK                     0x01
#define CR4_USERMODE_INSTRUCTION_PREVENTION(_)                       (((_) >> 11) & 0x01)
    UINT64 Reserved1                                               : 1;

    /**
     * @brief VMX-Enable
     *
     * [Bit 13] Enables VMX operation when set.
     *
     * @see Vol3C[23(INTRODUCTION TO VIRTUAL MACHINE EXTENSIONS)]
     */
    UINT64 VmxEnable                                               : 1;
#define CR4_VMX_ENABLE_BIT                                           13
#define CR4_VMX_ENABLE_FLAG                                          0x2000
#define CR4_VMX_ENABLE_MASK                                          0x01
#define CR4_VMX_ENABLE(_)                                            (((_) >> 13) & 0x01)

    /**
     * @brief SMX-Enable
     *
     * [Bit 14] Enables SMX operation when set.
     *
     * @see Vol2[6(SAFER MODE EXTENSIONS REFERENCE)]
     */
    UINT64 SmxEnable                                               : 1;
#define CR4_SMX_ENABLE_BIT                                           14
#define CR4_SMX_ENABLE_FLAG                                          0x4000
#define CR4_SMX_ENABLE_MASK                                          0x01
#define CR4_SMX_ENABLE(_)                                            (((_) >> 14) & 0x01)
    UINT64 Reserved2                                               : 1;

    /**
     * @brief FSGSBASE-Enable
     *
     * [Bit 16] Enables the instructions RDFSBASE, RDGSBASE, WRFSBASE, and WRGSBASE.
     */
    UINT64 FsgsbaseEnable                                          : 1;
#define CR4_FSGSBASE_ENABLE_BIT                                      16
#define CR4_FSGSBASE_ENABLE_FLAG                                     0x10000
#define CR4_FSGSBASE_ENABLE_MASK                                     0x01
#define CR4_FSGSBASE_ENABLE(_)                                       (((_) >> 16) & 0x01)

    /**
     * @brief PCID-Enable
     *
     * [Bit 17] Enables process-context identifiers (PCIDs) when set. Can be set only in IA-32e mode (if IA32_EFER.LMA = 1).
     *
     * @see Vol3A[4.10.1(Process-Context Identifiers (PCIDs))]
     */
    UINT64 PcidEnable                                              : 1;
#define CR4_PCID_ENABLE_BIT                                          17
#define CR4_PCID_ENABLE_FLAG                                         0x20000
#define CR4_PCID_ENABLE_MASK                                         0x01
#define CR4_PCID_ENABLE(_)                                           (((_) >> 17) & 0x01)

    /**
     * @brief XSAVE and Processor Extended States-Enable
     *
     * [Bit 18] When set, this flag:
     * -# indicates (via CPUID.01H:ECX.OSXSAVE[bit 27]) that the operating system supports the use of the XGETBV, XSAVE and
     * XRSTOR instructions by general software;
     * -# enables the XSAVE and XRSTOR instructions to save and restore the x87 FPU state (including MMX registers), the SSE
     * state (XMM registers and MXCSR), along with other processor extended states enabled in XCR0;
     * -# enables the processor to execute XGETBV and XSETBV instructions in order to read and write XCR0.
     *
     * @see Vol3A[2.6(EXTENDED CONTROL REGISTERS (INCLUDING XCR0))]
     * @see Vol3A[13(SYSTEM PROGRAMMING FOR INSTRUCTION SET EXTENSIONS AND PROCESSOR EXTENDED)]
     */
    UINT64 OsXsave                                                 : 1;
#define CR4_OS_XSAVE_BIT                                             18
#define CR4_OS_XSAVE_FLAG                                            0x40000
#define CR4_OS_XSAVE_MASK                                            0x01
#define CR4_OS_XSAVE(_)                                              (((_) >> 18) & 0x01)
    UINT64 Reserved3                                               : 1;

    /**
     * @brief SMEP-Enable
     *
     * [Bit 20] Enables supervisor-mode execution prevention (SMEP) when set.
     *
     * @see Vol3A[4.6(ACCESS RIGHTS)]
     */
    UINT64 SmepEnable                                              : 1;
#define CR4_SMEP_ENABLE_BIT                                          20
#define CR4_SMEP_ENABLE_FLAG                                         0x100000
#define CR4_SMEP_ENABLE_MASK                                         0x01
#define CR4_SMEP_ENABLE(_)                                           (((_) >> 20) & 0x01)

    /**
     * @brief SMAP-Enable
     *
     * [Bit 21] Enables supervisor-mode access prevention (SMAP) when set.
     *
     * @see Vol3A[4.6(ACCESS RIGHTS)]
     */
    UINT64 SmapEnable                                              : 1;
#define CR4_SMAP_ENABLE_BIT                                          21
#define CR4_SMAP_ENABLE_FLAG                                         0x200000
#define CR4_SMAP_ENABLE_MASK                                         0x01
#define CR4_SMAP_ENABLE(_)                                           (((_) >> 21) & 0x01)

    /**
     * @brief Protection-Key-Enable
     *
     * [Bit 22] Enables 4-level paging to associate each linear address with a protection key. The PKRU register specifies, for
     * each protection key, whether user-mode linear addresses with that protection key can be read or written. This bit also
     * enables access to the PKRU register using the RDPKRU and WRPKRU instructions.
     */
    UINT64 ProtectionKeyEnable                                     : 1;
#define CR4_PROTECTION_KEY_ENABLE_BIT                                22
#define CR4_PROTECTION_KEY_ENABLE_FLAG                               0x400000
#define CR4_PROTECTION_KEY_ENABLE_MASK                               0x01
#define CR4_PROTECTION_KEY_ENABLE(_)                                 (((_) >> 22) & 0x01)
    UINT64 Reserved4                                               : 41;
  };

  UINT64 Flags;
} CR4;

typedef union
{
  struct
  {
    /**
     * @brief Task Priority Level
     *
     * [Bits 3:0] This sets the threshold value corresponding to the highestpriority interrupt to be blocked. A value of 0
     * means all interrupts are enabled. This field is available in 64- bit mode. A value of 15 means all interrupts will be
     * disabled.
     */
    UINT64 TaskPriorityLevel                                       : 4;
#define CR8_TASK_PRIORITY_LEVEL_BIT                                  0
#define CR8_TASK_PRIORITY_LEVEL_FLAG                                 0x0F
#define CR8_TASK_PRIORITY_LEVEL_MASK                                 0x0F
#define CR8_TASK_PRIORITY_LEVEL(_)                                   (((_) >> 0) & 0x0F)

    /**
     * @brief Reserved
     *
     * [Bits 63:4] Reserved and must be written with zeros. Failure to do this causes a general-protection exception.
     */
    UINT64 Reserved                                                : 60;
#define CR8_RESERVED_BIT                                             4
#define CR8_RESERVED_FLAG                                            0xFFFFFFFFFFFFFFF0
#define CR8_RESERVED_MASK                                            0xFFFFFFFFFFFFFFF
#define CR8_RESERVED(_)                                              (((_) >> 4) & 0xFFFFFFFFFFFFFFF)
  };

  UINT64 Flags;
} CR8;


typedef union
{
  struct
  {
    /**
     * @brief B0 through B3 (breakpoint condition detected) flags
     *
     * [Bits 3:0] Indicates (when set) that its associated breakpoint condition was met when a debug exception was generated.
     * These flags are set if the condition described for each breakpoint by the LENn, and R/Wn flags in debug control register
     * DR7 is true. They may or may not be set if the breakpoint is not enabled by the Ln or the Gn flags in register DR7.
     * Therefore on a \#DB, a debug handler should check only those B0-B3 bits which correspond to an enabled breakpoint.
     */
    UINT64 BreakpointCondition                                     : 4;
#define DR6_BREAKPOINT_CONDITION_BIT                                 0
#define DR6_BREAKPOINT_CONDITION_FLAG                                0x0F
#define DR6_BREAKPOINT_CONDITION_MASK                                0x0F
#define DR6_BREAKPOINT_CONDITION(_)                                  (((_) >> 0) & 0x0F)
    UINT64 Reserved1                                               : 9;

    /**
     * @brief BD (debug register access detected) flag
     *
     * [Bit 13] Indicates that the next instruction in the instruction stream accesses one of the debug registers (DR0 through
     * DR7). This flag is enabled when the GD (general detect) flag in debug control register DR7 is set.
     *
     * @see Vol3B[17.2.4(Debug Control Register (DR7))]
     */
    UINT64 DebugRegisterAccessDetected                             : 1;
#define DR6_DEBUG_REGISTER_ACCESS_DETECTED_BIT                       13
#define DR6_DEBUG_REGISTER_ACCESS_DETECTED_FLAG                      0x2000
#define DR6_DEBUG_REGISTER_ACCESS_DETECTED_MASK                      0x01
#define DR6_DEBUG_REGISTER_ACCESS_DETECTED(_)                        (((_) >> 13) & 0x01)

    /**
     * @brief BS (single step) flag
     *
     * [Bit 14] Indicates (when set) that the debug exception was triggered by the singlestep execution mode (enabled with the
     * TF flag in the EFLAGS register). The single-step mode is the highestpriority debug exception. When the BS flag is set,
     * any of the other debug status bits also may be set.
     */
    UINT64 SingleInstruction                                       : 1;
#define DR6_SINGLE_INSTRUCTION_BIT                                   14
#define DR6_SINGLE_INSTRUCTION_FLAG                                  0x4000
#define DR6_SINGLE_INSTRUCTION_MASK                                  0x01
#define DR6_SINGLE_INSTRUCTION(_)                                    (((_) >> 14) & 0x01)

    /**
     * @brief BT (task switch) flag
     *
     * [Bit 15] Indicates (when set) that the debug exception was triggered by the singlestep execution mode (enabled with the
     * TF flag in the EFLAGS register). The single-step mode is the highestpriority debug exception. When the BS flag is set,
     * any of the other debug status bits also may be set.
     */
    UINT64 TaskSwitch                                              : 1;
#define DR6_TASK_SWITCH_BIT                                          15
#define DR6_TASK_SWITCH_FLAG                                         0x8000
#define DR6_TASK_SWITCH_MASK                                         0x01
#define DR6_TASK_SWITCH(_)                                           (((_) >> 15) & 0x01)

    /**
     * @brief RTM (restricted transactional memory) flag
     *
     * [Bit 16] Indicates (when clear) that a debug exception (\#DB) or breakpoint exception (\#BP) occurred inside an RTM
     * region while advanced debugging of RTM transactional regions was enabled. This bit is set for any other debug exception
     * (including all those that occur when advanced debugging of RTM transactional regions is not enabled). This bit is always
     * 1 if the processor does not support RTM.
     *
     * @see Vol3B[17.3.3(Debug Exceptions, Breakpoint Exceptions, and Restricted Transactional Memory (RTM))]
     */
    UINT64 RestrictedTransactionalMemory                           : 1;
#define DR6_RESTRICTED_TRANSACTIONAL_MEMORY_BIT                      16
#define DR6_RESTRICTED_TRANSACTIONAL_MEMORY_FLAG                     0x10000
#define DR6_RESTRICTED_TRANSACTIONAL_MEMORY_MASK                     0x01
#define DR6_RESTRICTED_TRANSACTIONAL_MEMORY(_)                       (((_) >> 16) & 0x01)
    UINT64 Reserved2                                               : 47;
  };

  UINT64 Flags;
} DR6;

typedef union
{
  struct
  {
    /**
     * @brief L0 through L3 (local breakpoint enable) flags (bits 0, 2, 4, and 6)
     *
     * [Bit 0] Enables (when set) the breakpoint condition for the associated breakpoint for the current task. When a
     * breakpoint condition is detected and its associated Ln flag is set, a debug exception is generated. The processor
     * automatically clears these flags on every task switch to avoid unwanted breakpoint conditions in the new task.
     */
    UINT64 LocalBreakpoint0                                        : 1;
#define DR7_LOCAL_BREAKPOINT_0_BIT                                   0
#define DR7_LOCAL_BREAKPOINT_0_FLAG                                  0x01
#define DR7_LOCAL_BREAKPOINT_0_MASK                                  0x01
#define DR7_LOCAL_BREAKPOINT_0(_)                                    (((_) >> 0) & 0x01)

    /**
     * @brief G0 through G3 (global breakpoint enable) flags (bits 1, 3, 5, and 7)
     *
     * [Bit 1] Enables (when set) the breakpoint condition for the associated breakpoint for all tasks. When a breakpoint
     * condition is detected and its associated Gn flag is set, a debug exception is generated. The processor does not clear
     * these flags on a task switch, allowing a breakpoint to be enabled for all tasks.
     */
    UINT64 GlobalBreakpoint0                                       : 1;
#define DR7_GLOBAL_BREAKPOINT_0_BIT                                  1
#define DR7_GLOBAL_BREAKPOINT_0_FLAG                                 0x02
#define DR7_GLOBAL_BREAKPOINT_0_MASK                                 0x01
#define DR7_GLOBAL_BREAKPOINT_0(_)                                   (((_) >> 1) & 0x01)
    UINT64 LocalBreakpoint1                                        : 1;
#define DR7_LOCAL_BREAKPOINT_1_BIT                                   2
#define DR7_LOCAL_BREAKPOINT_1_FLAG                                  0x04
#define DR7_LOCAL_BREAKPOINT_1_MASK                                  0x01
#define DR7_LOCAL_BREAKPOINT_1(_)                                    (((_) >> 2) & 0x01)
    UINT64 GlobalBreakpoint1                                       : 1;
#define DR7_GLOBAL_BREAKPOINT_1_BIT                                  3
#define DR7_GLOBAL_BREAKPOINT_1_FLAG                                 0x08
#define DR7_GLOBAL_BREAKPOINT_1_MASK                                 0x01
#define DR7_GLOBAL_BREAKPOINT_1(_)                                   (((_) >> 3) & 0x01)
    UINT64 LocalBreakpoint2                                        : 1;
#define DR7_LOCAL_BREAKPOINT_2_BIT                                   4
#define DR7_LOCAL_BREAKPOINT_2_FLAG                                  0x10
#define DR7_LOCAL_BREAKPOINT_2_MASK                                  0x01
#define DR7_LOCAL_BREAKPOINT_2(_)                                    (((_) >> 4) & 0x01)
    UINT64 GlobalBreakpoint2                                       : 1;
#define DR7_GLOBAL_BREAKPOINT_2_BIT                                  5
#define DR7_GLOBAL_BREAKPOINT_2_FLAG                                 0x20
#define DR7_GLOBAL_BREAKPOINT_2_MASK                                 0x01
#define DR7_GLOBAL_BREAKPOINT_2(_)                                   (((_) >> 5) & 0x01)
    UINT64 LocalBreakpoint3                                        : 1;
#define DR7_LOCAL_BREAKPOINT_3_BIT                                   6
#define DR7_LOCAL_BREAKPOINT_3_FLAG                                  0x40
#define DR7_LOCAL_BREAKPOINT_3_MASK                                  0x01
#define DR7_LOCAL_BREAKPOINT_3(_)                                    (((_) >> 6) & 0x01)
    UINT64 GlobalBreakpoint3                                       : 1;
#define DR7_GLOBAL_BREAKPOINT_3_BIT                                  7
#define DR7_GLOBAL_BREAKPOINT_3_FLAG                                 0x80
#define DR7_GLOBAL_BREAKPOINT_3_MASK                                 0x01
#define DR7_GLOBAL_BREAKPOINT_3(_)                                   (((_) >> 7) & 0x01)

    /**
     * @brief LE (local exact breakpoint enable)
     *
     * [Bit 8] This feature is not supported in the P6 family processors, later IA-32 processors, and Intel 64 processors. When
     * set, these flags cause the processor to detect the exact instruction that caused a data breakpoint condition. For
     * backward and forward compatibility with other Intel processors, we recommend that the LE and GE flags be set to 1 if
     * exact breakpoints are required.
     */
    UINT64 LocalExactBreakpoint                                    : 1;
#define DR7_LOCAL_EXACT_BREAKPOINT_BIT                               8
#define DR7_LOCAL_EXACT_BREAKPOINT_FLAG                              0x100
#define DR7_LOCAL_EXACT_BREAKPOINT_MASK                              0x01
#define DR7_LOCAL_EXACT_BREAKPOINT(_)                                (((_) >> 8) & 0x01)
    UINT64 GlobalExactBreakpoint                                   : 1;
#define DR7_GLOBAL_EXACT_BREAKPOINT_BIT                              9
#define DR7_GLOBAL_EXACT_BREAKPOINT_FLAG                             0x200
#define DR7_GLOBAL_EXACT_BREAKPOINT_MASK                             0x01
#define DR7_GLOBAL_EXACT_BREAKPOINT(_)                               (((_) >> 9) & 0x01)
    UINT64 Reserved1                                               : 1;

    /**
     * @brief RTM (restricted transactional memory) flag
     *
     * [Bit 11] Enables (when set) advanced debugging of RTM transactional regions. This advanced debugging is enabled only if
     * IA32_DEBUGCTL.RTM is also set.
     *
     * @see Vol3B[17.3.3(Debug Exceptions, Breakpoint Exceptions, and Restricted Transactional Memory (RTM))]
     */
    UINT64 RestrictedTransactionalMemory                           : 1;
#define DR7_RESTRICTED_TRANSACTIONAL_MEMORY_BIT                      11
#define DR7_RESTRICTED_TRANSACTIONAL_MEMORY_FLAG                     0x800
#define DR7_RESTRICTED_TRANSACTIONAL_MEMORY_MASK                     0x01
#define DR7_RESTRICTED_TRANSACTIONAL_MEMORY(_)                       (((_) >> 11) & 0x01)
    UINT64 Reserved2                                               : 1;

    /**
     * @brief GD (general detect enable) flag
     *
     * [Bit 13] Enables (when set) debug-register protection, which causes a debug exception to be generated prior to any MOV
     * instruction that accesses a debug register. When such a condition is detected, the BD flag in debug status register DR6
     * is set prior to generating the exception. This condition is provided to support in-circuit emulators.
     * When the emulator needs to access the debug registers, emulator software can set the GD flag to prevent interference
     * from the program currently executing on the processor.
     * The processor clears the GD flag upon entering to the debug exception handler, to allow the handler access to the debug
     * registers.
     */
    UINT64 GeneralDetect                                           : 1;
#define DR7_GENERAL_DETECT_BIT                                       13
#define DR7_GENERAL_DETECT_FLAG                                      0x2000
#define DR7_GENERAL_DETECT_MASK                                      0x01
#define DR7_GENERAL_DETECT(_)                                        (((_) >> 13) & 0x01)
    UINT64 Reserved3                                               : 2;

    /**
     * @brief R/W0 through R/W3 (read/write) fields (bits 16, 17, 20, 21, 24, 25, 28, and 29)
     *
     * [Bits 17:16] Specifies the breakpoint condition for the corresponding breakpoint. The DE (debug extensions) flag in
     * control register CR4 determines how the bits in the R/Wn fields are interpreted. When the DE flag is set, the processor
     * interprets bits as follows:
     * - 00 - Break on instruction execution only.
     * - 01 - Break on data writes only.
     * - 10 - Break on I/O reads or writes.
     * - 11 - Break on data reads or writes but not instruction fetches.
     * When the DE flag is clear, the processor interprets the R/Wn bits the same as for the Intel386(TM) and Intel486(TM)
     * processors, which is as follows:
     * - 00 - Break on instruction execution only.
     * - 01 - Break on data writes only.
     * - 10 - Undefined.
     * - 11 - Break on data reads or writes but not instruction fetches.
     */
    UINT64 ReadWrite0                                              : 2;
#define DR7_READ_WRITE_0_BIT                                         16
#define DR7_READ_WRITE_0_FLAG                                        0x30000
#define DR7_READ_WRITE_0_MASK                                        0x03
#define DR7_READ_WRITE_0(_)                                          (((_) >> 16) & 0x03)

    /**
     * @brief LEN0 through LEN3 (Length) fields (bits 18, 19, 22, 23, 26, 27, 30, and 31)
     *
     * [Bits 19:18] Specify the size of the memory location at the address specified in the corresponding breakpoint address
     * register (DR0 through DR3). These fields are interpreted as follows:
     * - 00 - 1-byte length.
     * - 01 - 2-byte length.
     * - 10 - Undefined (or 8 byte length, see note below).
     * - 11 - 4-byte length.
     * If the corresponding RWn field in register DR7 is 00 (instruction execution), then the LENn field should also be 00. The
     * effect of using other lengths is undefined.
     *
     * @see Vol3B[17.2.5(Breakpoint Field Recognition)]
     */
    UINT64 Length0                                                 : 2;
#define DR7_LENGTH_0_BIT                                             18
#define DR7_LENGTH_0_FLAG                                            0xC0000
#define DR7_LENGTH_0_MASK                                            0x03
#define DR7_LENGTH_0(_)                                              (((_) >> 18) & 0x03)
    UINT64 ReadWrite1                                              : 2;
#define DR7_READ_WRITE_1_BIT                                         20
#define DR7_READ_WRITE_1_FLAG                                        0x300000
#define DR7_READ_WRITE_1_MASK                                        0x03
#define DR7_READ_WRITE_1(_)                                          (((_) >> 20) & 0x03)
    UINT64 Length1                                                 : 2;
#define DR7_LENGTH_1_BIT                                             22
#define DR7_LENGTH_1_FLAG                                            0xC00000
#define DR7_LENGTH_1_MASK                                            0x03
#define DR7_LENGTH_1(_)                                              (((_) >> 22) & 0x03)
    UINT64 ReadWrite2                                              : 2;
#define DR7_READ_WRITE_2_BIT                                         24
#define DR7_READ_WRITE_2_FLAG                                        0x3000000
#define DR7_READ_WRITE_2_MASK                                        0x03
#define DR7_READ_WRITE_2(_)                                          (((_) >> 24) & 0x03)
    UINT64 Length2                                                 : 2;
#define DR7_LENGTH_2_BIT                                             26
#define DR7_LENGTH_2_FLAG                                            0xC000000
#define DR7_LENGTH_2_MASK                                            0x03
#define DR7_LENGTH_2(_)                                              (((_) >> 26) & 0x03)
    UINT64 ReadWrite3                                              : 2;
#define DR7_READ_WRITE_3_BIT                                         28
#define DR7_READ_WRITE_3_FLAG                                        0x30000000
#define DR7_READ_WRITE_3_MASK                                        0x03
#define DR7_READ_WRITE_3(_)                                          (((_) >> 28) & 0x03)
    UINT64 Length3                                                 : 2;
#define DR7_LENGTH_3_BIT                                             30
#define DR7_LENGTH_3_FLAG                                            0xC0000000
#define DR7_LENGTH_3_MASK                                            0x03
#define DR7_LENGTH_3(_)                                              (((_) >> 30) & 0x03)
    UINT64 Reserved4                                               : 32;
  };

  UINT64 Flags;
} DR7;



typedef union
{
    struct
    {
        UINT64 Present : 1;
        UINT64 Write : 1;
        UINT64 Supervisor : 1;
        UINT64 PageLevelWriteThrough : 1;
        UINT64 PageLevelCacheDisable : 1;
        UINT64 Accessed : 1;
        UINT64 Reserved1 : 1;
        UINT64 LargePage : 1;
        UINT64 Ignored1 : 4;
        UINT64 PageFrameNumber : 40;
        UINT64 Ignored2 : 11;
        UINT64 ExecuteDisable : 1;
    };

    UINT64 Flags;
} PML4E_64;

/**
 * @brief Format of a 4-Level Page-Directory-Pointer-Table Entry (PDPTE) that Maps a 1-GByte Page
 */
typedef union
{
  struct
  {
    /**
     * [Bit 0] Present; must be 1 to map a 1-GByte page.
     */
    UINT64 Present                                                 : 1;
#define PDPTE_1GB_64_PRESENT_BIT                                     0
#define PDPTE_1GB_64_PRESENT_FLAG                                    0x01
#define PDPTE_1GB_64_PRESENT_MASK                                    0x01
#define PDPTE_1GB_64_PRESENT(_)                                      (((_) >> 0) & 0x01)

    /**
     * [Bit 1] Read/write; if 0, writes may not be allowed to the 1-GByte page referenced by this entry.
     *
     * @see Vol3A[4.6(Access Rights)]
     */
    UINT64 Write                                                   : 1;
#define PDPTE_1GB_64_WRITE_BIT                                       1
#define PDPTE_1GB_64_WRITE_FLAG                                      0x02
#define PDPTE_1GB_64_WRITE_MASK                                      0x01
#define PDPTE_1GB_64_WRITE(_)                                        (((_) >> 1) & 0x01)

    /**
     * [Bit 2] User/supervisor; if 0, user-mode accesses are not allowed to the 1-GByte page referenced by this entry.
     *
     * @see Vol3A[4.6(Access Rights)]
     */
    UINT64 Supervisor                                              : 1;
#define PDPTE_1GB_64_SUPERVISOR_BIT                                  2
#define PDPTE_1GB_64_SUPERVISOR_FLAG                                 0x04
#define PDPTE_1GB_64_SUPERVISOR_MASK                                 0x01
#define PDPTE_1GB_64_SUPERVISOR(_)                                   (((_) >> 2) & 0x01)

    /**
     * [Bit 3] Page-level write-through; indirectly determines the memory type used to access the 1-GByte page referenced by
     * this entry.
     *
     * @see Vol3A[4.9.2(Paging and Memory Typing When the PAT is Supported (Pentium III and More Recent Processor Families))]
     */
    UINT64 PageLevelWriteThrough                                   : 1;
#define PDPTE_1GB_64_PAGE_LEVEL_WRITE_THROUGH_BIT                    3
#define PDPTE_1GB_64_PAGE_LEVEL_WRITE_THROUGH_FLAG                   0x08
#define PDPTE_1GB_64_PAGE_LEVEL_WRITE_THROUGH_MASK                   0x01
#define PDPTE_1GB_64_PAGE_LEVEL_WRITE_THROUGH(_)                     (((_) >> 3) & 0x01)

    /**
     * [Bit 4] Page-level cache disable; indirectly determines the memory type used to access the 1-GByte page referenced by
     * this entry.
     *
     * @see Vol3A[4.9.2(Paging and Memory Typing When the PAT is Supported (Pentium III and More Recent Processor Families))]
     */
    UINT64 PageLevelCacheDisable                                   : 1;
#define PDPTE_1GB_64_PAGE_LEVEL_CACHE_DISABLE_BIT                    4
#define PDPTE_1GB_64_PAGE_LEVEL_CACHE_DISABLE_FLAG                   0x10
#define PDPTE_1GB_64_PAGE_LEVEL_CACHE_DISABLE_MASK                   0x01
#define PDPTE_1GB_64_PAGE_LEVEL_CACHE_DISABLE(_)                     (((_) >> 4) & 0x01)

    /**
     * [Bit 5] Accessed; indicates whether software has accessed the 1-GByte page referenced by this entry.
     *
     * @see Vol3A[4.8(Accessed and Dirty Flags)]
     */
    UINT64 Accessed                                                : 1;
#define PDPTE_1GB_64_ACCESSED_BIT                                    5
#define PDPTE_1GB_64_ACCESSED_FLAG                                   0x20
#define PDPTE_1GB_64_ACCESSED_MASK                                   0x01
#define PDPTE_1GB_64_ACCESSED(_)                                     (((_) >> 5) & 0x01)

    /**
     * [Bit 6] Dirty; indicates whether software has written to the 1-GByte page referenced by this entry.
     *
     * @see Vol3A[4.8(Accessed and Dirty Flags)]
     */
    UINT64 Dirty                                                   : 1;
#define PDPTE_1GB_64_DIRTY_BIT                                       6
#define PDPTE_1GB_64_DIRTY_FLAG                                      0x40
#define PDPTE_1GB_64_DIRTY_MASK                                      0x01
#define PDPTE_1GB_64_DIRTY(_)                                        (((_) >> 6) & 0x01)

    /**
     * [Bit 7] Page size; must be 1 (otherwise, this entry references a page directory).
     */
    UINT64 LargePage                                               : 1;
#define PDPTE_1GB_64_LARGE_PAGE_BIT                                  7
#define PDPTE_1GB_64_LARGE_PAGE_FLAG                                 0x80
#define PDPTE_1GB_64_LARGE_PAGE_MASK                                 0x01
#define PDPTE_1GB_64_LARGE_PAGE(_)                                   (((_) >> 7) & 0x01)

    /**
     * [Bit 8] Global; if CR4.PGE = 1, determines whether the translation is global; ignored otherwise.
     *
     * @see Vol3A[4.10(Caching Translation Information)]
     */
    UINT64 Global                                                  : 1;
#define PDPTE_1GB_64_GLOBAL_BIT                                      8
#define PDPTE_1GB_64_GLOBAL_FLAG                                     0x100
#define PDPTE_1GB_64_GLOBAL_MASK                                     0x01
#define PDPTE_1GB_64_GLOBAL(_)                                       (((_) >> 8) & 0x01)

    /**
     * [Bits 11:9] Ignored.
     */
    UINT64 Ignored1                                                : 3;
#define PDPTE_1GB_64_IGNORED_1_BIT                                   9
#define PDPTE_1GB_64_IGNORED_1_FLAG                                  0xE00
#define PDPTE_1GB_64_IGNORED_1_MASK                                  0x07
#define PDPTE_1GB_64_IGNORED_1(_)                                    (((_) >> 9) & 0x07)

    /**
     * [Bit 12] Indirectly determines the memory type used to access the 1-GByte page referenced by this entry.
     *
     * @note The PAT is supported on all processors that support 4-level paging.
     * @see Vol3A[4.9.2(Paging and Memory Typing When the PAT is Supported (Pentium III and More Recent Processor Families))]
     */
    UINT64 Pat                                                     : 1;
#define PDPTE_1GB_64_PAT_BIT                                         12
#define PDPTE_1GB_64_PAT_FLAG                                        0x1000
#define PDPTE_1GB_64_PAT_MASK                                        0x01
#define PDPTE_1GB_64_PAT(_)                                          (((_) >> 12) & 0x01)
    UINT64 Reserved1                                               : 17;

    /**
     * [Bits 47:30] Physical address of the 1-GByte page referenced by this entry.
     */
    UINT64 PageFrameNumber                                         : 18;
#define PDPTE_1GB_64_PAGE_FRAME_NUMBER_BIT                           30
#define PDPTE_1GB_64_PAGE_FRAME_NUMBER_FLAG                          0xFFFFC0000000
#define PDPTE_1GB_64_PAGE_FRAME_NUMBER_MASK                          0x3FFFF
#define PDPTE_1GB_64_PAGE_FRAME_NUMBER(_)                            (((_) >> 30) & 0x3FFFF)
    UINT64 Reserved2                                               : 4;

    /**
     * [Bits 58:52] Ignored.
     */
    UINT64 Ignored2                                                : 7;
#define PDPTE_1GB_64_IGNORED_2_BIT                                   52
#define PDPTE_1GB_64_IGNORED_2_FLAG                                  0x7F0000000000000
#define PDPTE_1GB_64_IGNORED_2_MASK                                  0x7F
#define PDPTE_1GB_64_IGNORED_2(_)                                    (((_) >> 52) & 0x7F)

    /**
     * [Bits 62:59] Protection key; if CR4.PKE = 1, determines the protection key of the page; ignored otherwise.
     *
     * @see Vol3A[4.6.2(Protection Keys)]
     */
    UINT64 ProtectionKey                                           : 4;
#define PDPTE_1GB_64_PROTECTION_KEY_BIT                              59
#define PDPTE_1GB_64_PROTECTION_KEY_FLAG                             0x7800000000000000
#define PDPTE_1GB_64_PROTECTION_KEY_MASK                             0x0F
#define PDPTE_1GB_64_PROTECTION_KEY(_)                               (((_) >> 59) & 0x0F)

    /**
     * [Bit 63] If IA32_EFER.NXE = 1, execute-disable (if 1, instruction fetches are not allowed from the 1-GByte page
     * controlled by this entry); otherwise, reserved (must be 0).
     *
     * @see Vol3A[4.6(Access Rights)]
     */
    UINT64 ExecuteDisable                                          : 1;
#define PDPTE_1GB_64_EXECUTE_DISABLE_BIT                             63
#define PDPTE_1GB_64_EXECUTE_DISABLE_FLAG                            0x8000000000000000
#define PDPTE_1GB_64_EXECUTE_DISABLE_MASK                            0x01
#define PDPTE_1GB_64_EXECUTE_DISABLE(_)                              (((_) >> 63) & 0x01)
  };

  UINT64 Flags;
} PDPTE_1GB_64;
static_assert(sizeof(PDPTE_1GB_64) == 8, "size mismatch");
/**
 * @brief Format of a 4-Level Page-Directory-Pointer-Table Entry (PDPTE) that References a Page Directory
 */
typedef union
{
    struct
    {
        UINT64 Present : 1;
        UINT64 Write : 1;
        UINT64 Supervisor : 1;
        UINT64 PageLevelWriteThrough : 1;
        UINT64 PageLevelCacheDisable : 1;
        UINT64 Accessed : 1;
        UINT64 Reserved1 : 1;
        UINT64 LargePage : 1;
        UINT64 Ignored1 : 4;
        UINT64 PageFrameNumber : 40;
        UINT64 Ignored2 : 11;
        UINT64 ExecuteDisable : 1;
    };

    UINT64 Flags;
} PDPTE_64;
static_assert(sizeof(PDPTE_64) == 8, "size mismatch");
/**
 * @brief Format of a 4-Level Page-Directory Entry that Maps a 2-MByte Page
 */
 typedef union
 {
     struct
     {
         UINT64 Present : 1;
         UINT64 Write : 1;
         UINT64 Supervisor : 1;
         UINT64 PageLevelWriteThrough : 1;
         UINT64 PageLevelCacheDisable : 1;
         UINT64 Accessed : 1;
         UINT64 Dirty : 1;
         UINT64 LargePage : 1;
         UINT64 Global : 1;
         UINT64 Ignored1 : 3;
         UINT64 Pat : 1;
         UINT64 Reserved1 : 8;
         UINT64 PageFrameNumber : 27;
         UINT64 Reserved2 : 4;
         UINT64 Ignored2 : 7;
         UINT64 ProtectionKey : 4;
         UINT64 ExecuteDisable : 1;
     };

     UINT64 Flags;
 } PDE_2MB_64;
 static_assert(sizeof(PDE_2MB_64) == 8, "size mismatch");

 typedef union
 {
     struct
     {
         UINT64 Present : 1;
         UINT64 Write : 1;
         UINT64 Supervisor : 1;
         UINT64 PageLevelWriteThrough : 1;
         UINT64 PageLevelCacheDisable : 1;
         UINT64 Accessed : 1;
         UINT64 Reserved1 : 1;
         UINT64 LargePage : 1;
         UINT64 Ignored1 : 4;
         UINT64 PageFrameNumber : 40;
         UINT64 Ignored2 : 11;
         UINT64 ExecuteDisable : 1;
     };

     UINT64 Flags;
 } PDE_64;
 static_assert(sizeof(PDE_64) == 8, "size mismatch");



typedef union
{
  struct
  {
      UINT64 Present : 1;
      UINT64 Write : 1;
      UINT64 Supervisor : 1;
      UINT64 PageLevelWriteThrough : 1;
      UINT64 PageLevelCacheDisable : 1;
      UINT64 Accessed : 1;
      UINT64 Dirty : 1;
      UINT64 Pat : 1;
      UINT64 Global : 1;
      UINT64 Avl : 3;
      UINT64 PageFrameNumber : 40;
      UINT64 Ignored2 : 7;
      UINT64 ProtectionKey : 4;
      UINT64 ExecuteDisable : 1;
  };

  UINT64 Flags;
} PTE_64;
static_assert(sizeof(PTE_64) == 8, "size mismatch");

typedef union
{
    struct
    {
        UINT64 Present : 1;
        UINT64 Write : 1;
        UINT64 Supervisor : 1;
        UINT64 PageLevelWriteThrough : 1;
        UINT64 PageLevelCacheDisable : 1;
        UINT64 Accessed : 1;
        UINT64 Dirty : 1;
        UINT64 Pat : 1;
        UINT64 Global : 1;
        UINT64 Avl : 3;
        UINT64 PageFrameNumber : 40;
        UINT64 Ignored2 : 7;
        UINT64 ProtectionKey : 4;
        UINT64 ExecuteDisable : 1;
    };
    UINT64 Flags;
} PT_ENTRY_64;
static_assert(sizeof(PT_ENTRY_64) == 8, "size mismatch");

#define PML4E_ENTRY_COUNT_64                                         0x00000200
#define PDPTE_ENTRY_COUNT_64                                         0x00000200
#define PDE_ENTRY_COUNT_64                                           0x00000200
#define PTE_ENTRY_COUNT_64                                           0x00000200


/**
 * @defgroup SEGMENT_DESCRIPTORS \
 *           Segment descriptors
 * @{
 */
/**
 * @brief Pseudo-Descriptor Format (32-bit)
 *
 * @see Vol3A[3.5.1(Segment Descriptor Tables)] (reference)
 */
#pragma pack(push, 1)
typedef struct
{
  /**
   * Limit.
   */
  UINT16 Limit;

  /**
   * Base Address.
   */
  UINT32 BaseAddress;
} SEGMENT_DESCRIPTOR_REGISTER_32;
#pragma pack(pop)

/**
 * @brief Pseudo-Descriptor Format (64-bit)
 *
 * @see Vol3A[3.5.1(Segment Descriptor Tables)] (reference)
 */
#pragma pack(push, 1)
typedef struct
{
  /**
   * Limit.
   */
  UINT16 Limit;

  /**
   * Base Address.
   */
  UINT64 BaseAddress;
} SEGMENT_DESCRIPTOR_REGISTER_64;
#pragma pack(pop)

/**
 * @brief Segment access rights
 *
 * @see Vol2A[3.2(Instructions (A-L) | LAR-Load Access Rights Byte)] (reference)
 */
typedef union
{
  struct
  {
    UINT32 Reserved1                                               : 8;

    /**
     * @brief Type field
     *
     * [Bits 11:8] Indicates the segment or gate type and specifies the kinds of access that can be made to the segment and the
     * direction of growth. The interpretation of this field depends on whether the descriptor type flag specifies an
     * application (code or data) descriptor or a system descriptor. The encoding of the type field is different for code,
     * data, and system descriptors.
     *
     * @see Vol3A[3.4.5.1(Code- and Data-Segment Descriptor Types)]
     */
    UINT32 Type                                                    : 4;
#define SEGMENT_ACCESS_RIGHTS_TYPE_BIT                               8
#define SEGMENT_ACCESS_RIGHTS_TYPE_FLAG                              0xF00
#define SEGMENT_ACCESS_RIGHTS_TYPE_MASK                              0x0F
#define SEGMENT_ACCESS_RIGHTS_TYPE(_)                                (((_) >> 8) & 0x0F)

    /**
     * @brief S (descriptor type) flag
     *
     * [Bit 12] Specifies whether the segment descriptor is for a system segment (S flag is clear) or a code or data segment (S
     * flag is set).
     */
    UINT32 DescriptorType                                          : 1;
#define SEGMENT_ACCESS_RIGHTS_DESCRIPTOR_TYPE_BIT                    12
#define SEGMENT_ACCESS_RIGHTS_DESCRIPTOR_TYPE_FLAG                   0x1000
#define SEGMENT_ACCESS_RIGHTS_DESCRIPTOR_TYPE_MASK                   0x01
#define SEGMENT_ACCESS_RIGHTS_DESCRIPTOR_TYPE(_)                     (((_) >> 12) & 0x01)

    /**
     * @brief DPL (descriptor privilege level) field
     *
     * [Bits 14:13] Specifies the privilege level of the segment. The privilege level can range from 0 to 3, with 0 being the
     * most privileged level. The DPL is used to control access to the segment. See Section 5.5, "Privilege Levels", for a
     * description of the relationship of the DPL to the CPL of the executing code segment and the RPL of a segment selector.
     */
    UINT32 DescriptorPrivilegeLevel                                : 2;
#define SEGMENT_ACCESS_RIGHTS_DESCRIPTOR_PRIVILEGE_LEVEL_BIT         13
#define SEGMENT_ACCESS_RIGHTS_DESCRIPTOR_PRIVILEGE_LEVEL_FLAG        0x6000
#define SEGMENT_ACCESS_RIGHTS_DESCRIPTOR_PRIVILEGE_LEVEL_MASK        0x03
#define SEGMENT_ACCESS_RIGHTS_DESCRIPTOR_PRIVILEGE_LEVEL(_)          (((_) >> 13) & 0x03)

    /**
     * @brief P (segment-present) flag
     *
     * [Bit 15] Indicates whether the segment is present in memory (set) or not present (clear). If this flag is clear, the
     * processor generates a segment-not-present exception (\#NP) when a segment selector that points to the segment descriptor
     * is loaded into a segment register. Memory management software can use this flag to control which segments are actually
     * loaded into physical memory at a given time. It offers a control in addition to paging for managing virtual memory.
     */
    UINT32 Present                                                 : 1;
#define SEGMENT_ACCESS_RIGHTS_PRESENT_BIT                            15
#define SEGMENT_ACCESS_RIGHTS_PRESENT_FLAG                           0x8000
#define SEGMENT_ACCESS_RIGHTS_PRESENT_MASK                           0x01
#define SEGMENT_ACCESS_RIGHTS_PRESENT(_)                             (((_) >> 15) & 0x01)
    UINT32 Reserved2                                               : 4;

    /**
     * @brief Available bit
     *
     * [Bit 20] Bit 20 of the second doubleword of the segment descriptor is available for use by system software.
     */
    UINT32 System                                                  : 1;
#define SEGMENT_ACCESS_RIGHTS_SYSTEM_BIT                             20
#define SEGMENT_ACCESS_RIGHTS_SYSTEM_FLAG                            0x100000
#define SEGMENT_ACCESS_RIGHTS_SYSTEM_MASK                            0x01
#define SEGMENT_ACCESS_RIGHTS_SYSTEM(_)                              (((_) >> 20) & 0x01)

    /**
     * @brief L (64-bit code segment) flag
     *
     * [Bit 21] In IA-32e mode, bit 21 of the second doubleword of the segment descriptor indicates whether a code segment
     * contains native 64-bit code. A value of 1 indicates instructions in this code segment are executed in 64-bit mode. A
     * value of 0 indicates the instructions in this code segment are executed in compatibility mode. If L-bit is set, then
     * D-bit must be cleared. When not in IA-32e mode or for non-code segments, bit 21 is reserved and should always be set to
     * 0.
     */
    UINT32 LongMode                                                : 1;
#define SEGMENT_ACCESS_RIGHTS_LONG_MODE_BIT                          21
#define SEGMENT_ACCESS_RIGHTS_LONG_MODE_FLAG                         0x200000
#define SEGMENT_ACCESS_RIGHTS_LONG_MODE_MASK                         0x01
#define SEGMENT_ACCESS_RIGHTS_LONG_MODE(_)                           (((_) >> 21) & 0x01)

    /**
     * @brief D/B (default operation size/default stack pointer size and/or upper bound) flag
     *
     * [Bit 22] Performs different functions depending on whether the segment descriptor is an executable code segment, an
     * expand-down data segment, or a stack segment. (This flag should always be set to 1 for 32-bit code and data segments and
     * to 0 for 16-bit code and data segments.)
     * - Executable code segment. The flag is called the D flag and it indicates the default length for effective addresses and
     * operands referenced by instructions in the segment. If the flag is set, 32-bit addresses and 32-bit or 8-bit operands
     * are assumed; if it is clear, 16-bit addresses and 16-bit or 8-bit operands are assumed. The instruction prefix 66H can
     * be used to select an operand size other than the default, and the prefix 67H can be used select an address size other
     * than the default.
     * - Stack segment (data segment pointed to by the SS register). The flag is called the B (big) flag and it specifies the
     * size of the stack pointer used for implicit stack operations (such as pushes, pops, and calls). If the flag is set, a
     * 32-bit stack pointer is used, which is stored in the 32-bit ESP register; if the flag is clear, a 16-bit stack pointer
     * is used, which is stored in the 16- bit SP register. If the stack segment is set up to be an expand-down data segment
     * (described in the next paragraph), the B flag also specifies the upper bound of the stack segment.
     * - Expand-down data segment. The flag is called the B flag and it specifies the upper bound of the segment. If the flag
     * is set, the upper bound is FFFFFFFFH (4 GBytes); if the flag is clear, the upper bound is FFFFH (64 KBytes).
     */
    UINT32 DefaultBig                                              : 1;
#define SEGMENT_ACCESS_RIGHTS_DEFAULT_BIG_BIT                        22
#define SEGMENT_ACCESS_RIGHTS_DEFAULT_BIG_FLAG                       0x400000
#define SEGMENT_ACCESS_RIGHTS_DEFAULT_BIG_MASK                       0x01
#define SEGMENT_ACCESS_RIGHTS_DEFAULT_BIG(_)                         (((_) >> 22) & 0x01)

    /**
     * @brief G (granularity) flag
     *
     * [Bit 23] Determines the scaling of the segment limit field. When the granularity flag is clear, the segment limit is
     * interpreted in byte units; when flag is set, the segment limit is interpreted in 4-KByte units. (This flag does not
     * affect the granularity of the base address; it is always byte granular.) When the granularity flag is set, the twelve
     * least significant bits of an offset are not tested when checking the offset against the segment limit. For example, when
     * the granularity flag is set, a limit of 0 results in valid offsets from 0 to 4095.
     */
    UINT32 Granularity                                             : 1;
#define SEGMENT_ACCESS_RIGHTS_GRANULARITY_BIT                        23
#define SEGMENT_ACCESS_RIGHTS_GRANULARITY_FLAG                       0x800000
#define SEGMENT_ACCESS_RIGHTS_GRANULARITY_MASK                       0x01
#define SEGMENT_ACCESS_RIGHTS_GRANULARITY(_)                         (((_) >> 23) & 0x01)
    UINT32 Reserved3                                               : 8;
  };

  UINT32 Flags;
} SEGMENT_ACCESS_RIGHTS;

/**
 * @brief General Segment Descriptor (32-bit)
 *
 * A segment descriptor is a data structure in a GDT or LDT that provides the processor with the size and location of a
 * segment, as well as access control and status information. Segment descriptors are typically created by compilers,
 * linkers, loaders, or the operating system or executive, but not application programs.
 *
 * @see Vol3A[5.2(FIELDS AND FLAGS USED FOR SEGMENT-LEVEL AND PAGE-LEVEL PROTECTION)]
 * @see Vol3A[5.2.1(Code-Segment Descriptor in 64-bit Mode)]
 * @see Vol3A[5.8.3(Call Gates)]
 * @see Vol3A[6.11(IDT DESCRIPTORS)]
 * @see Vol3A[6.14.1(64-Bit Mode IDT)]
 * @see Vol3A[7.2.2(TSS Descriptor)]
 * @see Vol3A[7.2.3(TSS Descriptor in 64-bit mode)]
 * @see Vol3A[7.2.5(Task-Gate Descriptor)]
 * @see Vol3A[3.4.5(Segment Descriptors)] (reference)
 */
typedef struct
{
  /**
   * @brief Segment limit field (15:00)
   *
   * Specifies the size of the segment. The processor puts together the two segment limit fields to form a 20-bit value. The
   * processor interprets the segment limit in one of two ways, depending on the setting of the G (granularity) flag:
   * - If the granularity flag is clear, the segment size can range from 1 byte to 1 MByte, in byte increments.
   * - If the granularity flag is set, the segment size can range from 4 KBytes to 4 GBytes, in 4-KByte increments.
   * The processor uses the segment limit in two different ways, depending on whether the segment is an expand-up or an
   * expand-down segment. For expand-up segments, the offset in a logical address can range from 0 to the segment limit.
   * Offsets greater than the segment limit generate general-protection exceptions (\#GP, for all segments other than SS) or
   * stack-fault exceptions (\#SS for the SS segment). For expand-down segments, the segment limit has the reverse function;
   * the offset can range from the segment limit plus 1 to FFFFFFFFH or FFFFH, depending on the setting of the B flag.
   * Offsets less than or equal to the segment limit generate general-protection exceptions or stack-fault exceptions.
   * Decreasing the value in the segment limit field for an expanddown segment allocates new memory at the bottom of the
   * segment's address space, rather than at the top. IA-32 architecture stacks always grow downwards, making this mechanism
   * convenient for expandable stacks.
   *
   * @see Vol3A[3.4.5.1(Code- and Data-Segment Descriptor Types)]
   */
  UINT16 SegmentLimitLow;

  /**
   * @brief Base address field (15:00)
   *
   * Defines the location of byte 0 of the segment within the 4-GByte linear address space. The processor puts together the
   * three base address fields to form a single 32-bit value. Segment base addresses should be aligned to 16-byte boundaries.
   * Although 16-byte alignment is not required, this alignment allows programs to maximize performance by aligning code and
   * data on 16-byte boundaries.
   */
  UINT16 BaseAddressLow;
  /**
   * @brief Segment descriptor fields
   */
  union
  {
    struct
    {
      /**
       * [Bits 7:0] Base address field (23:16); see description of $BASE_LOW for more details.
       */
      UINT32 BaseAddressMiddle                                     : 8;
#define SEGMENT__BASE_ADDRESS_MIDDLE_BIT                             0
#define SEGMENT__BASE_ADDRESS_MIDDLE_FLAG                            0xFF
#define SEGMENT__BASE_ADDRESS_MIDDLE_MASK                            0xFF
#define SEGMENT__BASE_ADDRESS_MIDDLE(_)                              (((_) >> 0) & 0xFF)

      /**
       * @brief Type field
       *
       * [Bits 11:8] Indicates the segment or gate type and specifies the kinds of access that can be made to the segment and the
       * direction of growth. The interpretation of this field depends on whether the descriptor type flag specifies an
       * application (code or data) descriptor or a system descriptor. The encoding of the type field is different for code,
       * data, and system descriptors.
       *
       * @see Vol3A[3.4.5.1(Code- and Data-Segment Descriptor Types)]
       */
      UINT32 Type                                                  : 4;
#define SEGMENT__TYPE_BIT                                            8
#define SEGMENT__TYPE_FLAG                                           0xF00
#define SEGMENT__TYPE_MASK                                           0x0F
#define SEGMENT__TYPE(_)                                             (((_) >> 8) & 0x0F)

      /**
       * @brief S (descriptor type) flag
       *
       * [Bit 12] Specifies whether the segment descriptor is for a system segment (S flag is clear) or a code or data segment (S
       * flag is set).
       */
      UINT32 DescriptorType                                        : 1;
#define SEGMENT__DESCRIPTOR_TYPE_BIT                                 12
#define SEGMENT__DESCRIPTOR_TYPE_FLAG                                0x1000
#define SEGMENT__DESCRIPTOR_TYPE_MASK                                0x01
#define SEGMENT__DESCRIPTOR_TYPE(_)                                  (((_) >> 12) & 0x01)

      /**
       * @brief DPL (descriptor privilege level) field
       *
       * [Bits 14:13] Specifies the privilege level of the segment. The privilege level can range from 0 to 3, with 0 being the
       * most privileged level. The DPL is used to control access to the segment. See Section 5.5, "Privilege Levels", for a
       * description of the relationship of the DPL to the CPL of the executing code segment and the RPL of a segment selector.
       */
      UINT32 DescriptorPrivilegeLevel                              : 2;
#define SEGMENT__DESCRIPTOR_PRIVILEGE_LEVEL_BIT                      13
#define SEGMENT__DESCRIPTOR_PRIVILEGE_LEVEL_FLAG                     0x6000
#define SEGMENT__DESCRIPTOR_PRIVILEGE_LEVEL_MASK                     0x03
#define SEGMENT__DESCRIPTOR_PRIVILEGE_LEVEL(_)                       (((_) >> 13) & 0x03)

      /**
       * @brief P (segment-present) flag
       *
       * [Bit 15] Indicates whether the segment is present in memory (set) or not present (clear). If this flag is clear, the
       * processor generates a segment-not-present exception (\#NP) when a segment selector that points to the segment descriptor
       * is loaded into a segment register. Memory management software can use this flag to control which segments are actually
       * loaded into physical memory at a given time. It offers a control in addition to paging for managing virtual memory.
       */
      UINT32 Present                                               : 1;
#define SEGMENT__PRESENT_BIT                                         15
#define SEGMENT__PRESENT_FLAG                                        0x8000
#define SEGMENT__PRESENT_MASK                                        0x01
#define SEGMENT__PRESENT(_)                                          (((_) >> 15) & 0x01)

      /**
       * [Bits 19:16] Segment limit field (19:16); see description of $LIMIT_LOW for more details.
       */
      UINT32 SegmentLimitHigh                                      : 4;
#define SEGMENT__SEGMENT_LIMIT_HIGH_BIT                              16
#define SEGMENT__SEGMENT_LIMIT_HIGH_FLAG                             0xF0000
#define SEGMENT__SEGMENT_LIMIT_HIGH_MASK                             0x0F
#define SEGMENT__SEGMENT_LIMIT_HIGH(_)                               (((_) >> 16) & 0x0F)

      /**
       * @brief Available bit
       *
       * [Bit 20] Bit 20 of the second doubleword of the segment descriptor is available for use by system software.
       */
      UINT32 System                                                : 1;
#define SEGMENT__SYSTEM_BIT                                          20
#define SEGMENT__SYSTEM_FLAG                                         0x100000
#define SEGMENT__SYSTEM_MASK                                         0x01
#define SEGMENT__SYSTEM(_)                                           (((_) >> 20) & 0x01)

      /**
       * @brief L (64-bit code segment) flag
       *
       * [Bit 21] In IA-32e mode, bit 21 of the second doubleword of the segment descriptor indicates whether a code segment
       * contains native 64-bit code. A value of 1 indicates instructions in this code segment are executed in 64-bit mode. A
       * value of 0 indicates the instructions in this code segment are executed in compatibility mode. If L-bit is set, then
       * D-bit must be cleared. When not in IA-32e mode or for non-code segments, bit 21 is reserved and should always be set to
       * 0.
       */
      UINT32 LongMode                                              : 1;
#define SEGMENT__LONG_MODE_BIT                                       21
#define SEGMENT__LONG_MODE_FLAG                                      0x200000
#define SEGMENT__LONG_MODE_MASK                                      0x01
#define SEGMENT__LONG_MODE(_)                                        (((_) >> 21) & 0x01)

      /**
       * @brief D/B (default operation size/default stack pointer size and/or upper bound) flag
       *
       * [Bit 22] Performs different functions depending on whether the segment descriptor is an executable code segment, an
       * expand-down data segment, or a stack segment. (This flag should always be set to 1 for 32-bit code and data segments and
       * to 0 for 16-bit code and data segments.)
       * - Executable code segment. The flag is called the D flag and it indicates the default length for effective addresses and
       * operands referenced by instructions in the segment. If the flag is set, 32-bit addresses and 32-bit or 8-bit operands
       * are assumed; if it is clear, 16-bit addresses and 16-bit or 8-bit operands are assumed. The instruction prefix 66H can
       * be used to select an operand size other than the default, and the prefix 67H can be used select an address size other
       * than the default.
       * - Stack segment (data segment pointed to by the SS register). The flag is called the B (big) flag and it specifies the
       * size of the stack pointer used for implicit stack operations (such as pushes, pops, and calls). If the flag is set, a
       * 32-bit stack pointer is used, which is stored in the 32-bit ESP register; if the flag is clear, a 16-bit stack pointer
       * is used, which is stored in the 16- bit SP register. If the stack segment is set up to be an expand-down data segment
       * (described in the next paragraph), the B flag also specifies the upper bound of the stack segment.
       * - Expand-down data segment. The flag is called the B flag and it specifies the upper bound of the segment. If the flag
       * is set, the upper bound is FFFFFFFFH (4 GBytes); if the flag is clear, the upper bound is FFFFH (64 KBytes).
       */
      UINT32 DefaultBig                                            : 1;
#define SEGMENT__DEFAULT_BIG_BIT                                     22
#define SEGMENT__DEFAULT_BIG_FLAG                                    0x400000
#define SEGMENT__DEFAULT_BIG_MASK                                    0x01
#define SEGMENT__DEFAULT_BIG(_)                                      (((_) >> 22) & 0x01)

      /**
       * @brief G (granularity) flag
       *
       * [Bit 23] Determines the scaling of the segment limit field. When the granularity flag is clear, the segment limit is
       * interpreted in byte units; when flag is set, the segment limit is interpreted in 4-KByte units. (This flag does not
       * affect the granularity of the base address; it is always byte granular.) When the granularity flag is set, the twelve
       * least significant bits of an offset are not tested when checking the offset against the segment limit. For example, when
       * the granularity flag is set, a limit of 0 results in valid offsets from 0 to 4095.
       */
      UINT32 Granularity                                           : 1;
#define SEGMENT__GRANULARITY_BIT                                     23
#define SEGMENT__GRANULARITY_FLAG                                    0x800000
#define SEGMENT__GRANULARITY_MASK                                    0x01
#define SEGMENT__GRANULARITY(_)                                      (((_) >> 23) & 0x01)

      /**
       * [Bits 31:24] Base address field (31:24); see description of $BASE_LOW for more details.
       */
      UINT32 BaseAddressHigh                                       : 8;
#define SEGMENT__BASE_ADDRESS_HIGH_BIT                               24
#define SEGMENT__BASE_ADDRESS_HIGH_FLAG                              0xFF000000
#define SEGMENT__BASE_ADDRESS_HIGH_MASK                              0xFF
#define SEGMENT__BASE_ADDRESS_HIGH(_)                                (((_) >> 24) & 0xFF)
    };

    UINT32 Flags;
  } ;

} SEGMENT_DESCRIPTOR_32;

/**
 * @brief General Segment Descriptor (64-bit)
 *
 * A segment descriptor is a data structure in a GDT or LDT that provides the processor with the size and location of a
 * segment, as well as access control and status information. Segment descriptors are typically created by compilers,
 * linkers, loaders, or the operating system or executive, but not application programs.
 *
 * @see Vol3A[3.4.5(Segment Descriptors)] (reference)
 */
typedef struct
{
  /**
   * @brief Segment limit field (15:00)
   *
   * Specifies the size of the segment. The processor puts together the two segment limit fields to form a 20-bit value. The
   * processor interprets the segment limit in one of two ways, depending on the setting of the G (granularity) flag:
   * - If the granularity flag is clear, the segment size can range from 1 byte to 1 MByte, in byte increments.
   * - If the granularity flag is set, the segment size can range from 4 KBytes to 4 GBytes, in 4-KByte increments.
   * The processor uses the segment limit in two different ways, depending on whether the segment is an expand-up or an
   * expand-down segment. For expand-up segments, the offset in a logical address can range from 0 to the segment limit.
   * Offsets greater than the segment limit generate general-protection exceptions (\#GP, for all segments other than SS) or
   * stack-fault exceptions (\#SS for the SS segment). For expand-down segments, the segment limit has the reverse function;
   * the offset can range from the segment limit plus 1 to FFFFFFFFH or FFFFH, depending on the setting of the B flag.
   * Offsets less than or equal to the segment limit generate general-protection exceptions or stack-fault exceptions.
   * Decreasing the value in the segment limit field for an expanddown segment allocates new memory at the bottom of the
   * segment's address space, rather than at the top. IA-32 architecture stacks always grow downwards, making this mechanism
   * convenient for expandable stacks.
   *
   * @see Vol3A[3.4.5.1(Code- and Data-Segment Descriptor Types)]
   */
  UINT16 SegmentLimitLow;

  /**
   * @brief Base address field (15:00)
   *
   * Defines the location of byte 0 of the segment within the 4-GByte linear address space. The processor puts together the
   * three base address fields to form a single 32-bit value. Segment base addresses should be aligned to 16-byte boundaries.
   * Although 16-byte alignment is not required, this alignment allows programs to maximize performance by aligning code and
   * data on 16-byte boundaries.
   */
  UINT16 BaseAddressLow;
  /**
   * @brief Segment descriptor fields
   */
  union
  {
    struct
    {
      /**
       * [Bits 7:0] Base address field (23:16); see description of $BASE_LOW for more details.
       */
      UINT32 BaseAddressMiddle                                     : 8;
#define SEGMENT__BASE_ADDRESS_MIDDLE_BIT                             0
#define SEGMENT__BASE_ADDRESS_MIDDLE_FLAG                            0xFF
#define SEGMENT__BASE_ADDRESS_MIDDLE_MASK                            0xFF
#define SEGMENT__BASE_ADDRESS_MIDDLE(_)                              (((_) >> 0) & 0xFF)

      /**
       * @brief Type field
       *
       * [Bits 11:8] Indicates the segment or gate type and specifies the kinds of access that can be made to the segment and the
       * direction of growth. The interpretation of this field depends on whether the descriptor type flag specifies an
       * application (code or data) descriptor or a system descriptor. The encoding of the type field is different for code,
       * data, and system descriptors.
       *
       * @see Vol3A[3.4.5.1(Code- and Data-Segment Descriptor Types)]
       */
      UINT32 Type                                                  : 4;
#define SEGMENT__TYPE_BIT                                            8
#define SEGMENT__TYPE_FLAG                                           0xF00
#define SEGMENT__TYPE_MASK                                           0x0F
#define SEGMENT__TYPE(_)                                             (((_) >> 8) & 0x0F)

      /**
       * @brief S (descriptor type) flag
       *
       * [Bit 12] Specifies whether the segment descriptor is for a system segment (S flag is clear) or a code or data segment (S
       * flag is set).
       */
      UINT32 DescriptorType                                        : 1;
#define SEGMENT__DESCRIPTOR_TYPE_BIT                                 12
#define SEGMENT__DESCRIPTOR_TYPE_FLAG                                0x1000
#define SEGMENT__DESCRIPTOR_TYPE_MASK                                0x01
#define SEGMENT__DESCRIPTOR_TYPE(_)                                  (((_) >> 12) & 0x01)

      /**
       * @brief DPL (descriptor privilege level) field
       *
       * [Bits 14:13] Specifies the privilege level of the segment. The privilege level can range from 0 to 3, with 0 being the
       * most privileged level. The DPL is used to control access to the segment. See Section 5.5, "Privilege Levels", for a
       * description of the relationship of the DPL to the CPL of the executing code segment and the RPL of a segment selector.
       */
      UINT32 DescriptorPrivilegeLevel                              : 2;
#define SEGMENT__DESCRIPTOR_PRIVILEGE_LEVEL_BIT                      13
#define SEGMENT__DESCRIPTOR_PRIVILEGE_LEVEL_FLAG                     0x6000
#define SEGMENT__DESCRIPTOR_PRIVILEGE_LEVEL_MASK                     0x03
#define SEGMENT__DESCRIPTOR_PRIVILEGE_LEVEL(_)                       (((_) >> 13) & 0x03)

      /**
       * @brief P (segment-present) flag
       *
       * [Bit 15] Indicates whether the segment is present in memory (set) or not present (clear). If this flag is clear, the
       * processor generates a segment-not-present exception (\#NP) when a segment selector that points to the segment descriptor
       * is loaded into a segment register. Memory management software can use this flag to control which segments are actually
       * loaded into physical memory at a given time. It offers a control in addition to paging for managing virtual memory.
       */
      UINT32 Present                                               : 1;
#define SEGMENT__PRESENT_BIT                                         15
#define SEGMENT__PRESENT_FLAG                                        0x8000
#define SEGMENT__PRESENT_MASK                                        0x01
#define SEGMENT__PRESENT(_)                                          (((_) >> 15) & 0x01)

      /**
       * [Bits 19:16] Segment limit field (19:16); see description of $LIMIT_LOW for more details.
       */
      UINT32 SegmentLimitHigh                                      : 4;
#define SEGMENT__SEGMENT_LIMIT_HIGH_BIT                              16
#define SEGMENT__SEGMENT_LIMIT_HIGH_FLAG                             0xF0000
#define SEGMENT__SEGMENT_LIMIT_HIGH_MASK                             0x0F
#define SEGMENT__SEGMENT_LIMIT_HIGH(_)                               (((_) >> 16) & 0x0F)

      /**
       * @brief Available bit
       *
       * [Bit 20] Bit 20 of the second doubleword of the segment descriptor is available for use by system software.
       */
      UINT32 System                                                : 1;
#define SEGMENT__SYSTEM_BIT                                          20
#define SEGMENT__SYSTEM_FLAG                                         0x100000
#define SEGMENT__SYSTEM_MASK                                         0x01
#define SEGMENT__SYSTEM(_)                                           (((_) >> 20) & 0x01)

      /**
       * @brief L (64-bit code segment) flag
       *
       * [Bit 21] In IA-32e mode, bit 21 of the second doubleword of the segment descriptor indicates whether a code segment
       * contains native 64-bit code. A value of 1 indicates instructions in this code segment are executed in 64-bit mode. A
       * value of 0 indicates the instructions in this code segment are executed in compatibility mode. If L-bit is set, then
       * D-bit must be cleared. When not in IA-32e mode or for non-code segments, bit 21 is reserved and should always be set to
       * 0.
       */
      UINT32 LongMode                                              : 1;
#define SEGMENT__LONG_MODE_BIT                                       21
#define SEGMENT__LONG_MODE_FLAG                                      0x200000
#define SEGMENT__LONG_MODE_MASK                                      0x01
#define SEGMENT__LONG_MODE(_)                                        (((_) >> 21) & 0x01)

      /**
       * @brief D/B (default operation size/default stack pointer size and/or upper bound) flag
       *
       * [Bit 22] Performs different functions depending on whether the segment descriptor is an executable code segment, an
       * expand-down data segment, or a stack segment. (This flag should always be set to 1 for 32-bit code and data segments and
       * to 0 for 16-bit code and data segments.)
       * - Executable code segment. The flag is called the D flag and it indicates the default length for effective addresses and
       * operands referenced by instructions in the segment. If the flag is set, 32-bit addresses and 32-bit or 8-bit operands
       * are assumed; if it is clear, 16-bit addresses and 16-bit or 8-bit operands are assumed. The instruction prefix 66H can
       * be used to select an operand size other than the default, and the prefix 67H can be used select an address size other
       * than the default.
       * - Stack segment (data segment pointed to by the SS register). The flag is called the B (big) flag and it specifies the
       * size of the stack pointer used for implicit stack operations (such as pushes, pops, and calls). If the flag is set, a
       * 32-bit stack pointer is used, which is stored in the 32-bit ESP register; if the flag is clear, a 16-bit stack pointer
       * is used, which is stored in the 16- bit SP register. If the stack segment is set up to be an expand-down data segment
       * (described in the next paragraph), the B flag also specifies the upper bound of the stack segment.
       * - Expand-down data segment. The flag is called the B flag and it specifies the upper bound of the segment. If the flag
       * is set, the upper bound is FFFFFFFFH (4 GBytes); if the flag is clear, the upper bound is FFFFH (64 KBytes).
       */
      UINT32 DefaultBig                                            : 1;
#define SEGMENT__DEFAULT_BIG_BIT                                     22
#define SEGMENT__DEFAULT_BIG_FLAG                                    0x400000
#define SEGMENT__DEFAULT_BIG_MASK                                    0x01
#define SEGMENT__DEFAULT_BIG(_)                                      (((_) >> 22) & 0x01)

      /**
       * @brief G (granularity) flag
       *
       * [Bit 23] Determines the scaling of the segment limit field. When the granularity flag is clear, the segment limit is
       * interpreted in byte units; when flag is set, the segment limit is interpreted in 4-KByte units. (This flag does not
       * affect the granularity of the base address; it is always byte granular.) When the granularity flag is set, the twelve
       * least significant bits of an offset are not tested when checking the offset against the segment limit. For example, when
       * the granularity flag is set, a limit of 0 results in valid offsets from 0 to 4095.
       */
      UINT32 Granularity                                           : 1;
#define SEGMENT__GRANULARITY_BIT                                     23
#define SEGMENT__GRANULARITY_FLAG                                    0x800000
#define SEGMENT__GRANULARITY_MASK                                    0x01
#define SEGMENT__GRANULARITY(_)                                      (((_) >> 23) & 0x01)

      /**
       * [Bits 31:24] Base address field (31:24); see description of $BASE_LOW for more details.
       */
      UINT32 BaseAddressHigh                                       : 8;
#define SEGMENT__BASE_ADDRESS_HIGH_BIT                               24
#define SEGMENT__BASE_ADDRESS_HIGH_FLAG                              0xFF000000
#define SEGMENT__BASE_ADDRESS_HIGH_MASK                              0xFF
#define SEGMENT__BASE_ADDRESS_HIGH(_)                                (((_) >> 24) & 0xFF)
    };

    UINT32 Flags;
  } ;


  /**
   * Base address field (32:63); see description of $BASE_LOW for more details.
   */
  UINT32 BaseAddressUpper;


} SEGMENT_DESCRIPTOR_64;

#define SEGMENT_DESCRIPTOR_TYPE_SYSTEM                               0x00000000
#define SEGMENT_DESCRIPTOR_TYPE_CODE_OR_DATA                         0x00000001
/**
 * @defgroup SEGMENT_DESCRIPTOR_CODE_AND_DATA_TYPE \
 *           Code- and Data-Segment Descriptor Types
 *
 * When the S (descriptor type) flag in a segment descriptor is set, the descriptor is for either a code or a data segment.
 * The highest order bit of the type field (bit 11 of the second double word of the segment descriptor) then determines
 * whether the descriptor is for a data segment (clear) or a code segment (set). For data segments, the three low-order
 * bits of the type field (bits 8, 9, and 10) are interpreted as accessed (A), write-enable (W), and expansion-direction
 * (E). See Table 3-1 for a description of the encoding of the bits in the type field for code and data segments. Data
 * segments can be read-only or read/write segments, depending on the setting of the write-enable bit.
 *
 * @see Vol3A[3.4.5.1(Code- and Data-Segment Descriptor Types)] (reference)
 * @{
 */


typedef union
{
  struct
  {
    /**
     * [Bits 1:0] Specifies the privilege level of the selector. The privilege level can range from 0 to 3, with 0 being the
     * most privileged level.
     *
     * @see Vol3A[5.5(Privilege Levels)]
     */
    UINT16 RequestPrivilegeLevel                                   : 2;
#define SEGMENT_SELECTOR_REQUEST_PRIVILEGE_LEVEL_BIT                 0
#define SEGMENT_SELECTOR_REQUEST_PRIVILEGE_LEVEL_FLAG                0x03
#define SEGMENT_SELECTOR_REQUEST_PRIVILEGE_LEVEL_MASK                0x03
#define SEGMENT_SELECTOR_REQUEST_PRIVILEGE_LEVEL(_)                  (((_) >> 0) & 0x03)

    /**
     * [Bit 2] Specifies the descriptor table to use: clearing this flag selects the GDT; setting this flag selects the current
     * LDT.
     */
    UINT16 Table                                                   : 1;
#define SEGMENT_SELECTOR_TABLE_BIT                                   2
#define SEGMENT_SELECTOR_TABLE_FLAG                                  0x04
#define SEGMENT_SELECTOR_TABLE_MASK                                  0x01
#define SEGMENT_SELECTOR_TABLE(_)                                    (((_) >> 2) & 0x01)

    /**
     * [Bits 15:3] Selects one of 8192 descriptors in the GDT or LDT. The processor multiplies the index value by 8 (the number
     * of bytes in a segment descriptor) and adds the result to the base address of the GDT or LDT (from the GDTR or LDTR
     * register, respectively).
     */
    UINT16 Index                                                   : 13;
#define SEGMENT_SELECTOR_INDEX_BIT                                   3
#define SEGMENT_SELECTOR_INDEX_FLAG                                  0xFFF8
#define SEGMENT_SELECTOR_INDEX_MASK                                  0x1FFF
#define SEGMENT_SELECTOR_INDEX(_)                                    (((_) >> 3) & 0x1FFF)
  };

  UINT16 Flags;
} SEGMENT_SELECTOR;


typedef enum
{
  /**
   * #DE - Divide Error.
   * Source: DIV and IDIV instructions.
   * Error Code: No.
   */
  DivideError                                                  = 0x00000000,

  /**
   * #DB - Debug.
   * Source: Any code or data reference.
   * Error Code: No.
   */
  Debug                                                        = 0x00000001,

  /**
   * Nonmaskable Interrupt.
   * Source: Generated externally by asserting the processor's NMI pin or
   *         through an NMI request set by the I/O APIC to the local APIC.
   * Error Code: No.
   */
  Nmi                                                          = 0x00000002,

  /**
   * #BP - Breakpoint.
   * Source: INT3 instruction.
   * Error Code: No.
   */
  Breakpoint                                                   = 0x00000003,

  /**
   * #OF - Overflow.
   * Source: INTO instruction.
   * Error Code: No.
   */
  Overflow                                                     = 0x00000004,

  /**
   * #BR - BOUND Range Exceeded.
   * Source: BOUND instruction.
   * Error Code: No.
   */
  BoundRangeExceeded                                           = 0x00000005,

  /**
   * #UD - Invalid Opcode (Undefined Opcode).
   * Source: UD instruction or reserved opcode.
   * Error Code: No.
   */
  InvalidOpcode                                                = 0x00000006,

  /**
   * #NM - Device Not Available (No Math Coprocessor).
   * Source: Floating-point or WAIT/FWAIT instruction.
   * Error Code: No.
   */
  DeviceNotAvailable                                           = 0x00000007,

  /**
   * #DF - Double Fault.
   * Source: Any instruction that can generate an exception, an NMI, or an INTR.
   * Error Code: Yes (zero).
   */
  DoubleFault                                                  = 0x00000008,

  /**
   * #\## - Coprocessor Segment Overrun (reserved).
   * Source: Floating-point instruction.
   * Error Code: No.
   *
   * @note Processors after the Intel386 processor do not generate this exception.
   */
  CoprocessorSegmentOverrun                                    = 0x00000009,

  /**
   * #TS - Invalid TSS.
   * Source: Task switch or TSS access.
   * Error Code: Yes.
   */
  InvalidTss                                                   = 0x0000000A,

  /**
   * #NP - Segment Not Present.
   * Source: Loading segment registers or accessing system segments.
   * Error Code: Yes.
   */
  SegmentNotPresent                                            = 0x0000000B,

  /**
   * #SS - Stack Segment Fault.
   * Source: Stack operations and SS register loads.
   * Error Code: Yes.
   */
  StackSegmentFault                                            = 0x0000000C,

  /**
   * #GP - General Protection.
   * Source: Any memory reference and other protection checks.
   * Error Code: Yes.
   */
  GeneralProtection                                            = 0x0000000D,

  /**
   * #PF - Page Fault.
   * Source: Any memory reference.
   * Error Code: Yes.
   */
  PageFault                                                    = 0x0000000E,

  /**
   * #MF - Floating-Point Error (Math Fault).
   * Source: Floating-point or WAIT/FWAIT instruction.
   * Error Code: No.
   */
  X87FloatingPointError                                        = 0x00000010,

  /**
   * #AC - Alignment Check.
   * Source: Any data reference in memory.
   * Error Code: Yes.
   */
  AlignmentCheck                                               = 0x00000011,

  /**
   * #MC - Machine Check.
   * Source: Model dependent machine check errors.
   * Error Code: No.
   */
  MachineCheck                                                 = 0x00000012,

  /**
   * #XM - SIMD Floating-Point Numeric Error.
   * Source: SSE/SSE2/SSE3 floating-point instructions.
   * Error Code: No.
   */
  SimdFloatingPointError                                       = 0x00000013,

  /**
   * #VE - Virtualization Exception.
   * Source: EPT violations.
   * Error Code: No.
   */
  VirtualizationException                                      = 0x00000014,
} EXCEPTION_VECTOR;



 typedef union _ADDRESS_TRANSLATION_HELPER
 {
     //
     // Indexes to locate paging-structure entries corresponds to this virtual
     // address.
     //
     struct
     {
         UINT64 Unused : 12;         //< [11:0]
         UINT64 Pt : 9;              //< [20:12]
         UINT64 Pd : 9;              //< [29:21]
         UINT64 Pdpt : 9;            //< [38:30]
         UINT64 Pml4 : 9;            //< [47:39]
     } AsIndex;

     //
     // The page offset for each type of pages. For example, for 4KB pages, bits
     // [11:0] are treated as the page offset and Mapping4Kb can be used for it.
     //
     union
     {
         UINT64 Mapping4Kb : 12;     //< [11:0]
         UINT64 Mapping2Mb : 21;     //< [20:0]
         UINT64 Mapping1Gb : 30;     //< [29:0]
     } AsPageOffset;

     UINT64 AsUInt64;
 } ADDRESS_TRANSLATION_HELPER;

 typedef union
 {
     struct
     {
         /**
          * @brief Carry flag
          *
          * [Bit 0] See the description in EFLAGS.
          */
         UINT64 CarryFlag : 1;
#define RFLAGS_CARRY_FLAG_BIT                                        0
#define RFLAGS_CARRY_FLAG_FLAG                                       0x01
#define RFLAGS_CARRY_FLAG_MASK                                       0x01
#define RFLAGS_CARRY_FLAG(_)                                         (((_) >> 0) & 0x01)

         /**
          * [Bit 1] Reserved - always 1
          */
         UINT64 ReadAs1 : 1;
#define RFLAGS_READ_AS_1_BIT                                         1
#define RFLAGS_READ_AS_1_FLAG                                        0x02
#define RFLAGS_READ_AS_1_MASK                                        0x01
#define RFLAGS_READ_AS_1(_)                                          (((_) >> 1) & 0x01)

         /**
          * @brief Parity flag
          *
          * [Bit 2] See the description in EFLAGS.
          */
         UINT64 ParityFlag : 1;
#define RFLAGS_PARITY_FLAG_BIT                                       2
#define RFLAGS_PARITY_FLAG_FLAG                                      0x04
#define RFLAGS_PARITY_FLAG_MASK                                      0x01
#define RFLAGS_PARITY_FLAG(_)                                        (((_) >> 2) & 0x01)
         UINT64 Reserved1 : 1;

         /**
          * @brief Auxiliary Carry flag
          *
          * [Bit 4] See the description in EFLAGS.
          */
         UINT64 AuxiliaryCarryFlag : 1;
#define RFLAGS_AUXILIARY_CARRY_FLAG_BIT                              4
#define RFLAGS_AUXILIARY_CARRY_FLAG_FLAG                             0x10
#define RFLAGS_AUXILIARY_CARRY_FLAG_MASK                             0x01
#define RFLAGS_AUXILIARY_CARRY_FLAG(_)                               (((_) >> 4) & 0x01)
         UINT64 Reserved2 : 1;

         /**
          * @brief Zero flag
          *
          * [Bit 6] See the description in EFLAGS.
          */
         UINT64 ZeroFlag : 1;
#define RFLAGS_ZERO_FLAG_BIT                                         6
#define RFLAGS_ZERO_FLAG_FLAG                                        0x40
#define RFLAGS_ZERO_FLAG_MASK                                        0x01
#define RFLAGS_ZERO_FLAG(_)                                          (((_) >> 6) & 0x01)

         /**
          * @brief Sign flag
          *
          * [Bit 7] See the description in EFLAGS.
          */
         UINT64 SignFlag : 1;
#define RFLAGS_SIGN_FLAG_BIT                                         7
#define RFLAGS_SIGN_FLAG_FLAG                                        0x80
#define RFLAGS_SIGN_FLAG_MASK                                        0x01
#define RFLAGS_SIGN_FLAG(_)                                          (((_) >> 7) & 0x01)

         /**
          * @brief Trap flag
          *
          * [Bit 8] See the description in EFLAGS.
          */
         UINT64 TrapFlag : 1;
#define RFLAGS_TRAP_FLAG_BIT                                         8
#define RFLAGS_TRAP_FLAG_FLAG                                        0x100
#define RFLAGS_TRAP_FLAG_MASK                                        0x01
#define RFLAGS_TRAP_FLAG(_)                                          (((_) >> 8) & 0x01)

         /**
          * @brief Interrupt enable flag
          *
          * [Bit 9] See the description in EFLAGS.
          */
         UINT64 InterruptEnableFlag : 1;
#define RFLAGS_INTERRUPT_ENABLE_FLAG_BIT                             9
#define RFLAGS_INTERRUPT_ENABLE_FLAG_FLAG                            0x200
#define RFLAGS_INTERRUPT_ENABLE_FLAG_MASK                            0x01
#define RFLAGS_INTERRUPT_ENABLE_FLAG(_)                              (((_) >> 9) & 0x01)

         /**
          * @brief Direction flag
          *
          * [Bit 10] See the description in EFLAGS.
          */
         UINT64 DirectionFlag : 1;
#define RFLAGS_DIRECTION_FLAG_BIT                                    10
#define RFLAGS_DIRECTION_FLAG_FLAG                                   0x400
#define RFLAGS_DIRECTION_FLAG_MASK                                   0x01
#define RFLAGS_DIRECTION_FLAG(_)                                     (((_) >> 10) & 0x01)

         /**
          * @brief Overflow flag
          *
          * [Bit 11] See the description in EFLAGS.
          */
         UINT64 OverflowFlag : 1;
#define RFLAGS_OVERFLOW_FLAG_BIT                                     11
#define RFLAGS_OVERFLOW_FLAG_FLAG                                    0x800
#define RFLAGS_OVERFLOW_FLAG_MASK                                    0x01
#define RFLAGS_OVERFLOW_FLAG(_)                                      (((_) >> 11) & 0x01)

         /**
          * @brief I/O privilege level field
          *
          * [Bits 13:12] See the description in EFLAGS.
          */
         UINT64 IoPrivilegeLevel : 2;
#define RFLAGS_IO_PRIVILEGE_LEVEL_BIT                                12
#define RFLAGS_IO_PRIVILEGE_LEVEL_FLAG                               0x3000
#define RFLAGS_IO_PRIVILEGE_LEVEL_MASK                               0x03
#define RFLAGS_IO_PRIVILEGE_LEVEL(_)                                 (((_) >> 12) & 0x03)

         /**
          * @brief Nested task flag
          *
          * [Bit 14] See the description in EFLAGS.
          */
         UINT64 NestedTaskFlag : 1;
#define RFLAGS_NESTED_TASK_FLAG_BIT                                  14
#define RFLAGS_NESTED_TASK_FLAG_FLAG                                 0x4000
#define RFLAGS_NESTED_TASK_FLAG_MASK                                 0x01
#define RFLAGS_NESTED_TASK_FLAG(_)                                   (((_) >> 14) & 0x01)
         UINT64 Reserved3 : 1;

         /**
          * @brief Resume flag
          *
          * [Bit 16] See the description in EFLAGS.
          */
         UINT64 ResumeFlag : 1;
#define RFLAGS_RESUME_FLAG_BIT                                       16
#define RFLAGS_RESUME_FLAG_FLAG                                      0x10000
#define RFLAGS_RESUME_FLAG_MASK                                      0x01
#define RFLAGS_RESUME_FLAG(_)                                        (((_) >> 16) & 0x01)

         /**
          * @brief Virtual-8086 mode flag
          *
          * [Bit 17] See the description in EFLAGS.
          */
         UINT64 Virtual8086ModeFlag : 1;
#define RFLAGS_VIRTUAL_8086_MODE_FLAG_BIT                            17
#define RFLAGS_VIRTUAL_8086_MODE_FLAG_FLAG                           0x20000
#define RFLAGS_VIRTUAL_8086_MODE_FLAG_MASK                           0x01
#define RFLAGS_VIRTUAL_8086_MODE_FLAG(_)                             (((_) >> 17) & 0x01)

         /**
          * @brief Alignment check (or access control) flag
          *
          * [Bit 18] See the description in EFLAGS.
          *
          * @see Vol3A[4.6(ACCESS RIGHTS)]
          */
         UINT64 AlignmentCheckFlag : 1;
#define RFLAGS_ALIGNMENT_CHECK_FLAG_BIT                              18
#define RFLAGS_ALIGNMENT_CHECK_FLAG_FLAG                             0x40000
#define RFLAGS_ALIGNMENT_CHECK_FLAG_MASK                             0x01
#define RFLAGS_ALIGNMENT_CHECK_FLAG(_)                               (((_) >> 18) & 0x01)

         /**
          * @brief Virtual interrupt flag
          *
          * [Bit 19] See the description in EFLAGS.
          */
         UINT64 VirtualInterruptFlag : 1;
#define RFLAGS_VIRTUAL_INTERRUPT_FLAG_BIT                            19
#define RFLAGS_VIRTUAL_INTERRUPT_FLAG_FLAG                           0x80000
#define RFLAGS_VIRTUAL_INTERRUPT_FLAG_MASK                           0x01
#define RFLAGS_VIRTUAL_INTERRUPT_FLAG(_)                             (((_) >> 19) & 0x01)

         /**
          * @brief Virtual interrupt pending flag
          *
          * [Bit 20] See the description in EFLAGS.
          */
         UINT64 VirtualInterruptPendingFlag : 1;
#define RFLAGS_VIRTUAL_INTERRUPT_PENDING_FLAG_BIT                    20
#define RFLAGS_VIRTUAL_INTERRUPT_PENDING_FLAG_FLAG                   0x100000
#define RFLAGS_VIRTUAL_INTERRUPT_PENDING_FLAG_MASK                   0x01
#define RFLAGS_VIRTUAL_INTERRUPT_PENDING_FLAG(_)                     (((_) >> 20) & 0x01)

         /**
          * @brief Identification flag
          *
          * [Bit 21] See the description in EFLAGS.
          */
         UINT64 IdentificationFlag : 1;
#define RFLAGS_IDENTIFICATION_FLAG_BIT                               21
#define RFLAGS_IDENTIFICATION_FLAG_FLAG                              0x200000
#define RFLAGS_IDENTIFICATION_FLAG_MASK                              0x01
#define RFLAGS_IDENTIFICATION_FLAG(_)                                (((_) >> 21) & 0x01)
         UINT64 Reserved4 : 42;
     };

     UINT64 Flags;
 } RFLAGS;