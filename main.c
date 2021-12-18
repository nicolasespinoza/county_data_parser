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
        return -404404404;
    }
    char* data_no_quotes = strip_quotes(array_list_get_item(split_data, index));
    int converted = string_to_int(data_no_quotes);
    if (converted == -1) {
        return -404404404;
    }
    return converted;
}

float safe_data_fetch_float(struct arraylist* split_data, int index) {
    if (index >= split_data->number_of_items) {
        return -1.0f;
    }
    char* data_no_quotes = strip_quotes(array_list_get_item(split_data, index));
    float converted = string_to_float(data_no_quotes);
    if (converted == -1) {
        for (int i = 0; i < 10; i++) {
            printf("Here, value: %s\n", data_no_quotes);
        }
        return -1.0f;
    }
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
            array_list_add_to_end(county->general, strdup(converted));
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
        if (converted != -404404404) {
            iarray_list_add_to_end(county->incomes, converted);
        } else {
            return NULL;
        }
    }

    // Income (persons below poverty)
    float income_people_below_poverty = safe_data_fetch_float(split_data, 27);
    if (income_people_below_poverty != -1.0f) {
        county->income_people_below_poverty = income_people_below_poverty;
    } else {
        return NULL;
    }

    // 2014 Population
    int population_2014 = safe_data_fetch_int(split_data, 38);
    if (population_2014 != -404404404) {
        county->population_2014 = population_2014;
    } else {
        return NULL;
    }

    array_list_cleanup(split_data);
    return county;
}

char* get_state(struct county_info* county) {
    struct arraylist* general = county->general;
    return array_list_get_item(general, 1);
}

int get_total_population(struct arraylist* county_data) {
    int total_population = 0;
    for (int i = 0; i < county_data->number_of_items; i++) {
        struct county_info* county = array_list_get_item(county_data, i);
        total_population += county->population_2014;
    }
    return total_population;
}

int identifier_to_index(char* identifier) {
    if (strcmp(identifier, "Education.High School or Higher") == 0) { return 0;
    } else if (strcmp(identifier, "Education.Bachelor's Degree or Higher") == 0) { return 1;
    } else if (strcmp(identifier, "Ethnicities.American Indian and Alaska Native Alone") == 0) { return 0;
    } else if (strcmp(identifier, "Ethnicities.Asian Alone") == 0) { return 1;
    } else if (strcmp(identifier, "Ethnicities.Black Alone") == 0) { return 2;
    } else if (strcmp(identifier, "Ethnicities.Hispanic or Latino") == 0) { return 3;
    } else if (strcmp(identifier, "Ethnicities.Native Hawaiian and Other Pacific Islander Alone") == 0) { return 4;
    } else if (strcmp(identifier, "Ethnicities.Two or More Races") == 0) { return 5;
    } else if (strcmp(identifier, "Ethnicities.White Alone") == 0) { return 6;
    } else if (strcmp(identifier, "Ethnicities.White Alone, not Hispanic or Latino") == 0) { return 7;
    } else if (strcmp(identifier, "Income.Median Houseold Income") == 0) { return 0;
    } else if (strcmp(identifier, "Income.Per Capita Income") == 0) { return 1;
    } else if (strcmp(identifier, "Income.Persons Below Poverty Level") == 0) { return -1;
    } else {
        printf("Could not match index with identifier %s\n", identifier);
        return 0; // returns 0
    }
}

double get_data(struct county_info* county, char* field_name, struct arraylist* split_field_name) {
    int index = identifier_to_index(field_name);
    char* category_name = array_list_get_item(split_field_name, 0);
    char* specific_identifier = array_list_get_item(split_field_name, 1);

    if (strcmp(category_name, "Ethnicities") == 0) {
        return farray_list_get_item(county->ethnicities, index);
    } else if (strcmp(category_name, "Income") == 0) {
        if (strcmp(specific_identifier, "Persons Below Poverty Level") == 0) {
            return county->income_people_below_poverty;
        } else {
            return iarray_list_get_item(county->incomes, index);
        }
    } else if (strcmp(category_name, "Education") == 0) {
        return farray_list_get_item(county->educations, index);
    } else {
        printf("Problem\n");
        return -1.0;
    }
}

double get_county_field_data(struct county_info* county, char* field_name) {
    // currently ONLY supports eth, income, and education
    struct arraylist* split_field_name = split(field_name, ".");
    double data = get_data(county, field_name, split_field_name);

//    if (split_field_name->number_of_items == 1) {
//        printf("IDk\n");
//    } else if (split_field_name->number_of_items == 2) {
//
//
//        if (strcmp(category_name, "Income") == 0 && strcmp(specific_identifier, "Persons Below Poverty Level") == 0) {
//            return county->income_people_below_poverty;
//        } else {
//            return get_data(county, category_name, specific_identifier);
//        }
//    }

    array_list_cleanup(split_field_name);
    return data;
}

void handle_operation(struct arraylist* county_data, char* operation) {

    struct arraylist* arguments = split(operation, ":");
    if (arguments->number_of_items == 1) {
        if (strcmp(operation, "population-total") == 0) {
            printf("2014 population: %d\n", get_total_population(county_data));
        } else if (strcmp(operation, "display") == 0) {
            for (int i = 0; i < county_data->number_of_items; i++) {
                struct county_info* county = array_list_get_item(county_data, i);
                county_print(county);
            }
        }
    } else if (arguments->number_of_items == 2) {
        char* arg0 = array_list_get_item(arguments, 0);
        if (strcmp(arg0, "filter") == 0) {
            printf("filter\n");
        } else if (strcmp(arg0, "filter-state") == 0) {
            char* filter_state_abbreviation = array_list_get_item(arguments, 1);
            int matching_states = 0;

            for (int i = 0; i < county_data->number_of_items; i++) {
                struct county_info* county = array_list_get_item(county_data, i);
                char* county_state_abbreviation = get_state(county);

                if (strcmp(filter_state_abbreviation, county_state_abbreviation) == 0) {
                    matching_states++;
                }
            }

            if (matching_states == 0) {
                // clear the collection
            }

            // ensure print statement is in correct place?
            printf("Filter: state == %s (%d entries)\n", filter_state_abbreviation, matching_states);
        } else if (strcmp(arg0, "population") == 0) {
            printf("popuatlion\n");
        } else if (strcmp(arg0, "percent") == 0) {
            char* field_name = array_list_get_item(arguments, 1);
            double affected_population = 0;
            int total_population = get_total_population(county_data);
            double affected_percentage;

            for (int i = 0; i < county_data->number_of_items; i++) {
                struct county_info* county = array_list_get_item(county_data, i);
                double data = get_county_field_data(county, field_name);
                if (data > 0.0) {
                    affected_population += data;
                }
            }
            affected_percentage = (affected_population / total_population) * 100.0;

            printf("2014 %s percentage: %f\n", field_name, affected_percentage);
        }
    }

    array_list_cleanup(arguments);

}

struct arraylist* parse_demographics_file(char* file_name) {
    struct arraylist* county_data = array_list_new(sizeof(struct county_info));
    FILE* demographics_file = fopen(file_name, "r");

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    int line_size;
    //ssize_t line_size;
    //size_t line_size;
    int line_count = 0;

    getline(&line_buffer, &line_buffer_size, demographics_file); // first line of the file
    line_size = getline(&line_buffer, &line_buffer_size, demographics_file); // twice to ignore first line
    while (line_size >= 0) {
        struct county_info* county = line_to_county_info(line_buffer);
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
    //ssize_t line_size;
    //size_t line_size;
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
    for (int i = 0; i < county_data->number_of_items; i++) {
        county_cleanup(county_data->data[i]);
//        free(county_data->data[i]); // THIS IS NOT NEEDED
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

        county_array_list_cleanup(county_data);
        array_list_cleanup(operations);
    } else {
        printf("Syntax: ./a.out <demographics file> <operations file>\n");
    }

    return 0;
}

