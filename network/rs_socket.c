//本文件实现在rs_mq.h中定义的用于服务器端和客户端通信的函数

#include "rs_socket.h"

// 定义2个socket分别为服务器端监听socket和客户端通信socket,还定义了其他用于socket通信的相关变量
static int server_socket_fd, client_socket_fd, server_len, client_len;
static struct sockaddr_in server_address, client_address;
static fd_set readfds;
static int fd_index;

//实现rs_mq.h中定义的函数
//服务器启动
int server_start(){
	printf("%d: server starting ... \n", getpid());
	server_socket_fd=socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=htonl(INADDR_ANY);
	server_address.sin_port=htons(SERVER_PORT);
	server_len=sizeof(server_address);
	bind(server_socket_fd, (struct sockaddr* )&server_address, server_len);
	listen(server_socket_fd, CONNECT_MAX);
	FD_ZERO(&readfds);
	FD_SET(server_socket_fd, &readfds);
	printf("%d: server started. \n", getpid());
	return 1;
}

//服务器关闭
int server_end(){
	printf("%d: server ending ... \n", getpid());
	close(server_socket_fd);
	FD_ZERO(&readfds);
	printf("%d: server ended. \n", getpid());
	return 1;
}

//服务器端从客户端读取消息
int read_request_from_client(message_data* recieve_ptr){
	message message_passed;
	fd_set testfds;
	int result;
	int fd;
	printf("%d : start read request from client. \n", getpid());
	testfds=readfds;
	result=select(FD_SETSIZE, &testfds, (fd_set* )0, (fd_set* )0, (struct timeval* )0);
	if(result<1){
		printf("ERROR: fail to read request from client. \n");
		return 0;
	}
	for(fd=0; fd<FD_SETSIZE; fd++){
		if(FD_ISSET(fd, &testfds)){
			if(fd==server_socket_fd){
				client_len=sizeof(client_address);
				client_socket_fd=accept(server_socket_fd, (struct sockaddr* )&client_address, (size_t* )&client_len);
				FD_SET(client_socket_fd, &readfds);
				return 0;
			}else{
				read(fd, &message_passed, sizeof(message_passed));
				*recieve_ptr=message_passed.real_message;
				message_data message_data_cmp;
				memset(&message_data_cmp, 0, sizeof(message_data_cmp));
				if(!memcmp(&message_data_cmp, recieve_ptr, sizeof(message_data))){
					close(fd);
					FD_CLR(fd, &readfds);
					return 0;
				}
				fd_index=fd;
				printf("%d: haved read an request from client %s. \n", getpid(), message_passed.host_address);
				return 1;
			}
		}
	}
	return 0;
}

//服务器端发送消息给客户端
int send_response_to_client(const message_data real_message){
	message message_passed;
	printf("%d: start send response to client %s. \n", getpid(),real_message.host_address);
	strcpy(message_passed.host_address, real_message.host_address);
	message_passed.real_message=real_message;
	write(fd_index, &message_passed, sizeof(message_passed));
	printf("%d: haved send an response to client %s. \n", getpid(),real_message.host_address);
	return 1;
}

//客户端启动
int client_start(){
	int result;
#if DEBUG_TRACE
	printf("%d: client starting ... \n", getpid());
#endif
	client_socket_fd=socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=inet_addr("127.0.0.1");
	server_address.sin_port=htons(SERVER_PORT);
	server_len=sizeof(server_address);
	result=connect(client_socket_fd, (struct sockaddr *)&server_address, server_len);
	if(result==-1){
		printf("ERROR: fail to connect to server. \n");
		return 0;
	}
#if DEBUG_TRACE
	printf("%d: client started. \n", getpid());
#endif
	return 1;
}

//客户端关闭
int client_end(){
	printf("%d: client ending... \n", getpid());
	message_data real_message;
	memset(&real_message, 0, sizeof(real_message));
	send_request_to_server(real_message);
	close(client_socket_fd);
	printf("%d: client ended. \n", getpid());
	return 1;
}

//客户端发送消息给服务器端
int send_request_to_server(message_data real_message){
#if DEBUG_TRACE
	printf("%d: start send request to server. \n", getpid());
#endif
	message message_passed;
	strcpy(message_passed.host_address, real_message.host_address);
	message_passed.real_message=real_message;
	write(client_socket_fd, &message_passed, sizeof(message_passed));
#if DEBUG_TRACE
	printf("%d: end send request to server. \n", getpid());
#endif
	return 1;
}

//客户端读取服务器端传来的响应消息 
int read_response_from_server(message_data* recieve_ptr){
#if DEBUG_TRACE
	printf("%d: start read response from server. \n", getpid());
#endif
	message message_passed;
	read(client_socket_fd, &message_passed, sizeof(message_passed));
	*recieve_ptr=message_passed.real_message;
#if DEBUG_TRACE
	printf("%d: end read response from server. \n", getpid());
#endif
	return 1;
}
