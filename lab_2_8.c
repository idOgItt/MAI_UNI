#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

enum sum_string_status_code {
    sum_string_ok,
    sum_string_fail
};

enum sum_string_status_code sum_string(char** result, int base, int count, ...);

int main() {
    int base = 10;
    int count = 3;
    char* string_1 = "960";
    char* string_2 = "960";
    char* string_3 = "00";
    char* result = NULL;

    if (sum_string(&result, base, count, string_1, string_2, string_3) == sum_string_ok) {
        printf("The resulted sum is %s\n", result);
        free(result);
    } else {
        printf("The mistake occurred\n");
        return 1;
    }
    return 0;
}

enum sum_string_status_code sum_string(char** result, int base, int count, ...) {
    va_list strings;
    va_start(strings, count);

    *result = (char*)malloc(1024);
    if (*result == NULL) {
        va_end(strings);
        return sum_string_fail;
    }
    (*result)[0] = '\0';

    int carry = 0;

    int max_length = 0;
    int i;
    char** string_array = (char**)malloc(count * sizeof(char*));

    if (string_array == NULL) {
        free(*result);
        va_end(strings);
        return sum_string_fail;
    }

    for (i = 0; i < count; i++) {
        string_array[i] = va_arg(strings, char*);
        int length = strlen(string_array[i]);
        if (length > max_length) {
            max_length = length;
        }
    }

    va_end(strings);

    for (i = 0; i < max_length; i++) {
        int sum = 0;

        for (int j = 0; j < count; j++) {
            char* string = string_array[j];
            int length = strlen(string);

            if (i < length) {
                char ch = string[length - 1 - i];
                int digit = (ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'A' + 10);

                if (digit < 0 || digit >= base) {
                    free(*result);
                    free(string_array);
                    return sum_string_fail;
                }

                sum += digit;
            }
        }

        sum += carry;
        carry = sum / base;
        sum %= base;

        char digit_char = (sum < 10) ? ('0' + sum) : ('A' + sum - 10);

        memmove((*result) + 1, *result, strlen(*result) + 1);

        (*result)[0] = digit_char;
    }

    while (carry) {
        int sum = carry;
        carry = sum / base;
        sum %= base;

        char digit_char = (sum < 10) ? ('0' + sum) : ('A' + sum - 10);

        memmove((*result) + 1, *result, strlen(*result) + 1);

        (*result)[0] = digit_char;
    }

    free(string_array);

    return sum_string_ok;
}