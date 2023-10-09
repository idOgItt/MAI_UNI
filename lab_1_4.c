#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum arabic_number_status_code { arabic_number_file_invalid, arabic_number_ok};

enum latin_letters_status_code { latin_letters_file_invalid, latin_letters_ok};

enum exception_letters_status_code { 
    exception_letters_ok, 
    exception_letters_file_invalid
};

enum replace_letters_status_code{
    replace_letters_ok,
    replace_letters_file_invalid
};

enum arabic_number_status_code arabic_number(const char* file_path, char* output_file_path);

enum latin_letters_status_code latin_letters(const char* file_path, char* output_file_path);

enum exception_letters_status_code exception_letters(const char* file_path, const char* output_file_path);

enum replace_letters_status_code replace_non_alphanumeric(const char* file_path, const char* output_file_path);

int main (int argc, char* argv[]){
    if (argc < 3){
        printf("Недостаточное количество параметров\n");
        return 1;
    }

    char* flag = argv[1];
    const char* file_path = argv[2];
    char* output_file_path = NULL;

    if (flag[0] != '-' && flag[0] != '/'){
        printf("Неправильный флаг: %c\n", flag[0]);
        return 1;
    }

    if (flag[1] == 'n'){
        if (argc != 4){
            printf("Ожидается адрес на выходной файл\n");
            return 1;
        }
        else {
            output_file_path = argv[3];
        }
    }

    if (argc )

    switch(flag[1]){
        case 'd':
            switch (arabic_number(file_path, output_file_path))
            {
            case arabic_number_ok:
                printf("Файл был изменен");
                break;
            
            case arabic_number_file_invalid:
                printf("Не удалось открыть файл\n");
                return 1;
            }
            break;
        case 'i':
            switch (latin_letters(file_path, output_file_path))
            {
            case latin_letters_ok:
                printf("Файл был изменен");
                break;
            
            case latin_letters_file_invalid:
                printf("Не удалось открыть файл\n");
                return 1;
            }
            break;
        case 's':
            switch (exception_letters(file_path, output_file_path))
            {
            case exception_letters_ok:
                printf("Файл был изменен");
                break;
            
            case exception_letters_file_invalid:
                printf("Не удалось открыть файл\n");
                return 1;
            }
            break;
        case 'a':
            switch (replace_non_alphanumeric(file_path, output_file_path))
            {
            case replace_letters_ok:
                printf("Файл был изменен");
                break;
            
            case replace_letters_file_invalid:
                printf("Не удалось открыть файл\n");
                return 1;
            }
            break;
    }

    return 0;
}

enum arabic_number_status_code arabic_number(const char* file_path, char* output_file_path){
    FILE* input_file = fopen(file_path, "r");
    FILE* output_file;

    if (output_file_path == NULL) {
        output_file = fopen("out_file.txt", "w");
    } else {
        output_file = fopen(output_file_path, "w");
    }

    if (input_file == NULL || output_file == NULL) {
        return arabic_number_file_invalid;
    }

    int ch;
    while ((ch = fgetc(input_file)) != EOF) {
        if (!isdigit(ch)) {
            fputc(ch, output_file);
        }
    }

    fclose(input_file);
    fclose(output_file);

    return arabic_number_ok;
}

enum latin_letters_status_code latin_letters(const char* file_path, char* output_file_path){

    FILE* input_file = fopen(file_path, "r");
    FILE* output_file;

    if (input_file == NULL) {
        return latin_letters_file_invalid;
    }

    if (output_file_path == NULL) {
        output_file = fopen("out_file.txt", "w");
    } else {
        output_file = fopen(output_file_path, "w");
    }

    if (output_file == NULL) {
        fclose(input_file);
        return latin_letters_file_invalid;
    }

    int ch;
    int count = 0;
    int new_line_flag = 1; // Флаг для проверки начала новой строки

    while ((ch = fgetc(input_file)) != EOF) {
        if (isalpha(ch)) {
            count++;
        }

        // Записываем символ в выходной файл
        fputc(ch, output_file);

        if (ch == '\n') {
            // Если это символ новой строки, записываем количество букв и сбрасываем счетчик
            if (new_line_flag) {
                fprintf(output_file, " Letters Count: %d\n", count);
                count = 0;
                new_line_flag = 0;
            }
        } else {
            new_line_flag = 1;
        }
    }

    if (new_line_flag) {
        fprintf(output_file, " Letters Count: %d\n", count);
    }

    fclose(input_file);
    fclose(output_file);

    return latin_letters_ok;
}


enum exception_letters_status_code exception_letters(const char* file_path, const char* output_file_path) {
    FILE* input_file = fopen(file_path, "r");
    FILE* output_file;

    if (input_file == NULL) {
        return exception_letters_file_invalid;
    }

    if (output_file_path == NULL) {
        output_file = fopen("out_file.txt", "w");
    } else {
        output_file = fopen(output_file_path, "w");
    }

    if (output_file == NULL) {
        fclose(input_file);
        return exception_letters_file_invalid;
    }

    int ch;
    int count = 0;
    int new_line_flag = 1; // Флаг для проверки начала новой строки

    while ((ch = fgetc(input_file)) != EOF) {
        if (!isalpha(ch) && !isdigit(ch) && (ch != ' ')) {
            count++;
        }

        // Записываем символ в выходной файл
        fputc(ch, output_file);

        if (ch == '\n') {
            if (new_line_flag) {
                fprintf(output_file, " Non-Letters Count: %d\n", count);
                count = 0;
                new_line_flag = 0;
            }
        } else {
            new_line_flag = 1;
        }
    }

    if (new_line_flag) {
        fprintf(output_file, "\nNon-Letters Count: %d", count);
    }

    fclose(input_file);
    fclose(output_file);

    return exception_letters_ok;
}

enum replace_letters_status_code replace_non_alphanumeric(const char* file_path, const char* output_file_path) {
    FILE* input_file = fopen(file_path, "r");
    FILE* output_file;

    if (input_file == NULL) {
        return replace_letters_file_invalid;
    }

    if (output_file_path == NULL) {
        output_file = fopen("out_file.txt", "w");
    } else {
        output_file = fopen(output_file_path, "w");
    }

    if (output_file == NULL) {
        fclose(input_file);
        return replace_letters_file_invalid;
    }

    int ch;

    while ((ch = fgetc(input_file)) != EOF) {
        if (isalnum(ch) || isspace(ch)) {
            fputc(ch, output_file);
        } else {
            fprintf(output_file, "%02X", ch);
        }
    }

    fclose(input_file);
    fclose(output_file);

    return replace_letters_ok;
}