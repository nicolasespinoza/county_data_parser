#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/toolkit.h"
#include "util/array_list/array_list.h"
#include "county_info.h"
#include "util/array_list/farray_list.h"
#include "util/array_list/iarray_list.h"

// TODO:
// - 'If an entry in the file is malformed (e.g., there are missing fields or the data within a field cannot be
//      properly converted), then print an error message to stderr indicating the line number of the entry,
//      skip that entry, and continue processing"

char* safe_data_fetch_string(struct arraylist* split_data, int index) {
    if (index >= split_data->number_of_items) {
        return NULL;
    }
    return strip_quotes(array_list_get_item(split_data, index));
}

int safe_data_fetch_int(struct arraylist* split_data, int index) {
    if (index >= split_data->number_of_items) {
        return -1;
    }
    char* data_no_quotes = strip_quotes(array_list_get_item(split_data, index));
    int converted = string_to_int(data_no_quotes);
    if (converted == -1) {
        free(data_no_quotes);
        return -1;
    }
    free(data_no_quotes);
    return converted;
}

float safe_data_fetch_float(struct arraylist* split_data, int index) {
    if (index >= split_data->number_of_items) {
        // print something here with the line #. potentially send out to external function
        return -1.0f;
    }
    char* data_no_quotes = strip_quotes(array_list_get_item(split_data, index));
    float converted = string_to_float(data_no_quotes);
    if (converted == -1) {
        free(data_no_quotes);
        // print something here with the line #. potentially send out to external function
        return -1.0f;
    }
    free(data_no_quotes);
    return converted;
}

struct county_info* line_to_county_info(char* line) {
    struct arraylist* split_data = split(line, ",");
    struct county_info* county = county_info_create();

    // General (County Name, State)
    for (int i = 0; i < county->number_of_general_data; i++) {
        int general_index = iarray_list_get_item(county->general_indexes, i);
        char* converted = safe_data_fetch_string(split_data, general_index);
        if (converted != NULL) {
            array_list_add_to_end(county->general, converted);
        } else {
            return NULL;
        }
    }

    // Education
    for (int i = 0; i < county->number_of_educations; i++) {
        int education_index = iarray_list_get_item(county->education_indexes, i);
        float converted = safe_data_fetch_float(split_data, education_index);
        if (converted != -1.0f) {
            farray_list_add_to_end(county->educations, converted);
        } else {
            return NULL;
        }
    }

    // Ethnicities
    for (int i = 0; i < county->number_of_ethnicities; i++) {
        int ethnicity_index = iarray_list_get_item(county->ethnicities_indexes, i);
        float converted = safe_data_fetch_float(split_data, ethnicity_index);
        if (converted != -1.0f) {
            farray_list_add_to_end(county->ethnicities, converted);
        } else {
            return NULL;
        }
    }

    // Incomes
    for (int i = 0; i < county->number_of_incomes; i++) {
        int income_index = iarray_list_get_item(county->income_indexes, i);
        int converted = safe_data_fetch_int(split_data, income_index);
        if (converted != -1) {
            iarray_list_add_to_end(county->incomes, converted);
        } else {
            return NULL;
        }
    }

    // Income (persons below poverty)
    float income_people_below_poverty = safe_data_fetch_float(split_data, 28);
    if (income_people_below_poverty != -1.0f) {
        county->income_people_below_poverty = income_people_below_poverty;
    } else {
        return NULL;
    }

    // 2014 Population
    int population_2014 = safe_data_fetch_int(split_data, 39);
    if (population_2014 != -1) {
        county->population_2014 = population_2014;
    } else {
        return NULL;
    }

    array_list_cleanup(split_data);
    return county;
}

struct arraylist* parse_demographics_file(char* file_name) {
    struct arraylist* county_data = array_list_new(sizeof(struct county_info));
    FILE* demographics_file = fopen(file_name, "r");

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    int line_size;
//    size_t line_size;
    int line_count = 0;

    getline(&line_buffer, &line_buffer_size, demographics_file); // first line of the file
    line_size = getline(&line_buffer, &line_buffer_size, demographics_file); // twice to ignore first line
    while (line_size >= 0) {
        struct county_info* county = line_to_county_info(line_buffer);
        if (county != NULL) {
            county_print(county); // for debug purposes
            array_list_add_to_end(county_data, county);
        } else {
            printf("County data on line %d could not be processing. Skipping...\n", line_count);
        }
        line_size = getline(&line_buffer, &line_buffer_size, demographics_file); // move to next line
        line_count++;
    }
    printf("%d entries loaded from demographics file\n", line_count);

    free(line_buffer);
    fclose(demographics_file);
    return county_data;
}

void county_array_list_cleanup(struct arraylist* county_data) {
    for (int i = 0; i < county_data->number_of_items; i++) {
        county_cleanup(county_data->data[i]);
        free(county_data->data[i]);
    }
    free(county_data->data);
    free(county_data);
}

char* experiment(char* quoted_string) {
    char* copy = strdup(quoted_string);
    char* token = strtok(copy, "\"");
    while (token != NULL) {
        printf("Test: %s\n", token);
        if (strlen(token) > 0) {
//            free(copy);
            return token;
        }
        token = strtok(NULL, "\"");
    }
//    free(copy);
    return quoted_string;
}

int main(int number_of_arguments, char* arguments[]) {
//    if (number_of_arguments == 3) {
//        char* demographics_file_name = validate_file(arguments[1]);
//        char* operations_file_name = validate_file(arguments[2]);
//
//        struct arraylist* county_data = parse_demographics_file(demographics_file_name);
//        county_array_list_cleanup(county_data);
//    } else {
//        printf("Syntax: ./a.out <demographics file> <operations file>\n");
//    }

    char* quoted_string = "\"Test\"";
//    printf("My string before anything: %s\n", quoted_string);
    char* fixed = experiment(quoted_string);
    printf("Fixed: %s\n", fixed);
//    printf("Afterwards %s\n", quoted_string);

//    char* original = "\"Test\"";
//    printf("BEFORE ANYTHING: %s\n", original);
//    char* stripped = strip_quotes(original);
//    printf("Without quotes: %s\n", stripped);
//    free(stripped);

    return 0;
}

