#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

enum define_number_float_status_code {
    define_number_float_ok,
    define_number_float_fail
};

enum define_number_float_status_code define_number_float(double*** result, const int base, int count, ...) {
    va_list numbers;
    va_start(numbers, count);

    *result = (double**)malloc(count * sizeof(double*));
    
    for (int i = 0; i < count; i++) {
        (*result)[i] = (double*)malloc(2 * sizeof(double));

        if ((*result)[i] == NULL) {
            for (int j = 0; j < i; j++){
                free(*result[i]);
            }
            free(*result);
           return define_number_float_fail;
        }
    }

    for (int i = 0; i < count; i++) {
        double number = va_arg(numbers, double);
        if (number <= 0 || number >= 1) {
            for (int j = 0; j < count; j++) {
                free((*result)[j]);
            }
            free(*result);
            va_end(numbers);
            return define_number_float_fail;
        }

        (*result)[i][0] = number;
        int denominator = 1;
        int numerator;
        int points = 0;
        int isFinite = 0;
        int base_cp = base;

        if (base == 10){
            (*result)[i][1] = 1;
            continue;
        }
        
        do{
            number *= 10;
            denominator *= 10;
            points++;
        } while(fmod(number, 10.0) != 0);

        numerator = number;

        while (numerator != 0 && denominator != 0 && numerator % 10 == 0 && denominator % 10 == 0){
            numerator /= 10;
            denominator /= 10;
        }

        while (numerator != 0 && denominator != 0 && numerator % 2 == 0 && denominator % 2 == 0){
            numerator /= 2;
            denominator /= 2;
        }

        while (numerator != 0 && denominator != 0 && numerator % 5 == 0 && denominator % 5 == 0){
            numerator /= 5;
            denominator /= 5;
        }

        if (base_cp > denominator){
            while (base_cp % denominator == 0){
                base_cp /= denominator;
                if (base_cp == 1){
                    isFinite = 1;
                }
            }
            if (isFinite != 1 && base_cp < denominator){
                while (denominator % base_cp == 0){
                    denominator /= base_cp;
                    if (denominator == 1){
                       isFinite = 1;
                    }
                }
            }
        } else {
            while (denominator % base_cp == 0){
                denominator /= base_cp;
                if (denominator == 1){
                    isFinite = 1;
                }
            }
            if (isFinite != 1 && denominator < base_cp){
                while (base_cp % denominator == 0){
                    base_cp /= denominator;
                    if (base_cp == 1){
                       isFinite = 1;
                    }
                }
            }
        }


        (*result)[i][1] = isFinite;

    }

    va_end(numbers);
    return define_number_float_ok;
}

int main() {
    int base = 7;
    double real_1 = 0.97;
    double real_2 = 0.5;
    double real_3 = 0.25;
    double** result;
    int count = 3;

    if (define_number_float(&result, base, count, real_1, real_2, real_3) == define_number_float_ok) {
        for (int i = 0; i < count; i++) {
            printf("The define of the number %f in the system base %d -> %sfinite\n", result[i][0], base, result[i][1] ? "" : "in");
        }

        for (int i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
    } else {
        printf("The mistake occurred\n");
        return 1;
    }
    return 0;
}
