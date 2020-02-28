#ifndef CLIENT_H
#define CLIENT_H

#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QtNetwork>
#include <sys/types.h>
#include <QMainWindow>
#include <QKeyEvent>

#include "main_form.h"
#include "repswfail.h"

//��������С
#define MAX_SIZE 1024               //��������С
#define NAME_SIZE 15                //����
#define PSW_SIZE 15                 //����
//loading״̬
#define LOADING 1                   //��½�ɹ�
#define NOLOAD 0                    //��¼ʧ��
#define EX_PSW 2                    //�޸�����
//�����¼��kind
#define NOLINK 0                    //����ʧ��
#define NOUSER 1                    //û���û���
#define SECCUSS 2                   //�ɹ�
#define FIRSTLOAD 3                 //�޸�����
#define LOADED 4                    //�Ѿ���¼
#define NOPSW 5                     //�������
//�޸������kind
#define FIR_SEC_FAIL 0              //�޸������һ�εڶ��β�һ��
#define SET_PSW_FAIL 1              //�޸�����ʧ��

//client->server
#define login_send		0x1E000011	//0x1100001E
#define password_send	0x0F000014	//0x1400000F

#define msg_send		0x00000012	//0x12000000
#define file_send		0x00000013	//0x13000000

#define log_quest		0x0F000015	//0x15000000

#define logout_quest	0x00000016	//0x16000000

//server->client
#define login_success_re	0x0261	//0x6102
#define login_success		0x0161	//0x6101
#define login_pwset			0x0361	//0x6103
#define pwset_success		0x0063	//0x6300
#define pwset_failed		0x0163	//0x6301
#define login_notarget		0x0061	//0x6100
#define login_failed		0x0461	//0x6104

#define send_success		0x0062	//0x6200
#define	send_failed			0xFF62	//0x62FF
#define send_notarget		0xFE62	//0x62FE
#define	send_offline		0xFD62	//0x62FD

#define msg_get				0x0064	//0x6400
#define file_get			0x0065	//0x6500
#define log_get				0x0066	//0x6600
#define	log_get_ready		0x0069	//0x6900
#define log_get_over		0x006A	//0x6A00

#define friend_login		0x0067	//0x67O0
#define friend_logout		0x0167	//0x6701
#define force_logout		0x0068	//0x6800
struct friends {
    char status[1];
    char name[15];
    friends* next;
};
struct lis {
    int num;
    friends *header;//��һ��Ϊ��
};
struct msg_window{
    int num;
    Main_form *head;
};

#endif // CLIENT_H
