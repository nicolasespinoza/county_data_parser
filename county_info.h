struct county_info {
    // General
    // 0 = "County"
    // 1 = "State"
    struct arraylist* general;

    // Education
    // NOTE: switched based on csv order
    // 1 = "Education.Bachelor's Degree or Higher"
    // 0 = "Education.High School or Higher"
    struct farraylist* educations;

    // Ethnicities
    // 0 = "Ethnicities.American Indian and Alaska Native Alone"
    // 1 = "Ethnicities.Asian Alone"
    // 2 = "Ethnicities.Black Alone"
    // 3 = "Ethnicities.Hispanic or Latino"
    // 4 = "Ethnicities.Native Hawaiian and Other Pacific Islander Alone"
    // 5 = "Ethnicities.Two or More Races"
    // 6 = "Ethnicities.White Alone"
    // 7 = "Ethnicities.White Alone, not Hispanic or Latino"
    struct farraylist* ethnicities;

    // Income
    // 0 = "Income.Median Houseold Income"
    // 1 = "Income.Per Capita Income"
    struct iarraylist* incomes;

    // "Income.Persons Below Poverty Level"
    float income_people_below_poverty;

    // "Population.2014 Population"
    int population_2014;

    // Other
    int number_of_educations;
    struct iarraylist* education_indexes;
    int number_of_ethnicities;
    struct iarraylist* ethnicities_indexes;
    int number_of_general_data;
    struct iarraylist* general_indexes;
    int number_of_incomes;
    struct iarraylist* income_indexes;
};

struct county_info* _county_init();
struct county_info* county_create_from_line(char* line, int line_count);
void county_print(struct county_info* county_info);
void county_cleanup(struct county_info* county_info);
