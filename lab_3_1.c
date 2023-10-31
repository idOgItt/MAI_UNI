#include <stdio.h>
#include <stdlib.h>

enum number_to_base_status_code { number_to_base_ok, number_to_base_fail };

enum number_to_base_status_code number_to_base(int num, int** result);

int main() {
    int num = 8;
    int** result = NULL;

    result = (int**)malloc(sizeof(int*) * 5);

    if (result == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        result[i] = NULL;
    }

    if (number_to_base(num, result) == number_to_base_ok) {
        for (int i = 1; i <= 5; i++) {
            printf("The result in Base-%d: ", 1 << i);
            int j = 0;
            while (result[i-1][j] != -1) {
                printf("%d ", result[i-1][j]);
                j++;
            }
            printf("\n");
        }
    }

    for (int i = 0; i < 5; i++) {
        free(result[i]);
    }
    free(result);

    return 0;
}

enum number_to_base_status_code number_to_base(int init_num, int** result) {
    int mask = ~1;
    for (int base = 1; base <= 5; base++){
        int num = init_num;
        int i = 0;

        // Allocate memory for the result array
        result[base - 1] = (int*)malloc(sizeof(int) * 20);

        while (num > 0) {
            int remainder = num & ((1 << base) - 1);
            result[base - 1][i] = remainder;
            num >>= base;
            i++;
        }

        result[base - 1][i] = -1;
        /*base = base & mask;
        base = base | (mask << 1);*/
    }
    return number_to_base_ok;
}
