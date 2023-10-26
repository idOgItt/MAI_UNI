#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

enum define_number_float_status_code {
    define_number_float_ok,
    define_number_float_fail
};

enum define_number_float_status_code define_number_float(double*** result, int base, int count, ...) {
    va_list numbers;
    va_start(numbers, count);

    *result = (double**)malloc(count * sizeof(double*));
    for (int i = 0; i < count; i++) {
        (*result)[i] = (double*)malloc(2 * sizeof(double));
    }

    if (*result == NULL) {
        return define_number_float_fail;
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
        double denominator = 1e8;
        int numerator = (int)(number * denominator);
        int isFinite = 1;

        int base_copy = base;
        while (base_copy % 2 == 0) {
            base_copy /= 2;
        }
        while (base_copy % 5 == 0) {
            base_copy /= 5;
        }

        int gcd = 1;
        int a = numerator;
        int b = denominator;

        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        gcd = a;

        numerator /= gcd;
        denominator /= gcd;

        // Проверяем, имеет ли дробь конечное представление в системе с заданным основанием.
        if (fmod(denominator, base_copy) != 0) {
            isFinite = 0;
        }

        (*result)[i][1] = isFinite;
    }

    va_end(numbers);
    return define_number_float_ok;
}

int main() {
    int base = 8;
    double real_1 = 0.0101001;
    double real_2 = 0.064;
    double real_3 = 0.026;
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
