 //本文件为服务器端接口函数，负责服务器端程序的运行
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "rs_mq.h"

// 声明静态变量
static int server_state=1;

//定义信号处理函数
void catch_signals(){
	server_state=0;
}

//定义记录日志函数
void sys_log(char* log){
	char syslog_dir[10]="./sys_log\0";
	char log_file[12];
	char file_name[22];
	char cur_time[10];
	time_t now;
	struct tm* time_now;
	if(!opendir(syslog_dir))
		mkdir(syslog_dir, 0777);
	time(&now);
	time_now=localtime(&now);
	sprintf(log_file, "/%d-%d-%d", time_now->tm_year+1900, time_now->tm_mon+1, time_now->tm_mday);
	sprintf(cur_time, "%d:%d:%d ", time_now->tm_hour, time_now->tm_min, time_now->tm_sec);
	int i=0;
	for(; i!=10; i++)
		file_name[i]=syslog_dir[i];
	strcat(file_name, log_file);
	int fd=open(file_name, O_WRONLY|O_APPEND|O_CREAT, 0777);
	if(fd==-1){
		fprintf(stderr, "Open logfile %s error. \n", file_name);
		return;
	}
	write(fd, cur_time, strlen(cur_time));
	write(fd, log, strlen(log));
	write(fd, "\n", 1);
	close(fd);
}

//定义所有客户端请求命令的处理函数
void exec_request(const message_data message_request){
	message_data message_response;
	int end_index=0;
	char log[250];
	message_response=message_request;
	message_response.response=s_success;
	switch(message_request.request){
		case c_add_userinfo:
			if(!add_userinfo(message_request.userinfo_data)){
				message_response.response=s_failure;
				sprintf(log, " 用户 %s 注册失败.", message_request.userinfo_data.u_name);
				sys_log(log);
			}else{
				sprintf(log, " 用户 %s 注册成功.", message_request.userinfo_data.u_name);
				sys_log(log);
			}
			break;
		case c_get_userinfo:
			message_response.userinfo_data=get_userinfo(message_request.userinfo_data.u_name, message_request.userinfo_data.pwd);
			userinfo userinfo_cmp;
			memset(&userinfo_cmp, 0, sizeof(userinfo_cmp));
			if(!memcmp(&userinfo_cmp, &(message_response.userinfo_data), sizeof(userinfo)))
			{
				message_response.response=s_failure;
				sprintf(log, "获取用户 %s 资料失败.", message_request.userinfo_data.u_name);
				sys_log(log);
			}else{
				sprintf(log, " 获取用户 %s 资料成功.", message_request.userinfo_data.u_name);
				sys_log(log);
			}
			break;
		case c_modify_userinfo:
			if(!modify_userinfo(message_request.userinfo_data))
			{
				message_response.response=s_failure;
				sprintf(log, "修改用户 %s 资料失败.", message_request.userinfo_data.u_name);
				sys_log(log);
			}else{
				sprintf(log, " 修改用户 %s 资料成功.", message_request.userinfo_data.u_name);
				sys_log(log);
			}
				break;
		case c_get_traininfo_by_t_num:
			message_response.traininfo_data=get_traininfo_by_t_num(message_request.traininfo_data.t_num);
			traininfo traininfo_cmp;
			memset(&traininfo_cmp, 0, sizeof(traininfo_cmp));
			if(!memcmp(&traininfo_cmp, &(message_response.traininfo_data), sizeof(traininfo))){
				message_response.response=s_failure;
				sprintf(log, "用户 %s 获取车次 %s 信息失败.", message_request.userinfo_data.u_name, message_request.traininfo_data.t_num);
				sys_log(log);
			}else{
				sprintf(log, "用户 %s 获取车次 %s 信息成功.", message_request.userinfo_data.u_name, message_request.traininfo_data.t_num);
				sys_log(log);
			}
			break;
		case c_get_traininfo_by_o_and_t:
			while(!end_index){
				message_response.traininfo_data=get_traininfo_by_o_and_t(message_request.traininfo_data.originator, message_request.traininfo_data.terminal);
				traininfo traininfo_cmp;
				memset(&traininfo_cmp, 0, sizeof(traininfo_cmp));
				if(!memcmp(&traininfo_cmp, &(message_response.traininfo_data), sizeof(traininfo))){
					message_response.response=s_failure;
					end_index=1;
				}else{
					if(!send_response_to_client(message_response)){
						fprintf(stderr, "Server Warning: fail to response to %d. \n", message_response.client_pid);
						sprintf(log, "用户 %s 获取车次 %s 信息失败.", message_request.userinfo_data.u_name, message_response.traininfo_data.t_num);
						sys_log(log);
					}else{
						sprintf(log, "用户 %s 获取车次 %s 信息成功.", message_request.userinfo_data.u_name, message_response.traininfo_data.t_num);
						sys_log(log);
					}
				}
			}
			break;
		case c_get_ticket:
			message_response.ticket_data=get_ticket(message_request.ticket_data.tid, &(message_request.ticket_data.departure_date));
			ticket ticket_cmp;
			memset(&ticket_cmp, 0, sizeof(ticket_cmp));
			if(!memcmp(&ticket_cmp, &(message_response.ticket_data), sizeof(ticket)))
				message_response.response=s_failure;
			break;
		case c_modify_ticket:
			if(!modify_ticket(message_request.ticket_data))
				message_response.response=s_failure;
			break;
		case c_add_order:
			if(!add_order(message_request.order_data)){
				message_response.response=s_failure;
				sprintf(log, "用户 %s 购买 Tid 为 %d 的车次火车票 %d 张失败.", message_request.userinfo_data.u_name, message_request.order_data.tid, message_request.order_data.num);
				sys_log(log);
			}else{
				sprintf(log, "用户 %s 购买 Tid 为 %d 的车次火车票 %d 张成功.", message_request.userinfo_data.u_name, message_request.order_data.tid, message_request.order_data.num);
				sys_log(log);
			}
			break;
		case c_get_orders:
			while(!end_index){
				message_response.order_data=get_orders(message_request.order_data.uid);
				orders order_cmp;
				memset(&order_cmp, 0, sizeof(order_cmp));
				if(!memcmp(&order_cmp, &(message_response.order_data), sizeof(orders))){
					message_response.response=s_failure;
					end_index=1;
				}else{
					if(!send_response_to_client(message_response)){
						fprintf(stderr, "Server Warning: fail to response to %d. \n", message_response.client_pid);
						sprintf(log, "获取用户 %s 订单 %d 信息失败.", message_request.userinfo_data.u_name, message_response.order_data.oid);
						sys_log(log);
					}else{
						sprintf(log, "获取用户 %s 订单 %d 信息成功.", message_request.userinfo_data.u_name, message_response.order_data.oid);
						sys_log(log);
					}
				}
			}
			break;
		case c_modify_order:
			if(!modify_order(message_request.order_data)){
				message_response.response=s_failure;
				sprintf(log, "修改用户 %s 订单 %d 信息失败.", message_request.userinfo_data.u_name, message_request.order_data.oid);
				sys_log(log);
			}else{
				sprintf(log, "修改用户 %s 订单 %d 信息成功.", message_request.userinfo_data.u_name, message_request.order_data.oid);
				sys_log(log);
			}
			break;
		default:
			message_response.response=s_failure;
			break;
	}
	if(!send_response_to_client(message_response)){
		fprintf(stderr, "Server Warning: fail to response to %d. \n", message_response.client_pid);
		return;
	}
	return;
}

//服务器端执行入口函数
int main(int argc, char*argv[]){
	struct sigaction new_action, old_action;
	message_data message_request;

	new_action.sa_handler=catch_signals;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags=0;
	if ((sigaction(SIGINT, &new_action, &old_action) != 0) ||
        (sigaction(SIGHUP, &new_action, &old_action) != 0) ||
        (sigaction(SIGTERM, &new_action, &old_action) != 0)) {
        fprintf(stderr, "Server startup error, signal catching failed\n");
        exit(EXIT_FAILURE);
	}

	char* server_host="localhost"; 
	char* sql_username="zxc"; 
	char* sql_password="zxc"; 
	char* db_name="railwaysystem";

	if(!database_init(server_host, sql_username, sql_password, db_name)){
		fprintf(stderr, "Server error: could not init database. \n");
		sys_log("数据库连接失败.");
		exit(EXIT_FAILURE);
	}
	sys_log("数据库连接成功.");
	if(!server_start()){
		sys_log("服务器启动失败.");
		exit(EXIT_FAILURE);
	}
	sys_log("服务器启动成功.");
	while(server_state){
		if(read_request_from_client(&message_request)){
			exec_request(message_request);
		}else{
			if(server_state){
				fprintf(stderr, "Server ended, cannot read request from message queue. \n");
				sys_log("Server ended, cannot read request from message queue.");
			}
			server_state=0;
		}
	}
	database_close();
	server_end();
	sys_log("服务器关闭.");
	exit(EXIT_SUCCESS);
}
