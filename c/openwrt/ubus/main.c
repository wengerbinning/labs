
#include <stdio.h>

#include <libubus.h>

static struct ubus_context *ctx;

//
static struct blobmsg_policy ubus_main_command_policy [] = {};

static int ubus_main_command_callback (struct ubus_context *ctx, struct ubus_object *obj,
	struct ubus_request_data *req, const char *method, struct blob_attr *msg);

static struct ubus_method ubus_main_methods [] = {
	UBUS_METHOD("command", ubus_main_command_callback, ubus_main_command_policy),
};

static struct ubus_object_type ubus_main_type = UBUS_OBJECT_TYPE("devel", ubus_main_methods);

static struct ubus_object ubus_main_object = {
    .name = "devel",
    .type = &ubus_main_type,
    .methods = ubus_main_methods,
    .n_methods = ARRAY_SIZE(ubus_main_methods),
};

//
int main (void) {
	struct ubus_auto_conn ubus;

	printf("Running in here ...\n");

	ctx = ubus_connect(NULL);
	// ubus_auto_connect(&ubus);
	if (!ctx)
		printf("Failed to connect ubus !\n");

	printf("connect ubus.\n");

	if (ubus_add_object(ctx, &ubus_main_object))
		printf("Failed to add ubus object!\n");


	printf("Press any key to quit!\n");

	while (getchar())
		break;

	ubus_free(ctx);

	return 0;
}

static int ubus_main_command_callback (struct ubus_context *ctx, struct ubus_object *obj,
	struct ubus_request_data *req, const char *method, struct blob_attr *msg)
{
	return 0;
}