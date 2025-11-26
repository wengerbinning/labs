#ifndef __DATABASE_H__
#define __DATABASE_H__

#define SQL_CMD_MAXSIZE 1024

enum db_type {
	Mariadb,
} db_type_t;

typedef struct db_config {
	uint32_t port;
	char *hostname;
	char *username;
	char *password;
	char *database;
	db_type_t type;
} db_config_t;

typedef struct db_context {
	void *db;
	uint32_t port;
	char *hostname;
	char *username;
	char *password;
	char *database;
	db_type_t type;
} db_context_t;


db_context_t * db_init (db_config_t *conf);
db_context_t * db_exit (db_context_t *ctx);

#endif /* __DATABASE_H__ */
