/*
 *
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 *
 */

#ifndef __BCM_UDC_H
#define __BCM_UDC_H

#include <usbdevice.h>

#define DEVICE_STRING_LANGUAGE_ID_INDEX		0
#define DEVICE_STRING_PRODUCT_INDEX		1
#define DEVICE_STRING_SERIAL_NUMBER_INDEX	2
#define DEVICE_STRING_CONFIG_INDEX		3
#define DEVICE_STRING_INTERFACE_INDEX		4
#define DEVICE_STRING_MANUFACTURER_INDEX	5
#define DEVICE_STRING_MAX_INDEX		DEVICE_STRING_MANUFACTURER_INDEX

extern char *usb_device_strings[DEVICE_STRING_MAX_INDEX+1];

typedef int (*rx_type)(const unsigned char *buffer, unsigned int buffer_size);

extern void usb_set_interface_descriptor(struct usb_interface_descriptor *desc);
extern int usb_init_otg(rx_type rx);
extern void usb_shutdown_otg(void);
extern void usb_handle_ints_otg(void);
extern int usb_send_bulk_otg(uint8_t *buffer, uint32_t len);

#endif
