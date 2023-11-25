#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

enum handle_input_file_status_code {handle_input_file_ok, handle_input_file_fail};

enum insert_into_cache_file_status_code {insert_into_cache_file_exists, insert_into_cache_file_ok};

enum insert_into_cache_status_code {insert_into_cache_ok, insert_into_cache_fail};

enum create_cache_status_code {create_cache_ok, create_cache_fail};

enum free_hash_cache_status_code {free_hash_cache_ok};

enum rehash_cache_status_code {rehash_cache_ok, rehash_cache_fail};

enum check_and_resize_status_code {check_and_resize_ok, check_and_resize_fail};

typedef struct  Hash_Cash_Node{
    unsigned int key; // The key associated with the hash
    unsigned int hash;  // The calculated hash value
    char* value;
    struct Hash_Cash_Node* next;
} Hash_Cash_Node;

typedef struct {
    Hash_Cash_Node* entries;
    int size;
} Hash_Cache;

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

void trim(char *str) {
    int length = strlen(str);
    int start = 0;
    int end = length - 1;

    // Trim leading spaces and newline characters
    while (start < length && (str[start] == ' ' || str[start] == '\n')) {
        start++;
    }

    // Trim trailing spaces and newline characters
    while (end > start && (str[end] == ' ' || str[end] == '\n')) {
        end--;
    }

    // Shift characters to the beginning
    for (int i = start; i <= end; i++) {
        str[i - start] = str[i];
    }

    // Null-terminate the trimmed string
    str[end - start + 1] = '\0';
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
enum create_cache_status_code create_cache(Hash_Cache* cache, int size){
    cache->entries = (Hash_Cash_Node*)malloc(size * sizeof(Hash_Cash_Node));
    if (cache->entries == NULL){
        return create_cache_fail;
    }
    cache->size = size;
    return create_cache_ok;
}

// Function to calculate the key value of a <def_name>
unsigned int calculate_key(const char* string) {
    int base = 62;
    unsigned int key = 0;

    while (*string) {
        int digit = 0;
        
        if (*string >= 'a' && *string <= 'z') {
            digit = *string - 'a' + 36;
            key = key * base + digit;
        } else if (*string >= 'A' && *string <= 'Z') {
            digit = *string - 'A' + 10;
            key = key * base + digit;
        } else if (*string >= '0' && *string <= '9') {
            digit = *string - '0';
            key = key * base + digit;
        }

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
const char* get_value_from_cache(Hash_Cache* cache, const char* string_key, int HASHSIZE) {
    unsigned int key = calculate_key(string_key);
    unsigned int hash = calculate_hash(string_key, HASHSIZE);

    Hash_Cash_Node* node = &cache->entries[hash];
    while (node->next != NULL) {
        if (node->next->key == key) {
            return node->next->value;
        }
        node = node->next;
    }

    // The entry is not in the cache
    return NULL;
}


// Function to free memory allocated for the hash cache
enum free_hash_cache_status_code free_hash_cache(Hash_Cache* cache) {
    for (int i = 0; i < cache->size; i++) {
        Hash_Cash_Node* current = cache->entries[i].next;
        while (current != NULL) {
            Hash_Cash_Node* next = current->next;
            free(current->value);
            free(current);
            current = next;
        }
        // Set the next pointer to NULL after freeing the chain
        cache->entries[i].next = NULL;
    }
    free(cache->entries);
    return free_hash_cache_ok;
}



// Function to rehash the table
enum rehash_cache_status_code rehash_cache(Hash_Cache** old_cache, Hash_Cache** new_cache, int new_size) {
    // Create a new cache
    Hash_Cache* temp_new_cache = (Hash_Cache*)malloc(sizeof(Hash_Cache));
    
    if (temp_new_cache == NULL){
        return rehash_cache_fail;
    }

    if (create_cache(temp_new_cache, new_size) == create_cache_fail){
        return rehash_cache_fail;
    }

    for (int i = 0; i < (*old_cache)->size; i++) {
        Hash_Cash_Node* node = &((*old_cache)->entries[i]);
        while (node->next != NULL) {
            unsigned int key = node->next->key;
            char* value = node->next->value;
            unsigned int new_hash = key % new_size;

            Hash_Cash_Node* new_node = &((temp_new_cache)->entries[new_hash]);
            while (new_node->next != NULL) {
                new_node = new_node->next;
            }

            new_node->next = (Hash_Cash_Node*)malloc(sizeof(Hash_Cash_Node));
            
            if (temp_new_cache == NULL){
                return rehash_cache_fail;
            }

            new_node->next->key = key;
            new_node->next->hash = new_hash;
            new_node->next->value = value;
            new_node->next->next = NULL;

            node->next = node->next->next;
        }
    }

    // Free the old cache
    free_hash_cache(*old_cache);

    // Update the size of the old cache
    (*old_cache)->size = new_size;

    // Swap old_cache and temp_new_cache
    Hash_Cache* temp = *old_cache;
    *old_cache = temp_new_cache;
    temp_new_cache = temp;
    
    // Update the new_cache pointer in the calling code
    *new_cache = temp_new_cache;
    return rehash_cache_ok;
}


// Function to check the load_factor of chains, to resize or not
enum check_and_resize_status_code check_and_resize(Hash_Cache* cache, Hash_Cache* new_cache) {
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
        
        if (chain_length > longest_chain) {
            longest_chain = chain_length;
        }
    }

    // Check for significant difference
    if (longest_chain >= LOAD_FACTOR * shortest_chain) {
        int new_size = next_prime_after(cache->size);
        if (rehash_cache(&cache, &new_cache, new_size) == rehash_cache_fail){
            return check_and_resize_fail;
        }
    }
    return check_and_resize_ok;
}

// Function to insert a key and hash into the hash table
enum insert_into_cache_status_code insert_into_cache(Hash_Cache* cache, unsigned int key, unsigned int hash, const char* value, Hash_Cache* new_cache) {

    // Element does not exist, proceed with insertion
    Hash_Cash_Node* new_node = (Hash_Cash_Node*)malloc(sizeof(Hash_Cash_Node));
    
    if (new_node == NULL){
        return insert_into_cache_fail;
    }

    new_node->key = key;
    new_node->hash = hash;
    new_node->value = malloc(strlen(value) + 1);
    strcpy(new_node->value, value);
    new_node->value[strlen(value)] = '\0'; // NULL Terminate
    new_node->next = cache->entries[hash].next;
    cache->entries[hash].next = new_node;

    // Call check_and_resize after inserting an element
    if (check_and_resize(cache, new_cache) == check_and_resize_fail){
        return insert_into_cache_fail;
    }
    return insert_into_cache_ok;
}

enum insert_into_cache_file_status_code insert_into_cache_file(Hash_Cache* cache, const char* string_key, const char* value, Hash_Cache* new_cache){

    const char* existing_value = get_value_from_cache(cache, string_key, cache->size);
    if (existing_value != NULL) {
        // Element already exists, no need to insert
        return insert_into_cache_file_exists;
    }

    unsigned int key = calculate_key(string_key);
    unsigned int hash = calculate_hash(string_key, cache->size);
    insert_into_cache(cache, key, hash, value, new_cache);
    return insert_into_cache_file_ok;
}


// Function to handle input file
enum handle_input_file_status_code handle_input_file(char* input_file_path) {
    FILE* input_file = fopen(input_file_path, "r");
    
    if (input_file == NULL) {
        return handle_input_file_fail;
    }

    FILE* output_file = fopen("lab_4_1.txt", "w");

    if (output_file == NULL) {
        fclose(input_file);
        return handle_input_file_fail;
    }

    int HASHSIZE = 128;

    Hash_Cache cache, new_cache;
    create_cache(&cache, HASHSIZE);

    char line[256];
    while (fgets(line, sizeof(line), input_file) != NULL) {
        // Check if the line starts with "#define"
        if (strncmp(line, "#define", 7) == 0) {
            // Parse the line to extract <def_name> and <value>
            char def_name[128], value[128];
            if (sscanf(line, "#define %s %s\n", def_name, value) == 2) {
                // Calculate hash and insert into the cache
                printf("%s %s\n", def_name, value);
                insert_into_cache_file(&cache, def_name, value, &new_cache);
            }
        } else {
            // Replace occurrences of <def_name> with <value>
            char* token = strtok(line, " \n");
            while (token != NULL) {
                int length = strlen(token);
                printf("%s\n", token);
                if (token[strlen(token) - 1] == '\n' && token[strlen(token) - 2] == ';'){
                    token[strlen(token) - 2] = '\n';
                    token[strlen(token) - 1] = '\0';
                }
                if (token[strlen(token) - 1] == ';'){
                    printf("%s\n", token);
                    token[strlen(token) -1] = '\0';
                    printf("%s\n", token);
                }
                const char* value = get_value_from_cache(&cache, token, HASHSIZE);
                if (value != NULL) {
                    printf("%s\n",value);
                    if (token[length - 2] == '\n'){
                        token[length - 2] = '\0';
                        fprintf(output_file, "%s\n", value);
                    } else {
                        fprintf(output_file, "%s ", value);
                    }
                } else {
                    fprintf(output_file, "%s ", token);
                }

                token = strtok(NULL, " ");
            }
        }
    }

    fclose(input_file);
    fclose(output_file);
    free_hash_cache(&cache);

    return handle_input_file_ok;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file_path>\n", argv[0]);
        return 1;
    }

    char* input_file_path = argv[1];
    if (handle_input_file(input_file_path) == handle_input_file_ok) {
        printf("File is changed successfully\n");
    }

    return 0;
}