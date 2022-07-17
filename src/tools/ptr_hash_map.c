/*
 * @Author          : lihugang
 * @Date            : 2022-07-11 19:15:32
 * @LastEditTime    : 2022-07-12 11:59:48
 * @LastEditors     : lihugang
 * @Description     :
 * @FilePath        : \src\tools\ptr_hash_map.c
 * Copyright (c) lihugang
 * 长风破浪会有时 直挂云帆济沧海
 * There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 *  * * *
 * 是非成败转头空 青山依旧在 几度夕阳红
 * Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */
 //ptr_hash_map -> A hash map for pointers
#pragma once
#include "ptr_hash_map.h"

typedef struct _ptr_hash_map_node {
    void *value;
    uint64_t key;
    struct _ptr_hash_map_node *next;
}ptr_hash_map_node, *p_ptr_hash_map_node;

typedef struct _ptr_hash_map {
    p_ptr_hash_map_node * nodes;
    uint64_t map_size;
    uint8_t init_flag;
}ptr_hash_map, *p_ptr_hash_map;

uint64_t hash_map_calc_hash(uint64_t key, uint64_t mod) {
    return key * key % mod; //a easy hash
};

int8_t init_hash_map(p_ptr_hash_map map, uint64_t size) {
    map->map_size = size;
    map->nodes = (p_ptr_hash_map_node *)malloc(sizeof(ptr_hash_map_node) * (map->map_size));
    memset(map->nodes, 0, sizeof(ptr_hash_map_node) * (map->map_size));
    if (map->nodes == NULL) return PTR_HASH_MAP_ALLOC_MEMORY_FAILURE;
    return PTR_HASH_MAP_OPERATION_SUCCESS;
};

int8_t hash_map_set_value(p_ptr_hash_map map, uint64_t key, void *value) {
    static uint64_t map_index;
    map_index = hash_map_calc_hash(key, map->map_size);
    static p_ptr_hash_map_node node;
    node = map->nodes[map_index];
    while (node->next != NULL && node->key != key) node = node->next;
    if (node->next == NULL) {
        //The last of linked list
        //alloc memory for next node to store key and value
        node->next = (p_ptr_hash_map_node)malloc(sizeof(ptr_hash_map_node));
        if (node->next == NULL) return PTR_HASH_MAP_ALLOC_MEMORY_FAILURE; //Failed to allocate memory
        node = node->next;
        node->key = key;
        node->value = value;
        node->next = NULL;
        return PTR_HASH_MAP_OPERATION_SUCCESS;
    } else {
        //Find key
        node->value = value;
        return PTR_HASH_MAP_OPERATION_SUCCESS;
    };
};

void * hash_map_get_value(p_ptr_hash_map map, uint64_t key) {
    map->init_flag = 0;
    static uint64_t map_index;
    map_index = hash_map_calc_hash(key, map->map_size);
    static p_ptr_hash_map_node node;
    node = map->nodes[map_index];
    while (node->next != NULL && node->key != key) node = node->next;
    if (node->next == NULL) {
        //The last of linked list
        //not find
        return NULL;
    }
    else {
        //Find key
        return node->value;
    };
};

void hash_map_remove(p_ptr_hash_map map, uint64_t key) {
    static uint64_t map_index;
    map_index = hash_map_calc_hash(key, map->map_size);
    static p_ptr_hash_map_node node;
    node = map->nodes[map_index];
    while (node->next != NULL && node->next->key != key) node = node->next;
    if (node->next == NULL) {
        //The last of linked list
        //not find
    }
    else {
        //Find key
        //Free memory
        static p_ptr_hash_map_node free_node;
        free_node = node->next;
        node->next = node->next->next; //skip it
        free(free_node);
    };
};
