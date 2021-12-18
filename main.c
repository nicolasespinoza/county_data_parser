#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/toolkit.h"
#include "util/array_list/array_list.h"
#include "util/array_list/farray_list.h"
#include "util/array_list/iarray_list.h"
#include "util/array_list/filter_list.h"
#include "county_info.h"
#include "operator_handler.h"

// TODO:
// - 'If an entry in the file is malformed (e.g., there are missing fields or the data within a field cannot be
//      properly converted), then print an error message to stderr indicating the line number of the entry,
//      skip that entry, and continue processing"

struct arraylist* parse_demographics_file(char* file_name) {
    struct arraylist* county_data = array_list_new(sizeof(struct county_info));
    FILE* demographics_file = fopen(file_name, "r");

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    int line_size;
    int line_count = 0;

    getline(&line_buffer, &line_buffer_size, demographics_file); // first line of the file

    line_size = getline(&line_buffer, &line_buffer_size, demographics_file); // twice to ignore first line
    while (line_size >= 0) {
        struct county_info* county = county_create_from_line(line_buffer);
        if (county != NULL) {
//            county_print(county);
            array_list_add_to_end(county_data, county);
        } else {
            printf("County data on line %d could not be processed. Skipping...\n", line_count);
        }
        line_size = getline(&line_buffer, &line_buffer_size, demographics_file); // move to next line
        line_count++;
    }
    printf("%d records loaded\n", line_count);

    free(line_buffer);
    fclose(demographics_file);
    return county_data;
}

struct arraylist* parse_operations_file(char* file_name) {
    struct arraylist* operations = array_list_new(sizeof(char*));
    FILE* operations_file = fopen(file_name, "r");

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    int line_size;
    int line_count = 0;

    line_size = getline(&line_buffer, &line_buffer_size, operations_file); // twice to ignore first line
    while (line_size >= 0) {
        line_buffer[strcspn(line_buffer, "\n")] = 0;
        array_list_add_to_end(operations, strdup(line_buffer));
        line_size = getline(&line_buffer, &line_buffer_size, operations_file); // move to next line
        line_count++;
    }

    free(line_buffer);
    fclose(operations_file);
    return operations;
}

void county_array_list_cleanup(struct arraylist* county_data) {
    if (county_data->real_item_count > 0) {
        for (int i = 0; i < county_data->number_of_items; i++) {
            if (county_data->data[i] != NULL) {
                county_cleanup(county_data->data[i]);
            }
        }
    }
    free(county_data->data);
    free(county_data);
}

int main(int number_of_arguments, char* arguments[]) {
    if (number_of_arguments == 3) {
        char* demographics_file_name = validate_file(arguments[1]);
        char* operations_file_name = validate_file(arguments[2]);

        struct arraylist* county_data = parse_demographics_file(demographics_file_name);
        struct arraylist* operations = parse_operations_file(operations_file_name);

        for (int i = 0; i < operations->number_of_items; i++) {
            char* operation_string = (char*) array_list_get_item(operations, i);
            handle_operation(county_data, operation_string);
        }

//        handle_operation(county_data, "filter:Persons Below Poverty Level:ge:");

        county_array_list_cleanup(county_data);
        array_list_cleanup(operations);
    } else {
        printf("Syntax: ./a.out <demographics file> <operations file>\n");
    }

    return 0;
}

//int main() {
//    struct filterlist* list = filter_list_new(sizeof(char*));
//    char* item1 = "WOW LOL";
//    char* item2 = "AYYYY";
//    char* item3 = "IT WORKS???";
//    filter_list_add(list, "key1", item1);
//    filter_list_add(list, "key2", item2);
//    filter_list_add(list, "key4", item3);
//
//    printf("Number: %d\n", list->number_of_items);
//    printf("Item? %s\n", (char*) filter_list_get_item(list, "key2"));
//}
