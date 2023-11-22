#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum handle_file_status_code { handle_file_ok, handle_file_fail, handle_file_no_name, handle_file_no_operation };

enum perform_operation_status_code { perform_operation_zero, perform_operation_invalid, perform_operation_ok };

// Structure to represent a memory cell
typedef struct {
    char* variable;
    int data;
} MemoryCell;

// Structure to represent an instruction
typedef struct {
    MemoryCell* node;
    char* operation;
} Instruction;

// Structure to represent the interpreter state
typedef struct {
    MemoryCell* memory;
    int memory_size;
} Interpreter_state;

// Comparison function for qsort
int compare_memory_cells(const void* a, const void* b) {
    return strcmp(((MemoryCell*)a)->variable, ((MemoryCell*)b)->variable);
}

int is_operator(char c) {
    switch (c) {
        default:
            return 1;
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '^':
            return 0;
    }
}

// Function to find a variable in the memory
int find_variable(Interpreter_state* state, const char* variable_name) {
    for (int i = 0; i < state->memory_size; ++i) {
        if (strcmp(state->memory[i].variable, variable_name) == 0) {
            return i; // Variable found
        }
    }
    return -1; // Variable not found
}

// Function to sort the memory cells
void sort_memory_cells(Interpreter_state* state) {
    qsort(state->memory, state->memory_size, sizeof(MemoryCell), compare_memory_cells);
}

int isdigit_string(const char* string){
    int digit;
    int num;
    while (*string){
        if (isdigit(*string)){
            digit = *string - '0';
            num = num * 10 + digit;
        } else {
            return -1;
        }
        string++;
    }
    return num;
}

// Function to initialize variables
int initialize_variable(Interpreter_state* state, const char* variable_name, const char* value_name) {

    int value = 0;

    if (isdigit_string(value_name) != -1){

        value = isdigit_string(value_name);

    } else {
        int index_value = find_variable(state, value_name);
        
        if (index_value != -1){
        value = state->memory[index_value].data;
        } else {
            perror("There is no such a variable");
            return -1;
        }

    }

    // Check if the variable already exists
    int index = find_variable(state, variable_name);

    if (index != -1) {
        // Variable exists, update its value
        state->memory[index].data = value;
    } else {
        // Variable does not exist, create a new one
        state->memory_size++;
        state->memory = (MemoryCell*)realloc(state->memory, state->memory_size * sizeof(MemoryCell));

        if (state->memory == NULL) {
            perror("Memory allocation error");
            return -1; // Failure
        }

        // Initialize the new variable
        state->memory[state->memory_size - 1].variable = strdup(variable_name);

        if (state->memory[state->memory_size - 1].variable == NULL) {
            perror("Memory allocation error");
            return -1; // Failure
        }

        state->memory[state->memory_size - 1].data = value;
    }

    return 0; // Success
}

// Function to perform operations
enum perform_operation_status_code perform_operation(Interpreter_state* state, const char* variable_name, const char operation, const char* operand_name, int* result) {
    // Search for the variable
    int index = find_variable(state, variable_name);
    
    int operand = 0;

    if (isdigit_string(operand_name) != -1){

        operand = isdigit_string(operand_name);

    } else {
        int operand_value = find_variable(state, operand_name);
        
        if (operand_value != -1){
        operand = state->memory[operand_value].data;
        } else {
            perror("There is no such a variable");
            return perform_operation_invalid;
        }

    }

    if (index != -1) {
        // Variable found, perform the operation
        if (operation == '+') {
            *result = state->memory[index].data + operand;
        } else if (operation == '-') {
            *result = state->memory[index].data - operand;
        } else if (operation == '*') {
            *result = state->memory[index].data * operand;
        } else if (operation == '/') {
            // Non Zero
            if (operand != 0) {
                *result = state->memory[index].data / operand;
            } else {
                fprintf(stderr, "Error: Division by zero\n");
                return perform_operation_zero;
            }
        } else if (operation == '%') {
            // Non Zero
            if (operand != 0) {
                *result = state->memory[index].data % operand;
            } else {
                fprintf(stderr, "Error: Division by zero\n");
                return perform_operation_zero;
            }
        } else {
            fprintf(stderr, "Error: Unsupported operation\n");
            return perform_operation_invalid;
        }

        // Sort the memory cells after modification
        sort_memory_cells(state);
        return perform_operation_ok;
    } else {
        fprintf(stderr, "Error: Variable '%s' not found\n", variable_name);
        return perform_operation_invalid;
    }
}

// Function to free the interpreter state
void free_interpreter_state(Interpreter_state* state) {
    for (int i = 0; i < state->memory_size; ++i) {
        free(state->memory[i].variable);
    }
    free(state->memory);
}

void free_instructions(Instruction* instructions, int instruction_count) {
    for (int i = 0; i < instruction_count; ++i) {
        free(instructions[i].node);
        free(instructions[i].operation);
    }

    free(instructions);
}



// Function to execute instructions
void execute_instructions(Interpreter_state* state, Instruction* instructions, int instruction_count) {
    for (int i = 0; i < instruction_count; ++i) {

        // Print
        if (strcmp(instructions[i].operation, "print") == 0) {
            if (instructions[i].node[0].variable == NULL) {
                // Print all variables
                for (int j = 0; j < state->memory_size; j++) {
                    printf("%s = %d\n", state->memory[j].variable, state->memory[j].data);
                }
            } else {
                // Print the specified variable
                int index = find_variable(state, instructions[i].node[0].variable);

                if (index != -1) {
                    printf("%s = %d\n", instructions[i].node[0].variable, state->memory[index].data);
                } else {
                    fprintf(stderr, "Error: Variable '%s' not found\n", instructions[i].node[i].variable);
                }
            }
        }

        // Operations
        if (is_operator(instructions[i].operation[0])) {

            // Perform operation and initialize variable
            int result = 0;
            if (perform_operation(state, instructions[i].node[1].variable,
                                  instructions[i].operation[1], instructions[i].node[2].variable, &result) == perform_operation_ok);
            
            char* result_str;
            int char_num = 0;

            while(result != 0){
                result /= 10;
                char_num++;
            }

            result_str = malloc(sizeof(char) * char_num);
            // TODO check

            sprintf(result_str, "%d", result);
            
            if (instructions[i].operation[0] == '=') {
                initialize_variable(state, instructions[i].node[0].variable, result_str);
            }

            free(result_str);
        }

        if (instructions[i].operation[0] == '=') {
            initialize_variable(state, instructions[i].node[0].variable, instructions[i].node[1].variable);
        }
    }
}

// Function to handle the input file
enum handle_file_status_code handle_file(const char* input_file_path, Instruction** instructions, int* instructions_num) {
    FILE* input_file = fopen(input_file_path, "r");

    if (input_file == NULL) {
        return handle_file_fail;
    }

    char buffer[256];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), input_file)) {
        Instruction new_instruction;
        new_instruction.node = malloc(3 * sizeof(MemoryCell));
        new_instruction.operation = malloc(sizeof(char) * 2);

        // Using to parse the line
        char current_char;
        char* variable;
        int operators_num = 0;
        int variable_num = 0;
        int i = 0;

        variable = (char*)malloc(sizeof(char) * 256);

        for (int cc = 0; cc < strlen(buffer); cc++) {
            current_char = buffer[cc];

            if (current_char == ' ') {
                if (strcmp("print", variable) == 0) {
                    new_instruction.operation = strdup(variable);
                    // Skip "print" and read the variable until the semicolon
                    sscanf(buffer + cc, "print %s;", new_instruction.node[variable_num].variable);
                    break;
                }
            }

            if (current_char == ';') {
                if (strcmp("print", variable) == 0) {
                    new_instruction.node[variable_num].variable = strdup(variable);
                    break;
                }
            }

            if (is_operator(current_char)) {
                new_instruction.operation[operators_num] = current_char;
                operators_num++;
                new_instruction.node[variable_num].variable = strdup(variable);
                variable_num++;

                free(variable);
                variable = malloc(sizeof(char) * 256);
                i = 0;

            } else {
                variable[i] = current_char;
                i++;
            }
            variable = realloc(variable, (i + 1) * sizeof(char));
        }

        count++;

        // Resize the instructions array
        (*instructions) = (Instruction*)realloc(*instructions, (count) * sizeof(Instruction));

        if (*instructions == NULL) {
            free(variable);
            fclose(input_file);
            return handle_file_fail;
        }

        (*instructions)[count - 1] = new_instruction;
    }

    fclose(input_file);

    *instructions_num = count;

    return handle_file_ok;
}



int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* input_file_path = argv[1];
    int instruction_num;
    Instruction* instructions = NULL;
    Interpreter_state state = { NULL, 0 };

    // Handle the input file
    switch (handle_file(input_file_path, &instructions, &instruction_num)) {
    case handle_file_ok:
        // Sort the memory cells
        sort_memory_cells(&state);
        // Execute the instructions
        execute_instructions(&state, instructions, instruction_num);
        // Free memory
        free_interpreter_state(&state);
        free_instructions(instructions, instruction_num);
        break;
    case handle_file_fail:
        fprintf(stderr, "Error: Failed to handle the file\n");
        exit(EXIT_FAILURE);
    case handle_file_no_name:
        fprintf(stderr, "Error: Failed to allocate memory for variable name\n");
        exit(EXIT_FAILURE);
    case handle_file_no_operation:
        fprintf(stderr, "Error: Failed to allocate memory for operation\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
