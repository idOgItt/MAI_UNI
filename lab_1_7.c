#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum copy_lexem_status_code { copy_lexem_ok, copy_lexem_open_failure };
enum copy_special_lexem_status_code { copy_special_lexem_ok, copy_special_lexem_open_failure };

enum copy_lexem_status_code copy_lexem(char* path_file_input_1, char* path_file_input_2, char* path_output_file);
enum copy_special_lexem_status_code copy_special_lexem(char* path_file_input_1, char* path_output_file);

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Параметры для программы %s: <path_file_1> optional(<path_file_2>) <output_file>\n", argv[0]);
        return 1;
    }

    char* flag = argv[1];

    if ((flag[0] != '/') && (flag[0] != '-')) {
        printf("Введен неправильный символ для флага, используйте '/' или '-'\n");
        return 1;
    }

    if (argc < 4) {
        printf("Недостаточно параметров для выполнения команды\n");
        return 1;
    }

    char* path_file_input_1;
    char* path_file_input_2;
    char* path_output_file;

    switch (flag[1]) {

    case 'r':

        if (argc != 5) {
            printf("Неверное количество параметров. Используйте %s /r <file_1> <file_2> <output_file>\n", argv[0]);
            return 1;
        }

        path_file_input_1 = argv[2];
        path_file_input_2 = argv[3];
        path_output_file = argv[4];

        switch (copy_lexem(path_file_input_1, path_file_input_2, path_output_file)) {
        case copy_lexem_ok:
            printf("Процесс завершен. Результат записан в %s.\n", path_output_file);
            break;

        case copy_lexem_open_failure:
            printf("Возникла проблема при открытии файлов\n");
            break;
        }
        break;

    case 'a':

        if (argc != 4) {
            printf("Неверное количество параметров. Используйте %s /a <file_1> <output_file>\n", argv[0]);
            return 1;
        }

        path_file_input_1 = argv[2];
        path_output_file = argv[3];

        switch (copy_special_lexem(path_file_input_1, path_output_file)) {
        case copy_special_lexem_ok:
            printf("Процесс завершен. Результат записан в %s.\n", path_output_file);
            break;

        case copy_special_lexem_open_failure:
            printf("Возникла проблема при открытии файлов\n");
            break;
        }
        break;

    default:
        printf("Неизвестный флаг %s\n", flag);
        return 1;
    }

    return 0;
}

enum copy_lexem_status_code copy_lexem(char* path_file_input_1, char* path_file_input_2, char* path_output_file) {
    FILE* file1, * file2, * output_file;
    char* lexem_1 = NULL;
    char* lexem_2 = NULL;

    // Открываем два входных файла для чтения
    file1 = fopen(path_file_input_1, "r");
    file2 = fopen(path_file_input_2, "r");

    // Проверяем, удалось ли открыть файлы
    if (file1 == NULL || file2 == NULL) {
        return copy_lexem_open_failure;
    }

    // Открываем выходной файл для записи
    output_file = fopen(path_output_file, "w");

    if (output_file == NULL) {
        return copy_lexem_open_failure;
    }

    // Читаем лексемы из обоих файлов и записываем их в выходной файл
    while (fscanf(file1, "%ms", &lexem_1) == 1 && fscanf(file2, "%ms", &lexem_2) == 1) {
        fprintf(output_file, "%s %s ", lexem_1, lexem_2);
        free(lexem_1); // Освобождаем память после использования
        free(lexem_2);
        lexem_1 = NULL; // Обнуляем указатели
        lexem_2 = NULL;
    }

    // Записываем оставшиеся лексемы из первого файла
    while (fscanf(file1, "%ms", &lexem_1) == 1) {
        fprintf(output_file, "%s ", lexem_1);
        free(lexem_1);
        lexem_1 = NULL;
    }

    // Записываем оставшиеся лексемы из второго файла
    while (fscanf(file2, "%ms", &lexem_2) == 1) {
        fprintf(output_file, "%s ", lexem_2);
        free(lexem_2);
        lexem_2 = NULL;
    }

    // Закрываем все файлы
    fclose(file1);
    fclose(file2);
    fclose(output_file);

    return copy_lexem_ok;
}


enum copy_special_lexem_status_code copy_special_lexem(char* path_file_input_1, char* path_output_file) {
    FILE* file1, * output_file;
    char* lexem = NULL;
    int n = 0;

    // Открываем входной файл для чтения
    file1 = fopen(path_file_input_1, "r");

    // Проверяем, удалось ли открыть файл
    if (file1 == NULL) {
        return copy_special_lexem_open_failure;
    }

    // Открываем выходной файл для записи
    output_file = fopen(path_output_file, "w");

    if (output_file == NULL) {
        return copy_special_lexem_open_failure;
    }

    // Читаем лексемы из входного файла и записываем их в выходной файл
    while (fscanf(file1, "%ms", &lexem) == 1) {
        n++;

        if (n % 10 == 0) {
            for (int i = 0; lexem[i] != '\0'; i++) {
                if (lexem[i] >= 'A' && lexem[i] <= 'Z') {
                    lexem[i] = lexem[i] - 'A' + 'a';
                }
                // Преобразуем символ в ASCII и записываем его в системе счисления с основанием 4
                fprintf(output_file, "%o", lexem[i]);
            }
            fprintf(output_file, " ");
        }

        if ((n % 2 == 0) && (n % 10 != 0)) {
            for (int i = 0; lexem[i] != '\0'; i++) {
                if (lexem[i] >= 'A' && lexem[i] <= 'Z') {
                    lexem[i] = lexem[i] - 'A' + 'a';
                }
                fprintf(output_file, "%c", lexem[i]);
            }
            fprintf(output_file, " ");
        }

        if ((n % 5 == 0) && (n % 10 != 0)) {
            for (int i = 0; lexem[i] != '\0'; i++) {
                // Преобразуем символ в ASCII и записываем его в систему счисления с основанием 8 (восьмеричной системе)
                fprintf(output_file, "%o", lexem[i]);
            }
            fprintf(output_file, " ");
        }

        else {
            for (int i = 0; lexem[i] != '\0'; i++) {
                fprintf(output_file, "%c", lexem[i]);
            }
            fprintf(output_file, " ");
        }

        free(lexem); // Освобождаем память после использования
        lexem = NULL; // Обнуляем указатель
    }

    // Закрываем все файлы
    fclose(file1);
    fclose(output_file);

    return copy_special_lexem_ok;
}

