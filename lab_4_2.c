#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum execute_command_status_code {execute_command_ok, execute_command_invalid_syntax, execute_command_unknown_operation};

enum save_array_status_code {save_array_ok, save_array_fail};

enum load_array_status_code {load_array_ok, load_array_fail};

enum print_stats_status_code {print_stats_ok, print_stats_fail};

enum fill_random_status_code {fill_random_ok, fill_random_invalid_parameter};

enum concat_array_status_code {concat_array_ok, concat_arrays_invalid_parameter};

enum remove_elements_status_code {remove_elements_ok, remove_elements_invalid_parameter};

enum copy_elements_status_code {copy_elements_ok, copy_elements_invalid_parameter};

typedef struct {
    int* data;
    int size;
} int_array;

typedef struct {
    char name;
    int_array array;
} array_variable;

typedef struct {
    array_variable variables[26]; // A-Z
} interpreter_state;

// Function to load array from file
enum load_array_status_code load_array(array_variable* array_var, const char* input_file_path) {
    int MAX_ARRAY_SIZE = 500;
    FILE* input_file = fopen(input_file_path, "r");
    if (input_file == NULL) {
        return load_array_fail;
    }

    int value;
    int count = 0;

    while (fscanf(input_file, "%d", &value) == 1) {
        if (count == MAX_ARRAY_SIZE) {
            fprintf(stderr, "Array size exceeds the maximum allowed size.\n");
            fclose(input_file);
            return load_array_fail;
        }

        array_var->array.data[count++] = value;
    }

    array_var->array.size = count;
    fclose(input_file);
    return true;
}

// Function to save array to file
enum save_array_status_code save_array(const array_variable* array_var, const char* output_file_path) {
    FILE* output_file = fopen(output_file_path, "w");
    if (output_file == NULL) {
        return save_array_fail;
    }

    for (int i = 0; i < array_var->array.size; ++i) {
        fprintf(output_file, "%d ", array_var->array.data[i]);
    }

    fclose(output_file);
    return save_array_ok;
}

// Function to generate random values in an array
enum fill_random_status_code fill_random(array_variable* array_var, int count, int lb, int rb) {
    int MAX_ARRAY_SIZE = 500;
    if (count > MAX_ARRAY_SIZE) {
        return fill_random_invalid_parameter;
    }

    array_var->array.size = count;

    for (int i = 0; i < count; ++i) {
        array_var->array.data[i] = rand() % (rb - lb + 1) + lb;
    }

    return fill_random_ok;
}

// Function to concatenate arrays
enum concat_array_status_code concat_arrays(array_variable* result, const array_variable* a, const array_variable* b) {
    int MAX_ARRAY_SIZE = 500;
    int size = a->array.size + b->array.size;

    if (size > MAX_ARRAY_SIZE) {
        return concat_arrays_invalid_parameter;
    }

    memcpy(result->array.data, a->array.data, a->array.size * sizeof(int));
    memcpy(result->array.data + a->array.size, b->array.data, b->array.size * sizeof(int));

    result->array.size = size;

    return concat_array_ok;
}

// Function to free array memory
void free_array(array_variable* array_var) {
    free(array_var->array.data);
    array_var->array.size = 0;
}

// Function to remove elements from an array
enum remove_elements_status_code remove_elements(array_variable* array_var, int start, int count) {
    if (start < 0 || start >= array_var->array.size || count <= 0) {
        return remove_elements_invalid_parameter;
    }

    int remaining = array_var->array.size - start - count;

    memmove(array_var->array.data + start, array_var->array.data + start + count, remaining * sizeof(int));
    array_var->array.size -= count;
    return remove_elements_ok;
}

// Function to copy elements from one array to another
enum copy_elements_status_code copy_elements(array_variable* dest, const array_variable* src, int start, int end) {
    int MAX_ARRAY_SIZE = 500;
    if (start < 0 || end >= src->array.size || start > end) {
        return copy_elements_invalid_parameter;
    }

    int count = end - start + 1;

    if (count > MAX_ARRAY_SIZE) {
        return copy_elements_invalid_parameter;
    }

    memcpy(dest->array.data, src->array.data + start, count * sizeof(int));
    dest->array.size = count;
    return copy_elements_ok;
}

// Comparison function ascending order
int compare_asc(const void* a, const void* b){
    return (*(int*)a) - (*(int*)b);
}

// Comparison function for descending order
int compare_desc(const void* a, const void* b) {
    return (*(int*)b) - (*(int*)a);
}


// Function to sort array in ascending order
void sort_array_asc(array_variable* array_var) {
    qsort(array_var->array.data, array_var->array.size, sizeof(int), compare_asc);
}

// Function to sort array in descending order
void sort_array_desc(array_variable* array_var) {
    qsort(array_var->array.data, array_var->array.size, sizeof(int), compare_desc);
}

// Function to shuffle array
void shuffle_array(array_variable* array_var) {
    for (int i = array_var->array.size - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = array_var->array.data[i];
        array_var->array.data[i] = array_var->array.data[j];
        array_var->array.data[j] = temp;
    }
}

// Function to print array elements
enum print_stats_status_code print_stats(const array_variable* array_var, double* result) {
    if (result == NULL) {
        return print_stats_fail;
    }

    int size = array_var->array.size;

    if (size == 0) {
        return print_stats_fail; // Array is empty
    }

    // Calculate array statistics
    int max_value = array_var->array.data[0];
    int min_value = array_var->array.data[0];
    int max_index = 0;
    int min_index = 0;
    int sum = 0;
    int max_occurrences = 1;
    int mode = array_var->array.data[0];

    // Calculate initial sum
    for (int i = 0; i < size; ++i) {
        sum += array_var->array.data[i];

        // Find max and min values with their indices
        if (array_var->array.data[i] > max_value) {
            max_value = array_var->array.data[i];
            max_index = i;
        }

        if (array_var->array.data[i] < min_value) {
            min_value = array_var->array.data[i];
            min_index = i;
        }
    }

    // Calculate mean
    double mean = (double)sum / size;

    // Calculate maximum deviation
    int max_deviation = 0;
    for (int i = 0; i < size; ++i) {
        int deviation = abs(array_var->array.data[i] - mean);
        if (deviation > max_deviation) {
            max_deviation = deviation;
        }
    }

    // Find mode
    for (int i = 0; i < size; ++i) {
        int count = 1;

        for (int j = i + 1; j < size; ++j) {
            if (array_var->array.data[i] == array_var->array.data[j]) {
                ++count;
            }
        }

        if (count > max_occurrences || (count == max_occurrences && array_var->array.data[i] > mode)) {
            max_occurrences = count;
            mode = array_var->array.data[i];
        }
    }

    // Fill the result array
    result[0] = size;
    result[1] = max_value;
    result[2] = max_index;
    result[3] = min_value;
    result[4] = min_index;
    result[5] = mode;
    result[6] = max_occurrences;
    result[7] = mean;
    result[8] = max_deviation;

    return print_stats_ok;
}

// Function to execute a command
enum execute_command_status_code execute_command(interpreter_state* state, const char* command) {
    char operation[20];
    char array_name;
    int count, lb, rb;
    char target_array;
    int start, end;
    
    if (sscanf(command, "%s %c", operation, &array_name) != 2) {
        return execute_command_invalid_syntax;
    }

    if (strcmp(operation, "Load") == 0) {
        char file_name[50];
        if (sscanf(command, "Load %c, %s;", &array_name, file_name) == 2) {
            if (load_array(&state->variables[array_name - 'A'], file_name) == load_array_fail) {
                return execute_command_invalid_syntax;
            }
        } else {
            return execute_command_invalid_syntax;
        }
    } else if (strcmp(operation, "Save") == 0) {
        char file_name[50];
        if (sscanf(command, "Save %c, %s;", &array_name, file_name) == 2) {
            if (save_array(&state->variables[array_name - 'A'], file_name) == save_array_fail) {
                return execute_command_invalid_syntax;
            }
        } else {
            return execute_command_invalid_syntax;
        }
    } else if (strcmp(operation, "Rand") == 0) {
        if (sscanf(command, "Rand %c, %d, %d, %d;", &array_name, &count, &lb, &rb) == 4) {
            if (fill_random(&state->variables[array_name - 'A'], count, lb, rb) == fill_random_invalid_parameter) {
                return execute_command_invalid_syntax;
            }
        } else {
            return execute_command_invalid_syntax;
        }
    } else if (strcmp(operation, "Concat") == 0) {
        char second_array_name;
        if (sscanf(command, "Concat %c, %c;", &array_name, &second_array_name) == 2) {
            if (concat_arrays(&state->variables[array_name - 'A'],
                              &state->variables[array_name - 'A'],
                              &state->variables[second_array_name - 'A']) == concat_arrays_invalid_parameter) {
                return execute_command_invalid_syntax;
            }
        } else {
            return execute_command_invalid_syntax;
        }
    } else if (strcmp(operation, "Free") == 0) {
        if (sscanf(command, "Free %c;", &array_name) == 1) {
            free_array(&state->variables[array_name - 'A']);
        } else {
            return execute_command_invalid_syntax;
        }
    } else if (strcmp(operation, "Remove") == 0) {
        if (sscanf(command, "Remove %c, %d, %d;", &array_name, &start, &count) == 3) {
            if (remove_elements(&state->variables[array_name - 'A'], start, count) == remove_elements_invalid_parameter) {
                return execute_command_invalid_syntax;
            }
        } else {
            return execute_command_invalid_syntax;
        }
    } else if (strcmp(operation, "Copy") == 0) {
        char dest_array_name;
        if (sscanf(command, "Copy %c, %d, %d, %c;", &array_name, &start, &end, &dest_array_name) == 4) {
            if (copy_elements(&state->variables[dest_array_name - 'A'],
                              &state->variables[array_name - 'A'], start, end) == copy_elements_invalid_parameter) {
                return execute_command_invalid_syntax;
            }
        } else {
            return execute_command_invalid_syntax;
        }
    } else if (strcmp(operation, "Sort") == 0) {
        if (sscanf(command, "Sort %c%c;", &array_name, &target_array) == 2) {
            if (target_array == '+') {
                sort_array_asc(&state->variables[array_name - 'A']);
            } else if (target_array == '-') {
                sort_array_desc(&state->variables[array_name - 'A']);
            } else {
                return execute_command_invalid_syntax;
            }
        } else {
            return execute_command_invalid_syntax;
        }
    } else if (strcmp(operation, "Shuffle") == 0) {
        if (sscanf(command, "Shuffle %c;", &array_name) == 1) {
            shuffle_array(&state->variables[array_name - 'A']);
        } else {
            return execute_command_invalid_syntax;
        }
    } else if (strcmp(operation, "Stats") == 0) {
        if (sscanf(command, "Stats %c;", &array_name) == 1) {
            double result[9];
            if (print_stats(&state->variables[array_name - 'A'], result) == print_stats_fail) {
                return execute_command_invalid_syntax;
            }
            // Display the statistics
            printf("Size: %f\n", result[0]);
            printf("Max Value: %f\n", result[1]);
            printf("Max Index: %f\n", result[2]);
            printf("Min Value: %f\n", result[3]);
            printf("Min Index: %f\n", result[4]);
            printf("Mode: %f\n", result[5]);
            printf("Max Occurrences: %f\n", result[6]);
            printf("Mean: %f\n", result[7]);
            printf("Max Deviation: %f\n", result[8]);
        } else {
            return execute_command_invalid_syntax;
        }
    } else if (strcmp(operation, "Print") == 0) {
        char print_type[5];
        if (sscanf(command, "Print %c, %s", &array_name, print_type) == 2) {
            if (strcmp(print_type, "all;") == 0) {
                // Print all elements
                for (int i = 0; i < state->variables[array_name - 'A'].array.size; ++i) {
                    printf("%d ", state->variables[array_name - 'A'].array.data[i]);
                }
                printf("\n");
            } else {
                if (sscanf(command, "Print %c, %d, %d;", &array_name, &start, &end) == 3) {
                    // Print elements from start to end
                    for (int i = start; i <= end && i < state->variables[array_name - 'A'].array.size; ++i) {
                        printf("%d ", state->variables[array_name - 'A'].array.data[i]);
                    }
                    printf("\n");
                } else {
                    return execute_command_invalid_syntax;
                }
            }
        } else {
            return execute_command_invalid_syntax;
        }
    } else {
        return execute_command_unknown_operation;
    }

    return execute_command_ok;
}



int main(int argc, char* argv[]) {
    interpreter_state state;
    int MAX_ARRAY_SIZE = 500;
    char* instructions_file = argv[1];

    // Initialize array variables
    for (int i = 0; i < 26; ++i) {
        state.variables[i].name = 'A' + i;
        state.variables[i].array.data = (int*)malloc(MAX_ARRAY_SIZE * sizeof(int));
        state.variables[i].array.size = 0;
    }

    // Read and execute instructions from the file
    FILE* file = fopen(instructions_file, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", instructions_file);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        // Execute the command
        execute_command(&state, line);
    }

    fclose(file);

    // Cleanup
    for (int i = 0; i < 26; ++i) {
        free_array(&state.variables[i]);
    }

    return 0;
}

