/*
 * @Author          : lihugang
 * @Date            : 2022-07-08 12:35:45
 * @LastEditTime    : 2022-07-10 20:31:04
 * @LastEditors     : lihugang
 * @Description     :
 * @FilePath        : \src\http.c
 * Copyright (c) lihugang
 * 长风破浪会有时 直挂云帆济沧海
 * There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 *  * * *
 * 是非成败转头空 青山依旧在 几度夕阳红
 * Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */
#pragma once
#include <stdint.h>

void http_on_receive_request(uint64_t request_id) {
};
void http_on_receive_data(uint64_t request_id, const char *data, uint32_t len) {
    const int method_len = 1;
    static char * http_method[] = {
        "123"
    };
    static char tmp[32] = { 0 };
    static char status_405_forbidden_http[] = "HTTP/1.0 405 Forbidden\nServer: pws\n\nMethod Not Found";
    int get_method = str_indexOf(data, ' ', 0);
    strncpy(tmp, data, get_method);
    int match_method = 0;
    for (int i = 0; i < method_len; i++) {
        if (strcmp(tmp, http_method[i]) == 0) {
            match_method = 1;
            break;
        };
    };
    if (match_method == 0) {
        tcp_server_write_socket(request_id, status_405_forbidden_http, sizeof(status_405_forbidden_http) / sizeof(char));
        tcp_server_end_socket(request_id);
    };
};

int create_http_server(const char *host, uint16_t port, int maxConnections, int socket_waiting_queue_length, int maxThreads, uint32_t bufferSize) {
    int tcp_server_status = create_tcp_server(host, port, maxConnections, socket_waiting_queue_length, maxThreads, bufferSize, http_on_receive_request, http_on_receive_data);
    if (tcp_server_status) better_printf("Couldn't create tcp server! Error Code:%d\n", tcp_server_status);
    return 0;
};