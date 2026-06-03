#ifndef __VERSION_H__
#define __VERSION_H__

#define _0 0ULL
#define _1 1ULL
#define BIT(n)     (_1 << (n))
#define MASK(n)    (BIT(n) - 1)
#define BITS(n, m) (MASK((m) + (n)) ^ MASK(n))


#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PHASE 0
#define VERSION_PATCH 0

/* Version */
#define VER_NODE(v, n, s) (((v) >> ((n) * (s))) & MASK(s))
#define NODE_VER(o, n, s) (((o) & MASK(s)) << ((n) * (s)))
#define _V(a, b, n, s) (NODE_VER(a, 2*(n) + 1, (s)) | NODE_VER(b, 2*(n), (s)))

#define VER_SIZE 8
#define VER_MAJOR(v)  VER_NODE(v, 3, VER_SIZE)
#define VER_MINOR(v)  VER_NODE(v, 2, VER_SIZE)
#define VER_PHASE(v)  VER_NODE(v, 1, VER_SIZE)
#define VER_PATCH(v)  VER_NODE(v, 0, VER_SIZE)

#define VERFMT0 "%u.%u.%u.%u"
#define VERFMT1 "v" VERFMT0
#define VERFMT2 "V" VERFMT0
#define VER2STR(v) VER_MAJOR(v), VER_MINOR(v), VER_PHASE(v), VER_PATCH(v)
#define VERSION(a, b, m, n) (_V(a, b, 1, VER_SIZE) | _V(m, n, 0, VER_SIZE))
#define MAJOR_VERSION(a, b) _V(a, b, 0, VER_SIZE)
#define MINOR_VERSION(m, n) _V(m, n, 0, VER_SIZE)

/* Feature Phase */
#define FP_FIELD_MASK 0xFF
#define FP_MAJOR_OFST 0x08
#define FP_MINOR_OFST 0x00

#define FP_MAJOR(phase) ((phase & FP_MAJOR_OFST) >> FP_FIELD_MASK)
#define FP_MINOR(phase) ((phase & FP_MINOR_OFST) >> FP_FIELD_MASK)

#define PHASE(a,b) \
	(((a & FP_FIELD_MASK) << FP_MAJOR_OFST) | \
	 ((b & FP_FIELD_MASK) << FP_MINOR_OFST))

#define PHASESTR(phase) \
	FP_MAJOR(phase), FP_MINOR(phase)


int vercode;

unsigned int ver_major (unsigned int ver);
unsigned int ver_minor (unsigned int ver);
unsigned int ver_phase (unsigned int ver);
unsigned int ver_patch (unsigned int ver);

unsigned int version (unsigned int major, unsigned int minor,
	unsigned int phase, unsigned int patch);

#endif /* __VERSION_H__ */
