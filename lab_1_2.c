#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

# define M_PI 3.14159265358979323846

enum status_code {ok, fail, invalid_parameter};

enum status_code limit_e(unsigned precision, long double* result_limit_e);

enum status_code row_e(unsigned precision, long double* result_row_e);

enum status_code equation_e(unsigned int precision, long double* result_equation_e);

enum status_code limit_pi(unsigned int precision, long double* result_limit_pi);

enum status_code row_pi(unsigned precision, long double* result_row_pi);

enum status_code equation_pi(unsigned precision, long double* equation_row_pi);

enum status_code limit_sqrt_two(unsigned int precision, long double* result_limit_sqrt_two);

enum status_code row_sqrt_two(unsigned int precision, long double* result_row_sqrt_two);

enum status_code equation_sqrt_two(unsigned int precision, long double* result_equation_sqrt_two);

enum status_code row_log_two(unsigned int precision, long double* result_row_log_two);

enum status_code limit_log_two(unsigned int precision, long double* result_limit_log_two);

enum status_code equation_log_two(unsigned int precision, long double* result_equation_log_two);

enum status_code limit_gamma(unsigned int precision, long double* result_limit_gamma);

enum status_code row_gamma(unsigned int precision, long double* result_row_gamma);

enum status_code equation_gamma(unsigned int precision, long double* result_equation_gamma);


long double factorial_iterative(long double n) {
    if (n < 0) {
        return 0;
    }
    
    long double result = 1;

    for (int i = 2; i <= n; i++) {
        result *= i;
    }

    return result;
}


int main( int argc, char *argv[]) {
    if (argc != 2){
        printf("Программа:%s <число>\n", argv[0]);
        return 1;
    }

    int precision = atoi(argv[1]);

    if (precision <= 0){
        printf("Неправильное значение точности %d\n", precision);
        return 1;
    }
    
    long double result_limit_e, result_row_e, result_equation_e, result_limit_pi, result_row_pi, result_equation_pi, result_limit_log_two, result_row_log_two,
                result_equation_log_two, result_limit_sqrt_two, result_row_sqrt_two, result_equation_sqrt_two, result_limit_gamma, result_row_gamma, result_equation_gamma;

    if (limit_e(precision, &result_limit_e) == ok) {
        printf("Результат предела с точностью %d знаков: e ≈ %.20Lf\n", precision, result_limit_e);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }


    if (row_e(precision, &result_row_e) == ok) {
        printf("Результат ряда с точностью %d знаков: e ≈ %.20Lf\n", precision, result_row_e);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }

    if (equation_e(precision, &result_equation_e) == ok) {
        printf("Результат equation_e с точностью %d знаков: e ≈ %.20Lf\n", precision, result_equation_e);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }

    if (limit_pi(precision, &result_limit_pi) == ok) {
        printf("Результат предела с точностью %d знаков: pi ≈ %.20Lf\n", precision, result_limit_pi);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }

    if (row_pi(precision, &result_row_pi) == ok) {
        printf("Результат ряда с точностью %d знаков: pi ≈ %.20Lf\n", precision, result_row_pi);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }

    if (equation_pi(precision, &result_equation_pi) == ok) {
        printf("Результат equation_pi с точностью %d знаков: pi ≈ %.20Lf\n", precision, result_equation_pi);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }

    if (limit_log_two(precision, &result_limit_log_two) == ok) {
        printf("Результат limit с точностью %d знаков: log(2) ≈ %.20Lf\n", precision, result_limit_log_two);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }

    if (row_log_two(precision, &result_row_log_two) == ok) {
        printf("Результат ряда с точностью %d знаков: log(2) ≈ %.20Lf\n", precision, result_row_log_two);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }

    if (equation_log_two(precision, &result_equation_log_two) == ok) {
        printf("Результат equation_log(2) с точностью %d знаков: log(2) ≈ %.20Lf\n", precision, result_equation_log_two);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }

    if (limit_sqrt_two(precision, &result_limit_sqrt_two) == ok) {
        printf("Результат limit с точностью %d знаков: sqrt(2) ≈ %.20Lf\n", precision, result_limit_sqrt_two);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }

    if (row_sqrt_two(precision, &result_row_sqrt_two) == ok) {
        printf("Результат ряда с точностью %d знаков: sqrt(2) ≈ %.20Lf\n", precision, result_row_sqrt_two);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }

    if (equation_sqrt_two(precision, &result_equation_sqrt_two) == ok) {
        printf("Результат equation_sqrt_two с точностью %d знаков: sqrt(2) ≈ %.20Lf\n", precision, result_equation_sqrt_two);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }

    if (limit_gamma(precision, &result_limit_gamma) == ok) {
        printf("Результат limit_gamma с точностью %d знаков: gamma ≈ %.20Lf\n", precision, result_limit_gamma);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }

    if (row_gamma(precision, &result_row_gamma) == ok) {
        printf("Результат row_gamma с точностью %d знаков: gamma ≈ %.20Lf\n", precision, result_row_gamma);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }

    if (equation_gamma(precision, &result_equation_gamma) == ok) {
        printf("Результат equation_gamma с точностью %d знаков: gamma ≈ %.20Lf\n", precision, result_equation_gamma);
    } 
    
    else {
        printf("Ошибка при вычислении.\n");
        return 1;
    }


    return 0;

}

enum status_code limit_e(unsigned int precision, long double* result_limit_e){
    long double limit_0 = 0.0;
    long double limit_1 = 2.0;
    long double n = 1.0;
    long double epsilon = powl(10, -(signed)precision);

    while ((fabsl(limit_1 - limit_0) / fabsl(limit_1)) > epsilon) {
        limit_0 = limit_1;
        n++;
        limit_1 = powl((1.0 + (1.0 / n)), n);
    }

    *result_limit_e = limit_1;

    return ok;
}

enum status_code row_e(unsigned precision, long double* result_row_e){
    long double epsilon = powl(10, -(signed)precision);
    long double row_e = 0.0;
    long double term = 1.0;

    for(unsigned n = 1; term > epsilon; n++){
        row_e += term;
        term /= n;
    }

    *result_row_e = row_e;

    return ok;
}

enum status_code limit_pi(unsigned int precision, long double* result_limit_pi){
    long double limit_0 = 0.0;
    long double limit_1 = 4.0;
    long double n = 1.0;
    long double epsilon = powl(10, -(signed)precision);

    while (fabsl(limit_1 - limit_0) > epsilon) {
        limit_0 = limit_1;
        n++;
        limit_1 = powl((powl(2.0, n) * factorial_iterative(n)), 4) / (n *(powl(factorial_iterative(2.0 * n), 2)));
    }

    *result_limit_pi = limit_1;

    return ok;
}

enum status_code row_pi(unsigned precision, long double* result_row_pi){
    long double epsilon = powl(10, -(signed)precision);
    long double row_pi = 0.0;
    long double term = 1.0;
    unsigned n = 2;

    while (fabsl(term) > epsilon) {
        long double denominator = 2.0 * n - 1.0;

        if (n % 2 == 0) {
            row_pi += term;
        } else {
            row_pi -= term;
        }

        term = 1.0 / denominator;
        n++;
    }

    *result_row_pi = 4.0 * row_pi;

    return ok;
}


enum status_code limit_log_two(unsigned int precision, long double* result_limit_log_two){
    long double limit_0 = 0.0;
    long double limit_1 = 1.0;
    long double n = 1.0;
    long double epsilon = powl(10, -(signed)precision);

    while (fabsl(limit_1 - limit_0) > epsilon) {
        limit_0 = limit_1;
        n++;
        limit_1 = n * (powl(2, (1.0 / n)) - 1);
    }

    *result_limit_log_two = limit_1;

    return ok;
}

enum status_code row_log_two(unsigned int precision, long double* result_row_log_two){
    long double epsilon = powl(10, -(signed)precision);
    long double log_two = 0.0;
    long double term = 1.0;

    for(unsigned n = 1; term > epsilon; n++){
        
        term = 1.0 / n;

        if ((n-1) % 2 ==0){
            log_two += term;
        } else {
            log_two -= term;
        }

    }

    *result_row_log_two = log_two;

    return ok;
}

enum status_code limit_sqrt_two(unsigned int precision, long double* result_limit_sqrt_two){
    long double x = -0.5;
    long double epsilon = powl(10, -(signed)precision);

    for (int i = 0; i < 1000; i++) {
        long double x_next = x - (x*x / 2.0) + 1;

        if (fabsl(x_next - x) < epsilon) {
            *result_limit_sqrt_two = x_next;
            return ok;
        }

        x = x_next;
    }

    return fail;
}


enum status_code row_sqrt_two(unsigned int precision, long double* result_row_sqrt_two){
    long double sqrt_two = 1.0;

    if (precision == 0) {
        return invalid_parameter;
    }

    for (unsigned int k = 1; k <= precision; k++) {
        sqrt_two *= powl(2.0, 1.0 / powl(2.0, k));
    }

    *result_row_sqrt_two = sqrt_two;

    return ok;
}

enum status_code equation_sqrt_two(unsigned int precision, long double* result_equation_sqrt_two) {
    long double x = 2; 
    long double epsilon = powl(10, -(signed)precision);
    while (fabs(x * x - 2) > epsilon) {
        x = 0.5 * (x + 2 / x);
    }
    *result_equation_sqrt_two = x;
    return ok;
}

enum status_code equation_e(unsigned int precision, long double* result_equation_e){
    long double epsilon = powl(10, -(signed)precision);
    long double a, b ,c;
    a = 1.0;
    b = 3.0;

    while ((b - a) >= epsilon) {
        // Находим середину интервала
        c = (a + b) / 2.0;

        // Проверяем, есть ли корень внутри интервала [a, c]
        if (log(c) - 1.0 == 0.0) {
            *result_equation_e = c;
            return ok;
        }

        // Определяем новый интервал для следующей итерации
        if ((log(c) - 1.0) * (log(a) - 1.0) < 0) {
            b = c;
        } else {
            a = c;
        }
    }
    *result_equation_e = (a + b) / 2.0;
    return ok;
}


long double derivative_cos_x(long double x) {
    return -sin(x);
}

enum status_code equation_pi(unsigned int precision, long double* result_equation_pi) {
    long double x0 = 3.0;
    long double epsilon = powl(10, -(signed)precision);
    long double x = x0;

    while (1) {
        long double f_x = cos(x) + 1.0;
        long double f_prime_x = derivative_cos_x(x);

        if (fabsl(f_x) < epsilon) {
            *result_equation_pi = x;
            return ok;
        }

        if (fabsl(f_prime_x) < epsilon) {
            return fail;
        }

        x = x - f_x / f_prime_x;
    }

    return fail;
}

enum status_code equation_log_two(unsigned int precision, long double* result_equation_log_two){
    long double epsilon = powl(10, -(signed)precision);
    long double a, b ,c;
    a = 0.0;
    b = 1.0;

    while ((b - a) >= epsilon) {
        // Находим середину интервала
        c = (a + b) / 2.0;

        // Проверяем, есть ли корень внутри интервала [a, c]
        if (exp(c) - 2 == 0.0) {
            *result_equation_log_two = c;
            return ok;
            
        }

        // Определяем новый интервал для следующей итерации
        if ((exp(c) - 2) * (exp(a) - 2) < 0) {
            b = c;
        } else {
            a = c;
        }
    }
    *result_equation_log_two = (a + b) / 2.0;
    return ok;
}


long double limit_gamma_row(long double n){
    long double k = 1.0;
    long double result = 0;
    long double term = 0;

    for(k; k <= n; k++){
        result += term;
        if ((signed)k % 2 == 0){
            term = (factorial_iterative(n) / (factorial_iterative(k) * factorial_iterative(n - k))) * (logl(factorial_iterative(k)) / k);
        }
        else{
            term = -((factorial_iterative(n) / (factorial_iterative(k) * factorial_iterative(n - k))) * (logl(factorial_iterative(k)) / k));
        }
    }
    return result;
}


enum status_code limit_gamma(unsigned int precision, long double* result_limit_gamma){
    long double gamma = 0.0;
    long double epsilon = powl(10, -(signed)precision);

    for (unsigned int k = 1; ; k++) {
        long double term = 1.0 / k - logl(1.0 + 1.0 / k);

        if (fabsl(term) < epsilon) {
            break;
        }

        gamma += term;
    }

    *result_limit_gamma = gamma;

    return ok;
}

enum status_code row_gamma(unsigned int precision, long double* result_row_gamma){
    /*long double gamma = -(M_PI * M_PI) / 6;
    long double epsilon = powl(10, -(signed)precision);

    for (unsigned int k = 2; ; k++) {
        long double term = 1.0 / powl(sqrtl(k), 2.0) - 1.0 / k;

        if (fabsl(term) < epsilon) {
            break;
        }

        gamma += term;
    }

    *result_row_gamma = gamma;*/
    long double gamma = 0.0;
    long double epsilon = powl(10, -(signed)precision);

    for (unsigned int k = 1; ; k++) {
        long double term = 1.0 / k - logl(1.0 + 1.0 / k);

        if (fabsl(term) < epsilon) {
            break;
        }

        gamma += term;
    }

    *result_row_gamma = gamma;

    return ok;
}

int is_prime(int n) {
    if (n <= 1) {
        return 0;
    }
    if (n <= 3) {
        return 1;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return 0;
    }
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }
    return 1;
}

long double func(long double x, long double t) {
    long double sum = 0.0;
    int p;
    for (p = 3; p <= t; p+=2) {
        if (is_prime(p)) {
            sum *= (p - 1.0) / (p);
        }
    }
    return (1 / exp(x)) - 1e10 * sum;
}

enum status_code equation_gamma(unsigned int precision, long double* result_equation_gamma){
    long double epsilon = powl(10, -(signed)precision);
    long double a, b ,c , t;
    a = 0.1;
    b = 1.0;
    // t = 100000000;

    while ((b - a) >= epsilon) {
        // Находим середину интервала
        c = (a + b) / 2.0;

        // Проверяем, есть ли корень внутри интервала [a, c]
        if (func(c, t) == 0.0) {
            *result_equation_gamma = c;
            return ok;
            
        }

        // Определяем новый интервал для следующей итерации
        if ((func(c, t) ) * (func(a, t) ) < 0) {
            b = c;
        } else {
            a = c;
        }
    }
    *result_equation_gamma = (a + b) / 2.0;
    return ok;
}