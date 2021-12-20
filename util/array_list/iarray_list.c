#define _GNU_SOURCE

#include "iarray_list.h"
#include <stdlib.h>

struct iarraylist* iarray_list_new(size_t data_type_size) {
    struct iarraylist* list = malloc(sizeof(struct iarraylist));
    list->data_type_size = data_type_size;
    list->item_capacity = 1;
    list->number_of_items = 0;
    list->data = malloc(data_type_size * (list->item_capacity));

    return list;
}

void iresize_if_necessary(struct iarraylist* list) {
    int post_add_list_size = list->number_of_items + 1;
    if (post_add_list_size > list->item_capacity) { // resizing
        int resize_amount = list->item_capacity * 2;
        list->item_capacity = resize_amount;
        list->data = realloc(list->data, resize_amount * list->data_type_size);
    }
}

void iarray_list_add_to_end(struct iarraylist* list, int item) {
    iresize_if_necessary(list);
    list->data[list->number_of_items] = item;
    list->number_of_items += 1;
}

int iarray_list_get_item(struct iarraylist* list, int index) {
    if (index >= list->number_of_items) {
//        printf("Index %d out of bounds for array list size %d\n", index, list->number_of_items);
        return -1;
    }
    return list->data[index];
}

void iarray_list_cleanup(struct iarraylist* list) {
//    for (int i = 0; i < list->number_of_items; i++) {
//        free(list->data-files[i]);
//    }
    free(list->data);
    free(list);
}