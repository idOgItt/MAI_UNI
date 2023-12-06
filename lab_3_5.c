#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Status codes for students_search
enum student_search_status_code {student_search_ok, student_search_fail};

enum reading_students_status_code {reading_students_ok, reading_students_fail};

enum students_sort_status_code {students_sort_ok, students_sort_fail};

enum stdout_students_search_status_code {stdout_students_search_ok, stdout_students_search_fail};

enum stdout_students_higher_scores_status_code {stdout_students_higher_scores_ok, stdout_students_higher_scores_fail};

// Structure for incoming data from file
typedef struct {
    unsigned id;
    char* name; // Not NULL, Latin
    char* surname; // Not NULL, Latin
    char* group; // Not NULL
    unsigned char* scores;
} Student;

// Struct to hold comparison functions
typedef int (*compare_functions) (const void* a, const void* b);

int len_array(unsigned char** scores){
    int length = 0;
    unsigned char** tmp_score = scores;
    for (int i = 0; i < 5; i++){
        if (tmp_score[i][0]){
            length++;
        }
    }
    return length;
}

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
enum student_search_status_code student_search(Student* students, const int num_students, Student** result, const char* field, const char* argument, int* founded_students){

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

        for(int i = 0; i < strlen(argument); i++){
            id_to_find = atoi(argument);
        }

        for (int i = 0; i < num_students; i++){
            if ((students)[i].id == id_to_find){
                (*result)[found] = (students)[i];
                found++;
                if (found == 4){
                    *result = realloc(*result, (found * 2 + 1) * sizeof(Student));
                    
                    if (*result == NULL){
                        return student_search_fail;
                    }

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
            if (strcmp((students)[i].surname, argument) == 0){
                (*result)[found] = (students)[i];
                found++;
                if (found % 5 == 4){
                    *result = realloc(*result, (found * 2 + 1) * sizeof(Student));
                    
                    if (*result == NULL){
                        return student_search_fail;
                    }

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
            if (strcmp((students)[i].name, argument) == 0){
                (*result)[found] = (students)[i];
                found++;
                if (found == 4){
                    *result = realloc(*result, (found * 2 + 1) * sizeof(Student));
                    
                    if (*result == NULL){
                        return student_search_fail;
                    }

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
            if (strcmp((students)[i].group, argument) == 0){
                (*result)[found] = (students)[i];
                found++;
                if (found == 4){
                    *result = realloc(*result, (found * 2 + 1) * sizeof(Student));
                    
                    if (*result == NULL){
                        return student_search_fail;
                    }

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
    int num_string = 0;
    int data_errors = 0;

    // Allocate memory for students outside the loop
    *students = malloc(sizeof(Student));
    if (*students == NULL) {
        // Handle allocation failure
        return reading_students_fail;
    }

    while (fgets(buffer, sizeof(buffer), input_file) != NULL){
        int id;
        num_string++;
        char* name = (char*)malloc(sizeof(char) * 50);

        if (name == NULL){
            return reading_students_fail;
        }

        char* surname = (char*)malloc(sizeof(char) * 50);

        if (surname == NULL){
            free(name);
            return reading_students_fail;
        }

        char* group = (char*)malloc(sizeof(char) * 12);

        if (group == NULL){
            free(name); free(surname);
            return reading_students_fail;
        }

        unsigned char** scores = (unsigned char**)(malloc(sizeof(unsigned char*) * 5));

        if (scores == NULL){
            free(name); free(surname); free(group);
            return reading_students_fail;
        }

        for (int i = 0; i < 5; i++){
            scores[i] = (unsigned char*)malloc(sizeof(unsigned(char)));
        }

        if (sscanf(buffer, "%u %s %s %s %hhu %hhu %hhu %hhu %hhu", &id, name, surname, group, scores[0], scores[1], scores[2], scores[3], scores[4]) == 9){

            //printf("%hhn\n", scores);

            int is_error = 0;

            name = (char*)realloc(name, strlen(name) * sizeof(char));

            if (name == NULL){
                free(scores); free(surname); free(group);
                return reading_students_fail;
            }

            surname = (char*)realloc(surname, strlen(surname) * sizeof(char));
            
            if (surname == NULL){
                free(scores); free(name); free(group);
                return reading_students_fail;
            }

            group = (char*)realloc(group, strlen(group) * sizeof(char));

            if (group == NULL){
                free(scores); free(surname); free(name);
                return reading_students_fail;
            }

            int len_scores = len_array(scores);

            scores = (unsigned char**)realloc(scores, (len_scores * sizeof(unsigned char*)));

            if (scores == NULL){
                free(name); free(surname); free(group);
                return reading_students_fail;
            }

            if (id < 0){
                printf("An error occured in string: %d\n", num_string);
                data_errors++;
                is_error = 1;
                continue;
            }

            for (int i = 0; i < strlen(name); i++){
                if (!isalpha(name[i])){
                    printf("An error occured in string: %d\n", num_string);
                    data_errors++;
                    is_error = 1;
                    continue;
                }
                continue;
            }

            for (int i = 0; i < strlen(surname); i++){
                if (!isalpha(surname[i])){
                    printf("An error occured in string: %d\n", num_string);
                    data_errors++;
                    is_error = 1;
                    continue;
                }
            }

            if (len_array(scores) != 5){
                printf("An error occured in string: %d\n", num_string);
                data_errors++;
                is_error = 1;
                continue;
            }

            if (is_error){
                continue;
            }

            // Process the fields
            Student student;
            student.id = id;
            student.name = (char*)malloc(strlen(name) * sizeof(char));

            if (student.name == NULL){
                free(name); free(surname); free(group); free(scores);
                return reading_students_fail;
            }

            strcpy(student.name, name);
            student.surname = (char*)malloc(strlen(surname) * sizeof(char));

            if (student.surname == NULL){
                free(name); free(surname); free(group); free(scores); free(student.name);
                return reading_students_fail;
            }

            strcpy(student.surname, surname);
            student.group = (char*)malloc(strlen(group) * sizeof(char));

            if (student.group == NULL){
                free(name); free(surname); free(group); free(scores); free(student.name); free(student.surname);
                return reading_students_fail;
            }

            strcpy(student.group, group);
            student.scores = (unsigned char*)malloc(len_array(scores) * sizeof(unsigned char));

            if (student.scores == NULL){
                free(name); free(surname); free(group); free(scores); free(student.name); free(student.group); free(student.surname);
                return reading_students_fail;
            }
            
            for (int i = 0; i < len_array(scores); i++){
                student.scores[i] = (scores)[i][0];
            }

            // Adding the student to the vector
            *students = realloc(*students, (((*num_students) + 1)) * sizeof(Student));
            
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
            char* new_error;
            new_error = (char*)malloc(strlen(buffer) * sizeof(char));

            if (new_error == NULL) {
                // Handle allocation failure
                free(line_errors); // Free previously allocated memory
                return reading_students_fail;
            }

            new_error = strdup(buffer);

            // Resize the array of errors and add the new error
            line_errors = (char**)realloc(line_errors, (num_errors + 1) * sizeof(char*));

            if (line_errors == NULL) {
               // Handle reallocation failure
               free(new_error); // Free the newly allocated string
               return reading_students_fail;
            }

            line_errors[num_errors] = (char*)realloc(new_error, strlen(new_error) * sizeof(char));

            if (line_errors[num_errors] == NULL){
                return reading_students_fail;
            }

            line_errors[num_errors] = new_error;
            num_errors++;
        }

        if (name != NULL){
            free(name);
        }

        if (surname != NULL){
            free(surname);
        }

        if (group != NULL){
            free(group);
        }

        if (scores != NULL){
            free(scores);
        }

    }

    // Free each error string
    for (int i = 0; i < num_errors; i++) {
        printf("An error occured in reading line %d: %s\n", num_string, line_errors[i]);
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
                (*students)[i].name, (*students)[i].surname, (*students)[i].group, (*students)[i].scores[0]);
    }

    fclose(output_file);
    
    return students_sort_ok;
}

// Function to calculate average student's score for exams
double average_score_function(Student student){
    double average_score = 0;
    int length = 5;

    for (int i = 0; i < length; i++){
        int score = student.scores[i];
        average_score += (double) score;
    }

    return average_score / (double)length;
}

// Function to put the searched information into output file
enum stdout_students_search_status_code stdout_students_search( Student* students, const int num_students, Student** result, const char* field, const char* argument, const char* output_file_path, int* founded_students){
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
        for (int i = 0; i < *founded_students; i++){
            average_score = average_score_function((*result)[i]);
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
enum stdout_students_higher_scores_status_code stdout_students_higher_scores(Student* students, const int num_students, Student** result_average_scores, const char* output_file_path){
    double all_average_score = 0;
    int found = 0;
    FILE* output_file;

    output_file = fopen(output_file_path, "w");

    if (output_file == NULL){
        return stdout_students_higher_scores_fail;
    }

    (*result_average_scores) = (Student*)malloc(sizeof(Student) * 5);

    if (*result_average_scores == NULL){
        return stdout_students_higher_scores_fail;
    }

    for (int i = 0; i < num_students; i++){
        all_average_score += average_score_function((students)[i]);
    }

    all_average_score /= num_students;

    for (int i = 0; i < num_students; i++){
        
        if ((average_score_function((students)[i])) > all_average_score){
            (*result_average_scores)[found] = (students)[i];
            found++;

            if (found == 4){
                *result_average_scores = (Student*)realloc(*result_average_scores, (found * 2 + 1) * sizeof(Student));
                if (*result_average_scores == NULL){
                    return stdout_students_higher_scores_fail;
                }
            }
        } 
    }

    for (int i = 0; i < found; i++){
        fprintf(output_file, "Surname: %s, Name: %s \n", (*result_average_scores)[i].surname, (*result_average_scores)[i].name);
    }

    fclose(output_file);
    return stdout_students_higher_scores_ok;
}

int main (int argc, char* argv[]){

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
        printf("0. To exit \n");

        // TODO to char*
        int choice;
        scanf("%d", &choice);
        Student* result_found = NULL;
        Student* result_average_scores = NULL;
        char* field_search = (char*)malloc(sizeof(char) * 50);
        
        if (field_search == NULL){
            return -1;
        }

        char* field_sort = (char*)malloc(sizeof(char) * 20);

        if (field_sort == NULL){
            free(field_search);
            return -1;
        }

        char* output_file_path = (char*)malloc(sizeof(char) * 50);

        if (output_file_path == NULL){
            free(field_search); free(field_sort);
            return -1;
        }

        switch (choice)
        {
        case 1:
        // The choices for search
            printf("Where to put the result?\n");
            // TODO char
            int output_choice = 0;

            while (output_choice != 1 && output_choice != 2){
                printf("The choices are: <1> File <2> Command line\n");
                scanf("%d", &output_choice);
            }

            printf("For what field shoud the data be search : <id> <surname> <name> <group> ?");
            while (strcmp(field_search, "id") != 0 && strcmp(field_search, "surname") != 0 && strcmp(field_search, "name") != 0 && strcmp(field_search, "group") != 0){
                printf("The choices are: <id> <surname> <name> <group> \n");
                scanf("%s", field_search);
            }

            char* argument = (char*)malloc(50 * sizeof(char));

            printf("Enter the argument to find with\n");
            scanf("%s", argument);

            int founded_students_search = 0;

            // To print in file
            if (output_choice == 1){
                output_file_path = (char*)malloc(sizeof(char) * 50);
                printf("Enter the output_file_path: \n");
                scanf("%s", output_file_path);

                if (strcmp(input_file_path, output_file_path) == 0){
                    break;
                }

                if (stdout_students_search(students, num_students, &result_found, field_search, argument, output_file_path, &founded_students_search) == stdout_students_search_ok){
                    printf("The file has been changed\n");
                } else {
                    printf("Something went wrong\n");
                }
            

            // To print into command line
            } else if (output_choice == 2){

                if(student_search(students, num_students, &result_found, field_search, argument, &founded_students_search) == student_search_ok){
                    for (int i = 0; i < founded_students_search; i++){
                        printf("ID: %u, Name: %s, Surname: %s, Group: %s", result_found[i].id,
                                result_found[i].name, result_found[i].surname, result_found[i].group);
                        for (int j = 0; j < 5; j++){
                            printf(", Scores: %hhu", result_found[i].scores[j]);
                        }
                        printf("\n");
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
            //output_file_path = NULL;

            printf("What field should be sorted by <id> <surname> <name> <group> ?\n");
            scanf("%s", field_sort);

            while (strcmp(field_sort, "id") != 0 && strcmp(field_sort, "surname") != 0 && strcmp(field_sort, "name") != 0 && strcmp(field_sort, "group") != 0){
                printf("The choices are: <id> <surname> <name> <group> \n");
                scanf("%s", field_sort);
            }
            
            printf("By <a> increase or <b> decrease?\n");
            scanf("%c", &flag_sort);

            while (flag_sort != 'a' && flag_sort != 'b'){
                printf("The choices are: <a> increase <b> decrease\n");
                scanf("%c", &flag_sort);
            }

            printf("Enter the output_file_path: \n");
            scanf("%s", output_file_path);

            if (output_file_path == NULL){
                perror("No such a file\n");
                break;
            }

            if (strcmp(input_file_path, output_file_path) == 0){
                break;
            }

            compare_functions compar;

            // Defining compar according to choices
            if (flag_sort == 'a'){
                switch (field_sort[0])
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
                switch (field_sort[0])
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

            if(stdout_students_higher_scores(students, num_students, &result_average_scores, output_file_path) == stdout_students_higher_scores_ok){
                printf("The file was changed successfully\n");
            } else {
                printf("Something went wrong\n");
            }
            break;

        case 0:
            return 0;

        default:
            printf("There is not such a variant <1> <2> <3>\n");
            break;
        }
    }

    return 0;
}