/*** 
 * @Author          : lihugang
 * @Date            : 2022-07-07 22:57:21
 * @LastEditTime    : 2022-07-10 18:26:58
 * @LastEditors     : lihugang
 * @Description     : 
 * @FilePath        : \src\format.h
 * @Copyright (c) lihugang
 * @长风破浪会有时 直挂云帆济沧海
 * @There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 * @ * * * 
 * @是非成败转头空 青山依旧在 几度夕阳红
 * @Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */
typedef unsigned char bit8_t;
typedef unsigned char bool;

#define IP_STRING_LENGTH 16 //the maximum size of the string that will be used to store the IP address

#include "format.c"

//data valid check
bool is_invalid_ipv4(char const *const ipv4);