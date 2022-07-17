/*
 * @Author          : lihugang
 * @Date            : 2022-07-08 11:11:24
 * @LastEditTime    : 2022-07-08 13:15:15
 * @LastEditors     : lihugang
 * @Description     :
 * @FilePath        : \config.c
 * Copyright (c) lihugang
 * 长风破浪会有时 直挂云帆济沧海
 * There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 *  * * *
 * 是非成败转头空 青山依旧在 几度夕阳红
 * Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */
#pragma once
#include <stdio.h>
int read_config_file(char *buffer) {
    FILE *config_file_fp = fopen("config.json", "r");
    if (config_file_fp == NULL) {
        printf("Couldn't open configuration file.\nPlease check your configuration file path.\nIt must be in the './config.json'");
        exit(0);
    };
    int config_read_size = fread((void *)buffer, 1, DEFAULT_CONFIG_READ_SIZE, config_file_fp);
    return config_read_size;
};