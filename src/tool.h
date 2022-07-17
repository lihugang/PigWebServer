
/***
 * @Author          : lihugang
 * @Date            : 2022-07-08 10:14:48
 * @LastEditTime    : 2022-07-10 17:17:05
 * @LastEditors     : lihugang
 * @Description     :
 * @FilePath        : \src\tool.h
 * @Copyright (c) lihugang
 * @长风破浪会有时 直挂云帆济沧海
 * @There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 * @ * * *
 * @是非成败转头空 青山依旧在 几度夕阳红
 * @Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */
#pragma once
#include <stdarg.h>
#include <time.h>
#include <string.h>
#ifdef _WIN32
#include <process.h>
#include <windows.h>
#endif

#include "tools/third-party/cJSON.h"
#include "tools/third-party/cJSON.c"
 //cJSON

#include "tools/ptr_hash_map.h"
#include "tools/ptr_hash_map.c"
//Pointer Hash Table

#include "tool.c"

//basic convert tool
int convert_string_to_int(const char const *string, int start, int end);
int better_printf(const char *format, ...);
void bitset_set(uint8_t *bitset, uint64_t pos);
void bitset_delete(uint8_t *bitset, uint64_t pos);
int8_t bitset_get(uint8_t *bitset, uint64_t pos);
int str_indexOf(const char *str, char c, int start_pos);