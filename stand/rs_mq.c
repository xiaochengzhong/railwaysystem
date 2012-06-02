//本文件实现在rs_mq.h中定义的用于服务器端和客户端通信的函数

#include "rs_mq.h"

// 定义2个变量分别保存msgget函数返回的2个队列标识符
static int server_mq_id=-1;
static int client_mq_id=-1;

//实现rs_mq.h中定义的函数
//服务器启动
int server_start(){
	printf("%d: server starting ... \n", getpid());
	server_mq_id=msgget((key_t)SERVER_MQUEUE, 0666|IPC_CREAT);
	if(server_mq_id==-1)
		return 0;
	client_mq_id=msgget((key_t)CLIENT_MQUEUE, 0666|IPC_CREAT);
	if(client_mq_id==-1)
		return 0;
	printf("%d: server started. \n", getpid());
	return 1;
}

//服务器关闭
int server_end(){
	printf("%d: server ending ... \n", getpid());
	msgctl(server_mq_id, IPC_RMID, 0);
	msgctl(client_mq_id, IPC_RMID, 0);
	server_mq_id=-1;
	client_mq_id=-1;
	printf("%d: server ended. \n", getpid());
	return 1;
}

//服务器端从客户端读取消息
int read_request_from_client(message_data* recieve_ptr){
	message message_passed;
	printf("%d : start read request from client. \n", getpid());
	if(msgrcv(server_mq_id, (void *)&message_passed, sizeof(*recieve_ptr), 0, 0)==-1)
		return 0;
	*recieve_ptr=message_passed.real_message;
	printf("%d: haved read an request from client %ld. \n", getpid(), message_passed.client_pid);
	return 1;
}

//服务器端发送消息给客户端
int send_response_to_client(const message_data real_message){
	message message_passed;
	printf("%d: start send response to client %d. \n", getpid(),real_message.client_pid);
	message_passed.client_pid=real_message.client_pid;
	message_passed.real_message=real_message;
	if(msgsnd(client_mq_id, (void *)&message_passed, sizeof(real_message), 0)==-1)
		return 0;
	printf("%d: haved send an response to client %d. \n", getpid(),real_message.client_pid);
	return 1;
}

//客户端启动
int client_start(){
#if DEBUG_TRACE
	printf("%d: client starting ... \n", getpid());
#endif
	server_mq_id=msgget((key_t)SERVER_MQUEUE, 0666);
	if(server_mq_id==-1)
		return 0;
	client_mq_id=msgget((key_t)CLIENT_MQUEUE, 0666);
	if(client_mq_id==-1)
		return 0;
#if DEBUG_TRACE
	printf("%d: client started. \n", getpid());
#endif
	return 1;
}

//客户端关闭
int client_end(){
	printf("%d: client ending... \n", getpid());
	server_mq_id=-1;
	client_mq_id=-1;
	printf("%d: client ended. \n", getpid());
	return 1;
}

//客户端发送消息给服务器端
int send_request_to_server(message_data real_message){
#if DEBUG_TRACE
	printf("%d: start send request to server. \n", getpid());
#endif
	message message_passed;
	message_passed.client_pid=real_message.client_pid;
	message_passed.real_message=real_message;
	if(msgsnd(server_mq_id, (void *)&message_passed, sizeof(real_message), 0)==-1){
		perror("message send failed. \n");
		return 0;
	}
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
	if(msgrcv(client_mq_id, (void *)&message_passed, sizeof(*recieve_ptr), getpid(), 0)==-1)
		return 0;
	*recieve_ptr=message_passed.real_message;
#if DEBUG_TRACE
	printf("%d: end read response from server. \n", getpid());
#endif
	return 1;
}
