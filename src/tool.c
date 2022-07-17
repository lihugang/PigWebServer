/*
 * @Author          : lihugang
 * @Date            : 2022-07-08 11:25:47
 * @LastEditTime    : 2022-07-10 20:15:37
 * @LastEditors     : lihugang
 * @Description     :
 * @FilePath        : \src\tool.c
 * Copyright (c) lihugang
 * 长风破浪会有时 直挂云帆济沧海
 * There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 *  * * *
 * 是非成败转头空 青山依旧在 几度夕阳红
 * Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */
#pragma once
#include <stdint.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>

#ifdef X_RUN_FAST_MODE
 //Using map to instead of calculating to improve speed
int string_int_convert_table[128] = {
    0: '0',
    1 : '1',
    2 : '2',
    3 : '3',
    4 : '4',
    5 : '5',
    6 : '6',
    7 : '7',
    8 : '8',
    9 : '9',
    '0' : 0,
    '1' : 1,
    '2' : 2,
    '3' : 3,
    '4' : 4,
    '5' : 5,
    '6' : 6,
    '7' : 7,
    '8' : 8,
    '9' : 9
};
#endif
int convert_string_to_int(const char const *string, int start, int end) {
    int symbol_flag = (string[start] == '-') ? (++start, -1) : 1; //'+' or '-';
    // If number is negative, the start position next

    //Reading from the first
    int num = 0; //the number that convert
    for (int i = start; i <= end; ++i) {
        num *= 10;
#ifdef X_RUN_FAST_MODE
        num += *(string_int_convert_table + string[i]);
#else
        num += string[i] - '0';
#endif
    };
    return num * symbol_flag;
};

#ifdef _WIN32
CRITICAL_SECTION printf_critical_section;
#endif
inline int better_printf(const char *format, ...) {
    volatile static uint8_t firstExec = 1;
    va_list variable_arguments;
    va_start(variable_arguments, format);
    if (firstExec) {
        //Init critical section
#ifdef _WIN32
        InitializeCriticalSection(&printf_critical_section);
#endif
        firstExec = 0;
    };
#ifdef _WIN32
    EnterCriticalSection(&printf_critical_section);
#endif

    time_t currentTime; //Get time
    time(&currentTime);
    static char time_str[32] = {0};
    strcpy(time_str,ctime(&currentTime));
    time_str[strlen(time_str)-1] = '\0';
    fprintf(stdout, "[%s] ", time_str);
    int count = vfprintf(stdout, format, variable_arguments);

#ifdef _WIN32
    LeaveCriticalSection(&printf_critical_section);
#endif

    return count;
};

void bitset_set(uint8_t *bitset, uint64_t pos) {
    uint64_t index = pos / 8;
    bitset[index] |= (1 << (pos % 8));
};
void bitset_delete(uint8_t *bitset, uint64_t pos) {
    uint64_t index = pos / 8;
    bitset[index] &= ~(1 << (pos % 8));
};
int8_t bitset_get(uint8_t *bitset, uint64_t pos) {
    uint64_t index = pos / 8;
    return bitset[index] & (1 << (pos % 8));
};
int str_indexOf(const char *str, char c, int start_pos) {
    static int i;
    for (i = start_pos; str[i] != '\0' && str[i] != c; ++i);
    if (str[i] == c) return i;
    else return -1;
};