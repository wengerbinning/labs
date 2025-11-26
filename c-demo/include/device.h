#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MACFMT0 "%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx"      /* eg.:     c074ada1b2c3 */
#define MACFMT1 "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx" /* eg.:c0:74:ad:a1:b2:c3 */
#define MACFMT2 "%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX"      /* eg.:     C074ADA1B2C3 */
#define MACFMT3 "%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX" /* eg.:C0:74:AD:A1:B2:C3 */

#define _STR2MAC(m) &(m)[0], &(m)[1], &(m)[2], &(m)[3], &(m)[4], &(m)[5]
#define _MAC2STR(m)  (m)[0],  (m)[1],  (m)[2],  (m)[3],  (m)[4],  (m)[5]
#define STR2MAC(mac) _STR2MAC((unsigned char *)(mac))
#define MAC2STR(mac) _MAC2STR((unsigned char *)(mac))

/** This is an abstraction layer for device addresses.

 dev_addr_t - The device's address is essentially a MAC address, so her we use
 the same structure as that of the MAC address.

 Member Description:
  - addr.raw[6]: The original data of the address.
  - addr.oui[3]: Quickly extract information about equipment manufactures.
  - addr.eui[3]: The unique ID for each device.
  - addr.ig: Determine whether this address is a private address.
  - addr.ul: Determibe whether this address is a multicast address.
 */
typedef union dev_addr {
	unsigned char raw[6];
	struct {
		union { /* OUI */
			unsigned char oui[3];
			struct {
				unsigned char _0:6;
				unsigned char ig:1;
				unsigned char ul:1;
			};
		};
		union { /* EUI */
			unsigned char eui[3];
			// TODO - struct
		};
	};
} dev_addr_t;

typedef union def_oui { /* OUI */
	unsigned char raw[3];
	struct {
		unsigned char _0:6;
		unsigned char ig:1;
		unsigned char ul:1;
	};
} dev_oui_t;

typedef union def_eui { /* EUI */
	unsigned char raw[3];
	// TODO - struct
} dev_eui_t;

#define DEV_NAMEIZE 56
/* This is an abstraction layer for device. */
typedef struct dev_state {
	uint64_t flags;
	uint64_t value;
} dev_state_t;

typedef struct device {
	uint64_t id;
	char name[DEV_NAMEIZE + 1];
	dev_addr_t addr;
	dev_state_t state;
} device_t;

/* */
#define DEV_STAT_BOOT     0x00000001 /* device boot stats */
#define DEV_STAT_DEVELOP  0x00000002 /* develop */
#define DEV_STAT_ACTIVATE 0x00000004 /* active state */
#define DEV_STAT_REGIST   0x00000008 /* regist state */
#define DEV_STAT_MANAGE   0x00000010 /* management state */

/* */
#define DEV_STAT_POWEROFF 0x00010000 /* power off  */
#define DEV_STAT_REBOOT   0x00020000 /* reboot */
#define DEV_STAT_RESET    0x00040000 /* reset */
#define DEV_STAT_UPGRADE  0x00080000 /* upgrade */

static inline int dev_set_stat (device_t *dev, uint64_t stat, int val) {
	if (val) {
		dev->state.flags |= stat;
	} else {
		dev->state.flags &= ~stat;
	}
	return dev->state.flags;
}

static inline int dev_get_stat (device_t *dev, uint64_t stat) {
	return dev->state.flags;
}

#endif /* __DEVICE_H__ */
