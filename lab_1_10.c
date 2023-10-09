#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

long long int char_to_number(const char* token, int base);
char* base_to(int base, long long int max_number);

int main() {
    int base;
    printf("Введите основание системы счисления (от 2 до 36): ");
    scanf("%d", &base);

    if (base < 2 || base > 36) {
        printf("Основание системы счисления должно быть от 2 до 36.\n");
        return 1;
    }

    char input[100];
    long long int max_number = 0;
    char* base_9_representation = NULL;
    char* base_18_presentation = NULL;
    char* base_27_presentation = NULL;
    char* base_36_presentation = NULL;
    char* base_representation = NULL;

    while (1) {
        printf("Введите число в системе счисления %d (или 'Stop' для завершения): ", base);
        scanf("%s", input);

        if (strcmp(input, "Stop") == 0) {
            break;
        }

        char* token = strtok(input, " ");

        while (token != NULL) {
            // Проверяем, что введенное значение находится в допустимом диапазоне для выбранной системы счисления
            long long int number = char_to_number(token, base);
            int valid = 1;
            int len = strlen(token);
            for (int i = 0; i < len; i++) {
                if ((token[i] >= '0' && token[i] < '0' + base) || (token[i] >= 'A' && token[i] < 'A' + base - 10)) {
                    continue;
                } else {
                    valid = 0;
                    break;
                }
            }

if (!valid) {
    printf("Неверное значение для системы счисления %d: %s\n", base, token);
} else {
    // Сравнение с текущим максимальным числом по модулю
    long long int number = char_to_number(token, base);
    if (llabs(number) > llabs(max_number)) {
        max_number = number;
    }
}


            token = strtok(NULL, " ");
        }
    }

    // Вызываем функции для получения представлений числа в разных системах счисления
    base_representation = base_to(base, max_number);
    base_9_representation = base_to(9, max_number);
    base_18_presentation = base_to(18, max_number);
    base_27_presentation = base_to(27, max_number);
    base_36_presentation = base_to(36, max_number);

    printf("Максимальное число по модулю в системе по основанию 10: %lld\n", max_number);
    printf("Представление в системе счисления %d: %s\n", base, base_representation);
    printf("Представление в системе счисления 9: %s\n", base_9_representation);
    printf("Представление в системе счисления 18: %s\n", base_18_presentation);
    printf("Представление в системе счисления 27: %s\n", base_27_presentation);
    printf("Представление в системе счисления 36: %s\n", base_36_presentation);

    // Освобождаем память для представлений числа
    free(base_representation);
    free(base_9_representation);
    free(base_18_presentation);
    free(base_27_presentation);
    free(base_36_presentation);

    return 0;
}

long long int char_to_number(const char* token, int base) {
    long long int decimalValue = 0;
    int len = strlen(token);
    for (int i = 0; i < len; i++) {
        int digitValue;
        if (token[i] >= '0' && token[i] <= '9') {
            digitValue = token[i] - '0';
        } else if (token[i] >= 'A' && token[i] <= 'Z') {
            digitValue = token[i] - 'A' + 10;
        } else {
            continue;
        }
        decimalValue = decimalValue * base + digitValue;
    }
    return decimalValue;
}

char* base_to(int base, long long int max_number) {
    if (max_number == 0) {
        char* result = (char*)malloc(2);
        if (result == NULL) {
            return NULL; // Обработка ошибки, если не удалось выделить память
        }
        result[0] = '0';
        result[1] = '\0';
        return result;
    }

    int max_digits = (int)(log(llabs(max_number)) / log(base)) + 1;

    char* result = (char*)malloc(max_digits + 1);
    if (result == NULL) {
        return NULL; // Обработка ошибки, если не удалось выделить память
    }

    for (int i = max_digits - 1; i >= 0; i--) {
        int digit = llabs(max_number) % base;
        char digit_char;

        if (digit < 10) {
            digit_char = '0' + digit;
        } else {
            digit_char = 'A' + digit - 10;
        }

        result[i] = digit_char;
        max_number /= base;
    }

    result[max_digits] = '\0';

    return result;
}
