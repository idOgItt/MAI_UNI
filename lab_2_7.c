#include <stdio.h>
#include <math.h>

double equation_function(double x) {
    return x * x - 4;
}

double equ_function(double x){
    return x * x * x - x * x + 5 * x - 7;
}

double find_root(double a, double b, double epsilon, double (*equation)(double)) {
    if (equation(a) * equation(b) >= 0) {
        printf("Начальные границы не подходят. Убедитесь, что f(a) и f(b) имеют разные знаки.\n");
        return 0.0;
    }

    double c;

    while ((b - a) >= epsilon) {
        c = (a + b) / 2;

        if (equation(c) == 0.0) {
            break;
        } else if (equation(c) * equation(a) < 0) {
            b = c;
        } else {
            a = c;
        }
    }

    return c;
}

int main() {
    double a = 0.0;
    double b = 5.0;
    double epsilon = 0.0001;
    double result = find_root(a, b, epsilon, equation_function);
    double result_equ = find_root(a, b, epsilon, equ_function);

    if (result != 0.0) {
        printf("Корень уравнения: %lf\n", result);
    } else {
        printf("Корень не найден.\n");
    }

    if (result_equ != 0.0) {
        printf("Корень уравнения: %lf\n", result_equ);
    } else {
        printf("Корень не найден.\n");
    }

    return 0;
}
