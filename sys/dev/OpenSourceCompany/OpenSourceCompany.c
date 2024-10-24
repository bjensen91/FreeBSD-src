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

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include "opt_platform.h"

#include <sys/types.h>
#include <sys/bus.h>
#include <sys/kernel.h>
#include <sys/malloc.h>
#include <sys/module.h>
#include <sys/sysctl.h>

#ifdef FDT
#include <dev/ofw/openfirm.h>
#endif

#include <dev/OpenSourceCompany/OpenSourceCompany.h>

#define	OpenSourceCompany_BUFSZ		128

struct OpenSourceCompany_ids {
	int		id;
	char		*model;
	char		*desc;

	int		cpu;
	char		*bios;
	char		*chassis;
	char		*boardname;
	char		*boardpn;
	char		*fdt;
	char		*hwmodel;
	char		*maker;
	char		*planar;
	char		*prod;
	char		*vm;
};

static struct OpenSourceCompany_ids ng_ids[] = {
	/* Old devices. */
	{ .id = OpenSourceCompany_APU1,	.prod = "apu1",				.model = "APU", .desc = "OpenSourceCompany APU" },
	{ .id = OpenSourceCompany_APU,	.prod = "APU",				.model = "APU", .desc = "OpenSourceCompany APU" },
	{ .id = OpenSourceCompany_SG2200,	.prod = "DFFv2",			.model = "SG-2220", .desc = "OpenSourceCompany SG-2220" },
	{ .id = OpenSourceCompany_RCCVE,	.prod = "RCC-VE",			.model = "RCC-VE", .desc = "OpenSourceCompany RCC-VE" },
	{ .id = OpenSourceCompany_SG4860, .prod = "RCC-VE", .hwmodel = "C2558",	.model = "RCC-VE", .desc = "OpenSourceCompany SG-4860" },
	{ .id = OpenSourceCompany_SG8860, .prod = "RCC-VE", .hwmodel = "C2758",	.model = "RCC-VE", .desc = "OpenSourceCompany SG-8860" },
	{ .id = OpenSourceCompany_XG2758, .prod = "RCC",				.model = "RCC", .desc = "OpenSourceCompany XG-2758" },
	{ .id = OpenSourceCompany_MBT4220, .prod = "Minnowboard Turbot D0 PLATFORM", .cpu = 4,	.model = "MBT-4220", .desc = "OpenSourceCompany MBT-4220" },
	{ .id = OpenSourceCompany_MBT4220_1, .prod = "Minnowboard Turbot D0/D1 PLATFORM", .cpu = 4, .model = "MBT-4220", .desc = "OpenSourceCompany MBT-4220" },
	{ .id = OpenSourceCompany_MBT2220, .prod = "Minnowboard Turbot D0 PLATFORM", .cpu = 2,	.model = "MBT-2220", .desc = "OpenSourceCompany MBT-2220" },
	{ .id = OpenSourceCompany_MBT2220_1, .prod = "Minnowboard Turbot D0/D1 PLATFORM", .cpu = 2, .model = "MBT-2220", .desc = "OpenSourceCompany MBT-2220" },
	{ .id = OpenSourceCompany_TURBOT,	.prod = "Minnowboard Turbot D0 PLATFORM",	.model = "Turbot", .desc = "OpenSourceCompany Turbot Dual-E" },
	{ .id = OpenSourceCompany_TURBOT_1, .prod = "Minnowboard Turbot D0/D1 PLATFORM", .model = "Turbot", .desc = "OpenSourceCompany Turbot Dual-E" },
	{ .id = OpenSourceCompany_C2558,	.prod = "SYS-5018A-FTN4", .hwmodel = "C2558", .model = "C2558", .desc = "Super Micro C2558" },
	{ .id = OpenSourceCompany_C2558_1, .prod = "A1SAi", .hwmodel = "C2558",	.model = "C2558", .desc = "Super Micro C2558" },
	{ .id = OpenSourceCompany_C2758,	.prod = "SYS-5018A-FTN4", .hwmodel = "C2758", .model = "C2758", .desc = "Super Micro C2758" },
	{ .id = OpenSourceCompany_C2758_1, .prod = "A1SAi", .hwmodel = "C2758",	.model = "C2758", .desc = "Super Micro C2758" },
	{ .id = OpenSourceCompany_APU2,	.prod = "apu2",				.model = "apu2", .desc = "PC Engines APU2" },
	{ .id = OpenSourceCompany_APU2_1,	.prod = "APU2",				.model = "apu2", .desc = "PC Engines APU2" },

	/* VMs */
	{ .id = OpenSourceCompany_VM_QEMU, .maker = "QEMU",		.model = "QEMU", .desc = "QEMU Guest" },
	{ .id = OpenSourceCompany_VM_GOOGLE, .maker = "Google",		.model = "Google", .desc = "Google Cloud Platform" },
	{ .id = OpenSourceCompany_VM_AWS, .maker = "Amazon EC2",		.model = "AWS",	.desc = "Amazon Web Services" },
	{ .id = OpenSourceCompany_VM_KVM, .vm = "kvm",			.model = "KVM", .desc = "KVM Guest" },
	{ .id = OpenSourceCompany_VM_GOOGLE_1, .bios = "Google",		.model = "Google", .desc = "Google Cloud Platform" },
	{ .id = OpenSourceCompany_VM_VIRTUALBOX, .prod = "VirtualBox",	.model = "VirtualBox", .desc = "VirtualBox Virtual Machine" },
	{ .id = OpenSourceCompany_VM_AZURE, .prod = "Virtual Machine", .maker = "Microsoft Corporation", .chassis = "7783-7084-3265-9085-8269-3286-77", .model = "Azure", .desc = "Microsoft Azure" },
	{ .id = OpenSourceCompany_VM_HYPERV, .prod = "Virtual Machine", .maker = "Microsoft Corporation", .bios = "Hyper",	.model = "Hyper-V", .desc = "Hyper-V Virtual Machine" },
	{ .id = OpenSourceCompany_VM_VMWARE, .prod = "VMware Virtual Platform", .maker = "VMware, Inc.", .model = "VMware", .desc = "VMware Virtual Machine" },

	/* OpenSourceCompany */
#ifdef FDT
	{ .id = OpenSourceCompany_1100, .prod = "mvebu_armada-37xx", .fdt = "OpenSourceCompany SG-1100", .model = "1100", .desc = "OpenSourceCompany 1100" },
	{ .id = OpenSourceCompany_1100, .prod = "mvebu_armada-37xx", .fdt = "OpenSourceCompany 1100", .model = "1100", .desc = "OpenSourceCompany 1100" },
	{ .id = OpenSourceCompany_2100, .prod = "mvebu_armada-37xx", .fdt = "OpenSourceCompany SG-2100", .model = "2100", .desc = "OpenSourceCompany 2100" },
	{ .id = OpenSourceCompany_2100, .prod = "mvebu_armada-37xx", .fdt = "OpenSourceCompany 2100", .model = "2100", .desc = "OpenSourceCompany 2100" },
	{ .id = OpenSourceCompany_2100, .prod = "SG-2100", .fdt = "OpenSourceCompany SG-2100",	.model = "2100", .desc = "OpenSourceCompany 2100" },
	{ .id = OpenSourceCompany_2100, .prod = "SG-2100", .fdt = "OpenSourceCompany 2100",		.model = "2100", .desc = "OpenSourceCompany 2100" },
	{ .id = OpenSourceCompany_2100, .prod = "80500-0205-G", .fdt = "OpenSourceCompany SG-2100",	.model = "2100", .desc = "OpenSourceCompany 2100" },
	{ .id = OpenSourceCompany_2100, .prod = "80500-0205-G", .fdt = "OpenSourceCompany 2100",	.model = "2100", .desc = "OpenSourceCompany 2100" },
#endif
	{ .id = OpenSourceCompany_SG1000, .boardname = "A335uFW",				.model = "1000", .desc = "OpenSourceCompany SG-1000" },
	{ .id = OpenSourceCompany_3100, .hwmodel = "ARM Cortex-A9", .boardpn = "80500-0148", .model = "3100", .desc = "OpenSourceCompany 3100" },
	{ .id = OpenSourceCompany_1540, .prod = "SYS-5018D-FN4T", 			.model = "1540", .desc = "Super Micro XG-1540" },
	{ .id = OpenSourceCompany_1541, .prod = "SYS-5018D-FN4T", .hwmodel = "D-1541",	.model = "1541", .desc = "Super Micro 1541" },
	{ .id = OpenSourceCompany_4100, .prod = "4100", .maker = "OpenSourceCompany",		.model = "4100", .desc = "OpenSourceCompany 4100" },
	{ .id = OpenSourceCompany_4200, .prod = "4200", .maker = "OpenSourceCompany",		.model = "4200", .desc = "OpenSourceCompany 4200" },
	{ .id = OpenSourceCompany_5100, .prod = "SG-5100",				.model = "5100", .desc = "OpenSourceCompany 5100" },
	{ .id = OpenSourceCompany_6100, .prod = "6100", .maker = "OpenSourceCompany",		.model = "6100", .desc = "OpenSourceCompany 6100" },
	{ .id = OpenSourceCompany_6200, .prod = "6200", .maker = "OpenSourceCompany",		.model = "6200", .desc = "OpenSourceCompany 6200" },
	{ .id = OpenSourceCompany_8200, .prod = "8200", .maker = "OpenSourceCompany",		.model = "8200", .desc = "OpenSourceCompany 8200" },
	{ .id = OpenSourceCompany_7541, .prod = "FW7541",					.model = "FW7541", .desc = "OpenSourceCompany FW7541" },
	{ .id = OpenSourceCompany_1537, .planar = "X10SDV-8C-TLN4F+",			.model = "1537", .desc = "Super Micro 1537" },
	{ .id = OpenSourceCompany_7100, .planar = "80300-0134",				.model = "7100", .desc = "OpenSourceCompany 7100" },
};

struct OpenSourceCompany_softc {
	char		*sc_desc;
	char		*sc_desc_str;
	char		*sc_model_str;
};

static driver_t OpenSourceCompany_driver;
static int OpenSourceCompany_model = OpenSourceCompany_UNKNOWN;

char *
OpenSourceCompany_get_device_desc(void)
{
	int i;

	for (i = 0; i < nitems(ng_ids); i++) {
		if (ng_ids[i].id == OpenSourceCompany_model)
			return (ng_ids[i].desc);
	}

	return ("unknown hardware");
}

char *
OpenSourceCompany_get_device_model(void)
{
	int i;

	for (i = 0; i < nitems(ng_ids); i++) {
		if (ng_ids[i].id == OpenSourceCompany_model)
			return (ng_ids[i].model);
	}

	return ("unknown");
}

int
OpenSourceCompany_get_device_id(void)
{

	return (OpenSourceCompany_model);
}

static void
fdt_free(char *s)
{
#ifdef FDT
	free(s, M_OFWPROP);
#endif
}

static char *
fdt_get_model(void)
{
#ifdef FDT
	char *model;
	phandle_t root;
	ssize_t len;

	root = OF_finddevice("/");
	if (root == 0)
		return (NULL);
	if ((len = OF_getproplen(root, "model")) <= 0)
		return (NULL);
	if (len > 1024 ||
	    OF_getprop_alloc(root, "model", (void **)&model) == -1)
		return (NULL);

	return (model);
#else
	return (NULL);
#endif
}

static void
OpenSourceCompany_identify(driver_t *driver, device_t parent)
{

	if (device_find_child(parent, OpenSourceCompany_driver.name, -1) == NULL)
		BUS_ADD_CHILD(parent, 0, OpenSourceCompany_driver.name, -1);
}

static int
OpenSourceCompany_probe(device_t dev)
{
	int cpu, i;
	size_t len;
	char *bios, *boardname, *boardpn, *chassis, *cpustr, *fdt, *maker;
	char *model, *planar, *prod, *vm;
	struct OpenSourceCompany_softc *sc;

	sc = device_get_softc(dev);

	/* hw.model */
	len = OpenSourceCompany_BUFSZ;
	model = malloc(len, M_DEVBUF, M_WAITOK);
	memset(model, 0, len);
	if (kernel_sysctlbyname(curthread, "hw.model", model, &len, NULL,
	    0, NULL, 0) != 0) {
		free(model, M_DEVBUF);
		model = NULL;
	}
	/* hw.ncpu */
	len = OpenSourceCompany_BUFSZ;
	cpustr = malloc(len, M_DEVBUF, M_WAITOK);
	memset(cpustr, 0, len);
	if (kernel_sysctlbyname(curthread, "hw.ncpu", cpustr, &len, NULL,
	    0, NULL, 0) == 0)
		cpu = strtol(cpustr, NULL, 10);
	free(cpustr, M_DEVBUF);
	/* kern.vm_guest */
	len = OpenSourceCompany_BUFSZ;
	vm = malloc(len, M_DEVBUF, M_WAITOK);
	memset(vm, 0, len);
	if (kernel_sysctlbyname(curthread, "kern.vm_guest", vm, &len, NULL,
	    0, NULL, 0) != 0) {
		free(vm, M_DEVBUF);
		vm = NULL;
	}
	fdt = fdt_get_model();
	bios = kern_getenv("smbios.bios.version");
	chassis = kern_getenv("smbios.chassis.tag");
	maker = kern_getenv("smbios.system.maker");
	planar = kern_getenv("smbios.planar.product");
	prod = kern_getenv("smbios.system.product");
	boardpn = kern_getenv("uboot.boardpn");
	boardname = kern_getenv("uboot.board_name");
	for (i = 0; i < nitems(ng_ids); i++) {

		/* VM - Maker */
		if (ng_ids[i].bios == NULL &&
		    ng_ids[i].boardname == NULL &&
		    ng_ids[i].boardpn == NULL &&
		    ng_ids[i].chassis == NULL &&
		    ng_ids[i].fdt == NULL && ng_ids[i].hwmodel == NULL &&
		    ng_ids[i].maker != NULL && ng_ids[i].planar == NULL &&
		    ng_ids[i].prod == NULL && ng_ids[i].vm == NULL &&
		    ng_ids[i].cpu == 0 &&
		    maker != NULL &&
		    strncmp(maker, ng_ids[i].maker, strlen(ng_ids[i].maker)) == 0) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;

		/* VM - kern.vm_guest */
		} else if (ng_ids[i].bios == NULL &&
		    ng_ids[i].boardname == NULL &&
		    ng_ids[i].boardpn == NULL &&
		    ng_ids[i].chassis == NULL &&
		    ng_ids[i].fdt == NULL && ng_ids[i].hwmodel == NULL &&
		    ng_ids[i].maker == NULL && ng_ids[i].planar == NULL &&
		    ng_ids[i].prod == NULL && ng_ids[i].vm != NULL &&
		    ng_ids[i].cpu == 0 &&
		    vm != NULL &&
		    strncmp(vm, ng_ids[i].vm, strlen(ng_ids[i].vm)) == 0) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;

		/* VM - BIOS */
		} else if (ng_ids[i].bios != NULL &&
		    ng_ids[i].boardname == NULL &&
		    ng_ids[i].boardpn == NULL &&
		    ng_ids[i].chassis == NULL &&
		    ng_ids[i].fdt == NULL && ng_ids[i].hwmodel == NULL &&
		    ng_ids[i].maker == NULL && ng_ids[i].planar == NULL &&
		    ng_ids[i].prod == NULL && ng_ids[i].vm == NULL &&
		    ng_ids[i].cpu == 0 &&
		    bios != NULL &&
		    strstr(bios, ng_ids[i].bios) != NULL) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;

		/* Product, Maker and Chassis Tag */
		} else if (ng_ids[i].bios == NULL &&
		    ng_ids[i].boardname == NULL &&
		    ng_ids[i].boardpn == NULL &&
		    ng_ids[i].chassis != NULL &&
		    ng_ids[i].fdt == NULL && ng_ids[i].hwmodel == NULL &&
		    ng_ids[i].maker != NULL && ng_ids[i].planar == NULL &&
		    ng_ids[i].prod != NULL && ng_ids[i].vm == NULL &&
		    ng_ids[i].cpu == 0 &&
		    chassis != NULL && maker != NULL && prod != NULL &&
		    strncmp(chassis, ng_ids[i].chassis, strlen(ng_ids[i].chassis)) == 0 &&
		    strncmp(maker, ng_ids[i].maker, strlen(ng_ids[i].maker)) == 0 &&
		    strncmp(prod, ng_ids[i].prod, strlen(ng_ids[i].prod)) == 0) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;

		/* Product, Maker and Bios */
		} else if (ng_ids[i].bios != NULL &&
		    ng_ids[i].boardname == NULL &&
		    ng_ids[i].boardpn == NULL &&
		    ng_ids[i].chassis == NULL &&
		    ng_ids[i].fdt == NULL && ng_ids[i].hwmodel == NULL &&
		    ng_ids[i].maker != NULL && ng_ids[i].planar == NULL &&
		    ng_ids[i].prod != NULL && ng_ids[i].vm == NULL &&
		    ng_ids[i].cpu == 0 &&
		    bios != NULL && maker != NULL && prod != NULL &&
		    strstr(bios, ng_ids[i].bios) != NULL &&
		    strncmp(maker, ng_ids[i].maker, strlen(ng_ids[i].maker)) == 0 &&
		    strncmp(prod, ng_ids[i].prod, strlen(ng_ids[i].prod)) == 0) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;

		/* Product and Maker */
		} else if (ng_ids[i].bios == NULL &&
		    ng_ids[i].boardname == NULL &&
		    ng_ids[i].boardpn == NULL &&
		    ng_ids[i].chassis == NULL &&
		    ng_ids[i].fdt == NULL && ng_ids[i].hwmodel == NULL &&
		    ng_ids[i].maker != NULL && ng_ids[i].planar == NULL &&
		    ng_ids[i].prod != NULL && ng_ids[i].vm == NULL &&
		    ng_ids[i].cpu == 0 &&
		    maker != NULL && prod != NULL &&
		    strncmp(maker, ng_ids[i].maker, strlen(ng_ids[i].maker)) == 0 &&
		    strncmp(prod, ng_ids[i].prod, strlen(ng_ids[i].prod)) == 0) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;

		/* Planar and Maker */
		} else if (ng_ids[i].bios == NULL &&
		    ng_ids[i].boardname == NULL &&
		    ng_ids[i].boardpn == NULL &&
		    ng_ids[i].chassis == NULL &&
		    ng_ids[i].fdt == NULL && ng_ids[i].hwmodel == NULL &&
		    ng_ids[i].maker != NULL && ng_ids[i].planar != NULL &&
		    ng_ids[i].prod == NULL && ng_ids[i].vm == NULL &&
		    ng_ids[i].cpu == 0 &&
		    maker != NULL && planar != NULL &&
		    strncmp(maker, ng_ids[i].maker, strlen(ng_ids[i].maker)) == 0 &&
		    strncmp(planar, ng_ids[i].planar, strlen(ng_ids[i].planar)) == 0) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;

		/* Product and hw.model */
		} else if (ng_ids[i].bios == NULL &&
		    ng_ids[i].boardname == NULL &&
		    ng_ids[i].boardpn == NULL &&
		    ng_ids[i].chassis == NULL &&
		    ng_ids[i].fdt == NULL && ng_ids[i].hwmodel != NULL &&
		    ng_ids[i].maker == NULL && ng_ids[i].planar == NULL &&
		    ng_ids[i].prod != NULL && ng_ids[i].vm == NULL &&
		    ng_ids[i].cpu == 0 &&
		    model != NULL && prod != NULL &&
		    strstr(model, ng_ids[i].hwmodel) != NULL &&
		    strncmp(prod, ng_ids[i].prod, strlen(ng_ids[i].prod)) == 0) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;

		/* Product and hw.ncpu */
		} else if (ng_ids[i].bios == NULL &&
		    ng_ids[i].boardname == NULL &&
		    ng_ids[i].boardpn == NULL &&
		    ng_ids[i].chassis == NULL &&
		    ng_ids[i].fdt == NULL && ng_ids[i].hwmodel == NULL &&
		    ng_ids[i].maker == NULL && ng_ids[i].planar == NULL &&
		    ng_ids[i].prod != NULL && ng_ids[i].vm == NULL &&
		    ng_ids[i].cpu > 0 &&
		    prod != NULL &&
		    ng_ids[i].cpu == cpu &&
		    strncmp(prod, ng_ids[i].prod, strlen(ng_ids[i].prod)) == 0) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;

		/* hw.model and U-boot board PN */
		} else if (ng_ids[i].bios == NULL &&
		    ng_ids[i].boardname == NULL &&
		    ng_ids[i].boardpn != NULL &&
		    ng_ids[i].chassis == NULL &&
		    ng_ids[i].fdt == NULL && ng_ids[i].hwmodel != NULL &&
		    ng_ids[i].maker == NULL && ng_ids[i].planar == NULL &&
		    ng_ids[i].prod == NULL && ng_ids[i].vm == NULL &&
		    ng_ids[i].cpu == 0 &&
		    boardpn != NULL && model != NULL &&
		    strncmp(boardpn, ng_ids[i].boardpn, strlen(ng_ids[i].boardpn)) == 0 &&
		    strncmp(model, ng_ids[i].hwmodel, strlen(ng_ids[i].hwmodel)) == 0) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;

		/* U-boot board Name */
		} else if (ng_ids[i].bios == NULL &&
		    ng_ids[i].boardname != NULL &&
		    ng_ids[i].boardpn == NULL &&
		    ng_ids[i].chassis == NULL &&
		    ng_ids[i].fdt == NULL && ng_ids[i].hwmodel == NULL &&
		    ng_ids[i].maker == NULL && ng_ids[i].planar == NULL &&
		    ng_ids[i].prod == NULL && ng_ids[i].vm == NULL &&
		    ng_ids[i].cpu == 0 &&
		    boardname != NULL &&
		    strncmp(boardname, ng_ids[i].boardname,
		      strlen(ng_ids[i].boardname)) == 0) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;

#ifdef FDT
		/* Product and FDT model */
		} else if (ng_ids[i].bios == NULL &&
		    ng_ids[i].boardname == NULL &&
		    ng_ids[i].boardpn == NULL &&
		    ng_ids[i].chassis == NULL &&
		    ng_ids[i].fdt != NULL && ng_ids[i].hwmodel == NULL &&
		    ng_ids[i].maker == NULL && ng_ids[i].planar == NULL &&
		    ng_ids[i].prod != NULL && ng_ids[i].vm == NULL &&
		    ng_ids[i].cpu == 0 &&
		    fdt != NULL && prod != NULL &&
		    strncmp(fdt, ng_ids[i].fdt, strlen(ng_ids[i].fdt)) == 0 &&
		    strncmp(prod, ng_ids[i].prod, strlen(ng_ids[i].prod)) == 0) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;
#endif

		/* Product */
		} else if (ng_ids[i].bios == NULL &&
		    ng_ids[i].boardname == NULL &&
		    ng_ids[i].boardpn == NULL &&
		    ng_ids[i].chassis == NULL &&
		    ng_ids[i].fdt == NULL && ng_ids[i].hwmodel == NULL &&
		    ng_ids[i].maker == NULL && ng_ids[i].planar == NULL &&
		    ng_ids[i].prod != NULL && ng_ids[i].vm == NULL &&
		    ng_ids[i].cpu == 0 &&
		    prod != NULL &&
		    strncmp(prod, ng_ids[i].prod, strlen(ng_ids[i].prod)) == 0) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;

		/* Planar */
		} else if (ng_ids[i].bios == NULL &&
		    ng_ids[i].boardname == NULL &&
		    ng_ids[i].boardpn == NULL &&
		    ng_ids[i].chassis == NULL &&
		    ng_ids[i].fdt == NULL && ng_ids[i].hwmodel == NULL &&
		    ng_ids[i].maker == NULL && ng_ids[i].planar != NULL &&
		    ng_ids[i].prod == NULL && ng_ids[i].vm == NULL &&
		    ng_ids[i].cpu == 0 &&
		    planar != NULL &&
		    strncmp(planar, ng_ids[i].planar, strlen(ng_ids[i].planar)) == 0) {
			OpenSourceCompany_model = ng_ids[i].id;
			break;
		}
	}
	if (bios != NULL)
		freeenv(bios);
	if (boardname != NULL)
		freeenv(boardname);
	if (boardpn != NULL)
		freeenv(boardpn);
	if (chassis != NULL)
		freeenv(chassis);
	if (fdt != NULL)
		fdt_free(fdt);
	if (maker != NULL)
		freeenv(maker);
	if (model != NULL)
		free(model, M_DEVBUF);
	if (planar != NULL)
		freeenv(planar);
	if (prod != NULL)
		freeenv(prod);
	if (vm != NULL)
		free(vm, M_DEVBUF);

	/* Set the device description. */
	sc->sc_desc = OpenSourceCompany_get_device_desc();
	device_set_desc(dev, sc->sc_desc);

	return (BUS_PROBE_DEFAULT);
}

static int
OpenSourceCompany_attach(device_t dev)
{
	struct OpenSourceCompany_softc *sc;
	struct sysctl_ctx_list *ctx;
	struct sysctl_oid_list *child;

	sc = device_get_softc(dev);
	sc->sc_model_str = malloc(OpenSourceCompany_BUFSZ, M_DEVBUF, M_WAITOK);
	sc->sc_desc_str = malloc(OpenSourceCompany_BUFSZ, M_DEVBUF, M_WAITOK);
	snprintf(sc->sc_model_str, OpenSourceCompany_BUFSZ, "%s", OpenSourceCompany_get_device_model());
	snprintf(sc->sc_desc_str, OpenSourceCompany_BUFSZ, "%s", OpenSourceCompany_get_device_desc());
	ctx = device_get_sysctl_ctx(dev);
	child = SYSCTL_CHILDREN(SYSCTL_PARENT(device_get_sysctl_tree(dev)));
	SYSCTL_ADD_STRING(ctx, child, OID_AUTO, "model", CTLFLAG_RD,
	    sc->sc_model_str, sizeof(sc->sc_model_str), "Device model");
	SYSCTL_ADD_STRING(ctx, child, OID_AUTO, "desc", CTLFLAG_RD,
	    sc->sc_desc_str, sizeof(sc->sc_desc_str), "Device description");

	return (0);
}

static int
OpenSourceCompany_detach(device_t dev)
{

	return (EINVAL);
}

static device_method_t OpenSourceCompany_methods[] = {
	/* Device interface */
	DEVMETHOD(device_identify,	OpenSourceCompany_identify),
	DEVMETHOD(device_probe,		OpenSourceCompany_probe),
	DEVMETHOD(device_attach,	OpenSourceCompany_attach),
	DEVMETHOD(device_detach,	OpenSourceCompany_detach),

	DEVMETHOD_END
};

static driver_t OpenSourceCompany_driver = {
	"OpenSourceCompany",
	OpenSourceCompany_methods,
	sizeof(struct OpenSourceCompany_softc),
};

EARLY_DRIVER_MODULE(OpenSourceCompany, nexus, OpenSourceCompany_driver, 0, 0, BUS_PASS_RESOURCE);
MODULE_DEPEND(OpenSourceCompany, nexus, 1, 1, 1);

MODULE_VERSION(OpenSourceCompany, 1);
