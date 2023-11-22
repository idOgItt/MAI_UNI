#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    Trie_node** children;
    char character;
    int num_children;
    int degree;
} Trie_node;

typedef struct
{
    double koeff;
    Trie_node* variables;
    Monom* prev;
    Monom* next;
} Monom;

typedef struct{
    Monom* head;
    Monom* tail;
} Polynom;

typedef struct {
    Polynom* Polynom;
    char* operation;
} Instruction;


Trie_node* create_Trie_node(char character){
    Trie_node* new_node = (Trie_node*)malloc(sizeof(Trie_node));
    if (new_node){
        new_node->children = NULL;
        new_node->degree = -1;
        new_node->num_children = 0;
        new_node->character = character;
    }
    return new_node;
}

Trie_node* insert_variable(Trie_node* root, const char* variable, int degree) {
    Trie_node* current = root;

    for (int i = 0; variable[i] != '\0'; ++i) {
        char current_char = variable[i];
        int found = 0;

        for (int j = 0; j < current->num_children; j++) {
            if (current->children[j]->character == current_char) {
                current = current->children[j];
                found = 1;
                break;
            }
        }

        if (!found){
            Trie_node* new_node = create_Trie_node(variable[i]);
            current->num_children++;
            current->children = (Trie_node**)realloc(current->children, current->num_children * sizeof(Trie_node*));
            current->children[current->num_children - 1] = new_node;
            current = new_node;
        }
    }
    current->degree = current->degree + degree;
    return current;
}

void free_Trie(Trie_node* root){
    if(root){
        for (int i = 0; i < root->num_children; ++i){
            if (root->children[i])
                free_Trie(root->children[i]);
        }
        root->degree = -1;
        root->num_children = 0;
        free(root->children);
    }
    free(root);
}

void initialize_Polynom(Polynom* polynom) {
    polynom->head = NULL;
    polynom->tail = NULL;
}

Trie_node* copy_Trie_node(const Trie_node* root) {
    if (!root) {
        return NULL;
    }

    Trie_node* new_node = create_Trie_node(root->character);

    if (!new_node) {
        return NULL;
    }

    new_node->degree = root->degree;
    new_node->num_children = root->num_children;

    if (root->num_children > 0) {
        new_node->children = (Trie_node**)malloc(root->num_children * sizeof(Trie_node*));

        if (!new_node->children) {
            free(new_node);
            return NULL;
        }

        for (int i = 0; i < root->num_children; ++i) {
            new_node->children[i] = copy_Trie_node(root->children[i]);

            if (!new_node->children[i]) {
                free_Trie(new_node);
                return NULL;
            }
        }
    } else {
        new_node->children = NULL;
    }

    return new_node;
}

Monom create_Monom(double koeff, Trie_node* variable){
    Monom new_monom;
    new_monom.koeff = koeff;
    new_monom.variables = variable;

    return new_monom;
}

void add_Monom(Polynom* polynom, Monom* monom){
    if(!polynom->head){
        polynom->head = polynom->tail = monom;
    } else {
        polynom->tail->next = monom;
        monom->prev = polynom->tail;
        polynom->tail = monom;
        monom->next = NULL;
    }
}

void remove_last_char(char** str) {
    size_t len = strlen(*str);

    if (len > 0) {
        *str = realloc(*str, len);
    }
}

void print_Trie(Trie_node* root, Trie_node* end, char** variable){
    int found = 0;
    Trie_node* current = root;

    for (int i = 0; i < current->num_children; i++){
        if (current == end){
            (*variable) = (char*)realloc(*variable, sizeof(char) * (strlen(*variable) + 2));
            (*variable)[strlen(*variable) - 2] = current->character;
            (*variable)[strlen(*variable) - 1] = '\0';
            printf("%s", *variable);
        }
        if (current){
            (*variable) = (char*)realloc(*variable, sizeof(char) * (strlen(*variable) + 1));
            (*variable)[strlen(*variable) - 1] = current->character;
            current = current->children[i];
            print_Trie(current, end, variable);
        }
        if(current){
            remove_last_char(variable);
        }
        return;
    }
}

void print_monom(const Monom* monom, Trie_node* root){
    char* variable;
    if (monom->koeff != 1.00 || monom->koeff != 0){
        printf("%.2f", monom->koeff);
    }
    Trie_node* current = monom->variables; // char где конечная
    print_Trie(root, current, &variable);
}

void print_polynom(const Polynom* polynom){
    Monom* current = polynom->head;

    while (current){
        print_monom(current, current->variables);
        current = current->next;

        if (current) {
            printf(" * ");
        }
    }
    printf("\n");
}

void null_string(char** string, int* length){
    for (int i = 0; i < *length; i++){
            (*string)[i] = '\0';
    }
        *length = 0;
        (*string) = realloc((*string), sizeof(char));
}

void perform_operations (){
    
}

void handle_file(Instruction** instructions, Trie_node* Tries, Monom* Monoms, char* input_file, int* instruction_num){
    FILE* input_file = fopen(input_file, "r");
    
    if (input_file == NULL){
        return;
    }

    char c;
    int level = 0;
    int comment = 0;
    char* operation;
    char* variable;
    Instruction instruction;

    instruction.Polynom = malloc(sizeof(Polynom) * 2);
    instruction.Polynom[0].head = NULL;
    instruction.Polynom[1].head  = NULL;

    int polynom_num = 0;
    Trie_node root_before;
    Trie_node root_after;
    int operation_character = 0; // num for operation characters
    int brackets = 0;
    int variable_num = 0;
    int operand_level = 0;
    char* koeff;
    int koeff_num = 0;
    int degree_level = 0;
    int degree_num = 0;
    char* degree;
    int number_of_roots = 0; 
    int number_of_monoms = 0;
    int number_of_instructions = 0;

    while(c = (fgetc(input_file)) != EOF){
        if(c == '['){
            level++;
            continue;
        }
        if(c == ']'){
            level--;
            continue;
        }

        if (level == 0){
            if(comment == 0){
                if (c == '%'){
                    comment++;
                    continue;
                }
                // To parse others
                if (brackets == 0){
                    operation[operation_character] = c;
                    operation_character++;
                    if (c == '('){
                        // Check on ADD Multi and so on
                        if(!is_operation(operation)){
                            perror("Mistake in operation");
                            operation = NULL; // There is no such an operation
                        }
                        operation[operation_character] = '\0'; // Add at the end
                        strcpy(instruction.operation, operation); // copy operation
                        // NULL operation
                        null_string(&operation, operation_character);

                        brackets++;
                    }
                    // In brackets
                } else if (brackets == 1){
                    // Not an operand
                    if (operand_level == 0){
                        // Levevl if degree
                        if (degree_level == 1){
                            if (c == '_' || c == ',' || c == ')'){
                                degree_level--;
                                // Add null- terminated
                                koeff[koeff_num] = '\0';
                                variable[variable_num] = '\0';
                                degree[degree_num] = '\0';
                                // Add to BOR, create Monom and add to Polymon
                                Trie_node* variable_pointer = insert_variable(&root_before, variable, atoi(degree)); // In function created bor
                                Tries[number_of_roots] = root_before; // Save Roots Save the tries
                                Monom monom = create_Monom(convert_double(koeff), variable_pointer); // create monom
                                Monoms[number_of_monoms] = monom; // Copy monom, it refreshes;
                                instruction.Polynom = (Polynom*)realloc(instruction.Polynom, sizeof(Polynom) * (polynom_num + 1)); // increase polynom
                                add_Monom(&(instruction.Polynom[polynom_num]), &monom); // Add to polynom
                                polynom_num++;
                                //TODO Null variables, koeffs, degree
                                null_string(&variable, variable_num);
                                null_string(&koeff, koeff_num);
                                null_string(&degree, degree_num);

                                if (c == ','){
                                    root_before = root_after;
                                }

                                if (c == ')'){
                                    brackets--;
                                    //Renew In instruction, new_roots
                                    (*instructions)[number_of_instructions] = instruction; // Copied instuction
                                    number_of_instructions++;
                                    *instruction_num = number_of_instructions;
                                    Instruction instruction_new;
                                    instruction = instruction_new;
                                    Trie_node  root_before_new, root_after_new;
                                    root_before = root_before_new;
                                    root_after = root_after_new;
                                }
                                
                            } else {
                                if (isdigit(c)){
                                    degree[degree_num] = c;
                                    degree_num++;
                                } else {
                                    perror("Wrong degree");
                                }
                            }
                        } else if (degree_level == 0){
                            if (is_operand(c)){ // Function to check letters
                                // koefficient is in koef
                                operand_level++;
                            } else if (isdigit(c) || '.'){
                                koeff[koeff_num] = c;
                                koeff_num++;
                            } else {
                                perror("Wrong in coefficient");
                            }
                        }
                        // If in operand
                    } else if (operand_level == 1){
                        if (c == '^'){
                            operand_level--;
                        } else {
                            variable[variable_num] = c;
                            variable_num++;
                        }
                    }
                } else {
                    perror("Something wrong with brackets");
                }
                // Till here
            }
            if (comment == 1){
                if (c == '\n'){
                    comment--;
                }
            }
        }
    }
}