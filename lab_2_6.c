#include "functions.c"

#define STRING_SIZE 300
#define BUFF_SIZE 100
#define FLAG_SIZE 20

void print_errors(enum transfer_to_status_codes status) 
{
    if (status == out_of_range) 
    {
        printf("Number out of range\n");
    } 
    else if (status == memory_allocation_problem) 
    {
        printf("Memory allocation problem\n");
    }
    else 
    {
        printf("Unknown data type\n");
    }
}

int process_user_flags(const char* format, va_list* ptr, FILE* stream, char * str)
{
    int result = 0;
    int std_index = 0;
    while (*format)
    {
        if (*format == '%' && *(format + 1) == 'R' && *(format + 2) == 'o')
        {
            format += 3;
            int* num = va_arg(*ptr, int*);
            char roman_str[FOR_TRANSFER];
            if (stream != NULL) {if (fscanf(stream, "%s", roman_str) != 1) return -1;}
            else 
            {
                int num_chars_read;
                if (sscanf(str, "%s %n", roman_str, &num_chars_read) != 1) return -1;
                str += num_chars_read;
            }

            enum transfer_to_status_codes roman_status = roman_to_int(roman_str, num);
            if (roman_status == ok_transfer)
            {
                result++;
            }
            else
            {
                print_errors(roman_status);
                return -1;
            }
        }
        else if (*format == '%' && *(format + 1) == 'Z' && *(format + 2) == 'r')
        {
            format += 3;
            unsigned int* num = va_arg(*ptr, unsigned int*);
            char cyckendorf_str[FOR_TRANSFER];
            if (stream != NULL) {if (fscanf(stream, "%s", cyckendorf_str) != 1) return -1;}
            else 
            {
                int num_chars_read;
                if (sscanf(str, "%s %n", cyckendorf_str, &num_chars_read) != 1) return -1;
                str += num_chars_read;
            }

            enum transfer_to_status_codes cyckendorf_status = transfer_cyckendorf_to_int(cyckendorf_str, num);
            if (cyckendorf_status == ok_transfer)
            {
                result++;
            }
            else
            {
                print_errors(cyckendorf_status);
                return -1;
            }
        }
        else if (*format == '%' && (*(format + 2) == 'v' || *(format + 2) == 'V'))
        {
            char flag = *(format + 2);
            format += 3;
            int* num = va_arg(*ptr, int*);
            int base = va_arg(*ptr, int);
            char base_str[FOR_TRANSFER];
            if (stream != NULL) {if (fscanf(stream, "%s", base_str) != 1) return -1;}
            else 
            {
                int num_chars_read;
                if (sscanf(str, "%s %n", base_str, &num_chars_read) != 1) return -1;
                str += num_chars_read;
            }

            enum transfer_to_status_codes transfer_status = ss_to_base_10(base_str, base, num, flag);
            if (transfer_status == ok_transfer)
            {
                result++;
            }
            else
            {
                print_errors(transfer_status);
                return -1;
            }
        }
        else if (*format == '%')
        {
            char flag[FLAG_SIZE];
            int i = 0;
            flag[i] = *format; i++;
            format++;
            while (*format != ' ' && *format != '\t' && *format != '\n' && *format != '%' && *format != 'd' && *format != 'f' && *format != 'n' && *format != 'c' && *format != 's')
            {
                flag[i] = *format;
                format++; i++;
            }
            flag[i] = *format; i++; flag[i] = '\0';
            format++;
            if (flag[i - 1] == 'c')
            {
                char c;
                while ((stream != NULL && (c = fgetc(stream)) == ' ') || c == '\t' || c == '\n'){}
                if (c != EOF && stream != NULL) *(va_arg(*ptr, char*)) = c;
                while ((stream == NULL && (c = sscanf(str, "%c", &c)) == ' ') || c == '\t' || c == '\n'){str++;}
                if (c != '\0' && stream == NULL) *(va_arg(*ptr, char*)) = c;
            }
            else
            {
                if (stream != NULL) vfscanf(stream, flag, *ptr);
                else 
                {
                    int num_chars_read = 0;
                    char buffer[BUFF_SIZE];
                    if (sscanf(str, "%s %n", buffer, &num_chars_read) != 1) return -1;
                    vsscanf(buffer, flag, *ptr);
                    str += num_chars_read;
                }
            }
            result++;
        }
        else
        {
            format++;
        }
    }
    return result;
}

int overfscanf(FILE* stream, char* str, const char* format, ...)
{
    va_list ptr;
    va_start(ptr, format);

    int user_flags_length = process_user_flags(format, &ptr, stream, str);
    va_end(ptr);
    return user_flags_length;
}

int oversscanf(FILE* stream, char* str, const char* format, ...)
{
    va_list ptr;
    va_start(ptr, format);

    int user_flags_length = process_user_flags(format, &ptr, stream, str);
    va_end(ptr);
    return user_flags_length;
}