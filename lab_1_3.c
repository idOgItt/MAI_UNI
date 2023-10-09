#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <complex.h>

typedef struct {
    long double* data;
    unsigned int size;
    unsigned int capacity;
} vector;

enum equation_status_code {equation_ok, equation_fail, equation_invalid_parameter};

enum multiple_function_status_code {multiple_function_ok, multiple_function_fail, multiple_function_invalid_parameter};

enum side_square_triangle_status_code {side_square_triangle_ok, side_square_triangle_fail, side_square_triangle_invalid_parameter};

enum equation_status_code square_equation(long double epsilon, long double number_1, long double number_2, long double number_3, vector** result, int* index);

enum multiple_function_status_code multiple_function(long int number_1, long int number_2, int* result);

enum side_square_triangle_status_code side_square_triangle(long double epsilon, long double number_1, long double number_2, long double number_3, int* result);

void swap(long double* a, long double* b) {
    long double temp = *a;
    *a = *b;
    *b = temp;
}

void generate_permutations(long double* arr, int size, int index, long double** result, int* count) {
    if (index == size - 1) {
        // Копируем текущую перестановку в массив результатов
        for (int i = 0; i < size; i++) {
            (*result)[*count * size + i] = arr[i];
           // printf("count = %d arr[i]  %Lf \t *result = %Lf \n",*count, arr[i], (*result)[*count * size * i]);
        }
        (*count)++;
    } else {
        for (int i = index; i < size; i++) {
            swap(&arr[index], &arr[i]);
            generate_permutations(arr, size, index + 1, result, count);
            swap(&arr[index], &arr[i]); // Возвращаем элементы на место
        }
    }
}

long double* get_all_permutations(long double* arr, int size, int* count) {
    // Вычисляем количество перестановок (факториал размера массива)
    *count = 1;
    for (int i = 1; i <= size; i++) {
        *count *= i;
    }

   // printf("Permutations %d:\n", *count);
    // Выделяем память для массива результатов
    long double* result = (long double*)malloc(sizeof(long double) * size * (*count)); //check

    // Генерируем все перестановки
    * count = 0;// Счетчик для заполнения результатов
    generate_permutations(arr, size, 0, &result, count);

    /* for (int i = 0; i < size*(*count); i++){
        printf("Перестановка  result_all = %Lf , count_all = %d\n", result[i], *count);
    } */
    return result;
}

void create_vector(vector* v, unsigned int sz){
    v->size = sz;
    v->data = (long double*)malloc(sizeof(long double) * v->size);
    v->capacity = 0;
}

void push_back(vector* v, long double num){
    if (v->size == v->capacity){
        v->size++;
        v->data = (long double*)realloc(v->data, v->size * sizeof(long double));
    }
    
    v->data[v->capacity] = num;
    v->capacity++;
}

void print_vector(const vector* v) {
    for (int i = 0; i < v->capacity; i++) {
        printf("%Lf ", v->data[i]);
    }
    printf("\n");
}

void free_vector(vector* v) {
    free(v->data);
    v->size = 0;
    v->capacity = 0;
    v->data = NULL;
}



int main(int argc, char* argv[]) {

    if(!argv[1]){
        return 1;
    }

    char* flag = argv[1];

    if (flag[0] != '-' && flag[0] != '/'){
        printf("Неправильный флаг: %c\n", flag[0]);
        return 1;
    }

    long double epsilon_equation;
    long double number_1_equation;
    long double number_2_equation;
    long double number_3_equation;
    int index_square[6];
    vector* result_equation[6];

    long int number_1_multiple;
    long int number_2_multiple;

    long double epsilon_triangle;
    long double number_1_side;
    long double number_2_side;
    long double number_3_side;
    int side_result;
    int result_multiple;

    switch(flag[1]) {

        case 'q':

            if (argc != 6) {
                printf("Неверное количество параметров: <flag> <epsilon> <double> <double> <double>\n");
                return 1;
            }

            epsilon_equation = strtold(argv[2], NULL);
            number_1_equation = strtold(argv[3], NULL);
            number_2_equation = strtold(argv[4], NULL);
            number_3_equation = strtold(argv[5], NULL);

            for (int column = 0; column < 6; column++) {
                result_equation[column] = (vector*)malloc(sizeof(vector));
                create_vector(result_equation[column], 0);
                index_square[column] = 0;
            }

            switch (square_equation(epsilon_equation, number_1_equation, number_2_equation, number_3_equation, result_equation, index_square)) {

                case equation_ok:
                    for (int column = 0; column < 6; column++) {
                        if (index_square[column] == 1) {
                            printf("root_%d = %.5Lf + %.5Lfi\t", 1, result_equation[column]->data[0], result_equation[column]->data[1]);
                            printf("root_%d = %.5Lf - %.5Lfi\n", 2, result_equation[column]->data[0], result_equation[column]->data[1]);
                        } 
                        else {
                            printf("root_%d = %-10.5Lf\t", 1, result_equation[column]->data[0]);
                            printf("root_%d = %-10.5Lf\n", 2, result_equation[column]->data[1]);
                        }
                    }
                    break;

                case equation_fail:
                    break;
                case equation_invalid_parameter:
                    break;
            }

            for (int column = 0; column < 6; column++) {
                free_vector(result_equation[column]);
            }

            break;

        case 'm':

            if (argc != 4) {
                printf("Неверное количество параметров: <flag> <number> <number>\n");
                return 1;
            }

            number_1_multiple = strtol(argv[2], NULL, 10);
            number_2_multiple = strtol(argv[3], NULL, 10);

            switch (multiple_function(number_1_multiple, number_2_multiple, &result_multiple)) {

                case multiple_function_ok:
                    if (result_multiple == 1) {
                        printf("Число %ld не кратно %ld\n", number_1_multiple, number_2_multiple);
                        break;
                    }
                    else {
                        printf("Число %ld кратно %ld\n", number_1_multiple, number_2_multiple);
                        break;
                    }

                case multiple_function_invalid_parameter:
                    printf("Невалидные параметры\n");
                    return 1;

                case multiple_function_fail:
                    break;
            }

            break;

        case 't':

            if (argc != 6) {
                printf("Неверное количество параметров: <flag> <epsilon> <double> <double> <double>\n");
                return 1;
            }

            epsilon_triangle = strtold(argv[2], NULL);
            number_1_side = strtold(argv[3], NULL);
            number_2_side = strtold(argv[4], NULL);
            number_3_side = strtold(argv[5], NULL);

            switch (side_square_triangle(epsilon_triangle, number_1_side, number_2_side, number_3_side, &side_result)) {
                case side_square_triangle_ok:
                    if (side_result == 0) {
                        printf("Это прямоугольный треугольник\n");
                    }
                    else {
                        printf("Это не прямоугольный треугольник\n");
                    }
                    break;

                case side_square_triangle_invalid_parameter:
                    printf("Неправильно введены параметры\n");
                    return 1;
            }

            break;
    }

    return 0;
}



enum equation_status_code square_equation(long double epsilon, long double number_1, long double number_2, long double number_3, vector** result_equation, int* index_square){
    int size = 3;
    int j = 0;
    int count;
    long double numbers [3] = {number_1, number_2, number_3};
    long double* koeffs = get_all_permutations(numbers, size, &count);
    //for (int i = 0; i < size*(count); i++){
     //   printf("Перестановка  koeffs_equation = %Lf , count_equations = %d\n", koeffs[i], count);
    //}
    /* for (int j = 0; j < count; j++){
        for (int i = 0; i < size; i++) {
            printf("koeffs = %Lf \t count = %d\n", koeffs[ j * size + i], count);
        }
    } */
    //printf(" \nПромежуток  koeff, count = %d\n", count);
    long double discriminant[count];
    long double root_1[count], root_2[count];

    for (int i = 0; i < count; i++){
            discriminant[i] = koeffs[i * size + j] * koeffs[i * size + j] - 4.0 * koeffs[i * size + j + 1] * koeffs[i * size + j + 2];

            if (discriminant[i] < 0.0){
                long double real_part = -koeffs[i * size + j] / (2 * koeffs[i * size + j + 1]);
                long double imaginary_part = sqrtl(-discriminant[i]) / (2 * koeffs[i * size + j + 1]);
                index_square[i] = 1;
                root_1[i] = real_part;
                root_2[i] = imaginary_part;
            }

            else if ((discriminant[i]) > epsilon){
                root_1[i] = (-koeffs[i * size + j] + sqrtl(discriminant[i])) / (2 * koeffs[i * size + j + 1]);
                root_2[i] = (-koeffs[i * size + j] - sqrtl(discriminant[i])) / (2 * koeffs[i * size + j + 1]);
            }

            else if ((discriminant[i]) <= epsilon){
                root_1[i] = -koeffs[i * size + j] / (2 * koeffs[i * size + j + 1]);
                root_2[i] = root_1[i];
            }

        //printf("Корни 1 и 2 = %Lf, %Lf\n", root_1[i], root_2[i]);

        push_back(result_equation[i], root_1[i]);
        push_back(result_equation[i], root_2[i]);

        /* for (int row = 0; row < 2; row ++){
            printf("result%d = %Lf\t",row+1, result_equation[i]->data[row]);
        }
        printf("index_square[i] %d\n", index_square[i]); */
    }


    free(koeffs);
    return equation_ok;

}

enum multiple_function_status_code multiple_function(long int number_1, long int number_2, int* result){
    
    if (fabs(number_1) < fabs(number_2)){
        *result = 1;
        return multiple_function_invalid_parameter;
    }

    if ((number_1 % number_2) == 0){
        *result = 0;
        return multiple_function_ok;
    }

    else {
        *result = 1;
        return multiple_function_ok;
    }

    return multiple_function_fail;

}

enum side_square_triangle_status_code side_square_triangle(long double epsilon, long double number_1, long double number_2, long double number_3, int* result){

    if ((number_1 < 0) || (number_2 < 0) || (number_3 < 0)){
        *result = 1;
        return side_square_triangle_invalid_parameter;
    }

    if ((fabsl(number_1 * number_1 + number_2 * number_2 - number_3 * number_3) <= epsilon) ||
        (fabsl(number_1 * number_1 + number_3 * number_3 - number_2 * number_2) <= epsilon) ||
        (fabsl(number_3 * number_3 + number_2 * number_2 - number_1 * number_1) <= epsilon)) {
        *result = 0;
    } 
    else {
    // Условие не выполняется, это не прямоугольный треугольник
        *result = 1;
    }


    return side_square_triangle_ok;
}