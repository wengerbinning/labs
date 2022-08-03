#include <stdio.h>
#include <uci.h>

static struct uci_context *ctx;

int main (int agrc, char *argv[]) {
	struct uci_package *pkg;
	struct uci_element *e1, *e2;
	struct uci_section *s;
	char path[256] = {0};
	struct uci_ptr ptr;

	const char *val;

	int ret;
	ctx = uci_alloc_context();
	if (!ctx) {
		printf("Failed to create uci context!\n");
	}

	pkg = uci_lookup_package(ctx, "network");
	if (!pkg) {
		printf("Not found network!\n");
	}
	uci_unload(ctx, pkg);


	uci_load(ctx, "network", &pkg);

	printf("==== Traverse all configuration items ====\n");

	uci_foreach_element (&pkg->sections, e1) {
		struct uci_section *s = uci_to_section(e1);

		printf("section:%s, type:%s >>> \n", s->e.name, s->type);

		//
		uci_foreach_element (&s->options, e2) {
			struct uci_option *o = uci_to_option(e2);
			switch(o->type) {
				case  UCI_TYPE_STRING:
					printf("  option:%s, type:%d, value: %s\n", o->e.name, o->type, o->v.string);
					break;
				case UCI_TYPE_LIST: {
					struct uci_element *e;
					uci_foreach_element (&o->v.list, e) {
						printf("  list:%s, type:%d, value: %s\n", o->e.name, o->type, e->name);
					}
					break;
				}
			}

		}
	}

	printf("==== Parses the specificed format configuration item ====\n");
	snprintf(path, sizeof(path), "network.@globals[0].ula_prefix=434");
	if (UCI_OK == (ret = uci_lookup_ptr(ctx, &ptr, path, true))) {
		printf("package:%s, section: %s, option: %s, value: %s\n", ptr.package, ptr.section,
		ptr.option ? ptr.option : "NULL",
		ptr.value ? ptr.value : "NULL");

	} else {
		printf("Not found!\n");
	}


	printf("==== Search for a specified configuration item ====\n");
	snprintf(path, sizeof(path), "network.@globals[0]");
	uci_lookup_ptr(ctx, &ptr, path, true);
	if ((s = uci_lookup_section(ctx, pkg, ptr.section))) {
		struct uci_option *o;

		printf("section:%s, type:%s >>> \n", s->e.name, s->type);

		//
		o = uci_lookup_option(ctx, s, "ula_prefix");
		if ( !o | o->type != UCI_TYPE_STRING) {
			printf("option type: %d\n",o->type);
		} else {
			val = uci_lookup_option_string(ctx, s, "ula_prefix");
			printf("ula_prefix = %s\n", val);
		}
	} else {
		printf("Not found section\n");
	}










	// uci_lookup_option
	// uci_lookup_ptr

	uci_free_context(ctx);

	return 0;
}