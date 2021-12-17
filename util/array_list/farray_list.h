#include <stddef.h>

struct farraylist {
    float* data;
    size_t data_type_size;
    int number_of_items;
    int item_capacity;
};

struct farraylist* farray_list_new(size_t data_type_size);
void farray_list_add_to_end(struct farraylist* list, float item);
float farray_list_get_item(struct farraylist* list, int index);
void farray_list_cleanup(struct farraylist* list);