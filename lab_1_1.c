#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
    unsigned int* data;
    unsigned int size;
    unsigned int capacity;
} vector;

enum multiple_status {multiple_ok, multiple_ivalid, multiple_abscent};

enum degree_table_status {degree_table_ok, degree_table_overflow, degree_table_fail};

enum factorial_status_code {fsk_ok, fsk_overflow, fsk_invalid_parameter};

void create_vector(vector* v, unsigned int sz){
    v->size = sz;
    v->data = (unsigned int*)malloc(sizeof(unsigned int) * v->size);
    v->capacity = 0;
}

void push_back(vector* v, unsigned int num){
    if (v->size == v->capacity){
        v->size++;
        v->data = (unsigned int*)realloc(v->data, v->size * sizeof(unsigned int));
    }
    
    v->data[v->capacity] = num;
    v->capacity++;
}

void print_vector(const vector* v) {
    for (int i = 0; i < v->capacity; i++) {
        printf("%d ", v->data[i]);
    }
    printf("\n");
}

void free_vector(vector* v) {
    free(v->data);
    v->size = 0;
    v->capacity = 0;
    v->data = NULL;
}

enum multiple_status multiple(unsigned int number, vector* multiple_nums);

int prime_number(unsigned int number);

int digits_number(unsigned int number, vector* digits);

enum degree_table_status degree_table(unsigned int number, vector** table);

int sum_number(unsigned int number, unsigned long* result);

enum factorial_status_code factorial(unsigned int value, unsigned long* result);

int main( int argc, char *argv[]) {
    if (argc != 3){
        printf("Программа:%s <число> <флаг>\n", argv[0]);
        return 1;
    }

    unsigned int number = atoi(argv[1]);
    char* flag = argv[2];

    if (flag[0] != '-' && flag[0] != '/'){
        printf("Неправильный флаг: %c\n", flag[0]);
        return 1;
    }
    
    switch (flag[1]) {

        case 'h':
            vector multiple_nums;
            create_vector(&multiple_nums, 0);

            switch(multiple(number, &multiple_nums)){

                case multiple_ok:
                    printf("Числа кратные указанному числу %lu: ", number);
                    print_vector(&multiple_nums);
                    break;
                
                case multiple_abscent:
                    printf("Для указанного числа нет %lu, нет кратного в пределах 100\n", number);
                    break;
                
                case multiple_ivalid:
                    printf("Недопустимое значение числа: %lu\n", number);
                    break;
                    
            }
            free_vector(&multiple_nums);
            break;

        case 'p':
            
            switch(prime_number(number)){
                
                case 0:
                    printf("Число %lu является простым\n", number);
                    break;
                
                case 1:
                    printf("Число %lu составное\n", number);
                    break;
            }
            break;

        case 's':

            vector digits;
            create_vector(&digits, 0);
            
            switch(digits_number(number,&digits)){
                
                case 0:
                    printf("Цифра числа %lu: ", number);
                    print_vector(&digits);
                
                case -1:
                    printf("Ошибка");
            }
            free_vector(&digits);
            break;

        case 'e':
            
            vector* table[8];

            for( int column = 0; column < 8; column++){
                table[column] = (vector*)malloc(sizeof(vector));
                create_vector(table[column], 0);
            }

            switch(degree_table(number, table)){
                
                case degree_table_ok:
                    for (int row = 1; row <= number; row++){
                        for (int column = 0; column < 8; column ++){
                            printf("%2d^%d = %-10u\t", column + 2, row, table[column]->data[row-1]);
                        }
                        printf("\n");
                    }

                    for (int column = 0; column < 8; column ++){
                        free_vector(table[column]);
                    }

                    break;
                
                case degree_table_fail:
                    printf("Неверное значение числа: %lu\n", number);
                    break;
                
                case degree_table_overflow:
                    printf("Превышен лимит числа %lu, число не должно быть больше 10\n", number);
                    break;
            }
            
            break;

        case 'a':
            
            unsigned long result;
            result = 0;
            
            switch (sum_number(number, &result)){
                
                case 0:
                    printf("Сумма от 1 до %lu = %lu\n", number, result);
                    break;
                
                case 1:
                    printf("Неверное значение числа: %lu\n", number);
                    break;
            }

            break;

        case 'f':
            
            unsigned long factorial_result;
            
            switch (factorial(number, &factorial_result)){
                case fsk_ok:
                   printf("%u! = %lu\n", number, factorial_result);
                   break;
                
                case fsk_overflow:
                    printf("Переполнение!\n");
                    break;
                
                case fsk_invalid_parameter:
                    printf("Невалидный параметр\n");
                    break;
            }

            break;

        default:
            printf("Неизвестный флаг: %c\n", flag[1]);
            return 1;
    }

    return 0;
}


enum multiple_status multiple(unsigned int number, vector* multiple_nums){
    if (number > 50)
        return multiple_abscent;

    else if (number == 0)
        return multiple_ivalid;

    for (unsigned int multiple = number; multiple <= 100; multiple += number){
        push_back(multiple_nums, multiple);
    }

    return multiple_ok;
}

int prime_number(unsigned int number){
    if (number < 4)
        return 0;
    for (int i = 3; i <= sqrt(number); i+=2){
        if ((number % i) == 0)
            return 1;
    }
    return 0;
}

int digits_number(unsigned int number, vector* digits){

    int digits_number_recursive;

    if (digits == NULL) {
        return -1;
    }

    if (number == 0){
        return 0;
    }
    
    int digit = number % 10;

    digits_number_recursive = digits_number(number / 10, digits);
    if (digits_number_recursive == 0)
        push_back(digits,digit);

    return 0;
}

enum degree_table_status degree_table(unsigned int number, vector** table){
    unsigned int degree_result = 0;

    if (number > 9)
        return degree_table_overflow;

    else if ((number == 0) || (number == 1))
        return degree_table_fail;

    else
        for (int column = 0; column < 8; column ++){
            for (int row = 1; row <= number; row++){
                degree_result = pow(column + 2, row);
                push_back(table[column], degree_result);
            }
        }
    return degree_table_ok;   
}

int sum_number(unsigned int number, unsigned long* result){
    if (number == 0)
        return 1;

    *result = ((1 + number) * number / 2);

    return 0;
}

enum factorial_status_code factorial(unsigned int number, unsigned long* result){
    enum factorial_status_code recursive_status_code;
    if (number > 20)
        return fsk_invalid_parameter;

    if ((number == 0) || (number == 1)){
        *result = 1;
        return fsk_ok;
    }

    recursive_status_code = factorial(number - 1, result);
    if (recursive_status_code == fsk_ok)
        *result *= number;
    
    return recursive_status_code;
}