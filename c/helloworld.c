#include <stdio.h>

#define _BIT(n) (((1ULL << n) - 1))
#define  BIT(n) (1ULL | (_BIT(n) << 1))
#define BITS(n, m) ((m) <= (n) ? (BIT(n) - _BIT(m)) : 0)





typedef struct bitmap {
	int offset;
	int mask;
	char *name;
} regmap_t;

struct regmap {
	int default;
	struct bitmap map[32];
};

#define REGMAP_PREFIX "regmap_"
#define REG(name, val) \
	struct regmap _REGMAP(name) = { .default = val }
#define _REGMAP(name) REGMAP_PREFIX ## name

REG(demo, 0x0000);

static inline int set_map(struct regmap *reg, int n, int m, const char *name) {
	reg[m] = { .offset = m, .mask = BITS{n, m}, .name = name };
}
#define REG_ADD_MAP(label, n, m, name) set_map(_REGMAP(label), n, m, #name)

REG_ADD_MAP(demo, 31, 31, valid);
REG_ADD_MAP(demo, 31, 31, valid);



void reg_dump(struct regmap *reg) {
	int i;
	struct bitmap *map;

	for (i = 0; i < 32; i++) {
		map = &(reg->map[i]);
		if (!map->mask)
			continue;

		printf("%s: offset = %d, mask = %08X\n", map->name, map->offset, map->mask);
	}
}

#define REG_DUMP(name) reg_dump(_REGMAP(name))



int main (int argc, char *argv[]) {
	printf("Hello, World!\n");
	REG_DUMP(demo);
	return 0;
}








