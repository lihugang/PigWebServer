/*
 * @Author          : lihugang
 * @Date            : 2022-07-07 21:32:13
 * @LastEditTime    : 2022-07-10 16:04:19
 * @LastEditors     : lihugang
 * @Description     :
 * @FilePath        : \src\index.c
 * Copyright (c) lihugang
 * 长风破浪会有时 直挂云帆济沧海
 * There will be times when the wind and waves break, and the sails will be hung straight to the sea.
 *  * * *
 * 是非成败转头空 青山依旧在 几度夕阳红
 * Whether it's right or wrong, success or failure, it's all empty now, and it's all gone with the passage of time. The green hills of the year still exist, and the sun still rises and sets.
 */
#include "environment_detect.h" //check win32
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "config.h"
#include "tool.h"
#include "format.h"
#include "http.h"

int main(int argc, char **argv) {
    printf("Pig Web Server\n");
#ifdef DEBUG
    printf("Warning: This application is running in debug mode.\n");
#else
#ifdef RELEASE
    printf("Release version: %s\n", RELEASE);
#else
    printf("Warning: This project version is unknown.\n");
#endif
#endif
    printf("Compile time: %s %s\n\n", __DATE__, __TIME__);
    better_printf("\nReading Config...\n");
    char *config_content = (char *)malloc(DEFAULT_CONFIG_READ_SIZE * sizeof(char));
    if (config_content == NULL) {
        better_printf("Couldn't allocate memory for configuration file");
        exit(0);
    };

    int read_config_size = read_config_file(config_content);
    config_content[read_config_size] = '\0'; //end string

    better_printf("Parse config...\n");

    //Parse config.json
    cJSON *config_root = cJSON_Parse(config_content);
    if (!config_root) {
        better_printf("Couldn't parse configuration file");
        exit(0);
    };

    cJSON *config_ip_ptr = cJSON_GetObjectItem(config_root, "ip");
    cJSON *config_port_ptr = cJSON_GetObjectItem(config_root, "port");
    if (!config_ip_ptr) {
        better_printf("config.json: Missing ip");
        exit(0);
    };
    if (!config_port_ptr) {
        better_printf("config.json: Missing port");
    };

    //config_ip_ptr -> valuestring returns a pointer to the IP address
    //When I call cJSON_delete(config_root), the string address will be removed
    //So, I alloc a space for storing the IP address to prevent lacking memory

    char *ip = (char *)malloc(sizeof(char) * IP_STRING_LENGTH);
    strncpy(ip, config_ip_ptr->valuestring, IP_STRING_LENGTH - 1);// reserve a space for '\0'

    uint16_t port = config_port_ptr->valueint;
    if (!ip) {
        better_printf("config.json: Invalid type of 'ip'");
        exit(0);
    };
    if (!port) {
        better_printf("config.json: Invalid type of 'port'");
        exit(0);
    };


    //read connection config
    cJSON *connection_config = cJSON_GetObjectItem(config_root, "connection");

    int maxConnections = HTTP_DEFAULT_CONNECTIONS; //Default max connections: 128
    int http_socket_waiting_queue_len = HTTP_WAIT_FOR_PROCESS_QUEUE_LENGTH; //The length of the queue for connections that are waiting to process, default: let OS choose
    int maxThreads = HTTP_MAX_THREADS; // Maximum Threads that process connections
    uint32_t tcp_io_buffer_size = DEFAULT_TCP_IO_BUFFER_SIZE;

    if (connection_config != NULL) {
        cJSON *connection_config_maxConnections_ptr = cJSON_GetObjectItem(connection_config, "maxConnections");
        if (connection_config_maxConnections_ptr != NULL) {
            if (connection_config_maxConnections_ptr->valueint != 0) {
                if (connection_config_maxConnections_ptr->valueint > 0)
                    maxConnections = connection_config_maxConnections_ptr->valueint;
            };
        };
        cJSON *connection_config_waiting_queue_length_ptr = cJSON_GetObjectItem(connection_config, "waitingQueueLength");
        if (connection_config_waiting_queue_length_ptr != NULL) {
            if (connection_config_waiting_queue_length_ptr->valueint != 0) {
                if (connection_config_waiting_queue_length_ptr->valueint > 0)
                    http_socket_waiting_queue_len = connection_config_waiting_queue_length_ptr->valueint;
            };
        };
        cJSON *connection_config_max_threads = cJSON_GetObjectItem(connection_config, "maxThreads");
        if (connection_config_max_threads != NULL) {
            if (connection_config_max_threads->valueint != 0) {
                if (connection_config_max_threads->valueint > 0)
                    maxThreads = connection_config_max_threads->valueint;
            };
        };
        cJSON *connection_config_buffer_size = cJSON_GetObjectItem(connection_config, "bufferSize");
        if (connection_config_buffer_size != NULL) {
            if (connection_config_buffer_size->valueint != 0) {
                if (connection_config_buffer_size->valueint > 0)
                    tcp_io_buffer_size = (uint32_t)connection_config_buffer_size->valueint;
            };
        };
    };

    if (maxThreads < 1) {
        //Default value / Bad value
        //Choose CPU cores * 2 to create threads
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        maxThreads = (int)sysinfo.dwNumberOfProcessors;
    };

    better_printf("Connection configuration:\nmaxConnections = %d\nSocket Waiting Queue length = %d\nMaxThreads = %d\nTCP IOCP Temporary Buffer Size = %u\n", maxConnections, http_socket_waiting_queue_len, maxThreads, tcp_io_buffer_size);

    //free memory allocated(cJSON)
    cJSON_Delete(config_root);

    //free memory allocated(File Buffer)
    free(config_content);

    //Valid check
    //Only support ipv4
    if (is_invalid_ipv4(ip)) {
        better_printf("Invalid IP address");
        exit(0);
    };

    //is_invalid_port X
    //2^16 = 65536
    //The type of 'port' is uint16_t (0~65535)
    //If port > 65535, it will mod 65536
    //Not worry about it


    create_http_server(ip, port, maxConnections, http_socket_waiting_queue_len, maxThreads, tcp_io_buffer_size);

    free(ip);

    return 0;
};
