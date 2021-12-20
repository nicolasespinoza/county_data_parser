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

//char* strip_quotes(char* quoted_string) {
//    char* quoted_string_copy = strdup(quoted_string);
//    char* result = strtok(quoted_string_copy, "\"");
//    free(quoted_string_copy);
//    return result;
//}

char* strip_quotes(char* quoted_string) {
    struct arraylist* split_data = split(quoted_string, "\"");
    char* unquoted_string = strdup(array_list_get_item(split_data, 1));
    array_list_cleanup(split_data);
    return unquoted_string;
}

float string_to_float(char* string) {
    if (strcmp(string, "0.0") == 0 || strcmp(string, "0") == 0) {
        return 0.0f;
    }
    char* filler = "";
    float converted = strtof(string, &filler);
    return converted == 0.0 ? -1 : converted;
}

int string_to_int(char* value) {
    char* value_copy = strdup(value);

    if (strlen(value) == 0 || strcmp(value, "0") == 0 || strcmp(value, "0.0") == 0) {
        free(value_copy);
        return 0;
    }

    char* filler;
    long converted = strtol(value_copy, &filler, 10);

    if (converted == 0) {
        printf("[%s] is not a valid integer, ignoring\n", value_copy);
        free(value_copy);
        return -1;
    }

    free(value_copy);
    return converted;
}