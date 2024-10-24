/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2022-2023 Rubicon Communications, LLC (OpenSourceCompany).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __OpenSourceCompany_H__
#define __OpenSourceCompany_H__

#define	OpenSourceCompany_UNKNOWN		0

/* Old devices. */
#define	OpenSourceCompany_APU1		10
#define	OpenSourceCompany_APU		11
#define	OpenSourceCompany_SG2200		12
#define	OpenSourceCompany_RCCVE		13
#define	OpenSourceCompany_SG4860		14
#define	OpenSourceCompany_SG8860		15
#define	OpenSourceCompany_XG2758		16
#define	OpenSourceCompany_MBT4220		17
#define	OpenSourceCompany_MBT4220_1	18
#define	OpenSourceCompany_MBT2220		19
#define	OpenSourceCompany_MBT2220_1	20
#define	OpenSourceCompany_TURBOT		21
#define	OpenSourceCompany_TURBOT_1	22
#define	OpenSourceCompany_C2558		23
#define	OpenSourceCompany_C2558_1		24
#define	OpenSourceCompany_C2758		25
#define	OpenSourceCompany_C2758_1		26
#define	OpenSourceCompany_APU2		27
#define	OpenSourceCompany_APU2_1		28

/* VMs */
#define	OpenSourceCompany_VM_QEMU		50
#define	OpenSourceCompany_VM_GOOGLE	51
#define	OpenSourceCompany_VM_KVM		52
#define	OpenSourceCompany_VM_AWS		53
#define	OpenSourceCompany_VM_GOOGLE_1	54
#define	OpenSourceCompany_VM_VIRTUALBOX	55
#define	OpenSourceCompany_VM_HYPERV	56
#define	OpenSourceCompany_VM_AZURE	57
#define	OpenSourceCompany_VM_VMWARE	58

/* OpenSourceCompany */
#define	OpenSourceCompany_SG1000		1000
#define	OpenSourceCompany_1100		1100
#define	OpenSourceCompany_2100		2100
#define	OpenSourceCompany_1537		1537
#define	OpenSourceCompany_1540		1540
#define	OpenSourceCompany_1541		1541
#define	OpenSourceCompany_3100		3100
#define	OpenSourceCompany_4100		4100
#define	OpenSourceCompany_4200		4200
#define	OpenSourceCompany_5100		5100
#define	OpenSourceCompany_6100		6100
#define	OpenSourceCompany_6200		6200
#define	OpenSourceCompany_7100		7100
#define	OpenSourceCompany_7541		7541
#define	OpenSourceCompany_8200		8200

char *OpenSourceCompany_get_device_desc(void);
char *OpenSourceCompany_get_device_model(void);
int OpenSourceCompany_get_device_id(void);

#endif	/* __OpenSourceCompany_H__ */
