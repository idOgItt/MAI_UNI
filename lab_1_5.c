#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

enum status_code {ok, fail, invalid_parameter};

enum status_code sum_expanation(double epsilon, double *result_expanation);

enum status_code sum_two_expanation(double epsilon, double *result__two_expanation);

enum status_code sum_three_expanation(double epsilon, double *result_three_expanation);

enum status_code sum_expanation_expanation(double epsilon, double *result_expanation_expanation);

double factorial_iterative(double n) {
    if (n < 0) {
        return 0;
    }
    
    double result = 1;

    for (int i = 2; i <= n; i++) {
        result *= i;
    }

    return result;
}

int main (int argc, char *argv[]){

    if (argc != 2) {
        printf("Использование: %s <epsilon>\n", argv[0]);
        return 1;
    }

    double epsilon = atof(argv[1]);

    if (epsilon <= 0){
        printf("Epsilon must be epsilon > 0\n");
        return 1;
    }

    double result_expanation, result_two_expanation, result_three_expanation, result_expanation_expanation;

    switch(sum_expanation(epsilon, &result_expanation)){
        case ok:
            printf("Результат при epsilon %f = %.20f\n", epsilon, result_expanation);
            break;
        case fail:
            printf("Произошла ошибка при вычислениях.\n");
            break;
        case invalid_parameter:
            printf("Неверные параметры.\n");
            break;
    }

    switch(sum_two_expanation(epsilon, &result_two_expanation)){
        case ok:
            printf("Результат при epsilon %f = %.20f\n", epsilon, result_two_expanation);
            break;
        case fail:
            printf("Произошла ошибка при вычислениях.\n");
            break;
        case invalid_parameter:
            printf("Неверные параметры.\n");
            break;
    }

    switch(sum_three_expanation(epsilon, &result_three_expanation)){
        case ok:
            printf("Результат при epsilon %f = %.20f\n", epsilon, result_three_expanation);
            break;
        case fail:
            printf("Произошла ошибка при вычислениях.\n");
            break;
        case invalid_parameter:
            printf("Неверные параметры.\n");
            break;
    }

    switch(sum_expanation_expanation(epsilon, &result_expanation_expanation)){
        case ok:
            printf("Результат при epsilon %f = %.20f\n", epsilon, result_expanation_expanation);
            break;
        case fail:
            printf("Произошла ошибка при вычислениях.\n");
            break;
        case invalid_parameter:
            printf("Неверные параметры.\n");
            break;
    }

    return 0;
}

enum status_code sum_expanation(double epsilon, double *result_expanation){

    double term = 1.0;
    double result = 1.0;
    unsigned int n = 1;

    while (fabs(term) > epsilon) {
        result += term;
        n++;
        term /= n;
    }

    *result_expanation = result;

    return ok;
}

enum status_code sum_two_expanation(double epsilon, double *result_two_expanation){
    double term = 1.0;
    double result = 0.0;
    unsigned int  n = 0;
    double koeff = 0.5;

    while(fabs(term) > epsilon){
        result += term;
        n++;

        if (n % 2 == 0){
            term = (1.0 * powl(koeff, 2 * n))  / factorial_iterative(2 * n);
        }
        else {
            term = (-1.0 * powl(koeff, 2 * n)) / factorial_iterative(2 * n);
        }
    }

    *result_two_expanation = result;

    return ok;
}

enum status_code sum_three_expanation(double epsilon, double *result_three_expanation){
    double term = 1.0;
    double result = 0.0;
    double  n = 0;
    double koeff = 0.5;

    while (fabs(term) > epsilon){
        result += term;
        n++;
        term = (powl(koeff, 2.0 * n) * powl(3, 3.0 * n) * powl(factorial_iterative(n), 3)) / factorial_iterative(3.0 * n);
    }

    *result_three_expanation = result;

    return ok;
}

enum status_code sum_expanation_expanation(double epsilon, double *result_expanation_expanation) {
    double term = -0.125;
    double result = 0.0;
    double n = 1;
    double koeff = 0.5;
    double p, t;

    while (fabs(term) > epsilon) {
        result += term;
        n++;
        p = factorial_iterative(2 * n - 1);
        t = factorial_iterative(2 * n);

        if ((int)n % 2 == 0) {
            term = factorial_iterative(p) / (factorial_iterative(t) * pow(koeff, 2 * n));
        } else {
            term = -factorial_iterative(p) / (factorial_iterative(t) * pow(koeff, 2 * n));
        }
    }

    *result_expanation_expanation = result;

    return ok;
}