#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Status code for the function table_sort
enum table_sort_status_code {table_sort_ok, table_sort_fail};

enum reading_table_status_code {reading_table_ok, reading_table_fail, reading_table_file_failed, reading_table_error};

enum main_table_opearator_status_code 
{
    main_table_opearator_ok, 
    main_table_opearator_fail, 
    main_table_opearator_file_fail,
    main_table_opearator_flag_invalid
};


// Struct of fields in the file
typedef struct {
    unsigned id;
    char* name;
    char* surname;
    double salary;
} Employee;

// Comparison function for sorting based on salary
int compare_salary(const void* a, const void* b){
    return ((Employee*)a)->salary - ((Employee*)b)->salary;
}

// Comparison function for sorting based on salary in descending order
int compare_salary_desc(const void* a, const void* b) {
    return ((Employee*)b)->salary - ((Employee*)a)->salary;
}

// Comparison function for sorting based on surname
int compare_surname(const void *a, const void *b) {
    return strcmp(((Employee *)a)->surname, ((Employee *)b)->surname);
}

// Comparison function for sorting based on surname in decrease
int compare_surname_desc(const void *a, const void *b) {
    return -(strcmp(((Employee *)a)->surname, ((Employee *)b)->surname));
}

// Comparison function for sorting based on name
int compare_name(const void *a, const void *b) {
    return strcmp(((Employee *)a)->name, ((Employee *)b)->name);
}

// Comparison function for sorting based on name in decrease
int compare_name_desk(const void *a, const void *b) {
    return -(strcmp(((Employee *)a)->name, ((Employee *)b)->name));
}

// Comparison function for sorting based on id
int compare_id(const void *a, const void *b) {
    return ((Employee *)a)->id - ((Employee *)b)->id;
}

// Comparison function for sorting based on id in decrease
int compare_id_desc(const void *a, const void *b) {
    return -(((Employee *)a)->id - ((Employee *)b)->id);
}

// Struct to hold comparison functions
typedef struct {
    int (*salary)(const void*, const void*);
    int (*name)(const void*, const void*);
    int (*surname)(const void*, const void*);
    int (*id)(const void*, const void*);
} compare_functions;

// Function to read the table and form the structure
enum reading_table_status_code reading_table(FILE* input_file, Employee** employees, int* num_employees){

    char buffer[256];

    char** line_errors = NULL;
    int num_errors = 0;
    int num_string = 0;
    int data_errors = 0;

    // Allocate memory for employees outside the loop
    *employees = malloc(sizeof(Employee));
    if (*employees == NULL) {
        // Handle allocation failure
        return reading_table_fail;
    }

    while (fgets(buffer, sizeof(buffer), input_file) != NULL){
        int id;
        char* name = (char*)malloc(sizeof(char) * 50);

        if (name == NULL){
            return reading_table_fail;
        }

        char* surname = (char*)malloc(sizeof(char) * 50);

        if (surname == NULL){
            free(name);
            return reading_table_fail;
        }

        double salary;
        num_string++;

        if (sscanf(buffer, "%u %s %s %lf", &id, name, surname, &salary) == 4){

            name = (char*)realloc(name, sizeof(char) * strlen(name));
            
            if (name == NULL){
                free(surname);
                return reading_table_fail;
            }

            surname = (char*)realloc(surname, sizeof(char) * strlen(surname));

            if (surname == NULL){
                free(name);
                return reading_table_fail;
            }

            int is_error = 0;

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

            if (salary < 0){
                printf("An error occured in string: %d\n", num_string);
                data_errors++;
                is_error = 1;
                continue;
            }

            if (is_error){
                continue;
            }

            // Process the fields
            Employee employee;
            employee.id = id;

            employee.name = (char*)malloc(sizeof(char) * strlen(name));
            
            if (employee.name == NULL){
                return reading_table_fail;
            }

            strcpy(employee.name, name);
            
            employee.surname = (char*)malloc(sizeof(char) * strlen(name));
            
            if (employee.surname == NULL){
                return reading_table_fail;
            }

            strcpy(employee.surname, surname);
            employee.salary = salary;

            // Adding the employee to the vector
            *employees = realloc(*employees, (((*num_employees) + 1) * 2) * sizeof(Employee));
            
            if (*employees == NULL) {
                // Handle reallocation failure
                free(line_errors); // Free previously allocated memory
                return reading_table_fail;
            }

            (*employees)[(*num_employees)] = employee;
            (*num_employees)++;
        } else {
            // Handle the error and skip the line
            // Allocate memory for a new string and copy the buffer content
            char* new_error;
            new_error = (char*)malloc(strlen(buffer) * sizeof(char));

            if (new_error == NULL) {
                // Handle allocation failure
                free(line_errors); // Free previously allocated memory
                return reading_table_fail;
            }

            new_error = strdup(buffer);

            // Resize the array of errors and add the new error
            line_errors = (char**)realloc(line_errors, (num_errors + 1) * sizeof(char*));

            if (line_errors == NULL) {
               // Handle reallocation failure
               free(new_error); // Free the newly allocated string
               return reading_table_fail;
            }

            line_errors[num_errors] = (char*)realloc(new_error, strlen(new_error) * sizeof(char));

            if (line_errors[num_errors] == NULL){
                return reading_table_fail;
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
    }

    // Free each error string
    for (int i = 0; i < num_errors; i++) {
        printf("An error occured in reading line %d: %s\n", num_string, line_errors[i]);
        free(line_errors[i]);
    }

    return reading_table_ok;
}

// function to sort the vector of table fields
enum table_sort_status_code table_sort(int num_employees, Employee** employees, const compare_functions *compar){

    if (num_employees <= 0 || employees == NULL || compar == NULL){
        return table_sort_fail;
    }

    qsort(*employees, num_employees, sizeof(Employee), compar->salary);

    // Handle the case when salaries are equal
    for (int i = 1; i < num_employees; i++) {
        if ((*employees)[i - 1].salary == (*employees)[i].salary) {
            // Use name comparison
            int result = compar->name(&(*employees)[i - 1], &(*employees)[i]);

            if (result == 0) {
                // Names are equal, use surname comparison
                result = compar->surname(&(*employees)[i - 1], &(*employees)[i]);

                if (result == 0) {
                    // Surnames are equal, use ID comparison
                    result = compar->id(&(*employees)[i - 1], &(*employees)[i]);
                }
            }

            // If result is not 0, it means the order is incorrect, so swap the elements
            if (result > 0) {
                Employee temp = (*employees)[i - 1];
                (*employees)[i - 1] = (*employees)[i];
                (*employees)[i] = temp;
            }
        }
    }

    return table_sort_ok;
}

// Function to operate the table
enum main_table_opearator_status_code main_table_opearator(char* input_file_path, char* output_file_path, char flag){

    if (strcmp(input_file_path, output_file_path) == 0){
        return main_table_opearator_fail;
    }

    FILE* input_file;
    input_file = fopen(input_file_path, "r");
    if (input_file == NULL){
        perror("Error opening input file");
        return main_table_opearator_file_fail;
    }
    
    FILE* output_file;
    output_file = fopen(output_file_path, "w");

    if (output_file == NULL){
        perror("Error opening output file");
        fclose(input_file);
        return main_table_opearator_file_fail;
    }

    Employee* employees = NULL;
    int num_employees = 0;

    if (reading_table(input_file, &employees, &num_employees) == reading_table_fail){
        fclose(input_file);
        fclose(output_file);
        return main_table_opearator_fail;
    }
    
    fclose(input_file);

    compare_functions compar;

    switch (flag){
    case 'a':
        compar.salary = compare_salary;
        compar.name = compare_name;
        compar.surname = compare_surname;
        compar.id = compare_id;
        break;
    case 'b':
        compar.salary = compare_salary_desc;
        compar.name = compare_name_desk;
        compar.surname = compare_surname_desc;
        compar.id = compare_id_desc;
        break;
    default:
        fclose(output_file);
        free(employees);
        return main_table_opearator_flag_invalid;
    }

    if (table_sort(num_employees, &employees, &compar) == table_sort_fail) {
        fclose(output_file);
        free(employees);
        return main_table_opearator_fail;
    }

    for(int i = 0; i < num_employees; i++){
        fprintf(output_file, "ID: %u, Name: %s, Surname: %s, Salary: %.2f\n", employees[i].id,
                employees[i].name, employees[i].surname, employees[i].salary);
    }

    fclose(output_file);

    free(employees);

    return main_table_opearator_ok;
}

int main (int argc, char* argv []){

    if (argc != 4)
    {
        printf("Usage: %s <input_file_path> <flag> <output_file_path>\n", argv[0]);
        return 1;
    }

    char* input_file_path = argv[1];
    char* output_file_path = argv[3];
    char separ =  argv[2][0];

    if ((separ != '-' && separ != '/')){
        printf("Invalid flag  separator %c\n", separ);
        return 1;
    }

    char flag = argv[2][1];

    if (flag != 'a' && flag != 'b')
    {
        printf("Invalid flag: %c. Use 'a' or 'b'\n", flag);
        return 1;
    }

    if(main_table_opearator(input_file_path, output_file_path, flag) == main_table_opearator_ok){
        printf("The file was changed\n");
    } else {
        printf("An error occurred\n");
    }

    return 0;
}