#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> // для length
#include <time.h>


enum handle_input_file_status_code {handle_input_file_ok, handle_input_file_fail};

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

typedef struct  Hash_Cash_Node{
    unsigned int key; // The key associated with the hash
    unsigned int hash;  // The calculated hash value
    struct Hash_Cash_Node* next;
} Hash_Cash_Node;

typedef struct {
    Hash_Cash_Node* entries;
    int size;
} Hash_Cache;

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

// Function to check if the number is prime
bool is_prime(int num) {
    if (num < 2) {
        return false;
    }

    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) {
            return false;
        }
    }

    return true;
}

// Prime_number > given_number
int next_prime_after(int n) {
    int next_number = n + 1;

    while (!is_prime(next_number)) {
        ++next_number;
    }

    return next_number;
}

// Function to create the table
void create_cache(Hash_Cache* cache, int size){
    cache->entries = (Hash_Cash_Node*)malloc(size * sizeof(Hash_Cash_Node));
    cache->size = size;
}

// Function to calculate the key value of a <def_name>
unsigned int calculate_key(const char* string) {
    int base = 62;
    unsigned int key = 0;

    while (*string) {
        int digit = 0;

        if (*string >= 'a' && *string <= 'z') {
            digit = *string - 'a' + 36;
        } else if (*string >= 'A' && *string <= 'Z') {
            digit = *string - 'A' + 10;
        } else if (*string >= '0' && *string <= '9') {
            digit = *string - '0';
        }

        key = key * base + digit;
        string++;
    }

    return key;
}

// Function to calculate hash value;
unsigned int calculate_hash(const char* string, int HASHSIZE){
    unsigned int key = calculate_key(string);
    unsigned int hash = key % HASHSIZE;
    return hash;
}

// Function to get values from hash-table if not to fill in it
unsigned int get_hash_from_cache(Hash_Cache* cache, const char* string_key, int HASHSIZE){
    unsigned int key = calculate_key(string_key);
    unsigned int hash = key % HASHSIZE;
    
    Hash_Cash_Node* node = &cache->entries[hash];
    while (node->next != NULL) {
        if (node->next->key == key) {
            return node->next->hash;
        }
        node = node->next;
    }

    // The entry is not in the cache, calculate the hash and add a new node to the chain
    node = (Hash_Cash_Node*)malloc(sizeof(Hash_Cash_Node));
    node->next->key = key;
    node->next->hash = hash;
    node->next->next = NULL;

    return node->next->hash;
}

// Function to free memory allocated for the hash cache
void free_hash_cache(Hash_Cache* cache) {
    for (int i = 0; i < cache->size; i++) {
        Hash_Cash_Node* current = cache->entries[i].next;
        while (current != NULL) {
            Hash_Cash_Node* next = current->next;
            free(current);
            current = next;
        }
    }
}

// Function to rehash the table
void rehash_cache(Hash_Cache* old_cache, int new_size){
    Hash_Cache new_cache;
    create_cache(&new_cache, new_size);

    for (int i = 0; i < old_cache->size; i++){
        Hash_Cash_Node* node = &old_cache->entries[i];
        while (node->next != NULL){
            unsigned int key = node->next->key;
            unsigned int hash = node->next->hash;
            unsigned int new_hash = key % new_size;

            Hash_Cash_Node* new_node = &new_cache.entries[new_hash];
            while (new_node->next != NULL){
                new_node = new_node->next;
            }

            new_node->next = (Hash_Cash_Node*)malloc(sizeof(Hash_Cash_Node));
            new_node->next->key = key;
            new_node->next->hash = hash;
            new_node->next->next = NULL;
            
            // Move to next node and delete old one
            Hash_Cash_Node* temp = node->next;
            node->next = node->next->next;
            free(temp);
        }
    }

    free_hash_cache(old_cache);

    memcpy(old_cache, &new_cache, sizeof(Hash_Cache));
}

// Function to check the load_factor of chains, to resize or not
void check_and_resize(Hash_Cache* cache) {
    int LOAD_FACTOR = 2;
    int shortest_chain = 2;
    int longest_chain = 0;

    // Calculate chain lengths
    for (int i = 0; i < cache->size; i++) {
        int chain_length = 0;
        Hash_Cash_Node* node = &cache->entries[i];
        while (node->next != NULL) {
            chain_length++;
            node = node->next;
        }

        // Update shortest and longest chains
        if (chain_length < shortest_chain) {
            shortest_chain = chain_length;
        }
        if (chain_length > longest_chain) {
            longest_chain = chain_length;
        }
    }

    // Check for significant difference
    if (longest_chain >= LOAD_FACTOR * shortest_chain) {
        int new_size = next_prime_after(cache->size);
        rehash_cache(cache, new_size);
    }
}

enum handle_input_file_status_code handle_input_file(char* input_file_path){
    Cell **result;
    int num_files = 1;
    FILE* input_file = fopen(input_file_path, "r");
    FILE* output_file = fopen("lab_4_1.txt", "w");
    char* start_string = "#define ";
    char* pattern;


    statements stm = find_all_patterns(&result, pattern, num_files, input_file);

    if (stm == correct) {
        for (int i = 0; i < num_files; i++) {
            printf("Results for %s:\n", result[i]->path);

            for (int j = 0; j < result[i]->cnt; j++) {
                printf("Pattern found at line %d, index %d\n", result[i]->string_ns[j], result[i]->char_ns[j]);
            }

            printf("\n");
        }
    }

    // Don't forget to free the memory allocated for the results
    free_Cells(result, num_files);


    return handle_input_file_ok;
}


int main(int argc, char* argv[]){
    char* input_file_path = argv[1];
    if (handle_input_file(input_file_path) == handle_input_file_ok){
        printf("File is changed successfully\n");
    }

    return 0;
}
