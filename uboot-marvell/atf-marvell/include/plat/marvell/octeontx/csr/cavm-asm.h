/***********************license start***********************************
* Copyright (C) 2018 Marvell International Ltd.
* SPDX-License-Identifier: BSD-3-Clause
* https://spdx.org/licenses
***********************license end**************************************/

/**
 * @file
 *
 * This is file defines ASM primitives for the executive.

 * <hr>$Revision: 53373 $<hr>
 *
 * @defgroup __asm__ Assembly support
 * @{
 */

/* This header file can be included from a .S file.  Keep non-preprocessor
   things under !__ASSEMBLER__.  */
#ifndef __ASSEMBLER__

/* turn the variable name into a string */
#define __TMP_STR(x) __TMP_STR2(x)
#define __TMP_STR2(x) #x
#define __VASTR(...) #__VA_ARGS__

#define MRS_NV(reg, val) __asm__ ("mrs %x[rd]," #reg : [rd] "=r" (val))
#define MRS(reg, val) __asm__ volatile ("mrs %x[rd]," #reg : [rd] "=r" (val))
#define MSR(reg, val) __asm__ volatile ("msr " #reg ",%x[rd]" : : [rd] "r" (val))

/* Barriers: The CAVM uses non-shared memory (not inner or outer shared
    in ARM speak). Inner or Outer shared instructions won't work */
#define MB          __asm__ volatile ("dmb sy"      : : :"memory") /* Full memory barrier, like MIPS SYNC */
#define WMB         __asm__ volatile ("dmb st"      : : :"memory") /* Write memory barrier, like MIPS SYNCW */

/* other useful stuff */
#define WFE         __asm__ volatile ("wfe"         : : :"memory") /* Wait for event */
#define SEV         __asm__ volatile ("sev"         : : :"memory") /* Send global event */

// normal prefetches that use the pref instruction
#define PREFETCH_PREFX(type, address, offset) __asm__ volatile ("PRFUM " type ", [%[rbase],%[off]]" : : [rbase] "r" (address), [off] "I" (offset))
// a normal prefetch
#define PREFETCH(address, offset) PREFETCH_PREFX("PLDL1KEEP", address, offset)
#define ICACHE_INVALIDATE  { __asm__ volatile ("ic iallu" : : ); }    // invalidate entire icache

#define SYS_CVMCACHE_I_L2  "#0,c11,c1,#1"           // L2 Cache Cache Hit Invalidate
#define SYS_CVMCACHE_WBI_L2 "#0,c11,c1,#2"          // L2 Cache Cache Hit Writeback Invalidate
#define SYS_CVMCACHE_WB_L2 "#0,c11,c1,#3"           // L2 Cache Hit Writeback
#define SYS_CVMCACHE_LCK_L2 "#0,c11,c1,#4"          // L2 Cache Fetch and Lock
#define SYS_CVMCACHE_WBI_L2_INDEXED "#0,c11,c0,#5"  // L2 Cache Index Writeback Invalidate
#define SYS_CVMCACHE_LTG_L2_INDEXED "#0,c11,c0,#7"  // L2 Cache Index Load Tag
#define SYS_CVMCACHE_INVALL_DC "#0,c11,c0,#2"       // L1 Dcache Invalidate
#define CACHE_I_L2(address)   { __asm__ volatile ("sys " SYS_CVMCACHE_I_L2   ", %0" : : "r" (address)); } // Don't push to memory, invalidate, and unlock
#define CACHE_WBI_L2(address) { __asm__ volatile ("sys " SYS_CVMCACHE_WBI_L2 ", %0" : : "r" (address)); } // Push to memory, invalidate, and unlock
#define CACHE_WBI_L2_INDEXED(encoded) { __asm__ volatile ("sys " SYS_CVMCACHE_WBI_L2_INDEXED ", %0" : : "r" (encoded)); } // Push to memory, invalidate, and unlock, index by set/way
#define CACHE_WB_L2(address) { __asm__ volatile ("sys " SYS_CVMCACHE_WB_L2 ", %0" : : "r" (address)); } // Push to memory, don't invalidate, don't unlock
#define CACHE_LCK_L2(address) { __asm__ volatile ("sys " SYS_CVMCACHE_LCK_L2 ", %0" : : "r" (address)); } // Lock into L2
#define DCACHE_INVALIDATE { __asm__ volatile ("sys " SYS_CVMCACHE_INVALL_DC ", xzr"); } // Invalidate the entire Dcache on local core
#define CACHE_LTG_L2_INDEXED(encoded) { __asm__ volatile ("sys " SYS_CVMCACHE_LTG_L2_INDEXED ", %0" : : "r" (encoded)); } // Load L2 TAG, index by set/way 

#define STORE_PAIR(ptr, data1, data2) { __asm__ volatile ("stp %x[d1], %x[d2], [%[b]]" : [mem] "+m" (*(__uint128_t*)ptr) : [b] "r" (ptr), [d1] "r" (data1), [d2] "r" (data2)); }

#endif	/* __ASSEMBLER__ */

/** @} */
