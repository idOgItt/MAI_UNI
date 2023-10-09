/*
two argumments, argv[1] - argv[2];
argv[1] -> path _ inputfile
argv[2] -> path _ outputfile

file {
    23124adasAVS \t  dkasdwd, \n 21321 0000000001\
}

function min_base [2..36];
fprintf() - построчно выводит - число без ведущих нулей; min_base; base_10,

a = A;
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cstring>

enum file_min_base_status_code { file_min_base_ok, file_min_base_fail };

enum char_to_value_status_code { char_to_value_ok, char_to_value_failure};

enum convert_to_decimal_status_code {decimal_failure};

typedef struct {
    char* data;
    unsigned int size;
    unsigned int capacity;
} vector;

enum vector_status_code { vector_created_failure, realloc_failure, vector_ok, realloc_ok };

enum vector_status_code create_vector(vector* v, unsigned int sz) {
    v->size = sz;
    v->data = (char*)malloc(sizeof(char) * v->size);
    if (v->data == NULL) {
        return vector_created_failure;
    } else {
        v->capacity = 0;
        return vector_ok;
    }
}

enum vector_status_code push_back(vector* v, unsigned int num) {
    if (v->size == v->capacity) {
        v->size++;
        char* temp = (char*)malloc(sizeof(char) * v->size);
        if (temp == NULL) {
            return realloc_failure;
        }
        for (unsigned int i = 0; i < v->capacity; i++) {
            temp[i] = v->data[i];
        }
        free(v->data);
        v->data = temp;
    }

    v->data[v->capacity] = num;
    v->capacity++;
    return realloc_ok;
}

void free_vector(vector* v) {
    free(v->data);
    v->size = 0;
    v->capacity = 0;
    v->data = NULL;
}

int char_to_value(char c) {
    if (isdigit(c)) {
        return c - '0';
    } else if (isupper(c)) {
        return c - 'A' + 10;
    } else {
        // Обработка ошибки: символ не принадлежит системе счисления с базой.
        return char_to_value_failure;
    }
}


void min_base(int* max){
    return;
}

long long convert_to_decimal(const char* number, int max) {
    int base = max;
    long long decimal_value = 0;
    int length = strlen(number);

    for (int i = 0; i < length; i++) {
        int digit_value = char_to_value(number[i]);
        if (digit_value == -1) {
            return decimal_failure;
        }
        decimal_value = decimal_value * base + digit_value;
    }

    return decimal_value;
}

enum file_min_base_status_code file_min_base(const char* input_file_path, const char* output_file_path) {
    FILE* input_file = fopen(input_file_path, "r");
    FILE* output_file = fopen(output_file_path, "w");

    if (input_file == NULL || output_file == NULL) {
        return file_min_base_fail;
    }

    char ch;
    int max = 0;
    int start = 1;
    int in_word = 0;
    vector massive;
    enum vector_status_code status_code = create_vector(&massive, 100);

    while ((ch = fgetc(input_file)) != EOF) {
        if (isalnum(ch)) { // Check if the character is alphanumeric (letters or digits).
            if (start) {
                if (ch != '0') {
                    start = 0;
                }
            } else {
                if (isdigit(ch) || isupper(ch)) {
                    if ((int)ch > max) {
                        max = (int)ch;
                    }
                    fputc(ch, output_file);
                    push_back(&massive, ch);
                    in_word = 1;
                } else if (in_word) {
                    in_word = 0;
                    fputc(' ', output_file);
                    max = char_to_value((char)max);
                    fprintf(output_file, "%d", max + 1);
                    fputc(' ', output_file);
                    long long number = convert_to_decimal(massive.data, max);
                    fprintf(output_file, "%d", number);
                    fputc('\n', output_file);
                    free_vector(&massive);
                    max = 0;
                    start = 1;
                }
            }
        }
    }

    fclose(input_file);
    fclose(output_file);
    free_vector(&massive);

    return file_min_base_ok;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Parameters for the program: <path_input_file> <path_output_file>\n");
        return 1;
    }

    const char* input_file_path = argv[1];
    const char* output_file_path = argv[2];

    enum file_min_base_status_code status = file_min_base(input_file_path, output_file_path);

    if (status == file_min_base_fail) {
        printf("Error processing files.\n");
        return 1;
    }

    printf("File processing completed successfully.\n");

    return 0;
}
