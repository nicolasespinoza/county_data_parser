#include <string.h>
#include "util/array_list/array_list.h"
#include "util/toolkit.h"
#include "county_info.h"
#include "util/array_list/farray_list.h"
#include "util/array_list/iarray_list.h"

char* get_state(struct county_info* county) {
    struct arraylist* general = county->general;
    return array_list_get_item(general, 1);
}

int get_total_population(struct arraylist* county_data) {
    int total_population = 0;
    for (int i = 0; i < county_data->number_of_items; i++) {
        struct county_info* county = array_list_get_item(county_data, i);
        if (county == NULL) {
            continue;
        }
        total_population += county->population_2014;
    }
    return total_population;
}

int identifier_to_index(char* identifier) {
    if (strcmp(identifier, "Education.High School or Higher") == 0) { return 1;
    } else if (strcmp(identifier, "Education.Bachelor's Degree or Higher") == 0) { return 0;
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
    // TODO: currently ONLY supports eth, income, and education
    if (strcmp(field_name, "Population") == 0 || strcmp(field_name, "County") == 0 || strcmp(field_name, "State") == 0) {
        return -404.0;
    }

    struct arraylist* split_field_name = split(field_name, ".");
    double data = get_data(county, field_name, split_field_name);
    array_list_cleanup(split_field_name);
    return data;
}

double get_population_of_field_x(struct arraylist* county_data, char* field_name) {
    // ex: field_name=Education.Bachelor's Degree or Higher
    // returns the number of people with Bachelor's degree or higher from each county summed up
    double affected_population = 0;

    for (int i = 0; i < county_data->number_of_items; i++) {
        struct county_info* county = array_list_get_item(county_data, i);
        if (county == NULL) {
            continue;
        }
        double data = get_county_field_data(county, field_name);
        printf("Getting [%f] data from county [%s] for field [%s]\n", data, get_state(county), field_name);
        affected_population += data;
    }

    return affected_population;
}

double get_affected_population(struct arraylist* county_data, char* field_name) {
    double affected_population = 0;

    for (int i = 0; i < county_data->number_of_items; i++) {
        struct county_info *county = array_list_get_item(county_data, i);
        if (county == NULL) {
            continue;
        }

        double county_population = county->population_2014;
        double affected_percentage = get_county_field_data(county, field_name) / 100.0;
        affected_population += (county_population * affected_percentage);
//        printf("[%f]PERC of people of %f: %f\n", affected_percentage, county_population, affected_population);
    }
    return affected_population;
}

void handle_operation(struct arraylist* county_data, char* operation) {
    struct arraylist* arguments = split(operation, ":");
    if (arguments->number_of_items == 0) {
        return;
    }

    char* arg0 = array_list_get_item(arguments, 0);
    if (arguments->number_of_items == 1) {
        if (strcmp(operation, "population-total") == 0) {
            printf("2014 population: %d\n", get_total_population(county_data));
        } else if (strcmp(operation, "display") == 0) {
            for (int i = 0; i < county_data->number_of_items; i++) {
                struct county_info* county = array_list_get_item(county_data, i);
                if (county == NULL) {
                    continue;
                }
                county_print(county);
            }
        }
    } else if (arguments->number_of_items == 2) {

        if (strcmp(arg0, "filter-state") == 0) {
            char* filter_state_abbreviation = array_list_get_item(arguments, 1);
            int matching_states = 0;

            for (int i = 0; i < county_data->number_of_items; i++) {
                struct county_info* county = array_list_get_item(county_data, i);
                if (county == NULL) {
                    continue;
                }
                char* county_state_abbreviation = get_state(county);

                if (strcmp(filter_state_abbreviation, county_state_abbreviation) != 0) {
                    county_cleanup(county);
                    array_list_nullify_index(county_data, i);
                } else {
                    matching_states++;
                }
            }

            printf("Filter: state == %s (%d entries)\n", filter_state_abbreviation, county_data->real_item_count);
        } else if (strcmp(arg0, "population") == 0) {
            char* field_name = array_list_get_item(arguments, 1);
            printf("2014 %s population: %f\n", field_name, get_affected_population(county_data, field_name));
        } else if (strcmp(arg0, "percent") == 0) {
            char* field_name = array_list_get_item(arguments, 1);

            double affected_population = get_affected_population(county_data, field_name);
            double total_population = get_total_population(county_data);

            double affected_percentage = (affected_population / total_population) * 100.0;
            printf("2014 %s percentage: %f\n", field_name, affected_percentage);
        }
    } else if (arguments->number_of_items == 4) {

        if (strcmp(arg0, "filter") == 0) {
            char *field_name = array_list_get_item(arguments, 1);
            char *comparison_type = array_list_get_item(arguments, 2);
            int comparison_number = string_to_int(array_list_get_item(arguments, 3));

            for (int i = 0; i < county_data->number_of_items; i++) {
                struct county_info* county = array_list_get_item(county_data, i);
                if (county == NULL) {
                    continue;
                }
                double data = get_county_field_data(county, field_name);
                if (data == -404.0) {
                    fprintf(stderr, "Field %s not permitted on filter operation\n", field_name);
                    return;
                }

                if (strcmp(comparison_type, "ge") == 0) {
                    if (data < comparison_number) {
                        county_cleanup(county);
                        array_list_nullify_index(county_data, i);
                    }
                } else if (strcmp(comparison_type, "le") == 0) {
                    if (data > comparison_number) {
                        county_cleanup(county);
                        array_list_nullify_index(county_data, i);
                    }
                }
            }

            printf("Filter: %s %s %d (%d entries)\n", field_name, comparison_type, comparison_number,
                   county_data->real_item_count);

        }

    }

    array_list_cleanup(arguments);
}
