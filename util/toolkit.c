#include <stdlib.h>
#include <string.h>
#include "toolkit.h"
#include "stdio.h"
#include "array_list/array_list.h"

struct arraylist* split(char* string, char* delimiter) {
    struct arraylist* split_words = array_list_new(sizeof(char*));

    char* string_copy = strdup(string); // duplicates string dynamically
    char* start = string_copy;
    char* found = NULL;
    while ((found = strsep(&string_copy, delimiter)) != NULL) {
        array_list_add_to_end(split_words, strdup(found)); // NOTE: found uses strdup here
    }
    free(start);
    free(found);

    return split_words;
}

char* validate_file(char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Something went wrong opening file %s\n", file_name);
        exit(-1);
    }
    fclose(file);
    return file_name;
}

char* strip_quotes(char* quoted_string) {
//    char* string_copy = strdup(quoted_string);
    char* string_copy = strdup(quoted_string);
    char* start = string_copy;
    char* found = NULL;
//    char* it_worked;

    printf("Here\n");

    while ((found = strsep(&string_copy, "\"")) != NULL) {
        printf("Loop\n");
        if (strlen(found) > 0) {
//            free(string_copy);
//            printf("FOUND: [%s]\n", found);
//            it_worked = found;
            free(start);
//            free(string_copy);
            printf("I just wan tto return\n");
            return strdup(found);
        }
    }
//    it_worked = strdup(it_worked);
//    free(string_copy);
//    free(found);
//    free(string_copy);
    printf("Nothing\n");
    return quoted_string;
//    return quoted_string;
}

float string_to_float(char* string) {
    float converted = strtof(string, NULL);
    if (converted == 0.0 && strcmp(string, "0.0") != 0) {
        return -1;
    }
    return converted;
}

int string_to_int(char* value) {
    char* value_copy = strdup(value);

    if (strcmp(value, "0") == 0) {
        free(value_copy);
        return 0;
    }

    char* filler;
    long converted = strtol(value_copy, &filler, 10);

    if (converted == 0) {
        free(value_copy);
//        printf("[%s] is not a valid integer, exiting", value);
        return -1;
    }

    free(value_copy);
    return converted;
}