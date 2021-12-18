#include <stddef.h>

struct filterlist {
//    struct** entry entries;
    void** data;
    size_t data_type_size;
    int number_of_items;
    int item_capacity;
};

struct entry {
    char* key;
    void* data;
};

struct filterlist* filter_list_new(size_t data_type_size);
void filter_list_add(struct filterlist* list, char* key, void* item);
void* filter_list_get_item(struct filterlist* list, char* key);
void filter_list_remove_item(struct filterlist* list, char* key);
void filter_list_cleanup(struct filterlist* list);