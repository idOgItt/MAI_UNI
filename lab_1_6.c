#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

enum status_code {ok, fail, invalid_parameter};

enum status_code integral_a(long double epsilon, long double* result_integral_a) {
    long double a, b;
    a = 0.0;
    b = 1.0;
    int n = 1;
    long double h = (b - a) / n;
    long double result = 0.0;

    while(h - epsilon > 0){ // epsilon
        h = (b - a) / n;
        n++;
    }

    for (int i = 0; i < n; i++) {
        long double x = a + i * h; // Левая граница текущего прямоугольника
        result += exp(-(x + h / 2.0) * (x + h / 2.0) / 2.0); // Вычисляем значение функции в середине прямоугольника
    }

    *result_integral_a = result * h;

    return ok;
}


enum status_code integral_b(long double epsilon, long double* result_integral_b){
    long double a, b;
    a = 0.0;
    b = 1.0;
    int n = 1;
    long double h = (b - a) / n;
    long double result = 0.0;

     while(h - epsilon > 0){
        h = (b - a) / n;
        n++;
    }

    for (int i = 0; i < n; i++) {
        long double x = a + i * h; // Левая граница текущего прямоугольника
        result += log(1+(x + h / 2.0)) / (x + h / 2.0); // Вычисляем значение функции в середине прямоугольника
    }

    *result_integral_b = result * h;

    return ok;
}

enum status_code integral_c(long double epsilon, long double* result_integral_c){
    long double a, b;
    a = 0.0;
    b = 1.0;
    int n = 1;
    long double h = (b - a) / n;
    long double result = 0.0;

     while(h - epsilon > 0){
        h = (b - a) / n;
        n++;
    }

    for (int i = 0; i < n; i++) {
        long double x = a + i * h; // Левая граница текущего прямоугольника
        if ((x <= 0.0) || (x>= 1)){
            result += 0;
        }
        else {
            result += log(1 / (1 - (x + h / 2.0))); // Вычисляем значение функции в середине прямоугольника
        }
    }

    *result_integral_c = result * h;

    return ok;
}

enum status_code integral_d(long double epsilon, long double* result_integral_d){
    long double a, b;
    a = 0.0;
    b = 1.0;
    int n = 1;
    long double h = (b - a) / n;
    long double result = 0.0;

     while(h - epsilon > 0){
        h = (b - a) / n;
        n++;
    }

    for (int i = 0; i < n; i++) {
        long double x = a + i * h; // Левая граница текущего прямоугольника
        result += pow((x + h / 2.0), (x + h / 2.0)); // Вычисляем значение функции в середине прямоугольника
    }

    *result_integral_d = result * h;

    return ok;
}

int main(int argc, char* argv[]){
    
    if (argc != 2) {
        printf("Usage: %s <epsilon>\n", argv[0]);
        return 1;
}

    long double epsilon = strtold(argv[1], NULL);

    long double result_integral_a = 0, result_integral_b = 0, result_integral_c = 0, result_integral_d = 0;
    
    if (integral_a(epsilon, &result_integral_a) == ok){
        printf("Результат интеграла а. = %.20LF\n", result_integral_a);
    }

    if (integral_b(epsilon, &result_integral_b) == ok){
        printf("Результат интеграла b. = %.20LF\n", result_integral_b);
    }

    if (integral_c(epsilon, &result_integral_c) == ok){
        printf("Результат интеграла c. = %.20LF\n", result_integral_c);
    }

    if (integral_d(epsilon, &result_integral_d) == ok){
        printf("Результат интеграла d. = %.20LF\n", result_integral_d);
    }

    return 0;
}
