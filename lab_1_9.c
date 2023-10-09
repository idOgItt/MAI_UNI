#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define STACK 100

enum massive_sort_status_code {massive_sort_ok, massive_sort_fail};
enum dynamic_insert_status_code {dynamic_insert_ok, dynamic_insert_fail};
enum vector_status_code {vector_created_failure, realloc_failure, vector_ok, realloc_ok};

typedef struct {
    int* data;
    unsigned int size;
    unsigned int capacity;
} vector;


enum massive_sort_status_code massive_sort(unsigned int a, unsigned int b, int* result_sort);
enum dynamic_insert_status_code dynamic_insert(vector* result_sort);

void free_vector(vector* v);

void create_vector(vector* v, unsigned int sz, enum vector_status_code* status_code){
    v->size = sz;
    v->data = (int*)malloc(sizeof(int) * v->size);
    if (v->data == NULL) {
        *status_code = vector_created_failure;
        return;
    }
    else {
        *status_code = vector_ok;
    }
    v->capacity = 0;
}

void push_back(vector* v, int num, enum vector_status_code* status_code){
    int* temp;
    if (v->size == v->capacity){
        v->size++;
        temp = (int*)malloc(sizeof(int) * v->size); // check;
        
        for (unsigned int i = 0; i <= v->capacity; i++) {
           temp[i] = v->data[i];
        }

        free(v->data);
        v->data = temp;
        
        *status_code = realloc_ok;
    }
    
    v->data[v->capacity] = num;
    v->capacity++;
    *status_code = realloc_ok;
}

void print_vector(const vector* v) {
    for (int i = 0; i < v->capacity; i++) {
        printf("%d ", v->data[i]);
    }
    printf("\n");
}

void free_vector(vector* v) {
    free(v->data);
    v->size = 0;
    v->capacity = 0;
    v->data = NULL;
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}


int main (int argc, char* argv[]){
    if (argc != 3){
        printf ("Требуется %s <a> <b>\n", argv[0]);
        return 1;
    }

    unsigned int a = atoi(argv[1]);
    unsigned int b = atoi(argv[2]);
    enum vector_status_code status_code = vector_ok;

    int result_sort[STACK];

    vector result_insert;
    create_vector(&result_insert, 0,&status_code);

    switch(massive_sort(a, b, result_sort)){

        case massive_sort_ok:
            puts("Статический массив:");
            for (int i = 0; i < STACK; i++){
                printf("%d ", result_sort[i]);
            }
            printf("\n");
            break;
        break;

        case massive_sort_fail:
            printf("Не удалось\n");
            break;
        break;
    }

    switch(dynamic_insert(&result_insert)){

        case dynamic_insert_ok:
            puts("Динамический массив:");
            for (int i = 0; i <= result_insert.capacity; i++){
                printf("%d ", result_insert.data[i]);
            }
            printf("\n");
            break;
        break;

        case dynamic_insert_fail:
            printf("Не удалось\n");
            break;
        break;
    }    

    free_vector(&result_insert);

    return 0;
}


int function_sort(int* massive, unsigned int size){
    if (size <= 1){
        return 1;
    }

    int min_index = 0;
    int max_index = 0;

    for (int i = 1; i < size; i++){
        if (massive[i] < massive[min_index]){
            min_index = i;
        }
        if  (massive[i] > massive[max_index]){
            max_index = i;
        }
    }

    int temp = massive[min_index];
    massive[min_index] = massive[max_index];
    massive[max_index] = temp;

    return 0;
}

int binary_find(vector* massive, unsigned int num) {
    int low = 0;
    int high = massive->capacity;
    int closest = -1; // Initialize closest to an invalid value.

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (massive->data[mid] == num) {
            return massive->data[mid]; // Exact match found.
        } else if (massive->data[mid] < num) {
            low = mid + 1;
            closest = mid; // Update closest when the element is less than num.
        } else {
            high = mid - 1;
        }
    }

    // If num is not found, return the closest value.
    if (closest != -1) {
        return massive->data[closest];
    } else {
        // No element in the vector, return an appropriate value (e.g., -1).
        return -1;
    }
}


enum massive_sort_status_code massive_sort(unsigned int a, unsigned int b, int* result_sort){
    srand(time(NULL));
    unsigned int differ;
    if (a >= b){
        differ = a - b;
    }
    else{
        differ = b - a;
    }

    for( int i = 0; i < STACK; i++){
        result_sort[i] = rand() % differ;
    }

    qsort(result_sort, STACK, sizeof(int), compare);
    return massive_sort_ok;
}



enum dynamic_insert_status_code dynamic_insert(vector* result_insert){
    enum vector_status_code status_code = vector_ok;
    unsigned int POOL = 10;
    int differ = 1000;
    srand(time(NULL));
    int close;
    vector massive_a, massive_b;
    create_vector(&massive_a, POOL, &status_code);
    create_vector(&massive_b, POOL, &status_code);

    for (int i = 0; i < POOL; i++){
        push_back(&massive_a, rand() % differ, &status_code);
        push_back(&massive_b, rand() % differ, &status_code);
    }

    qsort(massive_b.data, POOL, sizeof(int), compare);

    for (int i = 0; i < POOL; i++){
        close = binary_find(&massive_b, massive_a.data[i]);
        push_back(result_insert, (close + massive_a.data[i]), &status_code);
    }

    free_vector(&massive_a);
    free_vector(&massive_b);
    return dynamic_insert_ok;
}