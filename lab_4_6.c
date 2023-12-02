#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    SUCCESS = 0,
    INCORRECT_INPUT,
    OVERFLOW_ERROR,
    MALLOC_ERROR,
    FILE_OPENING_ERROR,
    FILE_READING_ERROR,
    OPEN_BRACE_MISSING,
    CLOSE_BRACE_MISSING,
    UNKNOWN_SYMBOL,
    DIVISION_BY_ZERO,
    UNKNOWN_ERROR,
} ErrorCode;

static const char* errorMessages[] = {
    "Everything went fine",
    "Invalid input",
    "Stack overflow",
    "Sygmentation fault",
    "File invalid",
    "File reading error",
    "Missing '('",
    "Missing ')'",
    "Invalid char",
    "Devision zero",
    "Something went wrong"
};

#define BUFFER_SIZE 100
#define ALP_SIZE 26

typedef struct S_Node {
    int data;
    struct S_Node* next;
} S_Node;

typedef struct Stack {
    S_Node* top;
} Stack;

S_Node* create_S_Node(const int data) {
    S_Node* new_Node = (S_Node*)malloc(sizeof(S_Node));
    new_Node->data = data;
    new_Node->next = NULL;
    return new_Node;
}

void init_Stack(Stack* stack) {
    stack->top = NULL;
}

int is_Empty(Stack* stack) {
    return (stack->top == NULL);
}

void push(Stack* stack, const int data) {
    S_Node* new_Node = create_S_Node(data);
    new_Node->next = stack->top;
    stack->top = new_Node;
}

int pop(Stack* stack) {
    if (is_Empty(stack)) {
        printf("Стек пуст 🪣\n");
        return -1;
    }
    int popped_Data = stack->top->data;
    S_Node* temp = stack->top;
    stack->top = stack->top->next;
    free(temp);
    return popped_Data;
}

int peek(Stack* stack) {
    if (is_Empty(stack)) {
        printf("Стек пуст 🪣\n");
        return -1;
    }
    return stack->top->data;
}

void delete_Stack(Stack* stack) {
    while (stack->top != NULL) {
        S_Node* temp = stack->top;
        stack->top = stack->top->next;
        free(temp);
    }
}

int is_operator(const char chr) {
    switch (chr) {
        default:
            return 0;
        case '&':
        case '|':
        case '~':
        case '-': // ->
        case '+': // +>
        case '<': // <>
        case '=':
        case '!':
        case '?':
            return 1;
    }
}

int op_Prior(const char chr) {
    switch(chr) {
        default:
            return -1;
        case '|':
        case '-':
        case '<':
        case '=':
            return 1;
        case '?':
        case '!':
        case '+':
        case '&':
            return 2;
        case '~':
            return 3;
    }
}

ErrorCode check_Output_Overflow(const int i, char output[BUFFER_SIZE]) {
    if (i >= BUFFER_SIZE-1) {
        output[BUFFER_SIZE-1] = '\0';
        return OVERFLOW_ERROR;
    }
    return SUCCESS;
}

ErrorCode shunting_Yard(const char input[], int inpLen, char output[BUFFER_SIZE], int* var_Cnt, int var_Order[ALP_SIZE]) {
    if (var_Cnt == NULL) 
        return INCORRECT_INPUT;
    Stack stack;
    init_Stack(&stack);
    memset(output, '\0', BUFFER_SIZE);
    *var_Cnt = 0;
    memset(var_Order, 0, ALP_SIZE * sizeof(int));
    int j = 0;
    

    for (int i = 0; i < inpLen; ++i) {
        if (check_Output_Overflow(j, output) != SUCCESS) {
            delete_Stack(&stack);
            return OVERFLOW_ERROR;
        }
        char chr = input[i];

        if (isdigit(chr) || isalpha(chr)) 
        {
            if (isdigit(input[i+1]) || isalpha(input[i+1])) {
                delete_Stack(&stack);
                return INCORRECT_INPUT;
            }
            chr = toupper(chr);
            if (isalpha(chr) && !var_Order[chr-'A']) {
                var_Order[chr-'A'] = *var_Cnt + 1;
                chr = 'A' + (*var_Cnt)++;
                // ++*varCnt;
            }
            output[j++] = chr;
            output[j++] = ' ';
            continue;
        } 
        else if (is_operator(chr)) 
        {
            if (chr == '~') {
                output[j++] = '_';
                output[j++] = ' ';
            }
            char opr1 = chr;
            char opr2 = ' ';
            if (!is_Empty(&stack))
                opr2 = peek(&stack);
            
            // if (opr1 != '^') все операции левоассоциативны
            {   // левоассоциативность
                while (!is_Empty(&stack) && op_Prior(opr1) <= op_Prior(opr2) && is_operator(opr2)) 
                {
                    opr2 = pop(&stack);
                    output[j++] = opr2;
                    output[j++] = ' ';
                    if (check_Output_Overflow(j, output) != SUCCESS) {
                        delete_Stack(&stack);
                        return OVERFLOW_ERROR;
                    }
                    if (!is_Empty(&stack)) {
                        opr2 = peek(&stack);
                    } else {
                        break;
                    }
                }
                push(&stack, opr1);
            }
            // else 
            // {   // правоассоциативность
            //     while (!isEmpty(&stack) &&opPr(opr1) < opPr(opr2) && isoperator(opr2))
            //     {
            //         opr2 = pop(&stack);
            //         output[j++] = opr2;
            //         output[j++] = ' ';
            //         if (checkOutputOverflow(j, output) != SUCCESS) {
            //             deleteStack(&stack);
            //             return OVERFLOW_ERROR;
            //         }
            //         if (!isEmpty(&stack)) {
            //             opr2 = peek(&stack);
            //         } else {
            //             break;
            //         }
            //     }
            //     push(&stack, opr1);
            // }
        }
        else if (chr == '(')
        {
            push(&stack, '(');
        }
        else if (chr == ')')
        {
            while (!is_Empty(&stack) && peek(&stack) != '(') {
                output[j++] = pop(&stack);
                output[j++] = ' ';
            }
            if (is_Empty(&stack)) {
                delete_Stack(&stack);
                return OPEN_BRACE_MISSING;
            } else {
                pop(&stack);
            }
        }
        else if (!isspace(chr) && chr != '\0' && chr != '\n')
        {
            delete_Stack(&stack);
            return UNKNOWN_SYMBOL;
        }
    }
    while (!is_Empty(&stack))
    {
        if (check_Output_Overflow(j, output) != SUCCESS) {
            delete_Stack(&stack);
            return OVERFLOW_ERROR;
        }
        if (peek(&stack) == '(') {
            delete_Stack(&stack);
            return CLOSE_BRACE_MISSING;
        }

        output[j++] = pop(&stack);
        output[j++] = ' ';
    }
    return SUCCESS;
}

struct Node {
    char data;
    int isNumber;
    struct Node* left;
    struct Node* right;
};

struct Node* create_Node(int data, int isNumber) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL)
        return NULL;
    newNode->data = data;
    newNode->isNumber = isNumber;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void destroy_Tree(struct Node* root) {
    if (root == NULL)
        return;
    destroy_Tree(root->left);
    destroy_Tree(root->right);
    free(root);
}

// ЛКП - по возрастанию
void inorder_Traversal(struct Node* root) {
    if (root == NULL)
        return;
    inorder_Traversal(root->left);
    printf("%c ", root->data);
    inorder_Traversal(root->right);
}

ErrorCode at_Exit(ErrorCode code, struct Node* root) {
    destroy_Tree(root);
    return code;
}

ErrorCode convert_To_Expression_Tree(char* expression, struct Node** result) {
    struct Node* stack[BUFFER_SIZE];
    stack[0] = NULL;
    int top = -1;
    char* token = strtok(expression, " ");
    while (token != NULL) {
        if (isdigit(token[0]) || isalpha(token[0]) || token[0] == '_') {
            struct Node* newNode = create_Node(token[0], 1);
            if (newNode == NULL)
                return at_Exit(MALLOC_ERROR, stack[0]);
            
            stack[++top] = newNode;
        } else if (is_operator(token[0])) {
            if (top < 1) {
                return at_Exit(INCORRECT_INPUT, stack[0]);;
            }
            struct Node* newNode = create_Node(token[0], 0);
            if (newNode == NULL)
                return at_Exit(MALLOC_ERROR, stack[0]);

            newNode->right = stack[top--];
            newNode->left = stack[top--];
            stack[++top] = newNode;
        } else {
            return at_Exit(UNKNOWN_SYMBOL, stack[0]);
        }
        token = strtok(NULL, " ");
    }
    if (top != 0) {
        return at_Exit(INCORRECT_INPUT, stack[0]);
    }
    *result = stack[0];
    return SUCCESS;
}

int evaluate_Node(const struct Node* node, const int input[ALP_SIZE]) {
    if (node == NULL) return -1;
    if (node->isNumber && isdigit(node->data)) {
        return node->data - '0';
    }
    else if (node->data == '_') {
        return -1;
    }
    else if (node->isNumber) {
        return input[node->data - 'A'];
    } 
    else {
        int left = evaluate_Node(node->left, input);
        int right = evaluate_Node(node->right, input);
        switch (node->data) {
            // ru.cppreference.com/w/cpp/language/operator_precedence
            // ~0 не эквивалентно !0
            // & <=> &&
            // | <=> ||
            case '&':
                return left & right;
            case '|':
                return left | right;
            case '~':
                return !right; // '_' всегда справа
            case '-': // ->
                return (!left) | right;
            case '+': // +>
                // коимпликация (инверсия обратной импликации)
                return !( left | (!right) );
            case '<': // <>
                return left ^ right;
            case '=':
                return (left & right) | ((!left) & (!right));
            case '!':
                return !(left & right);
            case '?':
                return !(left | right);
            default:
                break;
        }
    }
    return -1;
}

ErrorCode print_Truth_Table(const struct Node* root, const int numVariables, FILE* file) {
    // numVariables: A -> Z
    if (numVariables < 1 || numVariables > ALP_SIZE)
        return INCORRECT_INPUT;

    int input[ALP_SIZE];

    for (int i = 0; i < (1 << numVariables); ++i) {
        for (int j = 0; j < numVariables; ++j) {
            // ABCD...Z <-> [1011...1]
            input[j] = (i >> (numVariables-1 - j)) & 1;
            fprintf(file, "%d ", input[j]);
        }
        fprintf(file, "=> %d\n", evaluate_Node(root, input));
    }
    fflush(file);
    return SUCCESS;
}

ErrorCode compress_Operators(char string[], size_t strLen) {
    for (size_t i = 0; i < strLen-1; ++i) {
        if (string[i] == '<' && string[i+1] != '>') {
            return INCORRECT_INPUT;
        } else if (string[i] == '-' && string[i+1] != '>') {
            return INCORRECT_INPUT;
        } else if (string[i] == '+' && string[i+1] != '>') {
            return INCORRECT_INPUT;
        } else if (string[i+1] == '>') {
            string[i+1] = ' ';
        }
    }
    return SUCCESS;
}

char generate_Random_Char() {
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    return charset[rand() % (sizeof(charset)-1)];
}

void generate_Random_File_Name(char *fileName, size_t nameLen) {
    for (size_t i = 0; i < nameLen-5; ++i) {
        fileName[i] = generate_Random_Char();
    }
    char ending[] = ".txt";
    for (size_t i = nameLen-5; i < nameLen; ++i) {
        fileName[i] = ending[i-(nameLen-5)];
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("%s <file>\n", argv[0]);
        return INCORRECT_INPUT;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
        fflush(stdout);
        return FILE_OPENING_ERROR;
    }

    char line[BUFFER_SIZE];
    size_t lineLen = sizeof(line);
    memset(line, '\0', lineLen);
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }
    fclose(file);

    printf("'%s'\n", line);
    fflush(stdout);

    if (compress_Operators(line, lineLen) != SUCCESS) {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }
    
    char expression[BUFFER_SIZE];
    int varOrder[ALP_SIZE];
    int varCnt;
    ErrorCode code = shunting_Yard(line, sizeof(line), expression, &varCnt, varOrder);
    if (code != SUCCESS) {
        printf("%s\n", errorMessages[code]);
        fflush(stdout);
        return code;
    }

    printf("'%s'\n", expression);
    fflush(stdout);
    struct Node* root;
    code = convert_To_Expression_Tree(expression, &root);
    if (code != SUCCESS) {
        printf("%s\n", errorMessages[code]);
        fflush(stdout);
        return code;
    }

    char fileName[10];
    srand(time(NULL));
    generate_Random_File_Name(fileName, sizeof(fileName));
    printf("'%s'\n", fileName);
    fflush(stdout);

    FILE *out = fopen(fileName, "w");
    if (out == NULL) {
        printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
        fflush(stdout);
        destroy_Tree(root);
        return FILE_OPENING_ERROR;
    }

    for (int i = 1; i <= ALP_SIZE; ++i) { // вывести букву порядка i
        for (int j = 0; j < ALP_SIZE; ++j) { // найти букву порядка i
            if (varOrder[j] == i) {
                fprintf(file, "%c ", 'A' + j);
                break;
            }
        }
    }
    fprintf(file, "=> Значение\n");
    code = print_Truth_Table(root, varCnt, out);
    if (code != SUCCESS) {
        printf("%s\n", errorMessages[code]);
        fflush(stdout);
        destroy_Tree(root);
        fclose(out);
        return code;
    }
    fclose(out);

    destroy_Tree(root);
    return 0; 
}