
/*
 * @Author          : lihugang
 * @Date            : 2022-07-07 22:57:24
 * @LastEditTime    : 2022-07-08 13:17:25
 * @LastEditors     : lihugang
 * @Description     :
 * @FilePath        : \format.c
 * Copyright (c) lihugang
 * 长风破浪会有时 直挂云帆济沧海
 * There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 *  * * *
 * 是非成败转头空 青山依旧在 几度夕阳红
 * Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */

bool is_invalid_ipv4(char const * const ipv4) {
    //valid ip address
    //(1~255).(0~255).(0~255).(0~255)

    int ptr = 0; //the pointer to the ipv4 string(current position)
    for (int i = 0; i < 4; i++) {
        int num = 0; //'xxx' in 'xxx.xxx.xxx.xxx'
        while (ipv4[ptr] == '.') ptr++;//next number
        int start_pos = ptr; //start position
        while (ipv4[ptr] != '.' && ipv4[ptr] != '\0' && ipv4[ptr] >= '0' && ipv4[ptr] <= '9') ptr++;
        if ((ipv4[ptr] < '0' || ipv4[ptr] > '9') && ipv4[ptr] != '.' && ipv4[ptr] != '\0') return TRUE; //Not a number or '.' or '\0'
        num = convert_string_to_int(ipv4, start_pos, ptr - 1); //not contains '.'
        if (num < 0 || num > 255) return TRUE; //Accept 0.0.0.0 (all addresses)
        if (i != 3 && ipv4[ptr] == '\0') //not end
            return TRUE;
    };
    return FALSE;
};