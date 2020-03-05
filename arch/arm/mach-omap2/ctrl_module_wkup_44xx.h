/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * OMAP44xx CTRL_MODULE_WKUP registers and bitfields
 *
 * Copyright (C) 2009-2010 Texas Instruments, Inc.
 *
 * Benoit Cousson (b-cousson@ti.com)
 * Santosh Shilimkar (santosh.shilimkar@ti.com)
 *
 * This file is automatically generated from the OMAP hardware databases.
 * We respectfully ask that any modifications to this file be coordinated
 * with the public linux-omap@vger.kernel.org mailing list and the
 * authors above to ensure that the autogeneration scripts are kept
 * up-to-date with the file contents.
 */

#ifndef __ARCH_ARM_MACH_OMAP2_CTRL_MODULE_WKUP_44XX_H
#define __ARCH_ARM_MACH_OMAP2_CTRL_MODULE_WKUP_44XX_H


/* Base address */
#define OMAP4_CTRL_MODULE_WKUP				0x4a30c000

/* Registers offset */
#define OMAP4_CTRL_MODULE_WKUP_IP_REVISION		0x0000
#define OMAP4_CTRL_MODULE_WKUP_IP_HWINFO		0x0004
#define OMAP4_CTRL_MODULE_WKUP_IP_SYSCONFIG		0x0010
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_0	0x0460
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_1	0x0464
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_2	0x0468
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_3	0x046c
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_4	0x0470
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_5	0x0474
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_6	0x0478
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_7	0x047c
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_8	0x0480
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_9	0x0484
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_10	0x0488
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_11	0x048c
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_12	0x0490
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_13	0x0494
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_14	0x0498
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_15	0x049c
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_16	0x04a0
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_17	0x04a4
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_18	0x04a8
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_19	0x04ac
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_20	0x04b0
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_21	0x04b4
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_22	0x04b8
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_23	0x04bc
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_24	0x04c0
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_25	0x04c4
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_26	0x04c8
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_27	0x04cc
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_28	0x04d0
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_29	0x04d4
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_30	0x04d8
#define OMAP4_CTRL_MODULE_WKUP_CONF_DEBUG_SEL_TST_31	0x04dc

/* Registers shifts and masks */

/* IP_REVISION */
#define OMAP4_IP_REV_SCHEME_SHIFT		30
#define OMAP4_IP_REV_SCHEME_MASK		(0x3 << 30)
#define OMAP4_IP_REV_FUNC_SHIFT			16
#define OMAP4_IP_REV_FUNC_MASK			(0xfff << 16)
#define OMAP4_IP_REV_RTL_SHIFT			11
#define OMAP4_IP_REV_RTL_MASK			(0x1f << 11)
#define OMAP4_IP_REV_MAJOR_SHIFT		8
#define OMAP4_IP_REV_MAJOR_MASK			(0x7 << 8)
#define OMAP4_IP_REV_CUSTOM_SHIFT		6
#define OMAP4_IP_REV_CUSTOM_MASK		(0x3 << 6)
#define OMAP4_IP_REV_MINOR_SHIFT		0
#define OMAP4_IP_REV_MINOR_MASK			(0x3f << 0)

/* IP_HWINFO */
#define OMAP4_IP_HWINFO_SHIFT			0
#define OMAP4_IP_HWINFO_MASK			(0xffffffff << 0)

/* IP_SYSCONFIG */
#define OMAP4_IP_SYSCONFIG_IDLEMODE_SHIFT	2
#define OMAP4_IP_SYSCONFIG_IDLEMODE_MASK	(0x3 << 2)

/* CONF_DEBUG_SEL_TST_0 */
#define OMAP4_WKUP_MODE_SHIFT			0
#define OMAP4_WKUP_MODE_MASK				(1 << 0)

#endif
