

/***
 * @Author          : lihugang
 * @Date            : 2022-07-08 12:37:16
 * @LastEditTime    : 2022-07-08 19:50:06
 * @LastEditors     : lihugang
 * @Description     :
 * @FilePath        : \tcp.h
 * @Copyright (c) lihugang
 * @长风破浪会有时 直挂云帆济沧海
 * @There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 * @ * * *
 * @是非成败转头空 青山依旧在 几度夕阳红
 * @Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */
#pragma once

#define DEFAULT_TCP_IO_BUFFER_SIZE 4096


#ifdef _WIN32
#include <stdint.h>
#include <winsock2.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#endif

#define WSAStartUpFailure -1
#define SocketCreateFailure -2
#define SocketBindFailure -3
#define SocketListenFailure -4
#define CreateIOCPFailure -5
#define CreateThreadFailure -6
#define TCPRecvFailure -7
#define ALLOCATE_MEMORY_FAILURE -517
#define UNKNOWN_ERROR -1023

#define IOCP_QUEUE_GET_STATUS_ERROR 1025L

#define TCP_IO_READ_MODE 0u
#define TCP_IO_WRITE_MODE 1u

#include "tcp.c"

#ifdef _WIN32
struct IOCP_IDENTIFIER;
typedef struct _IOCP_Buffer_Recorder iocp_rec,*p_iocp_rec;
struct thread_args;
static SOCKET create_tcp_socket(void);
static int socket_bind(SOCKET *serverSocket, const char *host, uint16_t port);
static int socket_listen(SOCKET *serverSocket, int socket_waiting_queue_length);
static DWORD WINAPI IOCP_work_thread(struct thread_args *args);

#endif
int create_tcp_server(const char *host, uint16_t port, int maxConnections, int socket_waiting_queue_length, int maxThreads, uint32_t bufferSize ,void (*onReceiveRequestCallback)(uint64_t request_id), void (*onReceiveDataCallback)(uint64_t request_id, const char *buffer, uint32_t data_length));
void tcp_server_write_socket(uint64_t request_id, char *buffer, uint32_t bufferSize);
void tcp_server_end_socket(uint64_t request_id);