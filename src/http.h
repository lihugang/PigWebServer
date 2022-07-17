
/***
 * @Author          : lihugang
 * @Date            : 2022-07-08 12:35:40
 * @LastEditTime    : 2022-07-09 14:46:32
 * @LastEditors     : lihugang
 * @Description     :
 * @FilePath        : \src\http.h
 * @Copyright (c) lihugang
 * @长风破浪会有时 直挂云帆济沧海
 * @There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 * @ * * *
 * @是非成败转头空 青山依旧在 几度夕阳红
 * @Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */
#define HTTP_DEFAULT_BUFFER_SIZE 4096
#define HTTP_DEFAULT_CONNECTIONS 128
#define HTTP_WAIT_FOR_PROCESS_QUEUE_LENGTH SOMAXCONN
#define HTTP_MAX_THREADS -1
 //default buffer size for http(tcp) request

#include "tcp.h"
#include "http.c"

int create_http_server(const char *host, uint16_t port, int maxConnections, int socket_waiting_queue_length, int maxThreads, uint32_t bufferSize);

void http_on_receive_request(uint64_t request_id);
void http_on_receive_data(uint64_t request_id, const char *data, uint32_t len);