#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/select.h>
#include<sys/types.h>
#include<sys/time.h>
#include<unistd.h>
#include"mysql.h"

#define SERCH '1'
#define HOT_FOOD '3'
#define REFER '2'
#define RELATION '4'

char read_buff[1024];
char write_buff[1024];

MYSQL *mysql;
MYSQL_RES *result;
MYSQL_ROW row;

void Resive_Msg(int, int, int);

int main(int argc, char *argv[])
{
    /* 初始化MYSQL变量，失败返回NULL */
    if((mysql = mysql_init(NULL)) == NULL) {
        printf("数据库初始化失败\n");
        return -1;
    }

    /* 连接数据库，失败返回NULL 
       1.mysql没运行
       2.没有指定的名称的数据库存在*/
    if(mysql_real_connect(mysql, "localhost", "root", "d1501450743h", "cookbook", 0, NULL, 0) == NULL){
        printf("数据库连接失败(%d)\n",mysql_error(mysql));
        return -1;
    }
    mysql_set_character_set(mysql, "gbk");
    printf("数据库连接成功\n");
    
    int PORT;
    PORT = atoi(argv[1]);

    //创建 socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        perror("socket 创建失败");
        exit(-1);
    }

    //准备通信地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_aton("172.17.0.8", &addr.sin_addr);

    //设置非阻塞
    int res;
    int iFlags;
    iFlags = fcntl(sockfd, F_GETFL, 0);
    iFlags |= O_NONBLOCK;
    iFlags |= O_NDELAY;
    res = fcntl(sockfd, F_SETFL, iFlags);
    if(res == -1){
        perror("设置非阻塞失败");
        exit(-1);
    }

    //绑定
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    res = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if(res == -1){
        perror("绑定失败");
        exit(-1);
    }

    
    //监听
    res = listen(sockfd, 5);
    if(res==-1){
        perror("监听失败");
        exit(-1);
    }
    printf("监听：%d，等待 client 连接\n", PORT);

    //处理连接
    int clientfd[10] = { 0 };
    fd_set server_fd_set;
    int max_fd = -1;
    struct timeval tv;
    while(1) {
        tv.tv_sec = 20;
        tv.tv_usec = 0;
        FD_ZERO(&server_fd_set);
        FD_SET(STDIN_FILENO, &server_fd_set);
        if(max_fd < STDIN_FILENO) {
            max_fd = STDIN_FILENO;
        }
        FD_SET(sockfd, &server_fd_set);
        if(max_fd < sockfd) {
            max_fd = sockfd;
        }
        int i;
        for(i = 0; i < 10; i++){
            if(clientfd[i] != 0){
                FD_SET(clientfd[i], &server_fd_set);
                if(max_fd < clientfd[i]){
                    max_fd = clientfd[i];
                }
            }
        }
        res = select(max_fd + 1, &server_fd_set, NULL, NULL, &tv);
        if(res < 0){
            printf("select 出错\n");
            continue;
        }
        else if(res == 0){
            //printf("select 超时\n");
            continue;
        }
        else {
            if(FD_ISSET(sockfd, &server_fd_set)){
                //连接请求
                struct sockaddr_in fromaddr;
                socklen_t address_len;
                int client_sockfd = accept(sockfd, (struct sockaddr *)&fromaddr, &address_len);
                if(client_sockfd > 0){
                    int index = -1;
                    for(i = 0; i < 10; i++){
                        if(clientfd[i] == 0){
                            index = i;
                            clientfd[i] = client_sockfd;
                            break;
                        }
                    }
                    if(index >= 0){
                        printf("新客户端(%d)加入成功，ip(%s)分配端口(%d)\n", index, inet_ntoa(fromaddr.sin_addr), ntohs(fromaddr.sin_port));
        			
                    }
                    else {
                        printf("新客户端(%d)加入失败，ip(%s)分配端口(%d)\n", index, inet_ntoa(fromaddr.sin_addr), ntohs(fromaddr.sin_port));
                    }
                }
            }
            for(i = 0; i < 10; i++){
                if(clientfd[i] != 0){
                    if(FD_ISSET(clientfd[i], &server_fd_set)){
                        int read_num = read(clientfd[i], read_buff, 1024);
                        if(read_num > 0){
                            Resive_Msg(clientfd[i], read_num, i);
                        }
                        else if(read_num < 0){
                            printf("接收消息出错\n");
                        }
                        else {
                            FD_CLR(clientfd[i], &server_fd_set);
                            clientfd[i] = 0;
                            printf("客户端(%d)退出\n", i);
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void Resive_Msg(int clientfd, int num, int clienti)
{
    memset(write_buff, 0, 1024);
    char *no_msg_find = "没有找到你要的信息哦~请重新查找";
    char type;
    memcpy(&type, read_buff, sizeof(char));
    if(type == SERCH){
        //搜索
        char *select_1_char = "select * from foodbook where foodname = '";
        char sql_select[200] = { 0 };
        char sig = 39;
        //拼接搜索语句
        memcpy(sql_select, select_1_char, strlen(select_1_char));
        memcpy(&sql_select[strlen(select_1_char)], &read_buff[1], num - 1);
        memcpy(&sql_select[strlen(select_1_char) + num - 1], &sig, 1);
        printf("SQL语句：%s\n", sql_select);
        //调用数据库
        if(mysql_query(mysql, sql_select) != 0){
            printf("查询失败！\n");
        }
        else {
            if((result = mysql_store_result(mysql)) == NULL){
                printf("保存结果失败！\n");
            }
            else {
                printf("搜索到(%d)条信息\n", (int)mysql_num_rows(result));
                if((int)mysql_num_rows(result) == 0){
                    memcpy(write_buff, no_msg_find, strlen(no_msg_find));
                    int write_num = write(clientfd, write_buff,strlen(write_buff));
                    printf("向(%d)发送成功(%d):%s\n", clienti, write_num, write_buff);
                }
                //发送搜索到的信息
                while((row = mysql_fetch_row(result)) != NULL){
                    sig = '\n';
                    memcpy(write_buff, row[0], strlen(row[0]));
                    memcpy(&write_buff[strlen(row[0])], &sig, 1);
                    memcpy(&write_buff[strlen(row[0]) + 1], row[1], strlen(row[1]));
                    memcpy(&write_buff[strlen(row[0]) + strlen(row[1]) + 1], &sig, 1);
                    memcpy(&write_buff[strlen(row[0]) + strlen(row[1]) + 2], row[2], strlen(row[2]));
                    int write_num = write(clientfd, write_buff,strlen(write_buff));
                    printf("向(%d)发送成功(%d):%s\n", clienti, write_num, write_buff);
                }
            }
            mysql_free_result(result);
        }
    }
    else if(type == REFER){
        //推荐
        char *sql_select = "select * from foodbook";
        printf("SQL语句：%s\n", sql_select);
        if(mysql_query(mysql, sql_select) != 0){
            printf("查询失败！\n");
        }
        else {
            if((result = mysql_store_result(mysql)) == NULL){
                printf("保存结果失败！\n");
            }
            else {
                printf("搜索到(%d)条信息\n", (int)mysql_num_rows(result));
                if((int)mysql_num_rows(result) == 0){
                    memcpy(write_buff, no_msg_find, strlen(no_msg_find));
                    int write_num = write(clientfd, write_buff,strlen(write_buff));
                    printf("向(%d)发送成功(%d):%s\n", clienti, write_num, write_buff);
                }
                srand((unsigned int)(time(NULL)));
                int count = 0;
                while((row = mysql_fetch_row(result)) != NULL){
                    int number1 = rand() % (int)mysql_num_rows(result);
                    int number2 = rand() % (int)mysql_num_rows(result);
                    int number3 = rand() % (int)mysql_num_rows(result);
                    if(count == number1/*  || count == number2 || count == number3*/){
                        char sig = '\n';
                        memcpy(write_buff, row[0], strlen(row[0]));
                        memcpy(&write_buff[strlen(row[0])], &sig, 1);
                        memcpy(&write_buff[strlen(row[0]) + 1], row[1], strlen(row[1]));
                        memcpy(&write_buff[strlen(row[0]) + strlen(row[1]) + 1], &sig, 1);
                        memcpy(&write_buff[strlen(row[0]) + strlen(row[1]) + 2], row[2], strlen(row[2]));
                        int write_num = write(clientfd, write_buff,strlen(write_buff));
                        printf("向(%d)发送成功(%d):%s\n", clienti, write_num, write_buff);
                    }
                    count++;
                }
            }
            mysql_free_result(result);
        }
    }
    else if(type == HOT_FOOD){
        //热门菜品
    }
    else if(type == RELATION){
        //关系
        char *select_1_char = "select distinct * from food_relation where (food1_name = '";
        char *select_2_char = "' and food2_name = '";
        char *select_3_char = "') or (food1_name = '";
        char *select_4_char = "' and food2_name = '";
        char *select_5_char = "')";
        char sql_select[200] = { 0 };
        char name1[16];
        char name2[16];
        char sig;

        memcpy(name1, &read_buff[1], 16);
        memcpy(name2, &read_buff[17], 16);

        memcpy(sql_select, select_1_char, strlen(select_1_char));//1
        memcpy(&sql_select[strlen(select_1_char)], name1, strlen(name1));
        memcpy(&sql_select[strlen(select_1_char) + strlen(name1)], select_2_char, strlen(select_2_char));//2
        memcpy(&sql_select[strlen(select_1_char) + strlen(name1) + strlen(select_2_char)], name2, strlen(name2));
        memcpy(&sql_select[strlen(select_1_char) + strlen(name1) + strlen(select_2_char) + strlen(name2)], select_3_char, strlen(select_3_char));//3
        memcpy(&sql_select[strlen(select_1_char) + strlen(name1) + strlen(select_2_char) + strlen(name2) + strlen(select_3_char)], name2, strlen(name2));
        memcpy(&sql_select[strlen(select_1_char) + strlen(name1) + strlen(select_2_char) + strlen(name2) + strlen(select_3_char) + strlen(name2)], select_4_char, strlen(select_4_char));//4
        memcpy(&sql_select[strlen(select_1_char) + strlen(name1) + strlen(select_2_char) + strlen(name2) + strlen(select_3_char) + strlen(name2) + strlen(select_4_char)], name1, strlen(name1));
        memcpy(&sql_select[strlen(select_1_char) + strlen(name1) + strlen(select_2_char) + strlen(name2) + strlen(select_3_char) + strlen(name2) + strlen(select_4_char) + strlen(name1)], select_5_char, strlen(select_5_char));//5

        printf("SQL语句：%s\n", sql_select);
        if(mysql_query(mysql, sql_select) != 0){
            printf("查询失败！\n");
        }
        else {
            if((result = mysql_store_result(mysql)) == NULL){
                printf("保存结果失败！\n");
            }
            else {
                printf("搜索到(%d)条信息\n", (int)mysql_num_rows(result));
                if((int)mysql_num_rows(result) == 0){
                    memcpy(write_buff, no_msg_find, strlen(no_msg_find));
                    int write_num = write(clientfd, write_buff,strlen(write_buff));
                    printf("向(%d)发送成功(%d):%s\n", clienti, write_num, write_buff);
                }
                while((row = mysql_fetch_row(result)) != NULL){
                    sig = '\n';
                    memcpy(write_buff, row[2], strlen(row[2]));
                    memcpy(&write_buff[strlen(row[2])], &sig, 1);
                    memcpy(&write_buff[strlen(row[2]) + 1], row[3], strlen(row[3]));
                    int write_num = write(clientfd, write_buff,strlen(write_buff));
                    printf("向(%d)发送成功(%d):%s\n", clienti, write_num, write_buff);
                }
            }
            mysql_free_result(result);
        }
    }
    else {
        //错误
        printf("从客户端接收错误消息:%s\n", read_buff);
    }
    return;
}