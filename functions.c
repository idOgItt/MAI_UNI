#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define FOR_TRANSFER 21
#define SIZE_TO_BASE 34
#define ALF 26


enum transfer_to_status_codes
{
    out_of_range,
    memory_allocation_problem,
    unknown_data_type,
    ok_transfer
};

enum transfer_to_status_codes roman_to_int(char *str, int * result) 
{
    int len = strlen(str);
    if (len > 20) return out_of_range;
    if (strcmp(str, "nihil") == 0)
    {
        *result = 0;
        return ok_transfer;
    }
    int values[ALF];
    for (int i = 0; i < ALF; i++) {values[i] = 0;}
    values['I' - 'A'] = 1; values['V' - 'A'] = 5; values['X' - 'A'] = 10;
    values['L' - 'A'] = 50; values['C' - 'A'] = 100; values['D' - 'A'] = 500;
    values['M' - 'A'] = 1000;

    *result = 0;
    int prev = 0;
    for (int i = len - 1; i >= 0; i--) 
    {
        int crnt = values[str[i] - 'A']; //Getting the required index
        if (!crnt) 
        {
            *result = -1;
            return unknown_data_type;
        }
        if (crnt < prev) 
        {
            *result -= crnt;
        } 
        else 
        {
            *result += crnt;
        }
        prev = crnt;
    }
    return ok_transfer;
}

enum transfer_to_status_codes transfer_cyckendorf_to_int(char *str, unsigned int *res)
{
    size_t len = strlen(str);
    if (len > 20 || len < 3)
    {
        return out_of_range;
    }
    *res = 0;
    int prevv = 0, prev = 1, crnt = 1;
    for (int i = 1; !(str[i] == '1' && str[i + 1] == '1'); i++)
    {
        if (str[i] == '1') *res += crnt;
        prevv = prev;
        prev = crnt;
        crnt = prevv + prev;
    }
    *res += crnt;
    return ok_transfer;
}



enum transfer_to_status_codes ss_to_base_10(char* str, int base, int * result, char flag) 
{
    *result = 0;
    int len = strlen(str);
    bool sign = false;
    if (len > 8) return out_of_range;
    if (base < 2 || base > 36) base = 10;
    if (str[0] == '-') sign = true;
    int i = len - 1;
    int multiplier = 1;
    int digit;
    for (; i >= 0 + sign; i--) 
    {
        char c = str[i];
        if (flag == 'V') digit = (c >= '0' && c <= '9') ? c - '0' : c - 'A' + 10;
        else digit = (c >= '0' && c <= '9') ? c - '0' : c - 'a' + 10;
        if (digit >= base) return out_of_range;
        *result = *result + digit * multiplier;
        multiplier *= base;
    }
    if (sign)
    {
        *result = -(*result);
    }
    return ok_transfer;
}