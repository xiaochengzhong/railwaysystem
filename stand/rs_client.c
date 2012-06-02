/*本文件为客户端接口函数，提供了假的数据库访问例程。实现了在头文件rs_dbm.h中定以
 * 的数据库操作函数，它的作用如同一个中转站，先把请求传递个服务器，然后从服务器
 * 接收返回响应
 */
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "rs_mq.h"

//定义静态变量
static pid_t mypid;
static FILE *work_file;
static int traininfo_num;
static int traininfo_index;
static int orders_num;
static int orders_index;
static char u_name[20];

// 客户端初始化函数
int database_init(const char* server_host, const char* sql_username, const char* sql_password, const char* db_name){
	if(!client_start())
		return 0;
	mypid=getpid();
	work_file=(FILE *)0;
	traininfo_num=0;
	traininfo_index=0;
	orders_num=0;
	orders_index=0;
	return 1;
}

// 客户端退出函数
int database_close(){
	if(!client_end())
		return 0;
	return 1;
}

//获取和验证用户资料，如果不存在用户，返回null值, 即memset 0 
userinfo get_userinfo(const char* u_name_ptr, const char* pwd_ptr){
	userinfo userinfo_to_return;
	message_data message_send;
	message_data message_recieve;
	memset(&userinfo_to_return, 0, sizeof(userinfo_to_return));
	message_send.client_pid=mypid;
	message_send.request=c_get_userinfo;
	strcpy(message_send.userinfo_data.u_name, u_name_ptr);
	strcpy(message_send.userinfo_data.pwd, pwd_ptr);
	if(send_request_to_server(message_send)){
		if(read_response_from_server(&message_recieve)){
			if(message_recieve.response == s_success){
				userinfo_to_return=message_recieve.userinfo_data;
				strcpy(u_name, userinfo_to_return.u_name);
			}else{
				return userinfo_to_return;
			}
		}else{
			fprintf(stderr, "Server failed to respond. \n");
		}
	}else{
		fprintf(stderr, "Server not accepting requests. \n");
	}
	return userinfo_to_return;
}

// 通过车次号获取火车消息, 如果不存在，返回NULL值,即memset 0 
traininfo get_traininfo_by_t_num(const char* t_num_ptr){
	traininfo traininfo_to_return;
	message_data message_send;
	message_data message_recieve;
	memset(&traininfo_to_return, 0, sizeof(traininfo_to_return));
	message_send.client_pid=mypid;
	message_send.request=c_get_traininfo_by_t_num;
	strcpy(message_send.traininfo_data.t_num, t_num_ptr);
	strcpy(message_send.userinfo_data.u_name, u_name);
	if(send_request_to_server(message_send)){
		if(read_response_from_server(&message_recieve)){
			if(message_recieve.response == s_success){
				traininfo_to_return=message_recieve.traininfo_data;
			}else{
				return traininfo_to_return;
			}
		}else{
			fprintf(stderr, "Server failed to respond. \n");
		}
	}else{
		fprintf(stderr, "Server not accepting requests. \n");
	}
	return traininfo_to_return;
}

// 通过出发站和到达站查询火车信息，如果不存在，返回NULL值,即memset 0 
traininfo get_traininfo_by_o_and_t(const char* originator_ptr, const char* terminal_ptr){
	traininfo traininfo_to_return;
	message_data message_send;
	message_data message_recieve;
	memset(&traininfo_to_return, 0, sizeof(traininfo_to_return));
	if(!traininfo_index){
		traininfo_index=1;
		if(work_file)
			fclose(work_file);
		work_file=tmpfile();
		if(!work_file)
			return traininfo_to_return;
		message_send.client_pid=mypid;
		message_send.request=c_get_traininfo_by_o_and_t;
		strcpy(message_send.traininfo_data.originator, originator_ptr);
		strcpy(message_send.traininfo_data.terminal, terminal_ptr);
		strcpy(message_send.userinfo_data.u_name, u_name);
		if(send_request_to_server(message_send)){
			while(read_response_from_server(&message_recieve)){
				if(message_recieve.response == s_success){
					fwrite(&(message_recieve.traininfo_data), sizeof(traininfo), 1, work_file);
					traininfo_num++;
				}else{
					break;
				}
			}
		}else{
		fprintf(stderr, "Server not accepting requests. \n");	
		}
		if(traininfo_num == 0){
			traininfo_index=0;
			fclose(work_file);
			work_file=(FILE *)0;
			memset(&traininfo_to_return, 0, sizeof(traininfo_to_return));
			return traininfo_to_return;
		}
		(void)fseek(work_file, 0L, SEEK_SET);
		fread(&traininfo_to_return, sizeof(traininfo), 1, work_file);
		traininfo_num--;
		return traininfo_to_return;
	}else{
		if(traininfo_num == 0){
			traininfo_index=0;
			fclose(work_file);
			work_file=(FILE *)0;
			memset(&traininfo_to_return, 0, sizeof(traininfo_to_return));
			return traininfo_to_return;
		}
		fread(&traininfo_to_return, sizeof(traininfo), 1, work_file);
		traininfo_num--;
		return traininfo_to_return;
	}
}

//通过tid和日期查询火车票信息, 如果不存在，返回空值，即memset 0 
ticket get_ticket(const int tid, const struct tm* departure_date_ptr){
	ticket ticket_to_return;
	message_data message_send;
	message_data message_recieve;
	memset(&ticket_to_return, 0, sizeof(ticket_to_return));
	message_send.client_pid=mypid;
	message_send.request=c_get_ticket;
	message_send.ticket_data.tid=tid;
	message_send.ticket_data.departure_date=*departure_date_ptr;
	strcpy(message_send.userinfo_data.u_name, u_name);
	if(send_request_to_server(message_send)){
		if(read_response_from_server(&message_recieve)){
			if(message_recieve.response == s_success){
				ticket_to_return=message_recieve.ticket_data;
			}else{
				return ticket_to_return;
			}
		}else{
			fprintf(stderr, "Server failed to respond. \n");
		}
	}else{
		fprintf(stderr, "Server not accepting requests. \n");
	}
	return ticket_to_return;
}

// 通过用户的uid获取用户以往的所有订单记录，如果不存在，返回空值，即memset 0  
orders get_orders(const int uid){
	orders order_to_return;
	message_data message_send;
	message_data message_recieve;
	memset(&order_to_return, 0, sizeof(order_to_return));
	if(!orders_index){
		orders_index=1;
		if(work_file)
			fclose(work_file);
		work_file=tmpfile();
		if(!work_file)
			return order_to_return;
		message_send.client_pid=mypid;
		message_send.request=c_get_orders;
		message_send.order_data.uid=uid;
		strcpy(message_send.userinfo_data.u_name, u_name);
		if(send_request_to_server(message_send)){
			while(read_response_from_server(&message_recieve)){
				if(message_recieve.response == s_success){
					fwrite(&(message_recieve.order_data), sizeof(orders), 1, work_file);
					orders_num++;
				}else{
					break;
				}
			}
		}else{
		fprintf(stderr, "Server not accepting requests. \n");	
		}
		if(orders_num == 0){
			orders_index=0;
			fclose(work_file);
			work_file=(FILE *)0;
			return order_to_return;
		}
		(void)fseek(work_file, 0L, SEEK_SET);
		fread(&order_to_return, sizeof(orders), 1, work_file);
		orders_num--;
		return order_to_return;
	}else{
		if(orders_num == 0){
			orders_index=0;
			fclose(work_file);
			work_file=(FILE *)0;
			return order_to_return;
		}
		fread(&order_to_return, sizeof(orders), 1, work_file);
		orders_num--;
		return order_to_return;
	}
}

// 向数据库中添加用户注册的资料
int add_userinfo(const userinfo userinfo_to_add){
	message_data message_send;
	message_data message_recieve;
	message_send.client_pid=mypid;
	message_send.request=c_add_userinfo;
	message_send.userinfo_data=userinfo_to_add;
	if(send_request_to_server(message_send)){
		if(read_response_from_server(&message_recieve)){
			if(message_recieve.response == s_success){
				return 1;
			}
		}else{
			fprintf(stderr, "Server failed to respond. \n");
		}
	}else{
		fprintf(stderr, "Server not accepting requests. \n");
	}
	return 0;
}

// 向数据库中添加订单
int add_order(orders order_to_add){
	message_data message_send;
	message_data message_recieve;
	message_send.client_pid=mypid;
	message_send.request=c_add_order;
	message_send.order_data=order_to_add;
	strcpy(message_send.userinfo_data.u_name, u_name);
	if(send_request_to_server(message_send)){
		if(read_response_from_server(&message_recieve)){
			if(message_recieve.response == s_success){
				return 1;
			}
		}else{
			fprintf(stderr, "Server failed to respond. \n");
		}
	}else{
		fprintf(stderr, "Server not accepting requests. \n");
	}
	return 0;

}

//修改用户资料
int modify_userinfo(const userinfo userinfo_to_modify){
	message_data message_send;
	message_data message_recieve;
	message_send.client_pid=mypid;
	message_send.request=c_modify_userinfo;
	message_send.userinfo_data=userinfo_to_modify;
	if(send_request_to_server(message_send)){
		if(read_response_from_server(&message_recieve)){
			if(message_recieve.response == s_success){
				return 1;
			}
		}else{
			fprintf(stderr, "Server failed to respond. \n");
		}
	}else{
		fprintf(stderr, "Server not accepting requests. \n");
	}
	return 0;
}

//修改订单信息
int modify_order(const orders order_to_modify){
	message_data message_send;
	message_data message_recieve;
	message_send.client_pid=mypid;
	message_send.request=c_modify_order;
	message_send.order_data=order_to_modify;
	strcpy(message_send.userinfo_data.u_name, u_name);
	if(send_request_to_server(message_send)){
		if(read_response_from_server(&message_recieve)){
			if(message_recieve.response == s_success){
				return 1;
			}
		}else{
			fprintf(stderr, "Server failed to respond. \n");
		}
	}else{
		fprintf(stderr, "Server not accepting requests. \n");
	}
	return 0;
}

//修改余票信息
int modify_ticket(const ticket ticket_to_modify){
	message_data message_send;
	message_data message_recieve;
	message_send.client_pid=mypid;
	message_send.request=c_modify_ticket;
	message_send.ticket_data=ticket_to_modify;
	strcpy(message_send.userinfo_data.u_name, u_name);
	if(send_request_to_server(message_send)){
		if(read_response_from_server(&message_recieve)){
			if(message_recieve.response == s_success){
				return 1;
			}
		}else{
			fprintf(stderr, "Server failed to respond. \n");
		}
	}else{
		fprintf(stderr, "Server not accepting requests. \n");
	}
	return 0;
}
