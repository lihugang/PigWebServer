/*** 
 * @Author          : lihugang
 * @Date            : 2022-07-08 10:28:20
 * @LastEditTime    : 2022-07-08 13:15:25
 * @LastEditors     : lihugang
 * @Description     : 
 * @FilePath        : \config.h
 * @Copyright (c) lihugang
 * @长风破浪会有时 直挂云帆济沧海
 * @There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 * @ * * * 
 * @是非成败转头空 青山依旧在 几度夕阳红
 * @Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */
#pragma once
#define DEFAULT_CONFIG_READ_SIZE 65536 //default read size 65536 bytes

#include "config.c"

/*** 
 * @function name: read_config_file
 * @Description : read the config from the disk and return reading sizes
 * @Author      : lihugang
 * @param        {char *} buffer
 * @return       {int} the size it reads
 * @canUse      : ok
 * @Date        : 2022-07-08 10:37:14
 * @LastEditors : lihugang
 */
int read_config_file(char * buffer);