#include <stdlib.h>
#include "county_info.h"
#include "util/array_list/array_list.h"
#include "util/array_list/iarray_list.h"
#include "util/array_list/farray_list.h"
#include <stdio.h>

struct county_info* county_info_create() {
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

// transfer helper function to here where you can just create a county_info pointer from a line. properly dispose of
// memory stuffsz

void county_print(struct county_info* county_info) {
    printf("\n%s, %s\n", (char*) array_list_get_item(county_info->general, 0), (char*) array_list_get_item(county_info->general, 1));
    printf("\tPopulation: %d\n", county_info->population_2014);
    printf("\tEducation\n");
    printf("\t\t>= High School: %f\n", farray_list_get_item(county_info->educations, 0));
    printf("\t\t>= Bachelor's: %f\n", farray_list_get_item(county_info->educations, 1));
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