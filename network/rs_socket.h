// 本文件定义服务器和客户端之间通信的接口函数和数据类型，服务器端和客户端使用socket进行通信
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include "rs_dbm.h"

#define ERR_TEXT_LEN 100
#define SERVER_PORT  10000
#define CONNECT_MAX  10

//定义客户端请求消息类型
typedef enum{
	c_add_userinfo=0,
	c_get_userinfo,
	c_modify_userinfo,
	c_get_traininfo_by_t_num,
	c_get_traininfo_by_o_and_t,
	c_get_ticket,
	c_modify_ticket,
	c_add_order,
	c_get_orders,
	c_modify_order
}client_request;

// 定义服务器返回消息类型
typedef enum{
	s_success=0,
	s_failure
}server_response;


// 定义消息格式
typedef struct{
	char		host_address[20];
	client_request	request;
	server_response	response;
	userinfo	userinfo_data;	
	ticket		ticket_data;
	traininfo	traininfo_data;
	orders		order_data;
	char		error_text[ERR_TEXT_LEN + 1];
}message_data;

// 定义传递的消息类型
typedef struct{
	char		host_address[20];
	message_data	real_message;
}message;

// 定义服务器/客户端的接口函数，负责服务器/客户端的通信
int server_start();
int server_end();
int read_request_from_client(message_data* recieve_ptr);
int send_response_to_client(const message_data real_message);
int client_start();
int client_end();
int send_request_to_server(message_data real_message);
int read_response_from_server(message_data* recieve_ptr);

