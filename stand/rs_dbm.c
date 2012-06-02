// 本文件实现在re_dbm.h中定义的函数

#include "rs_dbm.h"

//定义静态变量 
static MYSQL my_conn;
static int db_connected=0;
static int traininfo_index=0;
static int traininfo_num=0;
static int orders_index=0;
static MYSQL_RES *sta_res_ptr;

// 实现函数
//  数据库连接初始化
int database_init(const char* server_host, const char* sql_username, const char* sql_password, const char* db_name){
	if(db_connected==1)
		return 1;
	mysql_init(&my_conn);
	mysql_options(&my_conn, MYSQL_SET_CHARSET_NAME, "utf8");
	if(!mysql_real_connect(&my_conn, server_host, sql_username, sql_password, db_name, 0, NULL, 0)){
		fprintf(stderr, "Database connection error: %d, %s .\n", mysql_errno(&my_conn), mysql_error(&my_conn));
		return 0;
	}
	db_connected=1;
	return 1;
}

//数据库连接关闭
int database_close(){
	if(db_connected==1)
		mysql_close(&my_conn);
	db_connected=0;
	return 1;
}

//获取和验证用户资料，如果不存在用户，返回null值, 即memset 0 
userinfo get_userinfo(const char* u_name_ptr, const char* pwd_ptr){
	MYSQL_RES *res_ptr;
	MYSQL_ROW mysqlrow;
	int result;
	userinfo userinfo_to_return;
	char sql[250];
	char u_name[20];
	char pwd[20];
	memset(&userinfo_to_return, 0, sizeof(userinfo_to_return));
	strcpy(u_name, u_name_ptr);
	strcpy(pwd, pwd_ptr);
	sprintf(sql,"select * from userinfo where Uname= '%s' and Pwd= '%s'", u_name, pwd);
	result=mysql_query(&my_conn, sql);
	if(result){
		fprintf(stderr, "select error: %s. \n", mysql_error(&my_conn));
	}else{
		res_ptr=mysql_store_result(&my_conn);
		if(res_ptr){
			if(mysql_num_rows(res_ptr)>0){
				if((mysqlrow=mysql_fetch_row(res_ptr))){
					sscanf(mysqlrow[0], "%d", &(userinfo_to_return.uid));
					strcpy(userinfo_to_return.u_name, mysqlrow[1]);
					strcpy(userinfo_to_return.pwd, mysqlrow[2]);
					strcpy(userinfo_to_return.sex, mysqlrow[3]);
					sscanf(mysqlrow[4], "%d", &(userinfo_to_return.phone_num));
					strcpy(userinfo_to_return.email, mysqlrow[5]);
				}
			}else{
				mysql_free_result(res_ptr);
				return userinfo_to_return;
			}
			mysql_free_result(res_ptr);
		}
	}
	return userinfo_to_return;
}

// 通过车次号获取火车消息, 如果不存在，返回NULL值,即memset 0 
traininfo get_traininfo_by_t_num(const char* t_num_ptr){
	MYSQL_RES *res_ptr;
	MYSQL_ROW mysqlrow;
	int result;
	traininfo traininfo_to_return;
	char sql[250];
	memset(&traininfo_to_return, 0, sizeof(traininfo_to_return));
	sprintf(sql,"select * from traininfo where Tnum= '%s'", t_num_ptr);
	result=mysql_query(&my_conn, sql);
	if(result){
		fprintf(stderr, "select error: %s. \n", mysql_error(&my_conn));
	}else{
		res_ptr=mysql_store_result(&my_conn);
		if(res_ptr){
			if(mysql_num_rows(res_ptr)>0){
				if((mysqlrow=mysql_fetch_row(res_ptr))){
					sscanf(mysqlrow[0], "%d", &(traininfo_to_return.tid));
					strcpy(traininfo_to_return.t_num, mysqlrow[1]);
					strcpy(traininfo_to_return.originator, mysqlrow[2]);
					strcpy(traininfo_to_return.terminal, mysqlrow[3]);
					sscanf(mysqlrow[4], "%d:%d:%d", &(traininfo_to_return.departure_time.tm_hour), &(traininfo_to_return.departure_time.tm_min), &(traininfo_to_return.departure_time.tm_sec));
					sscanf(mysqlrow[5], "%d:%d:%d", &(traininfo_to_return.arrival_time.tm_hour), &(traininfo_to_return.arrival_time.tm_min), &(traininfo_to_return.arrival_time.tm_sec));
				}
			}else{
				mysql_free_result(res_ptr);
				return traininfo_to_return;
			}
			mysql_free_result(res_ptr);
		}
	}
	return traininfo_to_return;
}

// 通过出发站和到达站查询火车信息，如果不存在，返回NULL值,即memset 0 
traininfo get_traininfo_by_o_and_t(const char* originator_ptr, const char* terminal_ptr){
	MYSQL_ROW mysqlrow;
	int result;
	char sql[250];
	traininfo traininfo_to_return;
	memset(&traininfo_to_return, 0, sizeof(traininfo_to_return));
	if(!traininfo_index){
		sprintf(sql, "select * from traininfo where Originator='%s' and Terminal='%s'", originator_ptr, terminal_ptr);
		result=mysql_query(&my_conn, sql);
		if(result){
			fprintf(stderr, "select error: %s. \n", mysql_error(&my_conn));
			return traininfo_to_return;
		}
		sta_res_ptr=mysql_store_result(&my_conn);
		traininfo_num=(int)mysql_num_rows(sta_res_ptr);
		traininfo_index=1;
	}
	if(!sta_res_ptr){
		fprintf(stderr, "sta_res_ptr is NULL. \n");
		return traininfo_to_return;
	}else{
		if(traininfo_num!=0){
			if((mysqlrow=mysql_fetch_row(sta_res_ptr))){
						sscanf(mysqlrow[0], "%d", &(traininfo_to_return.tid));
						strcpy(traininfo_to_return.t_num, mysqlrow[1]);
						strcpy(traininfo_to_return.originator, mysqlrow[2]);
						strcpy(traininfo_to_return.terminal, mysqlrow[3]);
						sscanf(mysqlrow[4], "%d:%d:%d", &(traininfo_to_return.departure_time.tm_hour), &(traininfo_to_return.departure_time.tm_min), &(traininfo_to_return.departure_time.tm_sec));
						sscanf(mysqlrow[5], "%d:%d:%d", &(traininfo_to_return.arrival_time.tm_hour), &(traininfo_to_return.arrival_time.tm_min), &(traininfo_to_return.arrival_time.tm_sec));

			}
			traininfo_num--;
		}else{
			mysql_free_result(sta_res_ptr);
			sta_res_ptr=NULL;
			traininfo_index=0;
			return traininfo_to_return;
		}
	}
	return traininfo_to_return;
}

//通过tid和日期查询火车票信息, 如果不存在，返回空值，即memset 0 
ticket get_ticket(const int tid, const struct tm* departure_date_ptr){
	MYSQL_RES *res_ptr;
	MYSQL_ROW mysqlrow;
	int result;
	ticket ticket_to_return;
	char sql[250];
	char date[10];
	memset(&ticket_to_return, 0, sizeof(ticket_to_return));
	sprintf(date,"%d-%d-%d", departure_date_ptr->tm_year+1900, departure_date_ptr->tm_mon+1, departure_date_ptr->tm_mday);
	sprintf(sql,"select * from ticket where Tid= '%d' and DepartureDate= '%s'", tid, date);
	result=mysql_query(&my_conn, sql);
	if(result){
		fprintf(stderr, "select error: %s. \n", mysql_error(&my_conn));
	}else{
		res_ptr=mysql_store_result(&my_conn);
		if(res_ptr){
			if(mysql_num_rows(res_ptr)>0){
				if((mysqlrow=mysql_fetch_row(res_ptr))){
					ticket_to_return.tid=tid;
					ticket_to_return.departure_date=*departure_date_ptr;
					sscanf(mysqlrow[2], "%d", &(ticket_to_return.price));
					sscanf(mysqlrow[3], "%d", &(ticket_to_return.num));
				}
			}else{
				mysql_free_result(res_ptr);
				return ticket_to_return;
			}
			mysql_free_result(res_ptr);
		}
	}
	return ticket_to_return;
}

// 通过用户的uid获取用户以往的所有订单记录，如果不存在，返回空值，即memset 0  
orders get_orders(const int uid){
	MYSQL_ROW mysqlrow;
	int result;
	char sql[250];
	orders order_to_return;
	memset(&order_to_return, 0, sizeof(order_to_return));
	if(!orders_index){
		sprintf(sql, "select * from orders where Uid=%d", uid);
		result=mysql_query(&my_conn, sql);
		if(result){
			fprintf(stderr, "select error: %s. \n", mysql_error(&my_conn));
			return order_to_return;
		}
		sta_res_ptr=mysql_store_result(&my_conn);
		orders_index=1;
	}
	if(!sta_res_ptr){
		fprintf(stderr, "sta_res_ptr is NULL. \n");
		return order_to_return;
	}else{
		if((mysqlrow=mysql_fetch_row(sta_res_ptr))){
					sscanf(mysqlrow[0], "%d", &(order_to_return.oid));
					sscanf(mysqlrow[1], "%d", &(order_to_return.uid));
					sscanf(mysqlrow[2], "%d", &(order_to_return.tid));
					sscanf(mysqlrow[3], "%d-%d-%d", &(order_to_return.departure_date.tm_year), &(order_to_return.departure_date.tm_mon), &(order_to_return.departure_date.tm_mday));
					order_to_return.departure_date.tm_year-=1900;
					order_to_return.departure_date.tm_mon-=1;
					sscanf(mysqlrow[4], "%d", &(order_to_return.num));
					sscanf(mysqlrow[5], "%d", &(order_to_return.price));
					sscanf(mysqlrow[6], "%d-%d-%d %d:%d:%d", &(order_to_return.rise_time.tm_year), &(order_to_return.rise_time.tm_mon), &(order_to_return.rise_time.tm_mday), &(order_to_return.rise_time.tm_hour), &(order_to_return.rise_time.tm_min), &(order_to_return.rise_time.tm_sec));
					order_to_return.rise_time.tm_year-=1900;
					order_to_return.rise_time.tm_mon-=1;
					sscanf(mysqlrow[7], "%d", &(order_to_return.state));
		}else{
			mysql_free_result(sta_res_ptr);
			sta_res_ptr=NULL;
			orders_index=0;
			return order_to_return;
		}
	}
	return order_to_return;

}

// 向数据库中添加用户注册的资料
int add_userinfo(const userinfo userinfo_to_add){
	int result;
	char sql[250];
	if(!db_connected)
		return 0;
	sprintf(sql, "insert into userinfo(Uname, Pwd, Sex, PhoneNum, Email) values('%s', '%s', '%s', %d, '%s')", userinfo_to_add.u_name, userinfo_to_add.pwd, userinfo_to_add.sex, userinfo_to_add.phone_num, userinfo_to_add.email);
	result=mysql_query(&my_conn, sql);
	if(result){
		fprintf(stderr, "Insert error %d: %s. \n", mysql_errno(&my_conn), mysql_error(&my_conn));
		return 0;
	}
	return 1;
}

// 向数据库中添加订单
int add_order(orders order_to_add){
	int result;
	char sql[250];
	char departure_date[10];
	if(!db_connected)
		return 0;
	sprintf(departure_date, "%d-%d-%d", order_to_add.departure_date.tm_year+1900, order_to_add.departure_date.tm_mon+1, order_to_add.departure_date.tm_mday);
	sprintf(sql, "insert into orders(Uid, Tid, DepartureDate, Num, Price, RiseTime, State) values(%d, %d, '%s', %d, %d, FROM_UNIXTIME(%d), %d)", order_to_add.uid, order_to_add.tid, departure_date, order_to_add.num, order_to_add.price, (int )(mktime(&(order_to_add.rise_time))), order_to_add.state);
	result=mysql_query(&my_conn, sql);
	if(result){
		fprintf(stderr, "Insert error %d: %s. \n", mysql_errno(&my_conn), mysql_error(&my_conn));
		return 0;
	}
	return 1;
}

//修改用户资料
int modify_userinfo(const userinfo userinfo_to_modify){
	int result;
	char sql[250];
	if(!db_connected)
		return 0;
	sprintf(sql, "update userinfo set Uname='%s', Pwd='%s', Sex='%s', PhoneNum=%d, Email='%s' where Uid=%d ", userinfo_to_modify.u_name, userinfo_to_modify.pwd, userinfo_to_modify.sex, userinfo_to_modify.phone_num, userinfo_to_modify.email, userinfo_to_modify.uid);
	result=mysql_query(&my_conn, sql);
	if(result){
		fprintf(stderr, "Update error %d: %s. \n", mysql_errno(&my_conn), mysql_error(&my_conn));
		return 0;
	}
	return 1;
}

//修改订单信息
int modify_order(const orders order_to_modify){
	int result;
	char sql[250];
	if(!db_connected)
		return 0;
	sprintf(sql, "update orders set State=%d where Oid=%d ", order_to_modify.state, order_to_modify.oid);
	result=mysql_query(&my_conn, sql);
	if(result){
		fprintf(stderr, "Update error %d: %s. \n", mysql_errno(&my_conn), mysql_error(&my_conn));
		return 0;
	}
	return 1;
}

//修改余票信息
int modify_ticket(const ticket ticket_to_modify){
	int result;
	char sql[250];
	char date[10];
	if(!db_connected)
		return 0;
	sprintf(date,"%d-%d-%d", ticket_to_modify.departure_date.tm_year+1900, ticket_to_modify.departure_date.tm_mon+1, ticket_to_modify.departure_date.tm_mday);
	sprintf(sql, "update ticket set Num=%d where Tid=%d and DepartureDate='%s'", ticket_to_modify.num, ticket_to_modify.tid, date);
	result=mysql_query(&my_conn, sql);
	if(result){
		fprintf(stderr, "Update error %d: %s. \n", mysql_errno(&my_conn), mysql_error(&my_conn));
		return 0;
	}
	return 1;
}
