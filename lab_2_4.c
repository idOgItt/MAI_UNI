#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef struct point {
    double x, y;
} Point;

enum convex_polygon_status_code {convex_polygon_ok, convex_polygon_fail};

enum cross_product_status_code {cross_product_ok, cross_product_fail};

enum evaluate_polynomial_satus_code {evaluate_polynomial_ok, evaluate_polynomial_fail};

enum convex_polygon_status_code convex_polygon(int* convex, int count, ...);

enum cross_product_status_code cross_product(double* cross, Point, Point, Point);

enum evaluate_polynomial_satus_code evaluate_polynomial(double* evaluation, double x, int degree, ...);

int main() {
    int convex;
    int count = 4;
    Point A = {0.0, 0.0};
    Point B = {1.0, 1.0};
    Point C = {2.0, 0.0};
    Point D = {1.0, -1.0};

    double evaluation = 0;
    double x = 2.0;
    int degree = 3;
    double coeff3 = 1.0;
    double coeff2 = -2.0;
    double coeff1 = 3.0;
    double coeff0 = -4.0;

    if(convex_polygon(&convex, count, A, B, C, D) == convex_polygon_ok){
        if(convex){
            printf("It is a polygon\n");
        } else {
            printf("The polygon is not convex\n");
        }
    } else{
        printf("Error in calculation\n");
        return 1;
    }

    if(evaluate_polynomial(&evaluation, x, degree, coeff3, coeff2, coeff1, coeff0) == evaluate_polynomial_ok){
        printf("The value of the polynomial at x = %.2f is %.2f\n", x, evaluation);
    } else {
        printf("Error in calculations\n");
        return 1;
    }

    return 0;
}

enum convex_polygon_status_code convex_polygon(int* convex, int count, ...){
    va_list points;
    va_start(points, count);
    int sign = 0;
    Point* polygon_points = (Point*)malloc(count * sizeof(Point));

    for(int i = 0; i < count; i++){
        polygon_points[i] = va_arg(points, Point);
    }

    va_end(points);

    for(int i = 0; i < count; i++){

        double cross;

        int next = (i + 1) % count;
        int next_next = (i + 2) % count;

        if(cross_product(&cross, polygon_points[i], polygon_points[next], polygon_points[next_next]) != cross_product_ok){
            free(polygon_points);
            return convex_polygon_fail;
        }

        if(cross != 0){
            if(sign == 0) {
                sign = (cross > 0) ? 1 : -1;
            } else if (sign * cross < 0){
                *convex = 0;
                free(polygon_points);
                return convex_polygon_ok;
            }
        }
    }

    *convex = 1;
    free(polygon_points);
    return convex_polygon_ok;
}

enum cross_product_status_code cross_product(double* cross, Point A, Point B, Point C){
    double vector_1_X = B.x - A.x;
    double vector_1_Y = B.y - A.y;
    double vector_2_X = C.x - B.x;
    double vector_2_Y = C.y - B.y;

    *cross = vector_1_X * vector_2_Y - vector_1_Y * vector_2_X; 

    return cross_product_ok;
}

enum evaluate_polynomial_satus_code evaluate_polynomial(double* evaluation, double x, int degree, ...){
    va_list coefficients;
    va_start(coefficients, degree);

    double power_of_x = pow(x, degree);

    for (int i = 0; i <= degree; i++){
        double coefficient = va_arg(coefficients, double);
        *evaluation += coefficient * power_of_x;
        power_of_x /= x;
    }

    va_end(coefficients);

    return evaluate_polynomial_ok;
}