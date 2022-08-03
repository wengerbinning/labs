
#include <stdio.h>
#include <libubox/kvlist.h>

static struct kvlist kv_list;


typedef struct user_data {
    int id;
    char name[10];
} user_data;

int get_len(struct kvlist *kv, const void *data)
{
    return sizeof(struct user_data);
}

struct user_data data1={1,"data1"};
struct user_data data2={2,"data2"};
struct user_data data3={3,"data3"};


int main (int argc, char *argv[]) {
    struct user_data *p;
    const char *p_name;
    struct user_data *p_value;

    kvlist_init(&kv_list,get_len);

    kvlist_set(&kv_list,data1.name,&data1);
    kvlist_set(&kv_list,data2.name,&data2);
    kvlist_set(&kv_list,data3.name,&data3);

    p = (struct user_data *)kvlist_get(&kv_list,data1.name);

    p ? printf("user id %d\n", p->id): printf("null\n");


    kvlist_for_each(&kv_list,p_name,p_value)
    {
        printf("===================\n");
        printf("name=%s\n",p_name);
        printf("id=%d\n",p_value->id);
        printf("===================\n");
    }

    kvlist_delete(&kv_list,data1.name);

    kvlist_free(&kv_list);

    kvlist_for_each(&kv_list,p_name,p_value)
    {
        printf("===================\n");
        printf("name=%s\n",p_name);
        printf("id=%d\n",p_value->id);
        printf("===================\n");
    }

    return 0;
}