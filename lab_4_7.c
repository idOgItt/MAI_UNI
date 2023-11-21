#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum handle_file_status_code { handle_file_ok, handle_file_fail, handle_file_no_name, handle_file_no_operation };

enum perform_operation_status_code { perform_operation_zero, perform_operation_invalid, perform_operation_ok };

// Structure to represent a memory cell
typedef struct {
    char* variable;
    int data;
} MemoryCell;

// Structure to represent an instruction
typedef struct {
    MemoryCell node;
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

// Function to initialize variables
int initialize_variable(Interpreter_state* state, const char* variable_name, int value) {
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
enum perform_operation_status_code perform_operation(Interpreter_state* state, const char* variable_name, const char* operation, int operand) {
    // Search for the variable
    int index = find_variable(state, variable_name);

    if (index != -1) {
        // Variable found, perform the operation
        if (strcmp(operation, "+") == 0) {
            state->memory[index].data += operand;
        } else if (strcmp(operation, "-") == 0) {
            state->memory[index].data -= operand;
        } else if (strcmp(operation, "*") == 0) {
            state->memory[index].data *= operand;
        } else if (strcmp(operation, "/") == 0) {
            // Non Zero
            if (operand != 0) {
                state->memory[index].data /= operand;
            } else {
                fprintf(stderr, "Error: Division by zero\n");
                return perform_operation_zero;
            }
        } else if (strcmp(operation, "%") == 0) {
            // Non Zero
            if (operand != 0) {
                state->memory[index].data %= operand;
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

// Function to execute instructions
void execute_instructions(Interpreter_state* state, Instruction* instructions, int instruction_count) {
    for (int i = 0; i < instruction_count; ++i) {
        // Operations
        if (strcmp(instructions[i].operation, "=") == 0) {
            initialize_variable(state, instructions[i].node.variable, instructions[i].node.data);
        }

        if (strcmp(instructions[i].operation, "+") == 0 ||
            strcmp(instructions[i].operation, "-") == 0 ||
            strcmp(instructions[i].operation, "*") == 0 ||
            strcmp(instructions[i].operation, "/") == 0 ||
            strcmp(instructions[i].operation, "%") == 0) {

            // Perform operation and initialize variable
            int result = perform_operation(state, instructions[i].node.variable,
                                           instructions[i].operation, instructions[i].node.data);
        }

        // Print
        if (strcmp(instructions[i].operation, "print") == 0) {
            if (instructions[i].node.variable == NULL) {
                // Print all variables
                for (int j = 0; j < state->memory_size; j++) {
                    printf("%s = %d\n", state->memory[j].variable, state->memory[j].data);
                }
            } else {
                // Print the specified variable
                int index = find_variable(state, instructions[i].node.variable);

                if (index != -1) {
                    printf("%s = %d\n", instructions[i].node.variable, state->memory[index].data);
                } else {
                    fprintf(stderr, "Error: Variable '%s' not found\n", instructions[i].node.variable);
                }
            }
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
        Instruction* new_instruction = (Instruction*)malloc(sizeof(Instruction));

        if (new_instruction == NULL) {
            return handle_file_fail;
        }

        char* token = strtok(buffer, "= \t\n");
        printf("%s\n", token);

        if (token == NULL) {
            // NULL string
            free(new_instruction);
            continue;
        }

        new_instruction->node.variable = strdup(token);

        if (new_instruction->node.variable == NULL) {
            return handle_file_no_name;
        }

        // Operation
        token = strtok(NULL, "= \t\n");
        printf("%s\n", token);

        if (token == NULL) {
            fprintf(stderr, "Error: Missing operation in instruction\n");
            free(new_instruction->node.variable);
            free(new_instruction);
            continue;
        }

        new_instruction->operation = strdup(token);

        if (new_instruction->operation == NULL) {
            return handle_file_no_operation;
        }

        // Value
        token = strtok(NULL, "= \t;\n");
        printf("%s\n", token);

        if (token != NULL) {
            new_instruction->node.data = atoi(token);
        } else {
            new_instruction->node.data = 0;
        }

        count++;

        // Resize the instructions array
        (*instructions) = (Instruction*)realloc(*instructions, count * sizeof(Instruction));

        if (*instructions == NULL) {
            return handle_file_fail;
        }

        (*instructions)[count - 1] = *new_instruction;

        free(new_instruction);
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
        free(instructions);
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
