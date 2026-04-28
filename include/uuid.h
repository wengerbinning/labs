#ifndef __UUID_H__
#define __UUID_H__

#include <stdio.h>
#include <time.h>

typedef union uuid {
    unsigned char raw[16];
    struct {
        unsigned char _resv1[6];
        unsigned char _resv2: 4;
        unsigned char version:4;
        unsigned char _resv3[1];
        unsigned char _resv4: 4;
        unsigned char variant:4;
        unsigned char _resv5[7];
    };
    struct {
        unsigned int   stamp1;
        unsigned short stamp2;
        unsigned char  stamp4:4;
        unsigned char version:4;
        unsigned char  stamp5:4;
        unsigned char  stamp3:4;
        unsigned char  seq1:6;
        unsigned char variant:2;
        unsigned char  seq2;
        unsigned char  node[6];
    } v1;
} uuid_t;

#define _UUID_FMT(n) "%02x%02x"
#define _UUID1A _UUID_FMT(0) _UUID_FMT(1)
#define _UUID1B _UUID_FMT(2)
#define _UUID1C _UUID_FMT(3)
#define _UUID1D _UUID_FMT(4)
#define _UUID1E _UUID_FMT(5) _UUID_FMT(6) _UUID_FMT(7)

#define _UUID_STR(u, i)  (u)->raw[(2 * (i)) + 0], (u)->raw[(2* (i)) + 1]
#define _UUID2A(u) _UUID_STR(u, 0), _UUID_STR(u, 1)
#define _UUID2B(u) _UUID_STR(u, 2)
#define _UUID2C(u) _UUID_STR(u, 3)
#define _UUID2D(u) _UUID_STR(u, 4)
#define _UUID2E(u) _UUID_STR(u, 5), _UUID_STR(u, 6), _UUID_STR(u, 7)

/* UUID - 128bit(16 octet)
 * RFC 4122 + RFC9562 -> RFC 9562
 Standard Format:
    xxxxxxxx-xxxx-Axxx-Bxxx-xxxxxxxxxxxx
 */

#define UUID_FMT0 _UUID1A "-" _UUID1B "-" _UUID1C "-" _UUID1D "-" _UUID1E
#define UUID_FMT1 _UUID1A ""  _UUID1B ""  _UUID1C ""  _UUID1D ""  _UUID1E
#define UUID_FMT UUID_FMT0
#define UUID_STR(u)  _UUID2A(u), _UUID2B(u), _UUID2C(u), _UUID2D(u), _UUID2E(u)


/* V1 */
static inline int uuid_v1_set_timestamp (uuid_t *uuid, const time_t stamp) {
    uuid->v1.stamp1 = (stamp >> 0x00) & 0xFFFFFFFF;
    uuid->v1.stamp2 = (stamp >> 0x20) & 0x0000FFFF;
    uuid->v1.stamp3 = (stamp >> 0x30) & 0x0000000F;
    uuid->v1.stamp4 = (stamp >> 0x34) & 0x0000000F;
    uuid->v1.stamp5 = (stamp >> 0x38) & 0x0000000F;
    return 0;
}

static inline int uuid_v1_set_sequence (uuid_t *uuid, const unsigned short seq) {
    uuid->v1.seq1 = (seq >> 0x00) & 0x00FF;
    uuid->v1.seq2 = (seq >> 0x04) & 0xFFFF;
    return 0;
}

static inline time_t uuid_v1_get_timestamp (uuid_t *uuid) {
    time_t stamp = 0;
    stamp |= (uuid->v1.stamp1 & 0xFFFFFFFFULL) << 0x00;
    stamp |= (uuid->v1.stamp2 & 0x0000FFFFULL) << 0x20;
    stamp |= (uuid->v1.stamp3 & 0x0000000FULL) << 0x30;
    stamp |= (uuid->v1.stamp4 & 0x0000000FULL) << 0x34;
    stamp |= (uuid->v1.stamp5 & 0x0000000FULL) << 0x38;
    return stamp;
}

static inline unsigned short uuid_v1_get_sequence (uuid_t *uuid) {
    unsigned short seq = 0;

    seq |= (uuid->v1.seq1 & 0x00FF) << 0x00;
    seq |= (uuid->v1.seq2 & 0xFFFF) << 0x04;
    return seq;
}

#endif /* __UUID_H__ */
