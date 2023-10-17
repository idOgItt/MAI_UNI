#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

enum geom_average_status_code {geom_average_ok, geom_average_invalid_param};

enum power_status_code { power_ok, power_invalid_exponent };

enum geom_average_status_code geom_average(double* average, int count, ...);

enum power_status_code power(double base, int exponent, double *result);

int main(int argc, char* argv[]){

    double real_1 = 2.1;
    double real_2 = 4.1;
    double real_3 = 8.2;
    int count = 3;
    double average;
    double exponent;

    if(geom_average(&average, count, real_1, real_2, real_3) == geom_average_ok){
        printf("The geometric average is %lf\n", average);
    }

    if(geom_average(&average, count, real_1, real_2, real_3) == geom_average_invalid_param){
        printf("The parameters are wrong\n");
        return 1;
    }

    if (power(real_1, count, &exponent) == power_ok) {
        printf("%lf^%d = %lf\n", real_1, count, exponent);
    } else {
        printf("Error: Invalid exponent\n");
    }


    return 0;
}

enum geom_average_status_code geom_average(double* average, int count, ...){

    if (count < 0){
        return geom_average_invalid_param;
    }

    va_list numbers;
    double total = 1;

    va_start(numbers, count);

    for(int j = 1; j <= count; j++){
        total *= va_arg(numbers, double);
    }

    va_end(numbers);

    *average = pow(total, 1.0 / count);

    return geom_average_ok;
}

enum power_status_code power(double base, int exponent, double *result) {
    if (exponent == 0) {
        *result = 1;
        return power_ok;

    } else if (exponent % 2 == 0) {
        double half;
        enum power_status_code status = power(base, exponent / 2, &half);
        if (status != power_ok) {
            return status;
        }
        *result = half * half;
        return power_ok;

    } else if (exponent > 0) {
        double recursive_result;
        enum power_status_code status = power(base, exponent - 1, &recursive_result);
        if (status != power_ok) {
            return status;
        }
        *result = base * recursive_result;
        return power_ok;

    } else {
        double reciprocal;
        enum power_status_code status = power(base, -exponent, &reciprocal);

        if (status != power_ok) {
            return status;
        }

        *result = 1.0 / reciprocal;
        return power_ok;
    }
}