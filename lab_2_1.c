#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

enum len_status_code {len_ok, len_null_string};

enum reverse_string_status_code {reversed_string_null_string, reversed_string_memmory_allocation_error, reversed_string_ok};

enum sting_upper_status_code {upper_string_null_string, uppered_string_memmory_allocation_error, upper_string_ok};

enum strcatty_status_code {strcatty_memmory_allocation_fail, strcatty_ok};

enum concationation_string_status_code {concatenate_string_memmory_allocation_error, concatenate_string_null, concatenate_string_concatination_fail, concatenation_string_ok};

enum reorder_string_status_code {reorder_string_null_string, reorder_string_memmory_allocation_error, reorder_string_ok, reorder_string_concatination_fail};

enum len_status_code len(char* string, int* length);

enum reverse_string_status_code reverse_string(char* string, char** reversed_string);

enum sting_upper_status_code upper_string(char* string, char** uppered_string);

enum strcatty_status_code strcatty(char** concatenated_string, const char* string);

enum reorder_string_status_code reorder_string(char* string, char** reordered_string);

enum concationation_string_status_code concatenate_string(char** concatenated_string, int seed, char* string, ...);

int main (int argc, char* argv[]){

    if (argc < 3){
        printf("INcorrect number of parameters %s: <flag> <string> <....>\n", argv[0]);
        return 1;
    }

    char pre_def = argv[1][0];

    if (pre_def != '-' && pre_def != '/'){
        printf("Incorrect flag initialisation: <-> </>\n");
        return 1;
    }

    char flag = argv[1][1];

    char* string = argv[2];

    int length;
    char* reversed_string;
    char* uppered_string;
    char* reordered_string;
    char* concatenated_string;
    int seed;
    int num_strings;
    char** strings;

    switch (flag)
    {
    case 'l':
        switch(len(string, &length)){
            case len_ok:
                printf("The length of the string is: %d\n", length);
                break;

            case len_null_string:
                printf("It is a NULL\n");
                return 1;
        }
        break;
    
    case 'r':
        switch(reverse_string(string, &reversed_string)){
            case reversed_string_memmory_allocation_error:
                printf("Memory allocation error\n");
                return 1;

            case reversed_string_null_string:
                printf("It is a NULL\n");
                return 1;

            case reversed_string_ok:
                printf("The reversed string is: %s\n", reversed_string);
                free(reversed_string);
                break;
        }
        break;

    case 'u':
        switch(upper_string(string, &uppered_string)){
            case uppered_string_memmory_allocation_error:
                printf("Memory allocation error\n");
                return 1;

            case upper_string_null_string:
                printf("It is a NULL\n");
                return 1;

            case upper_string_ok:
                printf("The uppered string is: %s\n", uppered_string);
                free(uppered_string);
                break;
        }
        break;
    
    case 'n':
        switch (reorder_string(string, &reordered_string)){
            case reorder_string_null_string:
                printf("It is a NULL\n");
                return 1;
        
            case reorder_string_memmory_allocation_error:
                printf("Memory allocation error\n");
                return 1;
            
            case reorder_string_concatination_fail:
                printf("The formation of the string went wrong\n");
                return 1;

            case reorder_string_ok:
                printf("The reordered string is: %s\n", reordered_string);
                free(reordered_string);
                break;
        }
        break;

    case 'c':
        if (!(argc > 3)){
            printf("Not enought parameters for this flag: <flag> <string> <seed> <...(strings)>\n");
            return 1;
        }
        seed = atoi(argv[3]);
        num_strings = argc - 4;
        strings = (char**)malloc(num_strings * sizeof(char*));

        if (strings == NULL) {
            printf("Memory allocation error\n");
            return 1;
        }

        for(int i = 4; i < argc; i++){
            strings[i - 4] = argv[i];
        }

        switch (concatenate_string(&concatenated_string, seed, string, strings))
        {
        case concatenate_string_concatination_fail:
            printf("The conctatinations of strings fail\n");
            free(strings);
            return 1;
        
        case concatenate_string_memmory_allocation_error:
            printf("Memory allocation error\n");
            free(strings);
            return 1;

        case concatenate_string_null:
            printf("It is a NULL\n");
            free(strings);
            return 1;
        
        case concatenation_string_ok:
            printf("The resulted concatenated string is: %s\n", concatenated_string);
            free(concatenated_string);
            free(strings);
            break;
        }
        break;

    default:
        printf("There is no such a flag: <-l> <-r> <-u> <-n> <-c>\n");
        return 1;
        break;
    }
    
    return 0;
}

enum len_status_code len(char* string, int* length){

    if(string == NULL){
        return len_null_string;
    }

    int n = 0;

    for(;*string != '\0'; n++){
        string++;
    }

    *length = n;

    return len_ok;
}

enum reverse_string_status_code reverse_string(char* string, char** reversed_string){

    int length;

    if(len(string, &length) == len_null_string){
        return reversed_string_null_string;
    }

    *reversed_string = (char*)malloc(length + 1);

    if (*reversed_string == NULL){
        return reversed_string_memmory_allocation_error;
    }

    for (int i = 0; i < length; i++){
        (*reversed_string)[i] = string[length - 1 - i];
    }

    (*reversed_string)[length] = '\0';

    return reversed_string_ok;
}

enum sting_upper_status_code upper_string(char* string, char** uppered_string){

    int length;

    if(len(string, &length) == len_null_string){
        return upper_string_null_string;
    }

    *uppered_string = (char*)malloc(length + 1);

    if (*uppered_string == NULL){
        return uppered_string_memmory_allocation_error;
    }

    for(int i = 0; i < length; i++){

        if(i & 1){
            if (string[i] >= 'a' && string[i] <= 'z') {
                (*uppered_string)[i] = string[i] - 'a' + 'A';
            } else {
                (*uppered_string)[i] = string[i];
            }
        } else {
            (*uppered_string)[i] = string[i];
        }
    }

    (*uppered_string)[length] = '\0';

    return upper_string_ok;
}

enum strcatty_status_code strcatty(char** concatenated_string, const char* string){

    if((*concatenated_string == NULL) || (string == NULL)){
        return strcatty_memmory_allocation_fail;
    }

    while(*string != '\0'){
        *(*concatenated_string) = *string;
        string++;
        (*concatenated_string)++;
    }
    return strcatty_ok;
}

enum reorder_string_status_code reorder_string(char* string, char** reordered_string){

    int length;

    if(len(string, &length) == len_null_string){
        return reorder_string_null_string;
    }

    *reordered_string = (char*)malloc(length + 3);

    if (*reordered_string == NULL){
        return reorder_string_memmory_allocation_error;
    }

    int digit_start = 0;
    int letter_start = 0;
    int other_start = 0;

    for(int i = 0; i < length; i ++){
        if (isdigit(string[i])){
            (*reordered_string)[digit_start++] = string[i]; 
        } else if (isalpha(string[i])) {
            (*reordered_string)[letter_start++] = string[i];
        } else {
            (*reordered_string)[other_start++] = string[i];
        }
    }

    (*reordered_string)[digit_start] = '\0';
    (*reordered_string)[letter_start] = '\0';
    (*reordered_string)[other_start] = '\0';

    if(strcatty(&reordered_string, *reordered_string + digit_start) == strcatty_memmory_allocation_fail){
        return reorder_string_concatination_fail;
    }

    if(strcatty(&reordered_string, *reordered_string + letter_start) == strcatty_memmory_allocation_fail){
        return reorder_string_concatination_fail;
    }

    (*reordered_string)[length] = '\0';

    return reorder_string_ok;
}

enum concationation_string_status_code concatenate_string(char** concatenated_string, int seed, char* string, ...){
    va_list args;
    va_start(args, string);

    int count = 0;
    const char* arg;
    while((arg = va_arg(args, const char*)) != NULL){
        count++;
    }

    va_start(args, string);

    srand((unsigned int) seed);

    int* indicies = (int*)malloc(count * sizeof(int));
    if (indicies == NULL){
        return concatenate_string_memmory_allocation_error;
    }

    for (int i = 0; i < count; i++){
        indicies[i] = i;
    }

    for(int i = count - 1; i > 0; i--){
        int j = rand() % (i + 1);
        indicies[i] ^= indicies[j];
        indicies[j] ^= indicies[i];
        indicies[i] ^= indicies[j];
    }

    int total_length = 0;
    int length;
    for (int i = 0; i < count; i++){
        if(len(va_arg(args, const char*), &length) == len_ok){
            total_length += length;
        } else{
            return concatenate_string_null;
        }
    }

    *concatenated_string = (char*)malloc(total_length + 1);
    if (*concatenated_string == NULL) {
        free(indicies);
        return concatenate_string_memmory_allocation_error;
    }

    char* dest = *concatenated_string;
    for (int i = 0; i < count; i++){
        arg = va_arg(args, const char*);
        int index = indicies[i];
        if(strcatty(&dest, arg) == strcatty_ok){
            if(len(arg, &length) != len_ok) {
                return concatenate_string_null;
            }
            dest += length;
        } else{
            return concatenate_string_concatination_fail;
        }
    }

    *dest = '\0';
    **concatenated_string = *dest;


    free(indicies);

    return concatenation_string_ok;
}