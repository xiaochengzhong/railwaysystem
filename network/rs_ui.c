//本文件负责客户端的ui界面显示，使用的是curses函数库
#include <curses.h>
#include<locale.h>

#include "rs_socket.h"

#define MAX_SIZE 10

//定义静态变量
static message_data static_message;
static traininfo  static_traininfo[MAX_SIZE];
static ticket static_ticket[MAX_SIZE];
static orders static_orders[MAX_SIZE];

//系统内的所有界面显示函数声明
static void start_menu(void);
static void login_menu(void);
static void main_menu(void);
static void register_menu(void);
static void userinfo_menu(void);
static void select_traininfo_menu(void);
static void traininfo_menu(int);
static void order_menu(void);
static void modify_userinfo_menu(void);

//main函数
int main(int argc, char *argv[]){
	database_init("a","a","a","a");
	setlocale(LC_ALL, "");
	initscr();
	start_menu();
	endwin();
	database_close();
	exit(EXIT_SUCCESS);
}
//系统的初始进入界面
static void start_menu(){
	int index=1;
	char commend[1];
	erase();
	cbreak();
	noecho();
	mvprintw(3 ,25, "%s", " 欢迎访问模拟铁路订票系统！");
	mvprintw(8, 35, "%s", "1. 登录");
	mvprintw(9, 35, "%s", "2. 注册");
	mvprintw(10, 35, "%s", "3. 退出");
	refresh();
	while(index){
		commend[0]=getch();
		switch(commend[0]){
			case '1': 
				echo();
				nocbreak();
				login_menu(); 
				index=0; 
				break;
			case '2':
				echo();
				nocbreak();
				register_menu();
				index=0;
				break;
			case '3': index=0; break;
			default: mvprintw(13, 35, "%s", "无效的命令!"); break;
		}
	}
}

//用户注册页面
static void register_menu(){
	char phone_num[15];
	memset(static_message.userinfo_data.pwd, '\0', sizeof(static_message.userinfo_data.pwd));
	erase();
	mvprintw(3 ,25, "%s", " 欢迎访问模拟铁路订票系统！");
	mvprintw(5, 25, "%s", "请输入用户名：");
	getstr(static_message.userinfo_data.u_name);
	mvprintw(6, 25, "%s", "请输入密码：");
	refresh();
	cbreak();
	noecho();
	int i=0;
	while(i<20){
		static_message.userinfo_data.pwd[i]=getch();
		if(static_message.userinfo_data.pwd[i]=='\n'){
			static_message.userinfo_data.pwd[i]='\0';
			break;
		}
		move(6, 37+i);
		addch('*');
		refresh();
		i++;
	}
	echo();
	nocbreak();
	mvprintw(7, 25, "%s", "请输入性别(男/女)：");
	getstr(static_message.userinfo_data.sex);
	mvprintw(8, 25, "%s", "请输入电话号码：");
	getstr(phone_num);
	static_message.userinfo_data.phone_num=atoi(phone_num);
	mvprintw(9, 25, "%s", "请输入电子邮箱：");
	getstr(static_message.userinfo_data.email);
	int result=add_userinfo(static_message.userinfo_data);
	if(result){
		mvprintw(10, 25, "%s", " 注册成功，跳转中...");
		refresh();
		sleep(1);
		printw("%s", ".");
		refresh();
		sleep(1);
		printw("%s", ".");
		refresh();
		sleep(1);
		printw("%s", ".");
		refresh();
		sleep(1);
		main_menu();
	}else{
		mvprintw(10, 25, "%s", " 注册失败！跳转中...");
		refresh();
		sleep(3);
		start_menu();
	}
}

// 用户登录界面
static void login_menu(){
	memset(static_message.userinfo_data.pwd, '\0', sizeof(static_message.userinfo_data.pwd));
	erase();
	mvprintw(3 ,25, "%s", " 欢迎访问模拟铁路订票系统！");
	mvprintw(8, 35, "%s", " 用户名：");
	getstr(static_message.userinfo_data.u_name);
	mvprintw(11, 35, "%s", " 密  码：");
	refresh();
	cbreak();
	noecho();
	int i=0;
	while(i<20){
		static_message.userinfo_data.pwd[i]=getch();
		if(static_message.userinfo_data.pwd[i]=='\n'){
			static_message.userinfo_data.pwd[i]='\0';
			break;
		}
		move(11, 44+i);
		addch('*');
		refresh();
		i++;
	}
	echo();
	nocbreak();
	static_message.userinfo_data=get_userinfo(static_message.userinfo_data.u_name, static_message.userinfo_data.pwd);
	refresh();
	userinfo userinfo_cmp;
	memset(&userinfo_cmp, 0, sizeof(userinfo_cmp));
	if(memcmp(&userinfo_cmp, &(static_message.userinfo_data), sizeof(userinfo))){
		mvprintw(13, 35, "%s", "登录成功，跳转中");
		refresh();
		sleep(1);
		printw("%s", ".");
		refresh();
		sleep(1);
		printw("%s", ".");
		refresh();
		sleep(1);
		printw("%s", ".");
		refresh();
		sleep(1);
		main_menu();
	}else{
		mvprintw(13, 35, "%s", "登录失败，用户名或密码错误！跳转中...");
		refresh();
		sleep(3);
		start_menu();
	}
}

//登录后主页
static void main_menu(){
	int index=1;
	char commend[1];
	erase();
	cbreak();
	noecho();
	mvprintw(3 ,25, "%s", " 欢迎访问模拟铁路订票系统！");
	mvprintw(5 ,25, "%s", " 主菜单：");
	mvprintw(7, 30, "%s", "1. 个人资料");
	mvprintw(8, 30, "%s", "2. 车次信息");
	mvprintw(9, 30, "%s", "3. 我的订单");
	mvprintw(10, 30, "%s", "4. 注销登录");
	mvprintw(11, 30, "%s", "5. 退出系统");
	refresh();
	while(index){
		commend[0]=getch();
		switch(commend[0]){
			case '1': 
				echo();
				nocbreak();
				userinfo_menu(); 
				index=0; 
				break;
			case '2':
				echo();
				nocbreak();
				select_traininfo_menu();
				index=0;
				break;
			case '3':
				echo();
				nocbreak();
				order_menu();
				index=0;
				break;
			case '4':
				echo();
				nocbreak();
				start_menu();
				index=0;
				break;
			case '5': index=0; break;
			default: mvprintw(13, 30, "%s", "无效的命令!"); break;
		}
	}
}

//个人信息页面
static void userinfo_menu(){
	int index=1;
	char commend[1];
	erase();
	cbreak();
	noecho();
	mvprintw(3 ,25, "%s", " 欢迎访问模拟铁路订票系统！");
	mvprintw(5 ,25, "%s", " 个人资料：");
	mvprintw(7, 30, "a. 用户 id ：%d", static_message.userinfo_data.uid);
	mvprintw(8, 30, "b. 用户名：%s", static_message.userinfo_data.u_name);
	mvprintw(9, 30, "c. 性别：%s", static_message.userinfo_data.sex);
	mvprintw(10, 30, "d. 电话号码：%d", static_message.userinfo_data.phone_num);
	mvprintw(11, 30, "e. 邮箱：%s", static_message.userinfo_data.email);
	mvprintw(13, 30, "%s", "1. 修改个人资料");
	mvprintw(13, 50, "%s", "2. 返回上一页");
	refresh();
	while(index){
		commend[0]=getch();
		switch(commend[0]){
			case '1':
				echo();
				nocbreak();
				modify_userinfo_menu();
				index=0;
				break;
			case '2':
				echo();
				nocbreak();
				main_menu();
				index=0;
				break;
			default: mvprintw(15, 30, "%s", "无效的命令!"); break;
		}
	}
}

//查询车次信息页面
static void select_traininfo_menu(){
	int index=1;
	int num=0;
	char commend[1];
	char departure_date[20];
	traininfo traininfo_cmp;
	erase();
	cbreak();
	noecho();
	mvprintw(3, 25, "%s", " 欢迎访问模拟铁路订票系统！");
	mvprintw(7, 30, "%s", "1. 通过车次号查询");
	mvprintw(10, 30, "%s", "2. 通过出发站和到达站查询");
	refresh();
	while(index){
		commend[0]=getch();
		echo();
		nocbreak();
		switch(commend[0]){
			case '1':
				mvprintw(8, 30, "%s", "请输入车次号：");
				getstr(static_message.traininfo_data.t_num);
				mvprintw(9, 30,"%s", "请输入日期(yyyy/mm/dd)：");
				getstr(departure_date);
				sscanf(departure_date, "%d/%d/%d", &(static_message.ticket_data.departure_date.tm_year), &(static_message.ticket_data.departure_date.tm_mon), &(static_message.ticket_data.departure_date.tm_mday));
				static_message.ticket_data.departure_date.tm_year-=1900;
				static_message.ticket_data.departure_date.tm_mon-=1;
				static_message.traininfo_data=get_traininfo_by_t_num(static_message.traininfo_data.t_num);
				memset(&traininfo_cmp, 0, sizeof(traininfo_cmp));
				if(memcmp(&traininfo_cmp, &(static_message.traininfo_data), sizeof(traininfo))){
					static_traininfo[num]=static_message.traininfo_data;
					num++;
					traininfo_menu(num);
					index=0;
					break;
				}else{
					mvprintw(14, 30, "%s", "无效的车次号!跳转中...");
					refresh();
					sleep(3);
					main_menu();
					index=0;
					break;
				}
			case '2':
				mvprintw(11, 30, "%s", "请输入出发站：");
				getstr(static_message.traininfo_data.originator);
				mvprintw(12, 30, "%s", "请输入到达站：");
				getstr(static_message.traininfo_data.terminal);
				mvprintw(13, 30, "%s", "请输入日期(yyyy/mm/dd)：");
				getstr(departure_date);
				sscanf(departure_date, "%d/%d/%d", &(static_message.ticket_data.departure_date.tm_year), &(static_message.ticket_data.departure_date.tm_mon), &(static_message.ticket_data.departure_date.tm_mday));
				static_message.ticket_data.departure_date.tm_year-=1900;
				static_message.ticket_data.departure_date.tm_mon-=1;
				static_message.traininfo_data=get_traininfo_by_o_and_t(static_message.traininfo_data.originator, static_message.traininfo_data.terminal);
				memset(&traininfo_cmp, 0, sizeof(traininfo_cmp));
				while(memcmp(&traininfo_cmp, &(static_message.traininfo_data), sizeof(traininfo))){
					static_traininfo[num]=static_message.traininfo_data;
					static_message.traininfo_data=get_traininfo_by_o_and_t(static_message.traininfo_data.originator, static_message.traininfo_data.terminal);
					num++;
				}
				if(num!=0){
					traininfo_menu(num);
					index=0;
					break;
				}else{
					mvprintw(14, 30, "%s", "无效的出发站或到达站!跳转中...");
					refresh();
					sleep(3);
					main_menu();
					index=0;
					break;
				}
			default: mvprintw(14, 30, "%s", "无效的命令!"); break;
		}
	}
}

// 显示车次信息页面
static void traininfo_menu(int num){
	int index=1;
	int i_traininfo=0;
	int order_result=0;
	int ticket_result=0;
	char t_num[20];
	char ticket_num[10];
	char commend[1];
	time_t now;
	erase();
	cbreak();
	noecho();
	mvprintw(3, 25, "%s", "欢迎访问模拟铁路订票系统！");
	mvprintw(5, 1, "%s", "火车编号");
	mvprintw(5, 11, "%s", "日期");
	mvprintw(5, 21, "%s", "车次号");
	mvprintw(5, 29, "%s", "出发站");
	mvprintw(5, 37, "%s", "到达站");
	mvprintw(5, 45, "%s", "出发时间");
	mvprintw(5, 55, "%s", "到达时间");
	mvprintw(5, 65, "%s", "票价");
	mvprintw(5, 71, "%s", "余票张数");
	for(;num!=0;num--){
		mvprintw(num+5, 1, "%d", static_traininfo[num-1].tid);
		mvprintw(num+5, 11, "%d/%d/%d", static_message.ticket_data.departure_date.tm_year+1900, static_message.ticket_data.departure_date.tm_mon+1, static_message.ticket_data.departure_date.tm_mday);
		mvprintw(num+5, 21, "%s", static_traininfo[num-1].t_num);
		mvprintw(num+5, 29, "%s", static_traininfo[num-1].originator);
		mvprintw(num+5, 37, "%s", static_traininfo[num-1].terminal);
		mvprintw(num+5, 45, "%d:%d:%d", static_traininfo[num-1].departure_time.tm_hour, static_traininfo[num-1].departure_time.tm_min, static_traininfo[num-1].departure_time.tm_sec);
		mvprintw(num+5, 55, "%d:%d:%d", static_traininfo[num-1].arrival_time.tm_hour, static_traininfo[num-1].arrival_time.tm_min, static_traininfo[num-1].arrival_time.tm_sec);
		static_message.ticket_data=get_ticket(static_traininfo[num-1].tid, &(static_message.ticket_data.departure_date));
		static_ticket[num-1]=static_message.ticket_data;
		mvprintw(num+5, 65, "%d", static_ticket[num-1].price);
		mvprintw(num+5, 71, "%d", static_ticket[num-1].num);
	}
	mvprintw(20, 30, "%s", "1. 订票");
	mvprintw(20, 50, "%s", "2. 返回上一页");
	refresh();
	while(index){
		commend[0]=getch();
		switch(commend[0]){
			case '1':
				echo();
				nocbreak();
				mvprintw(21, 30, "请输入车次号:");
				getstr(t_num);
				mvprintw(22, 30, "请输入张数:");
				getstr(ticket_num);
				static_message.order_data.num=atoi(ticket_num);
				while(strcmp(static_traininfo[i_traininfo].t_num, t_num))
					i_traininfo++;
				static_message.order_data.uid=static_message.userinfo_data.uid;
				static_message.order_data.tid=static_traininfo[i_traininfo].tid;
				static_message.order_data.departure_date=static_message.ticket_data.departure_date;
				static_message.order_data.price=static_ticket[i_traininfo].price*static_message.order_data.num;
				time(&now);
				static_message.order_data.rise_time=*(localtime(&now));
				static_message.order_data.state=STATE_BEFORE_PAYMENT;
				static_ticket[i_traininfo].num-=static_message.order_data.num;
				order_result=add_order(static_message.order_data);
				ticket_result=modify_ticket(static_ticket[i_traininfo]);
				if(order_result&&ticket_result){
					mvprintw(23, 30, "%s", "订票成功,返回主界面中...！");
				}else{
					mvprintw(23, 30, "%s", "订票失败,返回主界面中...！");
				}
				refresh();
				sleep(3);
				main_menu();
				index=0;
				break;
			case '2':
				echo();
				nocbreak();
				main_menu();
				index=0;
				break;
			default: mvprintw(23, 30, "%s", "无效的命令!"); break;
		}
	}
}

//显示我的订单页面
static void order_menu(){
	int index=1;
	int orders_num=0;
	int num=0;
	int i_orders=0;
	int result=0;
	char commend[1];
	char state[20];
	char oid[10];
	orders order_cmp;
	static_message.order_data=get_orders(static_message.userinfo_data.uid);
	memset(&order_cmp, 0, sizeof(order_cmp));
	while(memcmp(&order_cmp, &(static_message.order_data), sizeof(orders))){
		static_orders[orders_num]=static_message.order_data;
		static_message.order_data=get_orders(static_message.userinfo_data.uid);
		orders_num++;
	}
	erase();
	cbreak();
	noecho();
	mvprintw(3, 25, "%s", "欢迎访问模拟铁路订票系统！");
	mvprintw(5, 1, "%s", "订单号");
	mvprintw(5, 8, "%s", "用户名");
	mvprintw(5, 16, "%s", "火车编号");
	mvprintw(5, 26, "%s", "出发日期");
	mvprintw(5, 36, "%s", "张数");
	mvprintw(5, 42, "%s", "总票价");
	mvprintw(5, 50, "%s", "订单生成时间");
	mvprintw(5, 70, "%s", "订单状态");
	while(num!=orders_num){
		mvprintw(num+6, 1, "%d", static_orders[num].oid);
		mvprintw(num+6, 8, "%s",static_message.userinfo_data.u_name);
		mvprintw(num+6, 16, "%d", static_orders[num].tid);
		mvprintw(num+6, 26, "%d/%d/%d", static_orders[num].departure_date.tm_year+1900, static_orders[num].departure_date.tm_mon+1, static_orders[num].departure_date.tm_mday);
		mvprintw(num+6, 36, "%d", static_orders[num].num);
		mvprintw(num+6, 42, "%d", static_orders[num].price);
		mvprintw(num+6, 50, "%d/%d/%d %d:%d:%d", static_orders[num].rise_time.tm_year+1900, static_orders[num].rise_time.tm_mon+1, static_orders[num].rise_time.tm_mday, static_orders[num].rise_time.tm_hour, static_orders[num].rise_time.tm_min, static_orders[num].rise_time.tm_sec);
		switch(static_orders[num].state){
			case STATE_BEFORE_PAYMENT:
				strcpy(state, "待付款");break;
			case STATE_AFTER_ISSUE:
				strcpy(state, "已付款");break;
			case STATE_SUCCESS:
				strcpy(state, "订单成功");break;
			case STATE_DELETE:
				strcpy(state, "已取消");break;
			default:
				strcpy(state, "\0");break;
			}
		mvprintw(num+6, 70, "%s", state);
		num++;
	}
	if(orders_num==0){
		mvprintw(20, 15, "%s", "对不起，系统中没有你的没有订单记录，返回主界面中...");
		refresh();
		sleep(3);
		echo();
		nocbreak();
		main_menu();
		return;
	}else{
		mvprintw(20, 25, "%s", "1. 确认订单");
		mvprintw(20, 35, "%s", "2. 取消订单");
		mvprintw(20, 45, "%s", "3. 返回上一页");
	}
	refresh();
	while(index){
		commend[0]=getch();
		switch(commend[0]){
			case '1':
				echo();
				nocbreak();
				mvprintw(22, 30, "%s", "请输入确认订单号：");
				getstr(oid);
				static_message.order_data.oid=atoi(oid);
				while(static_message.order_data.oid!=static_orders[i_orders].oid)
					i_orders++;
				static_message.order_data=static_orders[i_orders];
				static_message.order_data.state=STATE_SUCCESS;
				result=modify_order(static_message.order_data);
				if(result){
					mvprintw(23, 30, "%s", "确认成功！刷新中...");
				}else{
					mvprintw(23, 30, "%s", "确认失败！刷新中...");
				}
				refresh();
				sleep(3);
				order_menu();
				index=0;
				break;
			case '2':
				echo();
				nocbreak();
				mvprintw(22, 30, "%s", "请输入取消订单号：");
				getstr(oid);
				static_message.order_data.oid=atoi(oid);
				while(static_message.order_data.oid!=static_orders[i_orders].oid)
					i_orders++;
				static_message.order_data=static_orders[i_orders];
				static_message.order_data.state=STATE_DELETE;
				result=modify_order(static_message.order_data);
				if(result){
					mvprintw(23, 30, "%s", "取消成功！刷新中...");
				}else{
					mvprintw(23, 30, "%s", "取消失败！刷新中...");
				}
				refresh();
				sleep(3);
				order_menu();
				index=0;
				break;
			case '3':
				echo();
				nocbreak();
				main_menu();
				index=0;
				break;
			default: mvprintw(23, 30, "%s", "无效的命令!"); break;
		}
	}
}

//修改个人信息页面
static void modify_userinfo_menu(){
	erase();
	char phone_num[15];
	memset(static_message.userinfo_data.pwd, '\0', sizeof(static_message.userinfo_data.pwd));
	mvprintw(3 ,25, "%s", " 欢迎访问模拟铁路订票系统！");
	mvprintw(7, 30, "旧用户名：%s", static_message.userinfo_data.u_name);
	mvprintw(8, 30, "%s", "请输入新用户名：");
	getstr(static_message.userinfo_data.u_name);
	mvprintw(9, 30, "旧密码：%s", "**********");
	mvprintw(10, 30, "%s", "请输入新密码：");
	refresh();
	cbreak();
	noecho();
	int i=0;
	while(i<20){
		static_message.userinfo_data.pwd[i]=getch();
		if(static_message.userinfo_data.pwd[i]=='\n'){
			static_message.userinfo_data.pwd[i]='\0';
			break;
		}
		move(10, 45+i);
		addch('*');
		refresh();
		i++;
	}
	echo();
	nocbreak();
	mvprintw(11, 30, "旧性别：%s", static_message.userinfo_data.sex);
	mvprintw(12, 30, "%s", "请输入新性别(男/女)：");
	getstr(static_message.userinfo_data.sex);
	mvprintw(13, 30, "旧电话号码：%d", static_message.userinfo_data.phone_num);
	mvprintw(14, 30, "%s", "请输入新电话号码：");
	getstr(phone_num);
	static_message.userinfo_data.phone_num=atoi(phone_num);
	mvprintw(15, 30, "旧邮箱：%s", static_message.userinfo_data.email);
	mvprintw(16, 30, "%s", "请输入新邮箱：");
	getstr(static_message.userinfo_data.email);
	int result=modify_userinfo(static_message.userinfo_data);
	if(result){
		mvprintw(17, 30, "%s", " 修改成功!跳转中...");
		refresh();
		sleep(3);
		userinfo_menu();
	}else{
		mvprintw(17, 30, "%s", " 修改失败！跳转中...");
		refresh();
		sleep(3);
		userinfo_menu();
	}
}


//end
