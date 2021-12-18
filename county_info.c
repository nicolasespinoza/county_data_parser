#include <stdlib.h>
#include "county_info.h"
#include "util/array_list/array_list.h"
#include "util/array_list/iarray_list.h"
#include "util/array_list/farray_list.h"
#include "util/toolkit.h"
#include <stdio.h>
#include <string.h>

struct county_info* _county_init() {
    struct county_info* county_info = malloc(sizeof(struct county_info));
    county_info->general = array_list_new(sizeof(char*));
    county_info->educations = farray_list_new(sizeof(float));
    county_info->ethnicities = farray_list_new(sizeof(float));
    county_info->incomes = iarray_list_new(sizeof(int));
    county_info->income_people_below_poverty = 0;
    county_info->population_2014 = 0;

    county_info->number_of_general_data = 2;
    county_info->number_of_educations = 2;
    county_info->number_of_ethnicities = 8;
    county_info->number_of_incomes = 2;

    county_info->general_indexes = iarray_list_new(sizeof(int));
    for (int i = 0; i <= 1; i++) {
        iarray_list_add_to_end(county_info->general_indexes, i);
    }

    county_info->education_indexes = iarray_list_new(sizeof(int));
    for (int i = 5; i <= 6; i++) {
        iarray_list_add_to_end(county_info->education_indexes, i);
    }

    county_info->ethnicities_indexes = iarray_list_new(sizeof(int));
    for (int i = 11; i <= 19; i++) {
        iarray_list_add_to_end(county_info->ethnicities_indexes, i);
    }

    county_info->income_indexes = iarray_list_new(sizeof(int));
    for (int i = 25; i <= 26; i++) {
        iarray_list_add_to_end(county_info->income_indexes, i);
    }

    return county_info;
}

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

struct county_info* county_create_from_line(char* line) {
    struct arraylist* split_data = split(line, ",");
    struct county_info* county = _county_init();

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

void county_print(struct county_info* county_info) {
    printf("\n%s, %s\n", (char*) array_list_get_item(county_info->general, 0), (char*) array_list_get_item(county_info->general, 1));
    printf("\tPopulation: %d\n", county_info->population_2014);
    printf("\tEducation\n");
    printf("\t\t>= High School: %f\n", farray_list_get_item(county_info->educations, 1));
    printf("\t\t>= Bachelor's: %f\n", farray_list_get_item(county_info->educations, 0));
    printf("\tEthnicities\n");
    printf("\t\tAmerican Indian and Alaska Native Alone: %f\n", farray_list_get_item(county_info->ethnicities, 0));
    printf("\t\tAsian Alone: %f\n", farray_list_get_item(county_info->ethnicities, 1));
    printf("\t\tBlack Alone: %f\n", farray_list_get_item(county_info->ethnicities, 2));
    printf("\t\tHispanic or Latino: %f\n", farray_list_get_item(county_info->ethnicities, 3));
    printf("\t\tNative Hawaiian and Other Pacific Islander Alone: %f\n", farray_list_get_item(county_info->ethnicities, 4));
    printf("\t\tTwo or More Races: %f\n", farray_list_get_item(county_info->ethnicities, 5));
    printf("\t\tWhite Alone: %f\n", farray_list_get_item(county_info->ethnicities, 6));
    printf("\t\tWhite Alone, not Hispanic or Latino: %f\n", farray_list_get_item(county_info->ethnicities, 7));
    printf("\tIncome\n");
    printf("\t\tMedian Household: %d\n", iarray_list_get_item(county_info->incomes, 0));
    printf("\t\tPer Capita: %d\n", iarray_list_get_item(county_info->incomes, 1));
    printf("\t\tBelow Poverty Level: %f\n", county_info->income_people_below_poverty);
}

void county_cleanup(struct county_info* county_info) {
    array_list_cleanup(county_info->general);
    farray_list_cleanup(county_info->educations);
    farray_list_cleanup(county_info->ethnicities);
    iarray_list_cleanup(county_info->incomes);

    iarray_list_cleanup(county_info->general_indexes);
    iarray_list_cleanup(county_info->education_indexes);
    iarray_list_cleanup(county_info->ethnicities_indexes);
    iarray_list_cleanup(county_info->income_indexes);

    free(county_info);
}
