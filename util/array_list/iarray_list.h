#include <stddef.h>

struct iarraylist {
    int* data;
    size_t data_type_size;
    int number_of_items;
    int item_capacity;
};

struct iarraylist* iarray_list_new(size_t data_type_size);
void iarray_list_add_to_end(struct iarraylist* list, int item);
int iarray_list_get_item(struct iarraylist* list, int index);
void iarray_list_cleanup(struct iarraylist* list);