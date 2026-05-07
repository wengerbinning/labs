#include <time.h>
#include <stdio.h>

#include "database.h"

int main (int agrc, char *argv[]) {
	db_context_t *ctx;

	db_config_t root_conf = {
		.host = "localhost",
		.port = 3306,
		.username = "root",
		.password = "root",
	};

	if (!(ctx = db_init(&root_conf))) {
		fprintf(stderr, "mysql_init() failed\n");
		return -1;
	}

	printf("已连接数据库 %s@%s:%d 服务 ...\n", ctx->username, ctx->host, ctx->port);

	db_create_database(ctx, "test");

	sleep(5);

	db_delete_database(ctx, "admin");
	ctx = db_exit(ctx);
	return 0;
}
