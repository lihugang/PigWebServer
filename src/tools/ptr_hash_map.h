/*** 
 * @Author          : lihugang
 * @Date            : 2022-07-11 19:14:57
 * @LastEditTime    : 2022-07-11 19:27:14
 * @LastEditors     : lihugang
 * @Description     : 
 * @FilePath        : \tools\ptr_hash_map.h
 * @Copyright (c) lihugang
 * @长风破浪会有时 直挂云帆济沧海
 * @There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 * @ * * * 
 * @是非成败转头空 青山依旧在 几度夕阳红
 * @Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */
//ptr_hash_map -> A hash map for pointers
#pragma once
#include <stdint.h>
#include <stdlib.h>

typedef struct _ptr_hash_map_node ptr_hash_map_node, *p_ptr_hash_map_node;
typedef struct _ptr_hash_map ptr_hash_map, *p_ptr_hash_map;


#define PTR_HASH_MAP_OPERATION_SUCCESS 0
#define PTR_HASH_MAP_ALLOC_MEMORY_FAILURE -1

#include "ptr_hash_map.c"