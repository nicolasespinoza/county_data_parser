//#define _GNU_SOURCE
//
//#include "filter_list.h"
//#include <stdlib.h>
//#include <string.h>
//
//struct filterlist* filter_list_new(size_t data_type_size) {
//    struct filterlist* list;
//    list = malloc(sizeof(struct filterlist));
//    list->data_type_size = data_type_size;
//    list->item_capacity = 1;
//    list->number_of_items = 0;
//    list->data = malloc(data_type_size * (list->item_capacity));
//
//    return list;
//}
//
//void resize_if_necessary(struct filterlist* list) {
//    int post_add_list_size = list->number_of_items + 1;
//    if (post_add_list_size > list->item_capacity) { // resizing
//        int resize_amount = list->item_capacity * 2;
//        list->item_capacity = resize_amount;
//        list->data = realloc(list->data, resize_amount * list->data_type_size);
//    }
//}
//
//void filter_list_add(struct filterlist* list, char* key, void* item) {
//    resize_if_necessary(list);
//    list->data[list->number_of_items] = item; // NOTE: there is no copying done here
//    list->number_of_items += 1;
//}
//
//void* filter_list_get_item(struct filterlist* list, char* key) {
//    if (index >= list->number_of_items) {
////        printf("Index %d out of bounds for array list size %d\n", index, list->number_of_items);
//        return NULL;
//    }
//    return list->data[index];
//}
//
//void filter_list_remove_item(struct filterlist* list, char* key) {
//    for (int i = 0; i < list->number_of_items; i++) {
//
//    }
//}
//
//void filter_list_cleanup(struct filterlist* list) {
//    for (int i = 0; i < list->number_of_items; i++) {
//        free(list->data[i]);
//    }
//    free(list->data);
//    free(list);
//}