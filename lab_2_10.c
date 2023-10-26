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

    for (int i = 0; i <= degree; i++) {
        (*result)[i] = 0.0;
    }

    double x = a;
    double current_coefficient;

    for (int i = 0; i <= degree; i++) {
        current_coefficient = va_arg(coefficients, double);


        for (int j = i; j <= degree; j++) {
            double binomial = 1.0;

            for (int k = 0; k < i; k++) {
                binomial *= (x - a - k) / (k + 1);
            }

            (*result)[j] += current_coefficient * binomial;
        }
    }

    va_end(coefficients);
    return polynomial_ok;
}

int main() {
    double epsilon = 0.0001;
    double a = 2.0;
    double *result;
    int degree = 3;
    double coeff0 = 1.0;
    double coeff1 = 3.0;
    double coeff2 = 2.0;
    double coeff3 = 4.0;

    if (polynomial_expansion(epsilon, a, &result, degree, coeff0, coeff1, coeff2, coeff3) == polynomial_ok) {
        for (int i = 0; i <= degree; i++) {
            printf("Coefficient %d: %f\n", i, result[i]);
        }
        free(result);
    } else {
        printf("The expansion failed\n");
        return 1;
    }

    return 0;
}
