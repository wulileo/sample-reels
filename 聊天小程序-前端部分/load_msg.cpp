#include "load_msg.h"
#include "ui_load_msg.h"
#include "main_form.h"
#include "mainwindow.h"
#include "group_form.h"
#include "client.h"
#include "repsw.h"
#include<QString>

Load_msg::Load_msg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Load_msg)
{
    ui->setupUi(this);
}

Load_msg::~Load_msg()
{
    delete ui;
}

void Load_msg::showEvent(QShowEvent *event)
{
    QString n;
    if(kind == SECCUSS)
    {
        loading = LOADING;
        n = QTextCodec::codecForName("GBK")->toUnicode("��¼�ɹ�");
    }
    else if(kind == NOUSER)
    {
        loading = NOLOAD;
        n = QTextCodec::codecForName("GBK")->toUnicode("�û���Ϣ������");
    }
    else if(kind == FIRSTLOAD)
    {
        loading = EX_PSW;
        n = QTextCodec::codecForName("GBK")->toUnicode("��һ�ε�¼�����޸�����");
    }
    else if(kind == LOADED)
    {
        loading = LOADING;
        n = QTextCodec::codecForName("GBK")->toUnicode("�ѵ�¼����¼�ɹ�");
    }
    else if(kind == NOLINK)
    {
        loading = NOLOAD;
        n = QTextCodec::codecForName("GBK")->toUnicode("����ʧ��");
    }
    else if(kind == NOPSW)
    {
        loading = NOLOAD;
        n = QTextCodec::codecForName("GBK")->toUnicode("�������");
    }
    ui->msg->setText(n);
}

void Load_msg::on_OKbutton_clicked()
{
    if(loading == LOADING){
        Group_form *group = new Group_form;
        group->client = client;
        group->list = list;
        group->User = User;
        group->show();
        this->close();
    }
    else if(loading == EX_PSW){
        this->close();
        Repsw *repsw = new Repsw;
        repsw->list = list;
        repsw->client = client;
        repsw->show();
    }
    else if(loading == NOLOAD){
        this->close();
    }
}
