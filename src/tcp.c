/*
 * @Author          : lihugang
 * @Date            : 2022-07-08 12:37:19
 * @LastEditTime    : 2022-07-12 12:10:40
 * @LastEditors     : lihugang
 * @Description     :
 * @FilePath        : \src\tcp.c
 * Copyright (c) lihugang
 * 长风破浪会有时 直挂云帆济沧海
 * There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 *  * * *
 * 是非成败转头空 青山依旧在 几度夕阳红
 * Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */
#pragma once
#ifdef _WIN32
#include <stdint.h>
#include <winsock2.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#include "tcp.h"

typedef struct _IOCP_Buffer_Recorder {
    DWORD bytesSend; //Number of bytes sent
    DWORD bytesRecv; //Number of bytes received
    OVERLAPPED overlapped; //Overlapped IO
    WSABUF dataBuffer;
    SOCKET socket; //Socket
    uint8_t mode; //read or write mode
} iocp_rec, *p_iocp_rec; //pointer IOCP recorder

struct IOCP_IDENTIFIER {
    //SOCKET socket; //Client Socket (Bug)
    struct sockaddr_in *address; //Remote Address
    uint64_t socket_id; //Test for socket_id
    long connection_increment_id;
    p_iocp_rec iocp_buffer_descriptor; //pointer to iocp buffer
};

struct thread_args { //arguments that will be passed to thread
    HANDLE IOCP;
    int threadID; //thread ID
    void (*onReceiveRequestCallback)(uint64_t request_id);
    void (*onReceiveDataCallback)(uint64_t request_id, const char *buffer, uint32_t data_length);
    volatile long * connections_ptr; //pointer to record connections
    uint8_t * bitset; //Bitset to store connection mode
};

ptr_hash_map connections_map;

static int init_tcp_server(WSADATA *wsaData) {


    WORD socketVersion = MAKEWORD(2, 2); //word -> int32_t
    int8_t initResult;
    initResult = WSAStartup(socketVersion, wsaData);
    if (initResult) {
        //Failed to initialize server
        return WSAStartUpFailure;
    };
    //Init socket

    return 0;
};
static SOCKET create_tcp_socket(void) {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //AF_INET: Address format: hostname/ip:port
    //SOCK_STREAM: TCP STREAM
    return serverSocket;
};

static int socket_bind(SOCKET *serverSocket, const char *host, uint16_t port) {
    struct sockaddr_in addressInfo;
    addressInfo.sin_family = AF_INET; //Address Format: Internet
    addressInfo.sin_port = htons(port); //port
    addressInfo.sin_addr.S_un.S_addr = inet_addr(host); //address: ip
    int bind_status = bind(*serverSocket, (struct sockaddr *)&addressInfo, sizeof(addressInfo));
    if (bind_status == SOCKET_ERROR) return SocketBindFailure;
    return 0;
};

static int socket_listen(SOCKET *serverSocket, int socket_waiting_queue_length) {
    if (socket_waiting_queue_length < 0) socket_waiting_queue_length = SOMAXCONN;
    if (listen(*serverSocket, SOMAXCONN) == SOCKET_ERROR) return SocketListenFailure;
    return 0;
};

static DWORD WINAPI IOCP_work_thread(struct thread_args *args) {
    int threadID = args->threadID;
    HANDLE serverNetworkIOCP = args->IOCP;
    volatile long * connections_ptr = args->connections_ptr;

    DWORD BytesRecv = 0;
    struct IOCP_IDENTIFIER *connection_identifier = NULL;
    p_iocp_rec connection_info_recorder = malloc(sizeof(iocp_rec));
    while (1) {
        static int iocp_ok_status;
        //better_printf("Thread [%d] Waiting For Queue Completion\n",threadID);
        iocp_ok_status = GetQueuedCompletionStatus(
            serverNetworkIOCP,
            &BytesRecv,
            (LPDWORD)&connection_identifier,
            (LPOVERLAPPED *)&(connection_info_recorder->overlapped),
            INFINITE
            );
        //Get Queue IOCP Status
        //Wait INFINITE secs
        if (iocp_ok_status == 0) {
            //Not ok
            better_printf("Thread [%d] Get Error Code\n", threadID);
            switch (GetLastError()) {
            case WAIT_TIMEOUT: continue; //IOCP Wait OK Timeout

            case 64L: //64L -> ERROR_NETNAME_DELETED
            case 995L: //995L -> ERROR_OPERATION_ABORTED
                better_printf("Thread [%d] Connection closed", threadID);
                InterlockedDecrement(connections_ptr);
                continue;

            default: //get_queued_completion_status error
                better_printf("Thread [%d] Exited. Get IOCP Status Error", threadID);
                return IOCP_QUEUE_GET_STATUS_ERROR;
            };
        };

        better_printf("Thread [%d] Get OK Status Connection %u\n", threadID, connection_identifier->socket_id);
        // if (bitset_get(args->bitset,connection_identifier->connection_increment_id) == 0) { //read mode : 0
        if (1) {
            //Read mode
            better_printf("Thread [%d] Receive Data %u bytes from socket %u\n", threadID, BytesRecv, connection_identifier->socket_id);
            if (BytesRecv == 0) {
                //No data
                //End connection
                better_printf("Thread [%d] Connection %d closed", threadID, connection_identifier->socket_id);
                closesocket(connection_identifier->socket_id);
                free(connection_identifier->iocp_buffer_descriptor->dataBuffer.buf);
                GlobalFree(connection_identifier->iocp_buffer_descriptor);
                free(connection_identifier);
                InterlockedDecrement(connections_ptr);
                continue;
            }
            connection_identifier->iocp_buffer_descriptor->dataBuffer.buf[BytesRecv] = '\0'; //end string
            args->onReceiveDataCallback(connection_identifier->socket_id,connection_identifier->iocp_buffer_descriptor->dataBuffer.buf,BytesRecv);
            //This operation may block thread
            //Please run enough threads
        };
    };
};
#endif

int create_tcp_server(const char *host, uint16_t port, int maxConnections, int socket_waiting_queue_length, int maxThreads, uint32_t bufferSize, void (*onReceiveRequestCallback)(uint64_t request_id), void (*onReceiveDataCallback)(uint64_t request_id, const char *buffer, uint32_t data_length)) {
    if (maxConnections < 1) maxConnections = 1;
    if (connections_map.init_flag)
        if (init_hash_map(&connections_map,maxConnections))
            return ALLOCATE_MEMORY_FAILURE;
#ifdef _WIN32
    WSADATA wsaData; //wsa: Windows Socket 
    better_printf("TCP: Init Server\n");
    int init_status = init_tcp_server(&wsaData);
    if (init_status) return init_status; //Init failed

    HANDLE serverNetworkIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    //Uses IOCP tech to support more connections and get more efficiency

    if (serverNetworkIOCP == INVALID_HANDLE_VALUE) return CreateIOCPFailure;

    better_printf("TCP: Create Socket\n");
    SOCKET serverSocket = create_tcp_socket();
    if (serverSocket == INVALID_SOCKET) return SocketCreateFailure;

    better_printf("TCP: Bind Socket\n");
    int bind_status = socket_bind(&serverSocket, host, port);
    if (bind_status) return bind_status;

    better_printf("TCP: Listening socket\n");
    int listen_status = socket_listen(&serverSocket, socket_waiting_queue_length);
    if (listen_status) return listen_status;

    volatile long connections = 0;
    volatile long * connections_ptr = &connections;

    // better_printf("TCP: Allocating memory for connection bitsets\n");
    // uint8_t * bitset = (uint8_t *)malloc(sizeof(uint8_t) * (maxThreads + 1) / 8);
    // if (bitset == NULL) return ALLOCATE_MEMORY_FAILURE;
    // memset(bitset, 0, sizeof(uint8_t) * ((maxThreads + 1) / 8));


    better_printf("TCP: Creating threads\n");

    HANDLE *tcp_threads = (HANDLE *)malloc(sizeof(HANDLE) * maxThreads);
    if (tcp_threads == NULL) return ALLOCATE_MEMORY_FAILURE;
    struct thread_args *tcp_thread_args = (struct thread_args *)malloc(sizeof(struct thread_args) * maxThreads);
    if (tcp_thread_args == NULL) return ALLOCATE_MEMORY_FAILURE;
    for (int i = 0; i < maxThreads; ++i) {
        (tcp_thread_args + i)->threadID = i;
        (tcp_thread_args + i)->IOCP = serverNetworkIOCP;
        // (tcp_thread_args + i)->bitset = bitset;
        (tcp_thread_args + i)->onReceiveDataCallback = onReceiveDataCallback;
        (tcp_thread_args + i)->onReceiveRequestCallback = onReceiveRequestCallback;
        (tcp_thread_args + i)->connections_ptr = connections_ptr;
        tcp_threads[i] = CreateThread(NULL, 0, (LPVOID)IOCP_work_thread, tcp_thread_args + i, 0, NULL);
        better_printf("Starting thread %d\n", i);
        if (tcp_threads[i] == NULL) {
            better_printf("Trigger an error when creating thread %d\n", i);
            return CreateThreadFailure;
        };
    };

    better_printf("\x1B[33mServer listening on \x1B[31m%s:%d\x1B[37m\n", host, port);
    while (1) {
        //Waiting for request
        struct sockaddr_in remoteAddress; //Client address
        int remoteAddress_len = sizeof(remoteAddress);
        SOCKET clientConnection;
        //better_printf("Waiting for connections...\n");
        clientConnection = accept(serverSocket, (struct sockaddr *)&remoteAddress, &remoteAddress_len);
        if (connections > maxConnections) {
            better_printf("Connection [%d] Connection aborted: Over max connections %d\n", connections, maxConnections);
            closesocket(clientConnection);
            continue;
        };


        static int connection_increment_id;
        connection_increment_id = InterlockedIncrement(connections_ptr);
        //I'm lazy to realize map, so I use increment id and bitset instead
        //connections++;
        //better_printf("Connection [%d] New connection: socket_id = %d, ip:port = %s:%d\n", connections, clientConnection, inet_ntoa(remoteAddress.sin_addr), remoteAddress.sin_port);
        //Accept request

        struct IOCP_IDENTIFIER *connection_identifier = (struct IOCP_IDENTIFIER *)malloc(sizeof(struct IOCP_IDENTIFIER));
        //Allocate memory for the connection identifier

        if (connection_identifier == NULL) {
            better_printf("\x1B[31mError: Cannot allocate memory for Connection Identifier.\x1B[37m\n");
            closesocket(clientConnection); //Close Client Socket
            continue; //Waiting for next connection
        };

        p_iocp_rec connection_info_recorder = (p_iocp_rec)GlobalAlloc(GPTR, sizeof(iocp_rec));
        //Allocate memory for buffer

        if (connection_info_recorder == NULL) {
            better_printf("\x1B[31mError: Cannot allocate memory for IO Recorder.\x1B[37m\n");
            return ALLOCATE_MEMORY_FAILURE;
        };
        
        //Set identifier value
        connection_identifier->socket_id = (uint64_t)clientConnection;
        connection_identifier->connection_increment_id = connection_increment_id;
        connection_identifier->iocp_buffer_descriptor = connection_info_recorder;
        //if (hash_map_set_value(&connections_map,clientConnection,(void *)connections_ptr)) return UNKNOWN_ERROR;

        // bitset_delete(bitset, connection_increment_id);
        //Set read mode(0,read; 1,write)

        //Copy data
        // connection_identifier->socket = clientConnection;
        // connection_identifier->address = &remoteAddress;

        //Bind this connection to network IOCP
        CreateIoCompletionPort((HANDLE)clientConnection, serverNetworkIOCP, (DWORD)connection_identifier, 0);

        char *overlapped_io_buffer = (char *)malloc(sizeof(char) * bufferSize);
        //Allocate memory for (network Input Output Operation Data)

        if (overlapped_io_buffer == NULL) {
            better_printf("\x1B[31mError: Cannot allocate memory for Network IO Operation Data Buffer.\x1B[37m\n");

            //Free resources
            free(connection_identifier);
            free(tcp_threads);
            free(tcp_thread_args);
            WSACleanup();
            CloseHandle(serverNetworkIOCP);
            return ALLOCATE_MEMORY_FAILURE;
        };

        ZeroMemory(overlapped_io_buffer, sizeof(overlapped_io_buffer)); //Clear memory and set 0

        //struct IOCP_Buffer_Recorder *connection_info_recorder = (struct IOCP_Buffer_Recorder *)malloc(sizeof(struct IOCP_Buffer_Recorder));



        //Init the buffer recorder value
        connection_info_recorder->bytesSend = 0;
        connection_info_recorder->bytesRecv = 0;
        connection_info_recorder->socket = clientConnection;
        ZeroMemory(&(connection_info_recorder->overlapped), sizeof(connection_info_recorder->overlapped));
        connection_info_recorder->dataBuffer.len = bufferSize;
        connection_info_recorder->dataBuffer.buf = overlapped_io_buffer;
        connection_info_recorder->mode = TCP_IO_READ_MODE; //READ mode

        DWORD flags = 0;
        int recv_status = WSARecv(
            connection_identifier->socket_id,
            &(connection_info_recorder->dataBuffer),
            1,
            &(connection_info_recorder->bytesRecv),
            &flags,
            &(connection_info_recorder->overlapped),
            NULL);
        //Not block
        if (recv_status == SOCKET_ERROR) { //Client disconnected
            int err = WSAGetLastError();
            if (err != 997) { //997 -> wsa_io_pending magic number
                better_printf("Connection %d disconnect\n", connection_identifier->socket_id);
                closesocket(connection_identifier->socket_id); //Close socket
                free(tcp_threads);
                free(tcp_thread_args);
                free(overlapped_io_buffer);
                GlobalFree(connection_info_recorder);
                free(connection_identifier);
                WSACleanup();
                CloseHandle(serverNetworkIOCP);
                //Free resources allocated
                InterlockedDecrement(connections_ptr);
                //connections--;
            };
        };

        onReceiveRequestCallback((uint64_t)(connection_identifier->socket_id)); //Callback func
        //Notice: This operation may block main thread, and cannot accept request
        //"onReceiveRequestCallback" function should be tiny

    };


#endif
};

void tcp_server_write_socket(uint64_t request_id, char *buffer, uint32_t bufferSize) {
    better_printf("Send Data %u bytes to socket %u\n", bufferSize, request_id);
    SOCKET socket = request_id;
    WSABUF wsaBuffer[1];
    wsaBuffer[0].buf = buffer;
    wsaBuffer[0].len = bufferSize;
    WSASend(socket,wsaBuffer,1,NULL,1,NULL,NULL);
};
void tcp_server_end_socket(uint64_t request_id) {
    better_printf("End Socket %u\n", request_id);
    closesocket((SOCKET)request_id);
};