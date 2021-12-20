#define _GNU_SOURCE

#include "farray_list.h"
#include <stdlib.h>

struct farraylist* farray_list_new(size_t data_type_size) {
    struct farraylist* list = malloc(sizeof(struct farraylist));
    list->data_type_size = data_type_size;
    list->item_capacity = 1;
    list->number_of_items = 0;
    list->data = malloc(data_type_size * (list->item_capacity));

    return list;
}

void fresize_if_necessary(struct farraylist* list) {
    int post_add_list_size = list->number_of_items + 1;
    if (post_add_list_size > list->item_capacity) { // resizing
        int resize_amount = list->item_capacity * 2;
        list->item_capacity = resize_amount;
        list->data = realloc(list->data, resize_amount * list->data_type_size);
    }
}

void farray_list_add_to_end(struct farraylist* list, float item) {
    fresize_if_necessary(list);
    list->data[list->number_of_items] = item;
    list->number_of_items += 1;
}

float farray_list_get_item(struct farraylist* list, int index) {
    if (index >= list->number_of_items) {
//        printf("Index %d out of bounds for array list size %d\n", index, list->number_of_items);
        return -1.0f;
    }
    return list->data[index];
}

void farray_list_cleanup(struct farraylist* list) {
//    for (int i = 0; i < list->number_of_items; i++) {
//        free(list->data-files[i]);
//    }
    free(list->data);
    free(list);
}