#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum handle_file_status_code {handle_file_ok, handle_file_fails, handle_file_error, handle_file_unknown_operation};

enum execute_instructions_status_code {execute_instructions_ok, execute_instructions_fail, execute_instructions_no_variable, execute_instructions_no_operation};

enum isdigit_string_status_code {isdigit_string_no, isdigit_string_yes};

enum perform_operation_status_code {perform_operation_ok, perform_operation_fail, perform_operation_zero_division};

enum add_to_base_cell_status_code {add_to_base_cell_ok, add_to_base_cell_fail};

typedef struct {
    char* variable;
    int data;
} MemoryCell;

typedef struct {
    MemoryCell* node;
    char* operation;
} Instruction;

typedef struct {
    MemoryCell* memmory;
    int memmory_size;
} BaseCells;

int compare_memmory_cells(const void* a, const void *b){
    return strcmp(((MemoryCell*)a)->variable, ((MemoryCell*)b)->variable);
}

int is_operatorion(char c) {
    switch (c) {
        default:
            return 0;
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            return 1;
    }
}

enum add_to_base_cell_status_code add_to_base_cell(BaseCells* base_cell, MemoryCell node){
    
    if (base_cell == NULL){
        return add_to_base_cell_fail;
    }

    int size = base_cell->memmory_size;

    MemoryCell* tmp = (MemoryCell*)realloc(base_cell->memmory, sizeof(MemoryCell) * (size + 1));
    
    if (tmp == NULL){
        return add_to_base_cell_fail;
    }

    base_cell->memmory = tmp;

    base_cell->memmory[base_cell->memmory_size].variable = (char*)malloc(sizeof(char) * sizeof(node.variable));

    if (base_cell->memmory[base_cell->memmory_size].variable == NULL){
        free(tmp);
        return add_to_base_cell_fail;
    }

    strcpy(base_cell->memmory[base_cell->memmory_size].variable, node.variable);
    base_cell->memmory[base_cell->memmory_size].data = node.data;
    base_cell->memmory_size++;
    qsort(base_cell->memmory, base_cell->memmory_size, sizeof(MemoryCell), compare_memmory_cells);
    return add_to_base_cell_ok;
}

enum perform_operation_status_code perform_operation(int number_1, int number_2, char operation, int* result){

    if (operation == '+'){
        *result = number_1 + number_2;
    } else if (operation == '-'){
        *result = number_1 - number_2;
    } else if (operation == '*'){
        *result = number_1 * number_2;
    } else if (operation == '/'){
        if (number_2 == 0){
            return perform_operation_zero_division;
        }
        *result = number_1 / number_2;
    } else if (operation == '%'){
        if (number_2 == 0){
            return perform_operation_zero_division;
        }
        *result = number_1 % number_2;
    } else {
        return perform_operation_fail;
    }
    
    return perform_operation_ok;
}

enum isdigit_string_status_code isdigit_string(const char* string, int* number) {
    int digit;
    int num = 0;
    for (int i = 0; i < strlen(string); i++) {
        if (isdigit(string[i])) {
            digit = string[i] - '0';
            num = num * 10 + digit;
        } else {
            return isdigit_string_no;
        }
    }
    *number = num;
    return isdigit_string_yes;
}

// Function to perfrom dihitomia search
int find_variable(BaseCells* state, const char* variable_name) {
    int low = 0; int high = state->memmory_size - 1, mid, cmp;

    if(state->memmory_size < 0){
        return -1;
    }

    while (low <= high) {
        mid = (low + high) / 2;
        cmp = strcmp(state->memmory[mid].variable, variable_name);

        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}

enum execute_instructions_status_code execute_instructions (BaseCells* base_cell, const Instruction* instructions, int instructions_num){

    if (base_cell == NULL || instructions == NULL || instructions_num == 0){
        return execute_instructions_fail;
    }

    for (int instruction_index = 0; instruction_index < instructions_num; instruction_index++){
        Instruction instruction = instructions[instruction_index];
        
        if (strcmp(instruction.operation, "print") == 0){
            
            if (instruction.node != NULL){
                int index = find_variable(base_cell, instruction.node[0].variable);
                // Found variable
                if (index != -1){
                    printf("%s = %d\n", instruction.node[0].variable, base_cell->memmory[index].data);
                } else {
                // Not found
                printf("There is no such variable: %s\n", instruction.node[0].variable);
                return execute_instructions_no_variable;
                }

            } else if (instruction.node == NULL){
                for (int i = 0; i < base_cell->memmory_size; i++){
                    printf("%s = %d\n", base_cell->memmory[i].variable, base_cell->memmory[i].data);
                }
            }

        } else {
            // Some operation
            int number_1, number_2, number, number_1_index, number_2_index;
            int flag = 0;
            if (is_operatorion(instruction.operation[1])){
                // No digit number_2
                if (isdigit_string(instruction.node[2].variable, &number_2) == isdigit_string_no){
                    number_2_index = find_variable(base_cell, instruction.node[2].variable);
                    if (number_2_index == -1){
                        printf("There is no such variable: %s\n", instruction.node[2].variable);
                        return execute_instructions_fail;
                    }
                    number_2 = (*base_cell).memmory[number_2_index].data;
                }
                // No digit number_1
                if (isdigit_string(instruction.node[1].variable, &number_1) == isdigit_string_no){
                    number_1_index = find_variable(base_cell, instruction.node[1].variable);
                    if (number_1_index == -1){
                        printf("There is no such variable: %s\n", instruction.node[1].variable);
                        return execute_instructions_fail;
                    }
                    number_1 = (*base_cell).memmory[number_1_index].data;
                }
                // Perform operation
                enum perform_operation_status_code operation_result = perform_operation(number_1, number_2, instruction.operation[1], &number);
                if (operation_result == perform_operation_fail){
                    return execute_instructions_fail;
                } else if (operation_result == perform_operation_zero_division){
                    return execute_instructions_fail;
                }
                flag = 1;
            }
            // Initialise
            if (instruction.operation[0] == '='){
                int variable_index;
                int operand_index;
                MemoryCell variable_node;
                // No variable to create
                if ((variable_index = find_variable(base_cell, instruction.node[0].variable)) == -1){
                    variable_node.variable = (char*)malloc(sizeof(char) * sizeof(instruction.node[0].variable));

                    if (variable_node.variable == NULL){
                        return execute_instructions_fail;
                    }

                    strcpy(variable_node.variable, instruction.node[0].variable);
                    if (flag){
                        variable_node.data = number;
                    // it is a number
                    } else if (isdigit_string(instruction.node[1].variable, &number) == isdigit_string_yes){
                        variable_node.data = number;
                    // it is a variable
                    } else {

                        if ((operand_index = find_variable(base_cell, instruction.node[1].variable)) == -1){
                           printf("There is no such variable: %s\n", instruction.node[1].variable);
                           return execute_instructions_no_variable;
                        }
                        variable_node.data = base_cell->memmory[operand_index].data;
                    }

                    if (add_to_base_cell(base_cell, variable_node) != add_to_base_cell_ok){
                        free(variable_node.variable);
                        return execute_instructions_fail;
                    }

                    free(variable_node.variable);
                } else {
                    if (flag){
                        base_cell->memmory[variable_index].data = number;
                    // Number
                    } else if (isdigit_string(instruction.node[1].variable, &number) == isdigit_string_yes){
                        base_cell->memmory[variable_index].data = number;
                    // It is a variable
                    } else {
                        if ((operand_index = find_variable(base_cell, instruction.node[1].variable)) == -1){
                           printf("There is no such variable: %s\n", instruction.node[1].variable);
                           return execute_instructions_no_variable;
                        }
                        base_cell->memmory[variable_index].data = base_cell->memmory[operand_index].data;
                    }
                }
            } else {
                return execute_instructions_no_operation;
            }
        }
    }

    return execute_instructions_ok;
}

enum handle_file_status_code handle_file(char* input_file_path, Instruction** instructions, int* instructions_num){
    
    if (input_file_path == NULL || instructions == NULL || instructions_num == NULL){
        return handle_file_fails;
    }
    
    FILE* input_file = fopen(input_file_path, "r");

    if (input_file == NULL){
        return handle_file_error;
    }

    char buffer[256];
    int count = 0;

    while(fgets(buffer, sizeof(buffer), input_file)){
        
        Instruction new_instruction;
        int num_operation = 0;
        int num_variable = 0;
        char* variable = (char*)malloc(sizeof(char) * 50);

        if (variable == NULL){
            fclose(input_file);
            return handle_file_error;
        }

        char* operand_f = (char*)malloc(sizeof(char) * 50);
        
        if (operand_f == NULL){
            free(variable);
            fclose(input_file);
            return handle_file_error;
        }

        char* operand_s = (char*)malloc(sizeof(char) * 50);

        if (operand_s == NULL){
            free(variable);
            free(operand_f);
            fclose(input_file);
            return handle_file_error;
        }

        char* print_operation = (char*)malloc(sizeof(char) * 5);
        
        if (print_operation == NULL){
            free(variable);
            free(operand_f);
            free(operand_s);
            fclose(input_file);
            return handle_file_error;
        }

        char operation;

        if(sscanf(buffer, "%s = %s %c %[^;];", variable, operand_f, &operation, operand_s) == 4){
            // var = var + var; var = var;
            // var = var + var;

                variable = (char*)realloc(variable, sizeof(char) * (strlen(variable) + 1));
                
                if (variable == NULL){
                    free(operand_f); free(operand_s);
                    fclose(input_file);
                    return handle_file_error;
                }

                operand_f = (char*)realloc(operand_f, sizeof(char) * (strlen(operand_f) + 1));
                
                if (operand_f == NULL){
                    free(variable); free(operand_s);
                    fclose(input_file);
                    return handle_file_error;
                }

                operand_s = (char*)realloc(operand_s, sizeof(char) * (strlen(operand_s) + 1));

                if (operand_s == NULL){
                    free(operand_f); free(variable);
                    fclose(input_file);
                    return handle_file_error;
                }

                // operation is true
               if (is_operatorion(operation)){
                new_instruction.operation = (char*)malloc(sizeof(char) * 2);
                
                if (new_instruction.operation == NULL){
                    free(variable); free(operand_f); free(operand_s);
                    fclose(input_file);
                    return handle_file_error;
                }

                new_instruction.operation[num_operation] = '=';
                num_operation++;
                new_instruction.operation[num_operation] = operation;
                num_operation++;

                MemoryCell variable_node, operand_node_f, operand_node_s;
                
                variable_node.variable = (char*)malloc(sizeof(variable) * sizeof(char));

                if (variable_node.variable == NULL){
                    free(variable); free(operand_f); free(operand_s);
                    free(new_instruction.operation);
                    fclose(input_file);
                    return handle_file_error;
                }

                operand_node_f.variable = (char*)malloc(sizeof(operand_f) * sizeof(char));

                if (operand_node_f.variable == NULL){
                    free(variable); free(operand_f); free(operand_s);
                    free(new_instruction.operation);
                    free(variable_node.variable);
                    fclose(input_file);
                    return handle_file_error;
                }

                operand_node_s.variable = (char*)malloc(sizeof(operand_s) * sizeof(char));

                if (operand_node_s.variable == NULL){
                    free(variable); free(operand_f); free(operand_s);
                    free(new_instruction.operation);
                    free(operand_node_f.variable);
                    free(variable_node.variable);
                    fclose(input_file);
                    return handle_file_error;
                }

                strcpy(variable_node.variable, variable);
                strcpy(operand_node_f.variable, operand_f);
                strcpy(operand_node_s.variable, operand_s);
                
                new_instruction.node = (MemoryCell*)malloc(sizeof(MemoryCell) * 3);
                
                if (new_instruction.node == NULL){
                    free(variable); free(operand_f); free(operand_s);
                    free(new_instruction.operation);
                    free(operand_node_f.variable);
                    free(operand_node_s.variable);
                    free(variable_node.variable);
                    fclose(input_file);
                    return handle_file_error;
                }

                new_instruction.node[num_variable] = variable_node;
                num_variable++;
                new_instruction.node[num_variable] = operand_node_f;
                num_variable++;
                new_instruction.node[num_variable] = operand_node_s;
                num_variable++;

                free(variable); free(operand_f); free(operand_s);

               } else {
                return handle_file_unknown_operation;
               }

        // var = var
        } else if (sscanf(buffer, "%s = %[^;];", variable, operand_f) == 2) {

                variable = (char*)realloc(variable, sizeof(char) * (strlen(variable) + 1));
                operand_f = (char*)realloc(operand_f, sizeof(char) * (strlen(operand_f) + 1));

                MemoryCell variable_node, operand_f_node;

                new_instruction.operation = (char*)malloc(sizeof(char));

                if (new_instruction.operation == NULL){
                    free(variable); free(operand_f);
                    fclose(input_file);
                    return handle_file_error;
                }

                new_instruction.operation[num_operation] = '=';
                num_operation++;

                variable_node.variable = (char*)malloc((strlen(variable) + 1) * sizeof(char));
                
                if (variable_node.variable == NULL){
                    free(variable); free(operand_f);
                    free(new_instruction.operation);
                    fclose(input_file);
                    return handle_file_error;
                }

                strcpy(variable_node.variable, variable); 

                operand_f_node.variable = (char*)malloc((strlen(operand_f) + 1) * sizeof(char));

                if (variable_node.variable == NULL){
                    free(variable); free(operand_f);
                    free(new_instruction.operation);
                    free(variable_node.variable);
                    fclose(input_file);
                    return handle_file_error;
                }

                strcpy(operand_f_node.variable, operand_f); 

                new_instruction.node = (MemoryCell*)malloc(sizeof(MemoryCell) * 2);

                if (new_instruction.node == NULL){
                    free(variable); free(operand_f);
                    free(new_instruction.operation);
                    free(operand_f_node.variable);
                    free(variable_node.variable);
                    fclose(input_file);
                    return handle_file_error;
                }

                new_instruction.node[num_variable] = variable_node;
                num_variable++;
                new_instruction.node[num_variable] = operand_f_node;
                num_variable++;

                free(variable); free(operand_f);
        // print var; print;
        } else if (sscanf(buffer, "%s %[^;];", print_operation, variable) == 2){
            // print var;
            variable = (char*)realloc(variable, sizeof(char) * (strlen(variable) + 1));

            if(strcmp(print_operation, "print") == 0){

                new_instruction.operation = (char*)malloc(sizeof(char) * sizeof(print_operation));

                if (new_instruction.operation == NULL){
                    free(print_operation); free(variable);
                    fclose(input_file);
                    return handle_file_error;
                }

                strcpy(new_instruction.operation, print_operation);
                num_operation++;

                MemoryCell variable_node;

                new_instruction.node = (MemoryCell*)malloc(sizeof(MemoryCell));

                if (new_instruction.node == NULL){
                    free(print_operation); free(variable);
                    free(new_instruction.operation);
                    fclose(input_file);
                    return handle_file_error;
                }

                variable_node.variable = (char*)malloc(sizeof(char) * sizeof(variable));

                if (new_instruction.node->variable == NULL){
                    free(print_operation); free(variable);
                    free(new_instruction.operation);
                    free(new_instruction.node);
                    fclose(input_file);
                    return handle_file_error;
                }

                strcpy(variable_node.variable, variable);

                new_instruction.node[num_variable] = variable_node;
                num_variable++;

                free(print_operation); free(variable);

            } else {
                free (print_operation); free(variable);
                fclose(input_file);
                return handle_file_error;
            }

        //print all
        } else if (sscanf(buffer, "%[^;];", print_operation) == 1){
            if (strcmp(print_operation, "print") == 0){
                new_instruction.operation = (char*)malloc(sizeof(print_operation) * sizeof(char));

                if (new_instruction.operation == NULL){
                    free(print_operation);
                    fclose(input_file);
                    return handle_file_error;
                }

                strcpy(new_instruction.operation, print_operation);
                num_operation++;

                new_instruction.node = NULL;

            } else {
                free(print_operation);
                fclose(input_file);
                return handle_file_error;
            }
        } else {
            fclose(input_file);
            return handle_file_fails;
        }

        if (count % 4 == 0){
            if (*instructions == NULL) {
                *instructions = (Instruction*)malloc(sizeof(Instruction) * (count + 5));
            } else {
                Instruction* tmp = (Instruction*)realloc(*instructions, sizeof(Instruction) * (count + 5));
                if (tmp == NULL) {
                    fclose(input_file);
                    return handle_file_error;
                }
                *instructions = tmp;
            }
        }

        (*instructions)[count] = new_instruction;
        count++;
    }

    (*instructions) = (Instruction*)realloc((*instructions), sizeof(Instruction) * count);

    if ((*instructions) == NULL){
        fclose(input_file);
        return handle_file_error;
    }

    *instructions_num = count;

    fclose(input_file);

    return handle_file_ok;
}

int main (int argc, char* argv[]){
    if (argc != 2){
        printf("Usage: %s <input_file>\n", argv[0]);
        return -1;
    }

    char* input_file_path = argv[1];
    int instructions_num;
    Instruction* instructions = NULL;
    BaseCells bace_cell;
    bace_cell.memmory_size = 0;
    bace_cell.memmory = NULL;

    if (handle_file(input_file_path, &instructions, &instructions_num) == handle_file_ok){
        if(execute_instructions(&bace_cell, instructions, instructions_num) == execute_instructions_ok){
            printf("Everything went fine\n");
        } else {
            printf("Something went wrong\n");
            return -1;
        }
    }

    return 0;
}