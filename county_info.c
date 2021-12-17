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
    for (int i = 26; i <= 27; i++) {
        iarray_list_add_to_end(county_info->income_indexes, i);
    }

    return county_info;
}

// transfer helper function to here where you can just create a county_info pointer from a line. properly dispose of
// memory stuffsz

void county_print(struct county_info* county_info) {
    printf("\nCounty: %s, %s\n", (char*) array_list_get_item(county_info->general, 0), (char*) array_list_get_item(county_info->general, 1));
    printf("Education.Bachelor's Degree or Higher: %f\n", farray_list_get_item(county_info->educations, 0));
    printf("Education.High School or Higher: %f\n", farray_list_get_item(county_info->educations, 1));
    printf("Ethnicities.American Indian and Alaska Native Alone: %f\n", farray_list_get_item(county_info->ethnicities, 0));
    printf("Ethnicities.Asian Alone: %f\n", farray_list_get_item(county_info->ethnicities, 1));
    printf("Ethnicities.Black Alone: %f\n", farray_list_get_item(county_info->ethnicities, 2));
    printf("Ethnicities.Hispanic or Latino: %f\n", farray_list_get_item(county_info->ethnicities, 3));
    printf("Ethnicities.Native Hawaiian and Other Pacific Islander Alone: %f\n", farray_list_get_item(county_info->ethnicities, 4));
    printf("Ethnicities.Two or More Races: %f\n", farray_list_get_item(county_info->ethnicities, 5));
    printf("Ethnicities.White Alone: %f\n", farray_list_get_item(county_info->ethnicities, 6));
    printf("Ethnicities.White Alone, not Hispanic or Latino: %f\n", farray_list_get_item(county_info->ethnicities, 7));
    printf("Income.Median Houseold Income: %d\n", iarray_list_get_item(county_info->incomes, 0));
    printf("Income.Per Capita Income: %d\n", iarray_list_get_item(county_info->incomes, 1));
    printf("Income.Persons Below Poverty Level: %f\n", county_info->income_people_below_poverty);
    printf("2014 Population: %d\n", county_info->population_2014);
    printf("\n");
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
}