//定义数据库操作所需的数据结构和函数
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include "mysql.h"

// 定义数据结构userinfo
typedef struct{
	int 	uid;
	char 	u_name[20];
	char 	pwd[20];
	char 	sex[10];
	int 	phone_num;
	char 	email[20];
}userinfo;

// 定义数据结构traininfo
typedef struct{
	int	tid;
	char	t_num[20];
	char	originator[20];
	char	terminal[20];
	struct tm departure_time;
	struct tm arrival_time;
}traininfo;

// 定义数据结构ticket
typedef struct{
	int	tid;
	struct tm departure_date;
	int	price;
	int	num;
}ticket;

//定义数据结构orders
typedef struct{
	int	oid;
	int	uid;
	int	tid;
	struct tm departure_date;
	int	num;
	int	price;
	struct tm rise_time;
	int	state;
}orders;

// 定义订单状态state类型
#define	STATE_BEFORE_PAYMENT 0
#define	STATE_AFTER_ISSUE    1
#define STATE_SUCCESS	     2
#define STATE_DELETE         3

//定义数据库操作函数
int database_init(const char* server_host, const char* sql_username, const char* sql_password, const char* db_name);
int database_close();

userinfo get_userinfo(const char* u_name_ptr, const char* pwd_ptr);
traininfo get_traininfo_by_t_num(const char* t_num_ptr);
traininfo get_traininfo_by_o_and_t(const char* originator_ptr, const char* terminal_ptr);
ticket get_ticket(const int tid, const struct tm* departure_date_ptr);
orders get_orders(const int uid);

int add_userinfo(const userinfo userinfo_to_add);
int add_order(orders order_to_add);

int modify_userinfo(const userinfo userinfo_to_modify);
int modify_order(const orders order_to_modify);
int modify_ticket(const ticket ticket_to_modify);

