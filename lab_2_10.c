#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

enum polynomial_status_code {
    polynomial_ok,
    polynomial_fail
};

enum polynomial_status_code polynomial_expansion(
    double epsilon, double a, double **result,
    int degree, ...
) {
    va_list coefficients;
    va_start(coefficients, degree);

    *result = (double *)malloc((degree + 1) * sizeof(double));
    if (*result == NULL) {
        va_end(coefficients);
        return polynomial_fail;
    }

    // Если a равно 0, просто копируем коэффициенты
    if (a == 0) {
        for (int i = 0; i <= degree; i++) {
            (*result)[i] = va_arg(coefficients, double);
        }
        va_end(coefficients);
        return polynomial_ok;
    }

    double *temp = (double *)malloc((degree + 1) * sizeof(double));
    if (temp == NULL) {
        free(*result);
        va_end(coefficients);
        return polynomial_fail;
    }

    for (int i = 0; i <= degree; i++) {
        temp[i] = va_arg(coefficients, double);
    }
    va_end(coefficients);

    // Расчет коэффициентов результирующего многочлена
    for (int i = 0; i <= degree; i++) {
        double term = 1.0;
        double sum = temp[i];

        for (int j = 1; j <= i; j++) {
            term *= (a - j + 1) / j;
            sum += term * temp[i - j];
        }

        (*result)[i] = sum;
    }

    free(temp);

    return polynomial_ok;
}

int main() {
    double epsilon = 0.0001;
    double a = 2.0;
    double *result;
    int degree = 3;
    double coeff0 = 1.0;
    double coeff1 = -4.0;
    double coeff2 = 0.0;
    double coeff3 = 1.0;

    if (polynomial_expansion(epsilon, a, &result, degree, coeff0, coeff1, coeff2, coeff3) == polynomial_ok) {
        printf("Original polynomial: f(x) = %f + %fx + %fx^2 + %fx^3\n", coeff0, coeff1, coeff2, coeff3);

        printf("Expanded polynomial: f(x) = %f", result[0]);
        for (int i = 1; i <= degree; i++) {
            printf(" + %f(x - %.2lf)^%d", result[i], a, i);
        }
        printf("\n");

        free(result);
    } else {
        printf("The expansion failed\n");
        return 1;
    }

    return 0;
}