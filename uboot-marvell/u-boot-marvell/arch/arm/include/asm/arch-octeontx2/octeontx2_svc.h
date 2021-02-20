/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:    GPL-2.0
 * https://spdx.org/licenses
 */

#ifndef __OCTEONTX2_SVC_H__
#define __OCTEONTX2_SVC_H__

/* SMC function IDs for general purpose queries */

#define OCTEONTX2_SVC_CALL_COUNT		0xc200ff00
#define OCTEONTX2_SVC_UID		0xc200ff01

#define OCTEONTX2_SVC_VERSION		0xc200ff03

/* OcteonTX Service Calls version numbers */
#define OCTEONTX2_VERSION_MAJOR	0x1
#define OCTEONTX2_VERSION_MINOR	0x0

/* x1 - node number
 */
#define OCTEONTX2_DRAM_SIZE		0xc2000301
#define OCTEONTX2_NODE_COUNT		0xc2000601
#define OCTEONTX2_DISABLE_RVU_LFS	0xc2000b01

#endif /* __OCTEONTX2_SVC_H__ */
