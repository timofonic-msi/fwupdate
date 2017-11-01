/*
 * dell-wmi-smi - kernel interface to SMI over WMI
 *
 * Copyright 2017 Dell, Inc.
 *
 * See "COPYING" for license terms.
 *
 * Author: Mario Limonciello <mario.limonciello@dell.com>
 */

#ifndef _DELL_WMI_SMI_H_
#define _DELL_WMI_SMI_H_

#include <linux/version.h>
#include <sys/ioctl.h>

/* TODO: not yet upstream, make sure to update to where it really lands
 * #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,16,0)
 * #include <linux/wmi.h>
 * #else
 */
#include <linux/ioctl.h>
#include <linux/types.h>

/* WMI bus will filter all WMI vendor driver requests through this IOC */
#define WMI_IOC 'W'

/* This structure may be modified by the firmware when we enter
 * system management mode through SMM, hence the volatiles
 */
struct calling_interface_buffer {
	__u16 cmd_class;
	__u16 cmd_select;
	__volatile__ __u32 input[4];
	__volatile__ __u32 output[4];
} __attribute__((packed));

struct dell_wmi_extensions {
	__u32 argattrib;
	__u32 blength;
	__u8 data[];
} __attribute__((packed));

struct dell_wmi_smbios_buffer {
	__u64 length;
	struct calling_interface_buffer std;
	struct dell_wmi_extensions	ext;
} __attribute__((packed));

/* Whitelisted smbios class/select commands */
#define CLASS_TOKEN_READ	0
#define CLASS_TOKEN_WRITE	1
#define SELECT_TOKEN_STD	0
#define SELECT_TOKEN_BAT	1
#define SELECT_TOKEN_AC		2
#define CLASS_FLASH_INTERFACE	7
#define SELECT_FLASH_INTERFACE	3
#define CLASS_ADMIN_PROP	10
#define SELECT_ADMIN_PROP	3
#define CLASS_INFO		17
#define SELECT_RFKILL		11
#define SELECT_APP_REGISTRATION	3
#define SELECT_DOCK		22

/* whitelisted tokens */
#define CAPSULE_EN_TOKEN	0x0461
#define CAPSULE_DIS_TOKEN	0x0462
#define WSMT_EN_TOKEN		0x04EC
#define WSMT_DIS_TOKEN		0x04ED

/* Dell SMBIOS calling IOCTL command used by dell-smbios-wmi */
#define DELL_WMI_SMBIOS_CMD	_IOWR(WMI_IOC, 0, struct dell_wmi_smbios_buffer)

/* TODO: not yet upstream
 * #endif
 */

/* these aren't defined upstream but used in fwupdate */
#define DELL_ADMIN_MASK 0xF
#define DELL_ADMIN_INSTALLED 0

/* device files to interact with over wmi */
#define DELL_WMI_CHAR "/dev/wmi/dell-smbios"
#define TOKENS_SYSFS "/sys/devices/platform/dell-smbios.0/tokens"

#endif
