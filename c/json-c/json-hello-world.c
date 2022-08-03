#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

char *str = "{ \
    \"name\":\"Wenger Binning\",  \
    \"version\":\"v0.0.0.1\"            \
}";

int main(int argc, char *argv[]) {
    struct json_tokener *tok;
    struct json_object  *obj, *cur;

    tok = json_tokener_new();
    if (!tok) {
        return -1;
    }

    obj = json_tokener_parse_ex(tok, str, strlen(str));
    if (!obj) {
        printf("Parser errir\n");
    }

    json_object_object_get_ex(obj, "name", &cur);
    printf("Name value: %s\n", json_object_get_string(cur));

    json_object_object_get_ex(obj, "version", &cur);
    printf("Version value: %s\n", json_object_get_string(cur));

    return 0;
}
