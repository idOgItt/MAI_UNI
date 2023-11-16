#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Status codes for students_search
enum student_search_status_code {student_search_ok, student_search_fail};

enum reading_students_status_code {reading_students_ok, reading_students_fail};

enum students_sort_status_code {students_sort_ok, students_sort_fail};

enum stdout_students_search_status_code {stdout_students_search_ok, stdout_students_search_fail};

enum stdout_students_higher_scores_status_code {stdout_students_higher_scores_ok, stdout_students_higher_scores_fail};

// Structure for incoming data from file
typedef struct {
    unsigned id;
    char name[50]; // Not NULL, Latin
    char surname[50]; // Not NULL, Latin
    char group[12]; // Not NULL
    unsigned char scores[5];
} Student;

// Struct to hold comparison functions
typedef (*compare_functions) (const void* a, const void* b);

// Comparison function for sorting based on group
int compare_group(const void* a, const void* b){
    return ((Student*)a)->group - ((Student*)b)->group;
}

// Comparison function for sorting based on group in descending order
int compare_group_desc(const void* a, const void* b) {
    return ((Student*)b)->group - ((Student*)a)->group;
}

// Comparison function for sorting based on surname
int compare_surname(const void *a, const void *b) {
    return strcmp(((Student*)a)->surname, ((Student*)b)->surname);
}

// Comparison function for sorting based on surname in decrease
int compare_surname_desc(const void *a, const void *b) {
    return -(strcmp(((Student*)a)->surname, ((Student*)b)->surname));
}

// Comparison function for sorting based on name
int compare_name(const void *a, const void *b) {
    return strcmp(((Student*)a)->name, ((Student*)b)->name);
}

// Comparison function for sorting based on name in decrease
int compare_name_desk(const void *a, const void *b) {
    return -(strcmp(((Student*)a)->name, ((Student*)b)->name));
}

// Comparison function for sorting based on id
int compare_id(const void *a, const void *b) {
    return ((Student*)a)->id - ((Student*)b)->id;
}

// Comparison function for sorting based on id in decrease
int compare_id_desc(const void *a, const void *b) {
    return -(((Student*)a)->id - ((Student*)b)->id);
}

// Function to search the students by id || surname || name || group;
enum student_search_status_code student_search(const Student** students, const int num_students, Student** result, const char* field, const char* argument, int* founded_students){

    int found = 0; // The founded fields
    *result = malloc(sizeof(Student) * 5);

    if (*result == NULL){
        return student_search_fail;
    }

    switch (field[0])
    {
    // Search by id
    case 'i':
        unsigned id_to_find = 0;

        for(argument; *argument != '\0'; argument++){
            unsigned digit = *argument;
            id_to_find = id_to_find * 10 + digit;
        }

        for (int i = 0; i < num_students; i++){
            if ((*students)[i].id == id_to_find){
                (*result)[found] = (*students)[i];
                found++;
                if (found == 4){
                    *result = realloc(*result, (found * 2 + 1) * sizeof(Student));
                    // TODO Analyse allocation error
                }
            } 
        }

        if (found == 0){
            *result = NULL;
        }

        (*founded_students) = found;

        break;

    // Search by surname
    case 's':

        for (int i = 0; i < num_students; i++){
            if (strcmp((*students)[i].surname, argument) == 0){
                (*result)[found] = (*students)[i];
                found++;
                if (found == 4){
                    *result = realloc(*result, (found * 2 + 1) * sizeof(Student));
                    // TODO Analyse allocation error
                }
            } 
        }

        if (found == 0){
            *result = NULL;
        }

        (*founded_students) = found;

        break;
    
    // Search by name
    case 'n':
        
        for (int i = 0; i < num_students; i++){
            if (strcmp((*students)[i].name, argument) == 0){
                (*result)[found] = (*students)[i];
                found++;
                if (found == 4){
                    *result = realloc(*result, (found * 2 + 1) * sizeof(Student));
                    // TODO Analyse allocation error
                }
            } 
        }

        if (found == 0){
            *result = NULL;
        }

        (*founded_students) = found;

        break;
    
    // Search by group
    case 'g':

        for (int i = 0; i < num_students; i++){
            if (strcmp((*students)[i].group, argument) == 0){
                (*result)[found] = (*students)[i];
                found++;
                if (found == 4){
                    *result = realloc(*result, (found * 2 + 1) * sizeof(Student));
                    // TODO Analyse allocation error
                }
            } 
        }

        if (found == 0){
            *result = NULL;
        }

        (*founded_students) = found;

        break;
    
    // Invalid flag
    default:
        return student_search_fail;
    }

    return student_search_ok;
}

// Function to analyse the data in input_file
enum reading_students_status_code reading_students(FILE* input_file, Student** students, int* num_students){

    char buffer[256];

    char** line_errors = NULL;
    int num_errors = 0;

    // Allocate memory for students outside the loop
    *students = malloc(sizeof(Student));
    if (*students == NULL) {
        // Handle allocation failure
        return reading_students_fail;
    }

    while (fgets(buffer, sizeof(buffer), input_file) != NULL){
        unsigned id;
        char name[50];
        char surname[50];
        char  group[12];
        unsigned char scores[5];

        if (sscanf(buffer, "%u %s %s %s %hhu", &id, name, surname, group, scores) == 5){
            // Process the fields
            Student student;
            student.id = id;
            strcpy(student.name, name);
            strcpy(student.surname, surname);
            strcpy(student.group, group);
            strcpy(student.scores, scores);
            // TODO DATA VALIDATION

            // Adding the student to the vector
            *students = realloc(*students, (((*num_students) + 1) * 2) * sizeof(Student));
            
            if (*students == NULL) {
                // Handle reallocation failure
                free(line_errors); // Free previously allocated memory
                return reading_students_fail;
            }

            (*students)[(*num_students)] = student;
            (*num_students)++;
        } else {
            // Handle the error and skip the line
            // Allocate memory for a new string and copy the buffer content
            char* new_error = strdup(buffer);

            if (new_error == NULL) {
                // Handle allocation failure
                free(line_errors); // Free previously allocated memory
                return reading_students_fail;
            }

            // Resize the array of errors and add the new error
            line_errors = realloc(line_errors, (num_errors + 1) * sizeof(char*));

            if (line_errors == NULL) {
               // Handle reallocation failure
               free(new_error); // Free the newly allocated string
               return reading_students_fail;
            }

            line_errors[num_errors] = new_error;
            num_errors++;
        }
    }

    // TODO handle line errors

    // Free each error string
    for (int i = 0; i < num_errors; i++) {
        free(line_errors[i]);
    }

    return reading_students_ok;
}

// Function to sort the vector of table fields
enum students_sort_status_code students_sort(const int num_students, Student** students, const compare_functions compar, const char* output_file_path){
    FILE* output_file = fopen(output_file_path, "w");

    if (output_file == NULL){
        return students_sort_fail;
    }

    if (num_students <= 0 || students == NULL || compar == NULL){
        return students_sort_fail;
    }

    qsort(*students, num_students, sizeof(Student), compar);

    for(int i = 0; i < num_students; i++){
        fprintf(output_file, "ID: %u, Name: %s, Surname: %s, Group: %s, Scores: %hhu\n", (*students)[i].id,
                (*students)[i].name, (*students)[i].surname, (*students)[i].group, (*students)[i].scores);
    }

    fclose(output_file);
    
    return students_sort_ok;
}

// Function to calculate average student's score for exams
double average_score_function(Student student){
    double average_score = 0;

    for (int i = 0; i < 5; i++){
        int score = student.scores[i];
        average_score += (double) score;
    }

    return average_score / 5.0;
}

// Function to put the searched information into output file
enum stdout_students_search_status_code stdout_students_search(const Student** students, const int num_students, Student** result, const char* field, const char* argument, const char* output_file_path, int* founded_students){
    FILE* output_file;
    double average_score;

    // Go to function search
    if (student_search(students, num_students, result, field, argument, founded_students) == student_search_ok){
        output_file = fopen(output_file_path, "w");

        if (output_file == NULL){
            perror("Error writting in the file");
            return stdout_students_search_fail;
        }

        if (*result == NULL){
            return stdout_students_search_ok;
        }

        // Write into file
        for (int i = 0; i < founded_students; i++){
            double average_score = average_score_function((*result)[i]);
            fprintf(output_file, "ID: %u, Name: %s, Surname: %s, Group: %s Average score: %.2f\n", (*result)[i].id,
                (*result)[i].name, (*result)[i].surname, (*result)[i].group, average_score);
        }
    } else {
        return stdout_students_search_fail;
    }

    fclose(output_file);

    return stdout_students_search_ok;
}

// Function to calculate and output students whose average is greater than all_average score
enum stdout_students_higher_scores_status_code stdout_students_higher_scores(const Student** students, const int num_students, Student** result_average_scores, const char* output_file_path){
    double all_average_score = 0;
    int found = 0;
    FILE* output_file;

    output_file = fopen(output_file_path, "w");

    if (output_file == NULL){
        return stdout_students_higher_scores_fail;
    }

    (*result_average_scores) = malloc(sizeof(Student) * 5);

    for (int i = 0; i < num_students; i++){
        all_average_score += average_score_function((*students)[i]);
    }

    all_average_score /= num_students;

    for (int i = 0; i < num_students; i++){
        
        if ((average_score_function((*students)[i])) > all_average_score){
            (*result_average_scores)[found] = (*students)[i];
            found++;

            if (found == 4){
                *result_average_scores = realloc(*result_average_scores, (found * 2 + 1) * sizeof(Student));
                // TODO Analyse allocation error
            }
        } 
    }

    for (int i = 0; i < found; i++){
        fprintf(output_file, "Surname: %s, Name: %s \n", (*result_average_scores)[found].surname, (*result_average_scores)[found].name);
    }

    fclose(output_file);
    return stdout_students_higher_scores_ok;
}

int main (int argc, char argv[]){

    // Handle the input_string
    if (argc != 2)
    {
        printf("Usage: %s <input_file_path>\n", argv[0]);
        return 1;
    }

    char* input_file_path = argv[1];
    FILE* input_file = fopen(input_file_path, "r");

    if (input_file == NULL){
        printf("Error opening the starting file\n");
        return 1;
    }

    // Hande the file data
    Student* students = NULL;
    int num_students = 0;
    if (reading_students(input_file, &students, &num_students) == reading_students_ok){
        printf("File reading success\n");
    } else {
        printf("The input file reading error\n");
        return 1;
    }

    // The iterative console
    while (1) {
        printf("Choose an appropriate action.\n");
        printf("1. Search\n");
        printf("2. Sort file\n");
        printf("3. Students' average higher \n");

        int choice;
        scanf("%d", &choice);
        Student* result_found;
        Student* result_average_scores;
        char* field;
        char* output_file_path = NULL;

        switch (choice)
        {
        case 1:
        // The choices for search
            printf("Where to put the result?\n");
            int output_choice = 0;

            while (output_choice != 1 || output_choice != 2){
                printf("The choices are: <1> File <2> Command line\n");
                scanf("%d", &output_choice);
            }

            printf("For what field shoud the data be search : <id> <surname> <name> <group> ?");
            while (field != "id" || field != "surname" || field != "name" || field != "group"){
                printf("The choices are: <id> <surname> <name> <group> \n");
                scanf("%s", field);
            }

            char* argument = NULL;

            printf("Enter the argument to find with\n");
            scanf("%s", argument);

            int founded_students_search = 0;

            // To print in file
            if (output_choice == 1){
                output_file_path = NULL;
                printf("Enter the output_file_path: \n");
                scanf("%s", output_file_path);

                if (stdout_students_search(&students, num_students, &result_found, field, argument, output_file_path, &founded_students_search) == stdout_students_search_ok){
                    printf("The file has been changed\n");
                } else {
                    printf("Something went wrong\n");
                }
            }

            // To print into command line
            if (output_choice == 2){

                if(student_search(&students, num_students, &result_found, field, argument, &founded_students_search) == student_search_ok){
                    for (int i = 0; i < founded_students_search; i++){
                        printf("ID: %u, Name: %s, Surname: %s, Group: %s Scores: %s\n", result_found[i].id,
                                result_found[i].name, result_found[i].surname, result_found[i].group, result_found[i].scores);
                    }
                } else {
                    printf("Something went wrong\n");
                }

            } else {
                printf("Wrong choice\n");
            }
            break;
        
        case 2:
        // The choices for sort;
            char flag_sort;
            output_file_path = NULL;

            printf("What field should be sorted by ?\n");
            scanf("%s", field);

            while (field != "id" || field != "surname" || field != "name" || field != "group"){
                printf("The choices are: <id> <surname> <name> <group> \n");
                scanf("%s", field);
            }
            
            printf("By <a> increase or <b> decrease?\n");
            scanf("%c", flag_sort);

            while (flag_sort != 'a' || flag_sort != 'b'){
                printf("The choices are: <a> increase <b> decrease\n");
                scanf("%c", flag_sort);
            }

            printf("Enter the output_file_path: \n");
            scanf("%s", output_file_path);

            compare_functions compar;

            // Defining compar according to choices
            if (flag_sort == 'a'){
                switch (field[0])
                {
                case 'i':
                    compar = compare_id;
                    break;

                case 's':
                    compar = compare_surname;
                    break;

                case 'n':
                    compar = compare_name;
                    break;

                case 'g':
                    compar = compare_group;
                    break;

                default:
                    printf("The incorrect field\n");
                    break;
                }
            } else{
                switch (field[0])
                {
                case 'i':
                    compar = compare_id_desc;
                    break;

                case 's':
                    compar = compare_surname_desc;
                    break;

                case 'n':
                    compar = compare_name_desk;
                    break;

                case 'g':
                    compar = compare_group_desc;
                    break;

                default:
                    printf("The incorrect field\n");
                    break;
                }
            }

            if (students_sort(num_students, &students, compar, output_file_path) == students_sort_ok){
                printf("The file was changed\n");
            } else {
                printf("There was an error\n");
            }
            break;

        case 3:
            printf("Enter the output_file_path: \n");
            scanf("%s", output_file_path);

            if(stdout_students_higher_scores(&students, num_students, &result_average_scores, output_file_path) == stdout_students_higher_scores_ok){
                printf("The file was changed successfully\n");
            } else {
                printf("Something went wrong\n");
            }
            break;

        default:
            printf("There is not such a variant <1> <2> <3>\n");
            break;
        }
    }

    return 0;
}