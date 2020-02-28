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
    /* ��ʼ��MYSQL������ʧ�ܷ���NULL */
    if((mysql = mysql_init(NULL)) == NULL) {
        printf("���ݿ��ʼ��ʧ��\n");
        return -1;
    }

    /* �������ݿ⣬ʧ�ܷ���NULL 
       1.mysqlû����
       2.û��ָ�������Ƶ����ݿ����*/
    if(mysql_real_connect(mysql, "localhost", "root", "d1501450743h", "cookbook", 0, NULL, 0) == NULL){
        printf("���ݿ�����ʧ��(%d)\n",mysql_error(mysql));
        return -1;
    }
    mysql_set_character_set(mysql, "gbk");
    printf("���ݿ����ӳɹ�\n");
    
    int PORT;
    PORT = atoi(argv[1]);

    //���� socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        perror("socket ����ʧ��");
        exit(-1);
    }

    //׼��ͨ�ŵ�ַ
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_aton("172.17.0.8", &addr.sin_addr);

    //���÷�����
    int res;
    int iFlags;
    iFlags = fcntl(sockfd, F_GETFL, 0);
    iFlags |= O_NONBLOCK;
    iFlags |= O_NDELAY;
    res = fcntl(sockfd, F_SETFL, iFlags);
    if(res == -1){
        perror("���÷�����ʧ��");
        exit(-1);
    }

    //��
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    res = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if(res == -1){
        perror("��ʧ��");
        exit(-1);
    }

    
    //����
    res = listen(sockfd, 5);
    if(res==-1){
        perror("����ʧ��");
        exit(-1);
    }
    printf("������%d���ȴ� client ����\n", PORT);

    //��������
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
            printf("select ����\n");
            continue;
        }
        else if(res == 0){
            //printf("select ��ʱ\n");
            continue;
        }
        else {
            if(FD_ISSET(sockfd, &server_fd_set)){
                //��������
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
                        printf("�¿ͻ���(%d)����ɹ���ip(%s)����˿�(%d)\n", index, inet_ntoa(fromaddr.sin_addr), ntohs(fromaddr.sin_port));
        			
                    }
                    else {
                        printf("�¿ͻ���(%d)����ʧ�ܣ�ip(%s)����˿�(%d)\n", index, inet_ntoa(fromaddr.sin_addr), ntohs(fromaddr.sin_port));
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
                            printf("������Ϣ����\n");
                        }
                        else {
                            FD_CLR(clientfd[i], &server_fd_set);
                            clientfd[i] = 0;
                            printf("�ͻ���(%d)�˳�\n", i);
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
    char *no_msg_find = "û���ҵ���Ҫ����ϢŶ~�����²���";
    char type;
    memcpy(&type, read_buff, sizeof(char));
    if(type == SERCH){
        //����
        char *select_1_char = "select * from foodbook where foodname = '";
        char sql_select[200] = { 0 };
        char sig = 39;
        //ƴ���������
        memcpy(sql_select, select_1_char, strlen(select_1_char));
        memcpy(&sql_select[strlen(select_1_char)], &read_buff[1], num - 1);
        memcpy(&sql_select[strlen(select_1_char) + num - 1], &sig, 1);
        printf("SQL��䣺%s\n", sql_select);
        //�������ݿ�
        if(mysql_query(mysql, sql_select) != 0){
            printf("��ѯʧ�ܣ�\n");
        }
        else {
            if((result = mysql_store_result(mysql)) == NULL){
                printf("������ʧ�ܣ�\n");
            }
            else {
                printf("������(%d)����Ϣ\n", (int)mysql_num_rows(result));
                if((int)mysql_num_rows(result) == 0){
                    memcpy(write_buff, no_msg_find, strlen(no_msg_find));
                    int write_num = write(clientfd, write_buff,strlen(write_buff));
                    printf("��(%d)���ͳɹ�(%d):%s\n", clienti, write_num, write_buff);
                }
                //��������������Ϣ
                while((row = mysql_fetch_row(result)) != NULL){
                    sig = '\n';
                    memcpy(write_buff, row[0], strlen(row[0]));
                    memcpy(&write_buff[strlen(row[0])], &sig, 1);
                    memcpy(&write_buff[strlen(row[0]) + 1], row[1], strlen(row[1]));
                    memcpy(&write_buff[strlen(row[0]) + strlen(row[1]) + 1], &sig, 1);
                    memcpy(&write_buff[strlen(row[0]) + strlen(row[1]) + 2], row[2], strlen(row[2]));
                    int write_num = write(clientfd, write_buff,strlen(write_buff));
                    printf("��(%d)���ͳɹ�(%d):%s\n", clienti, write_num, write_buff);
                }
            }
            mysql_free_result(result);
        }
    }
    else if(type == REFER){
        //�Ƽ�
        char *sql_select = "select * from foodbook";
        printf("SQL��䣺%s\n", sql_select);
        if(mysql_query(mysql, sql_select) != 0){
            printf("��ѯʧ�ܣ�\n");
        }
        else {
            if((result = mysql_store_result(mysql)) == NULL){
                printf("������ʧ�ܣ�\n");
            }
            else {
                printf("������(%d)����Ϣ\n", (int)mysql_num_rows(result));
                if((int)mysql_num_rows(result) == 0){
                    memcpy(write_buff, no_msg_find, strlen(no_msg_find));
                    int write_num = write(clientfd, write_buff,strlen(write_buff));
                    printf("��(%d)���ͳɹ�(%d):%s\n", clienti, write_num, write_buff);
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
                        printf("��(%d)���ͳɹ�(%d):%s\n", clienti, write_num, write_buff);
                    }
                    count++;
                }
            }
            mysql_free_result(result);
        }
    }
    else if(type == HOT_FOOD){
        //���Ų�Ʒ
    }
    else if(type == RELATION){
        //��ϵ
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

        printf("SQL��䣺%s\n", sql_select);
        if(mysql_query(mysql, sql_select) != 0){
            printf("��ѯʧ�ܣ�\n");
        }
        else {
            if((result = mysql_store_result(mysql)) == NULL){
                printf("������ʧ�ܣ�\n");
            }
            else {
                printf("������(%d)����Ϣ\n", (int)mysql_num_rows(result));
                if((int)mysql_num_rows(result) == 0){
                    memcpy(write_buff, no_msg_find, strlen(no_msg_find));
                    int write_num = write(clientfd, write_buff,strlen(write_buff));
                    printf("��(%d)���ͳɹ�(%d):%s\n", clienti, write_num, write_buff);
                }
                while((row = mysql_fetch_row(result)) != NULL){
                    sig = '\n';
                    memcpy(write_buff, row[2], strlen(row[2]));
                    memcpy(&write_buff[strlen(row[2])], &sig, 1);
                    memcpy(&write_buff[strlen(row[2]) + 1], row[3], strlen(row[3]));
                    int write_num = write(clientfd, write_buff,strlen(write_buff));
                    printf("��(%d)���ͳɹ�(%d):%s\n", clienti, write_num, write_buff);
                }
            }
            mysql_free_result(result);
        }
    }
    else {
        //����
        printf("�ӿͻ��˽��մ�����Ϣ:%s\n", read_buff);
    }
    return;
}