#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> // для length
#include <time.h>

typedef enum {
    unknow_state,
    correct,
    running_error,
    file_error,
    memory_error,
    all_nums,
} statements;

typedef struct {
    int *string_ns;
    int *char_ns;
    int cnt;
    char *path;
    statements stm;
} Cell;

bool is_equal_str(char *str1, char *str2, int l) {
    for (int i = 0; i < l; i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}

void move_char_left(char *str, int l) {
    if (l == 1) {
        return;
    }
    for (int i = 0; i < l - 1; i++) {
        str[i] = str[i + 1];
    }
}

void move_int_left(int *str, int l) {
    if (l == 1) {
        return;
    }
    for (int i = 0; i < l - 1; i++) {
        str[i] = str[i + 1];
    }
}

void free_Cell(Cell *c) {
    if (c == NULL) {
        return;
    }
    free(c->char_ns);
    free(c->string_ns);
    free(c);
}

void free_Cells(Cell **c, int n) {
    for (int i = 0; i < n; i++) {
        free_Cell(c[i]);
    }
    free(c);
}

statements find_pattern(char *pattern, char *buffer, int l, FILE *file, Cell **cell) {
    if (l == 0) {
        Cell *cel = (Cell *)malloc(sizeof(Cell));
        cel->stm = all_nums;
        *cell = cel;
        return correct;
    }

    size_t bytes_read = 0;
    int buffer_size = 2;

    int *mas = (int *)malloc(sizeof(int) * l + 1);
    if (mas == NULL) {
        return memory_error;
    }

    int *str_i = (int *)malloc(sizeof(int) * buffer_size);
    if (str_i == NULL) {
        free(mas);
        return memory_error;
    }

    int *ind_i = (int *)malloc(sizeof(int) * buffer_size);
    if (ind_i == NULL) {
        free(mas);
        free(str_i);
        return memory_error;
    }

    char c;

    int string_n = 1, char_n = 1;
    int fake2 = 0;
    for (int i = 0; i < l; i++) {
        if (pattern[i] == '\n') {
            fake2++;
        }
    }

    mas[0] = 1;
    while (bytes_read != l && (c = fgetc(file)) != EOF) {
        char_n++;
        if (c == '\n') {
            mas[bytes_read] = 1;
            string_n++;
            char_n = 1;
        } else if (bytes_read > 0) {
            mas[bytes_read] = mas[bytes_read - 1] + 1;
        }
        buffer[bytes_read] = c;
        bytes_read++;
    }

    if (bytes_read != l) {
        free(mas);
        return file_error;
    }
    int cnt = 0;
    if (is_equal_str(pattern, buffer, l)) {
        ind_i[cnt] = 1;
        str_i[cnt] = 1;
        cnt++;
    }
    char_n++;

    while ((c = fgetc(file)) != EOF) {
        move_char_left(buffer, l);
        move_int_left(mas, l);

        if (c == '\n') {
            string_n++;
            mas[l - 1] = 1;
        } else if (l - 2 > 0) {
            mas[l - 1] = mas[l - 2] + 1;
        } else {
            mas[l - 1] = mas[l - 1] + 1;
        }

        buffer[l - 1] = c;
        if (is_equal_str(pattern, buffer, l)) {
            if (buffer_size < cnt + 1) {
                buffer_size *= 2;
                int *new_str_i = (int *)realloc(str_i, sizeof(int) * buffer_size);
                if (new_str_i == NULL) {
                    free(str_i);
                    free(ind_i);
                    return memory_error;
                }
                str_i = new_str_i;

                int *new_ind_i = (int *)realloc(ind_i, sizeof(int) * buffer_size);
                if (new_ind_i == NULL) {
                    free(str_i);
                    free(ind_i);
                    return memory_error;
                }
                ind_i = new_ind_i;
            }
            ind_i[cnt] = l > 1 ? mas[0] - 1 : mas[0];
            str_i[cnt] = string_n - fake2;
            cnt++;
        }
        char_n++;
    }

    Cell *cel = (Cell *)malloc(sizeof(Cell));
    cel->char_ns = ind_i;
    cel->cnt = cnt;
    cel->string_ns = str_i;
    cel->stm = correct;
    *cell = cel;

    free(mas);
    return correct;
}

statements find_all_patterns(Cell ***result, char *pattern, int num, ...) {
    if (num <= 0) {
        return running_error;
    }

    int l = strlen(pattern);
    char *buffer = (char *)malloc(sizeof(char) * (l + 1));

    if (buffer == NULL) {
        return running_error;
    }

    va_list args;
    va_start(args, num);

    Cell **cells = (Cell **)malloc(sizeof(Cell *) * num);

    if (cells == NULL) {
        free(buffer);
        return memory_error;
    }

    for (int i = 0; i < num; i++) {
        char *path = va_arg(args, char *);
        FILE *file = fopen(path, "r");
        if (file == NULL) {
            free_Cells(cells, num);
            free(buffer);
            return file_error;
        }

        Cell *local_res = NULL;

        statements stm = find_pattern(pattern, buffer, l, file, &local_res);

        if (stm != correct) {
            free_Cells(cells, num);
            free_Cell(local_res);
            free(buffer);
            return stm;
        }

        local_res->path = path;
        cells[i] = local_res;

        fclose(file);
    }

    va_end(args);
    free(buffer);
    *result = cells;

    return correct;
}


int main() {
    char *pattern = "asdsadsadas";  // Replace with your pattern
    int num_files = 1;  // Replace with the number of files you want to search

    // Replace these file paths with the paths of the files you want to search
    char *file1 = "file1.txt";

    Cell **result;

    statements stm = find_all_patterns(&result, pattern, num_files, file1);

    if (stm == correct) {
        for (int i = 0; i < num_files; i++) {
            printf("Results for %s:\n", result[i]->path);

            for (int j = 0; j < result[i]->cnt; j++) {
                printf("Pattern found at line %d, index %d\n", result[i]->string_ns[j], result[i]->char_ns[j]);
            }

            printf("\n");
        }

        // Don't forget to free the memory allocated for the results
        free_Cells(result, num_files);
    } else {
        printf("Error: ");

        switch (stm) {
            case running_error:
                printf("Running error\n");
                break;
            case file_error:
                printf("File error\n");
                break;
            case memory_error:
                printf("Memory error\n");
                break;
            default:
                printf("Unknown error\n");
        }
    }

    return 0;
}
