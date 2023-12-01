#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum handle_file_status_code {handle_file_ok, handle_file_fail};

typedef struct {
    int level;
    char letter;
    //Node* parent;
} Node;

enum handle_file_status_code handle_file(char* input_file_path, char* output_file_path){
    FILE* input_file = fopen(input_file_path, "r");

    if (input_file == NULL){
        return handle_file_fail;
    }

    FILE* output_file = fopen(output_file_path, "w");

    if (output_file == NULL){
        fclose(input_file);
        return handle_file_fail;
    }

    char buffer[256];

    while(fgets(buffer, sizeof(buffer), input_file)){
        int brackets_level = 0;
        char c;
        int letter_index = 0;
        int new_letters = 0;
        for(int i = 0; i < strlen(buffer); i++){
            c = buffer[i];
            if (isalpha(c)){
                for (int j = 0; j <= brackets_level; j++){
                    fprintf(output_file, "\n");
                }
                for (int space = 1; space < letter_index + brackets_level; space++){
                    fprintf(output_file, " ");
                }
                fprintf(output_file, "%c", c);
                new_letters++;
            }
            if (c == '('){
                letter_index++;
                brackets_level++;
                new_letters = 0;
            }
            if (c == ')'){
                brackets_level--;
                letter_index -= new_letters - 1;
            }
        }
        fprintf(output_file,"\n");
    }

    fclose(output_file);
    fclose(input_file);

    return handle_file_ok;
}

int main(int argc, char* argv[]){

    if (argc != 3)
    {
        printf("Usage: %s <input_file_path> <output_file_path>\n", argv[0]);
        return 1;
    }

    char* input_file_path = argv[1];
    char* output_file_path = argv[2];

    handle_file(input_file_path, output_file_path);
}