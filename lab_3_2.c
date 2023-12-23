#include <stdarg.h>
#include <math.h>
#include <stdio.h>

enum find_longest_vector_status_code {invalid_parameter, ok, fail};

typedef struct {
    double* coordinates;
    int dimension;
} Vector;

// function for power (log2n)
/*
double power (double num, long deg){
    double result = 1;

    while(deg){
        if (deg % 2 == 0){
            deg /= 2;
            result *= num;
        }
        else {
            deg--;
            result *= num;
        }
    }
    return  result;
} */

// Pointer to a function to calculate norm
typedef double (*norm_function)(Vector *);

// function to calculate norm -> infinite
double infinity_norm(Vector* vector){
    double max = 0.0;

    for (int i = 0; i < vector->dimension; ++i) {
        double value = fabs(vector->coordinates[i]);

        if (value > max){
            max = value;
        }
    }
    return max;
}

// function to calculate p_norm; p >= 1;
double p_norm(Vector* vector, double p){

    if (p < 1){
        return invalid_parameter;
    }

    double sum = 0;

    for (int i = 0; i < vector->dimension; ++i) {
        sum += pow(fabs(vector->coordinates[i]), p);
    }

    return pow(sum, 1.0 / p);
}

// function to calculate a_norm;
double a_norm(Vector* vector, double matrix[][3], int dimension){
    double result = 0.0;

    for(int row = 0; row < dimension; row++){
        for(int column = 0; column < dimension; column++){
            result += matrix[row][column] * vector->coordinates[row] * vector->coordinates[column];
        }
    }
    return sqrt(result);
}

// Universal function to calculate vector's norm (a_norm, p_norm, infinity_norm)
Vector find_longest_vector(int num_vectors, double matrix[][3], int dimension, norm_function norm, ...){
    va_list vectors;
    va_start(vectors, norm);

    Vector longest_vector;
    double max_length = 0.0;

    for (int i = 0; i < num_vectors; i++){
        Vector current_vector = va_arg(vectors, Vector);
        double current_length = norm(&current_vector);

        if (current_length > max_length){
            max_length = current_length;
            longest_vector = current_vector;
        }
    }
    va_end(vectors);

    return  longest_vector;
}

int main(){
    int dimension = 3;

    double coordinates_1[] = {1.0, 2.0, 3.0};
    double coordinates_2[] = {-1.0, 4.0, -2.9};

    Vector vector_1 = {coordinates_1, dimension};
    Vector vector_2 = {coordinates_2, dimension};

    // random matrix
    double  matrix[3][3] = {{2.0, 0.0, 0.0},
                            {0.0, 1.5, 0.0},
                            {0.0, 0.0, 3.0}};

    // The longest vector calculated via norm
    Vector longest_infinity_norm = find_longest_vector(2, matrix, 0, (norm_function) infinity_norm, vector_1, vector_2);
    Vector longest_p_norm = find_longest_vector(2, matrix, 0, (norm_function) p_norm, vector_1, vector_2, 3.0);
    Vector longest_a_norm = find_longest_vector(2, matrix, dimension, (norm_function) a_norm, vector_1, vector_2);

    // Output results
    printf("Longest vector by infinity norm: (%f, %f, %f)\n", longest_infinity_norm.coordinates[0],
           longest_infinity_norm.coordinates[1], longest_infinity_norm.coordinates[2]);

    printf("Longest vector by 2-norm: (%f, %f, %f)\n", longest_p_norm.coordinates[0],
           longest_p_norm.coordinates[1], longest_p_norm.coordinates[2]);

    printf("Longest vector by A-norm: (%f, %f, %f)\n", longest_a_norm.coordinates[0],
           longest_a_norm.coordinates[1], longest_a_norm.coordinates[2]);

    return 0;
}