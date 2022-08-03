#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <uci.h>

enum server_type {
	SERV_CONFIG,
	SERV_FIRMWARE,
	SERV_TYPE_MAX
};


typedef struct prov {
    struct {
        unsigned int log;
        unsigned int role;
    } sys;

#define RES_MODEL		0
#define RES_FAMILIES	1
#define RES_PLATFORM	2
#define RES_FILES_MAX	3
	struct {
		int type;
		char files[RES_FILES_MAX];
	} res;

    struct {
        unsigned int proto;
        char *server[SERV_TYPE_MAX];
        unsigned int dhcp_option;
        bool boot;
    } cnf;





} prov_t;

#define error(fmt, ...)     printf(fmt "\n", ## __VA_ARGS__)
#define warning(fmt, ...)   printf(fmt "\n", ## __VA_ARGS__)
#define debug(fmt, ...)     printf("[%s: %d] "fmt"\n",__func__, __LINE__,  ## __VA_ARGS__)


// system.system.log_level

// controller.controller.role

// grandstream.provision.protocol
// grandstream.provision.allow_dhcp_override
// grandstream.provision.config_server
// grandstream.provision.server
// grandstream.provision.on_boot

// provision.provision.downgrade_prohibited
// provision.provision.allow_dhcp_override
// provision.provision.server
// provision.provision.on_boot


static  struct uci_option * config_lookup_option (struct uci_context *ctx, struct uci_package *pkg, const char *p) {
	char path[256 + 1] = {0};
	struct uci_ptr ptr;
	struct uci_section *s;

	if (!p)
		return NULL;

	snprintf(path, sizeof(path) - 1, "%s.%s", pkg->e.name, p);
	uci_lookup_ptr(ctx, &ptr, path, true);
	if ((s = uci_lookup_section(ctx, pkg, ptr.section))) {
		return uci_lookup_option(ctx, s, ptr.option);
	} else {
		debug("Not found %s.%s.%s", ptr.package, ptr.section, ptr.option);
	}

	return NULL;
}

int prov_config_init (prov_t *prov) {
	struct uci_context *ctx;
	struct uci_package *system = NULL, *controller = NULL;
	struct uci_package *provision = NULL, *grandstream = NULL;
	struct uci_section *s;
	struct uci_option *o;
	bool load = false;

	ctx = uci_alloc_context();

	/*
	 * system configuration
	 */
	uci_load(ctx, "system", &system);
	if (system) {
		// system.@system[0].log_level
		if ((o = config_lookup_option(ctx, system, "@system[0].log_level"))) {
			debug("Found %s.%s.%s", o->section->package->e.name, o->section->e.name, o->e.name);
			switch (o->type) {
				case UCI_TYPE_STRING:
					printf("value: \'%s\'\n", o->v.string);
					break;
				case UCI_TYPE_LIST: {
					struct uci_element *e;
					uci_foreach_element (&o->v.list, e) {
						printf("value: \'%s\'\n", o->e.name, o->type, e->name);
					}
					break;
				}
			}
		}
		// unload system configuration.
		uci_unload(ctx, system);
	} else {
		error("Not found system configuration package!");
	}

	/*
	 * controller configuration
	 */
	uci_load(ctx, "controller", &controller);
	if (controller) {
		// controller.@controller[0].role
		if ((o = config_lookup_option(ctx, controller, "@controller[0].role"))) {
			debug("Found %s.%s.%s", o->section->package->e.name, o->section->e.name, o->e.name);
			switch (o->type) {
				case UCI_TYPE_STRING:
					printf("value: \'%s\'\n", o->v.string);
					break;
				case UCI_TYPE_LIST: {
					struct uci_element *e;
					uci_foreach_element (&o->v.list, e) {
						printf("value: \'%s\'\n", o->e.name, o->type, e->name);
					}
					break;
				}
			}
		}
		// unload controller configuration.
		uci_unload(ctx, controller);
	} else {
		error("Not found controller configuration package!");
	}

	/*
	 * provision configuration
	 */
	uci_load(ctx, "provision", &provision);

	if (provision) {
		// provision.@provision[0].allow_dhcp_override
		if ((o = config_lookup_option(ctx, provision, "@provision[0].allow_dhcp_override"))) {
			debug("Found %s.%s.%s", o->section->package->e.name, o->section->e.name, o->e.name);
			switch (o->type) {
				case UCI_TYPE_STRING:
					printf("value: \'%s\'\n", o->v.string);
					break;
				case UCI_TYPE_LIST: {
					struct uci_element *e;
					uci_foreach_element (&o->v.list, e) {
						printf("value: \'%s\'\n", o->e.name, o->type, e->name);
					}
					break;
				}
			}
		}

		// provision.@provision[0].downgrade_prohibited
		if ((o = config_lookup_option(ctx, provision, "@provision[0].downgrade_prohibited"))) {
			debug("Found %s.%s.%s", o->section->package->e.name, o->section->e.name, o->e.name);
			switch (o->type) {
				case UCI_TYPE_STRING:
					printf("value: \'%s\'\n", o->v.string);
					break;
				case UCI_TYPE_LIST: {
					struct uci_element *e;
					uci_foreach_element (&o->v.list, e) {
						printf("value: \'%s\'\n", o->e.name, o->type, e->name);
					}
					break;
				}
			}
		}

		// provision.@provision[0].on_boot
		if ((o = config_lookup_option(ctx, provision, "@provision[0].on_boot"))) {
			debug("Found %s.%s.%s", o->section->package->e.name, o->section->e.name, o->e.name);
			switch (o->type) {
				case UCI_TYPE_STRING:
					printf("value: \'%s\'\n", o->v.string);
					break;
				case UCI_TYPE_LIST: {
					struct uci_element *e;
					uci_foreach_element (&o->v.list, e) {
						printf("value: \'%s\'\n", o->e.name, o->type, e->name);
					}
					break;
				}
			}
		}

		// provision.@provision[0].config_server
		if ((o = config_lookup_option(ctx, provision, "@provision[0].config_server"))) {
			debug("Found %s.%s.%s", o->section->package->e.name, o->section->e.name, o->e.name);
			switch (o->type) {
				case UCI_TYPE_STRING:
					printf("value: \'%s\'\n", o->v.string);
					prov->cnf.server[SERV_CONFIG] = strdup(o->v.string);
					break;
				case UCI_TYPE_LIST: {
					struct uci_element *e;

					uci_foreach_element (&o->v.list, e) {
						printf("value: \'%s\'\n", o->e.name, o->type, e->name);
					}
					break;
				}
			}
		}

		// provision.@provision[0].server
		if ((o = config_lookup_option(ctx, provision, "@provision[0].server"))) {
			debug("Found %s.%s.%s", o->section->package->e.name, o->section->e.name, o->e.name);
			switch (o->type) {
				case UCI_TYPE_STRING:
					printf("value: \'%s\'\n", o->v.string);
					prov->cnf.server[SERV_FIRMWARE] = strdup(o->v.string);
					break;
				case UCI_TYPE_LIST: {
					struct uci_element *e;
					uci_foreach_element (&o->v.list, e) {
						printf("value: \'%s\'\n", o->e.name, o->type, e->name);
					}
					break;
				}
			}
		}



		// unload provision configuration.
		load = true;
		uci_unload(ctx, provision);
	} else {
		error("Not found provision configuration package!");
	}

	/*
	 * grandstream configuration
	 */

	if (!load)
		uci_load(ctx, "grandstream", &grandstream);

	if (grandstream) {
		// grandstream.@provision[0].allow_dhcp_override
		if ((o = config_lookup_option(ctx, grandstream, "@provision[0].allow_dhcp_override"))) {
			debug("Found %s.%s.%s", o->section->package->e.name, o->section->e.name, o->e.name);
			switch (o->type) {
				case UCI_TYPE_STRING:
					printf("value: \'%s\'\n", o->v.string);
					break;
				case UCI_TYPE_LIST: {
					struct uci_element *e;
					uci_foreach_element (&o->v.list, e) {
						printf("value: \'%s\'\n", o->e.name, o->type, e->name);
					}
					break;
				}
			}
		}

		// grandstream.@provision[0].downgrade_prohibited
		if ((o = config_lookup_option(ctx, grandstream, "@provision[0].downgrade_prohibited"))) {
			debug("Found %s.%s.%s", o->section->package->e.name, o->section->e.name, o->e.name);
			switch (o->type) {
				case UCI_TYPE_STRING:
					printf("value: \'%s\'\n", o->v.string);
					break;
				case UCI_TYPE_LIST: {
					struct uci_element *e;
					uci_foreach_element (&o->v.list, e) {
						printf("value: \'%s\'\n", o->e.name, o->type, e->name);
					}
					break;
				}
			}
		}

		// grandstream.@provision[0].on_boot
		if ((o = config_lookup_option(ctx, grandstream, "@provision[0].on_boot"))) {
			debug("Found %s.%s.%s", o->section->package->e.name, o->section->e.name, o->e.name);
			switch (o->type) {
				case UCI_TYPE_STRING:
					printf("value: \'%s\'\n", o->v.string);
					break;
				case UCI_TYPE_LIST: {
					struct uci_element *e;
					uci_foreach_element (&o->v.list, e) {
						printf("value: \'%s\'\n", o->e.name, o->type, e->name);
					}
					break;
				}
			}
		}

		// grandstream.@provision[0].server
		if ((o = config_lookup_option(ctx, grandstream, "@provision[0].server"))) {
			debug("Found %s.%s.%s", o->section->package->e.name, o->section->e.name, o->e.name);
			switch (o->type) {
				case UCI_TYPE_STRING:
					printf("value: \'%s\'\n", o->v.string);
					break;
				case UCI_TYPE_LIST: {
					struct uci_element *e;
					uci_foreach_element (&o->v.list, e) {
						printf("value: \'%s\'\n", o->e.name, o->type, e->name);
					}
					break;
				}
			}
		}

		// grandstream.@provision[0].config_server
		if ((o = config_lookup_option(ctx, grandstream, "@provision[0].config_server"))) {
			debug("Found %s.%s.%s", o->section->package->e.name, o->section->e.name, o->e.name);
			switch (o->type) {
				case UCI_TYPE_STRING:
					printf("value: \'%s\'\n", o->v.string);
					break;
				case UCI_TYPE_LIST: {
					struct uci_element *e;
					uci_foreach_element (&o->v.list, e) {
						printf("value: \'%s\'\n", o->e.name, o->type, e->name);
					}
					break;
				}
			}
		}

		// unload grandstream configuration.
		uci_unload(ctx, grandstream);
	} else {
		if (!load)
			error("Not found grandstream configuration package!");
	}

	return 0;
}

// int prov_dhcp_option_init (prov_t *prov) {

// 	145 tftp

// 	SERVER override_tftp_server

// 	prefix 232 firmware
// 	prefix 234 config


// 	name gwn.
// 	name cfg

// 	suffix 233 firmare
// 	suffix 235 config





// }


void prov_config_exit (prov_t *prov) {

	free(prov->cnf.server[SERV_CONFIG]);
	free(prov->cnf.server[SERV_FIRMWARE]);

}

prov_t prov;




int main(int argc, char *arhv[]) {
    prov_config_init(&prov);




	printf("config server: %s\n", prov.cnf.server[SERV_CONFIG]);
	printf("firmware server: %s\n", prov.cnf.server[SERV_FIRMWARE]);

	prov_config_exit(&prov);

    return 0;
}