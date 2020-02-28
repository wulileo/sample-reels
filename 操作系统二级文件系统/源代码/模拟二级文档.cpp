#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <bitset>
using namespace std;

#define COMNUM 100//�����е��ַ�����С
#define DISSIZE 16*1024*1024//���̴�С����λByte��
#define DISNAME "MyDisk.img"//���̣�·��������
#define BLOCKSIZE 512
#define INODESIZE 128

//���ִ���ֵ��
#define NAME_TOO_LONG 1
#define NO_INODE 2
#define FATHER_IS_NOT_ROOT 3
#define IS_EXIST 4
#define NO_FATHER_FIND 5

class SuperBlock {
public:
	int block_size;     //block��С����λByte��
	int block_num;      //block����
	int Inode_size;     //inode��С����λByte��
	int Inode_num;      //inode����
	int block_unuse_num;//blockδʹ������
	int Inode_unuse_num;//inodeδʹ������
	char filler[488];   //superblock�հ����ȡ����ַ��Χ��24~447

	void SuperBlockSet(int, int, int, int, int, int);
};

void SuperBlock::SuperBlockSet(int b_s, int b_n, int i_s, int i_n, int b_u_n, int i_u_n) {
	block_size = b_s;
	block_num = b_n;
	Inode_size = i_s;
	Inode_num = i_n;
	block_unuse_num = b_u_n;
	Inode_unuse_num = i_u_n;
	for (int i = 0; i < 424; i++) {
		filler[i] = '\0';
	}
}

class Inode {//һ��inodeռ��128���ֽ�
public:
	int kind;          //���ͣ�1ΪĿ¼��2ΪС�ļ���3Ϊ�����ļ���4Ϊ�����ļ�
	int creat_time;    //����ʱ�䣨ʱ���е����ֱ�ʾ��1970�굽���ھ�����������
	int amend_time;    //�޸�ʱ��
	int read_time;     //��ȡʱ��
	int size;          //�ļ���С=ռ�ÿ���*512����λ�ֽڣ�
	int loc_num;       //���Ϊ�ļ����ļ�ռ�õ�ַ������
	char name[16];     //�ļ�/Ŀ¼��
	int location[10];  //�ļ�������
	int filler[12];    //���
	Inode() {
		for (int i = 0; i < 16; i++) {
			name[i] = '\0';
		}
		for (int i = 0; i < 10; i++) {
			location[i] = 0;
		}
		for (int i = 0; i < 12; i++) {
			filler[i] = 0;
		}
	}
};

void Fformat();
void Mkdir(char*);
void Ls();
void Fcreat(char*);
void Fdelete(char*);
void Remove(char*);
void Fopen(char*);
void Fwrite(int);
void Fread(int);

void IOread(int);
void IOwrite(int);
void buff_zero();//��ջ�����
void SuperBlock_Change();//����superblock
int Judge_Name(char*, int);//�ж����ֺϷ���
void DeleteInode(int);//ɾ��һ��inode
void DeleteChild(char*);//ɾ���ӽڵ㣬ɾ��Ŀ¼ʱ����ɾ����Ŀ¼��Ŀ¼���ļ�
int Serch_Block();//Ѱ�ҿ�block
void Block_Set_Num(int, int);//blockռ��λ�ı�
int Inode_Loc_Num(Inode);//����inode���ؿ��
void Loc_Change(Inode, int);//���µ�ַ��

void File_In_Mydisk();

int running = true;
SuperBlock superblock;
char buff[BLOCKSIZE];//�����
int open = 0;
/*���ļ��ı�־λ
//���Ϊ0���ʾû���ļ���
//���Ϊ������
//�����򿪵��ļ���Inode��ʼλ����ֵӦΪ512��39*128+512֮��*/
int seek = 0;//�ļ���дλ��
bitset<4096> bits;//blockռ�����
char writebuff[1024] = { 0 };
char readbuff[1024];

int main()
{
	/*//����DISSIZE��С���ļ��������
	ofstream mydisk(DISNAME);
	if (mydisk.is_open()) {
		for (int i = 0; i < DISSIZE; i++) {
			mydisk << '\0';
		}
		mydisk.close();
	}
	*/
	cout << "Impor 'help' and you can see the commonds." << endl;
	for (int i = 0; i < 1024; i++) {
		writebuff[i] = '0' + i % 10;
	}
	IOread(0);
	memcpy(&superblock, buff, 512);
	char com[COMNUM];
	while (running) {
		cout << "[/]#";
		cin.getline(com, COMNUM);
		if (strncmp(com, "logout", 6) == 0 || strncmp(com, "LOGOUT", 6) == 0) {//�˳���
			running = false;
		}
		else if (strncmp(com, "fformat", 7) == 0 || strncmp(com, "FFORMAT", 7) == 0) {//��ʽ���ļ����
			//cout << "fformat" << endl;
			//д��SuperBlock��Ϣ
			Fformat();
			open = 0;
		}
		else if (strncmp(com, "ls", 2) == 0 || strncmp(com, "LS", 2) == 0) {//��Ŀ¼��
			//cout << "ls" << endl;
			Ls();
		}
		else if (strncmp(com, "mkdir", 5) == 0 || strncmp(com, "MKDIR", 5) == 0) {//����Ŀ¼��
			//cout << "mkdir" << endl;
			char name[COMNUM - 5];
			int com_count;
			for (com_count = 0; com_count < COMNUM; com_count++) {
				if (com_count > 5 && com[com_count] != ' ')break;
			}
			memcpy(name, &com[com_count], COMNUM - com_count);
			Mkdir(name);
		}
		else if (strncmp(com, "fcreat", 6) == 0 || strncmp(com, "FCREAT", 6) == 0) {//�½��ļ���
			//cout << "fcreat" << endl;
			char name[COMNUM - 6];
			int com_count;
			for (com_count = 0; com_count < COMNUM; com_count++) {
				if (com_count > 6 && com[com_count] != ' ')break;
			}
			memcpy(name, &com[com_count], COMNUM - com_count);
			Fcreat(name);
		}
		else if (strncmp(com, "fopen", 5) == 0 || strncmp(com, "FOPEN", 5) == 0) {//���ļ���
			//cout << "fopen" << endl;
			char name[COMNUM - 5];
			int com_count;
			for (com_count = 0; com_count < COMNUM; com_count++) {
				if (com_count > 5 && com[com_count] != ' ')break;
			}
			memcpy(name, &com[com_count], COMNUM - com_count);
			Fopen(name);
		}
		else if (strncmp(com, "fclose", 6) == 0 || strncmp(com, "FCLOSE", 6) == 0) {//�ر��ļ���
			//cout << "fclose" << endl;
			open = 0;
		}
		else if (strncmp(com, "fread", 5) == 0 || strncmp(com, "FREAD", 5) == 0) {//���ļ���
			//cout << "fread" << endl;
			if (open <= 0) {
				cout << "Please fopen file first." << endl;
				continue;
			}
			char num[COMNUM - 5];
			int com_count;
			for (com_count = 0; com_count < COMNUM; com_count++) {
				if (com_count > 5 && com[com_count] != ' ')break;
			}
			memcpy(num, &com[com_count], COMNUM - com_count);
			if (atoi(num) > 1024 || atoi(num) < 1) {
				cout << "Reading or write 1-1024 bytes once." << endl;
				continue;
			}
			Fread(atoi(num));
		}
		else if (strncmp(com, "fwrite", 6) == 0 || strncmp(com, "FWRITE", 6) == 0) {//д�ļ���
			//cout << "fwrite" << endl;
			if (open <= 0) {
				cout << "Please fopen file first." << endl;
				continue;
			}
			char num[COMNUM - 6];
			int com_count;
			for (com_count = 0; com_count < COMNUM; com_count++) {
				if (com_count > 6 && com[com_count] != ' ')break;
			}
			memcpy(num, &com[com_count], COMNUM - com_count);
			if (atoi(num) > 1024 || atoi(num) < 1) {
				cout << "Reading or write 1-1024 bytes once." << endl;
				continue;
			}
			Fwrite(atoi(num));
		}
		else if (strncmp(com, "flseek", 6) == 0 || strncmp(com, "FLSEEK", 6) == 0) {//��λ�ļ���дָ���
			//cout << "flseek" << endl;
			if (open <= 0) {
				cout << "Please fopen file first." << endl;
				continue;
			}
			char num[COMNUM - 6];
			int com_count;
			for (com_count = 0; com_count < COMNUM; com_count++) {
				if (com_count > 6 && com[com_count] != ' ')break;
			}
			memcpy(num, &com[com_count], COMNUM - com_count);
			seek = atoi(num);
		}
		else if (strncmp(com, "fdelete", 7) == 0 || strncmp(com, "FDELETE", 7) == 0) {//ɾ���ļ���
			//cout << "fdelete" << endl;
			if (open > 0) {
				cout << "Please close file before delete file." << endl;
				continue;
			}
			char name[COMNUM - 7];
			int com_count;
			for (com_count = 0; com_count < COMNUM; com_count++) {
				if (com_count > 7 && com[com_count] != ' ')break;
			}
			memcpy(name, &com[com_count], COMNUM - com_count);
			Fdelete(name);
		}
		else if (strncmp(com, "remove", 6) == 0 || strncmp(com, "REMOVE", 6) == 0) {//ɾ��Ŀ¼��
			//cout << "remove" << endl;
			if (open > 0) {
				cout << "Please close file before remove file." << endl;
				continue;
			}
			char name[COMNUM - 6];
			int com_count;
			for (com_count = 0; com_count < COMNUM; com_count++) {
				if (com_count > 6 && com[com_count] != ' ')break;
			}
			memcpy(name, &com[com_count], COMNUM - com_count);
			Remove(name);
		}
		else if (strncmp(com, "help", 6) == 0 || strncmp(com, "HELP", 6) == 0) {//������Ŀ��
			cout << "All commond and example for commond:" << endl;
			cout << "fformat (or FFORMAT):     fformat      ��ʼ������" << endl;
			cout << "mkdir (or MKDIR):         mkdir /1     ����Ŀ¼" << endl;
			cout << "ls (or LS):               ls           ��ȡ������Ϣ" << endl;
			cout << "fcreat (or FCREAT):       fcreat /1    ����/1�ļ�" << endl;
			cout << "fdelete (or FDELETE):     fdelete /1   ɾ��/1�ļ�" << endl;
			cout << "remove (or REMOVE):       remove /1    ɾ��/1Ŀ¼" << endl;
			cout << "fopen (or FOPEN):         fopen /1     ��/1�ļ�" << endl;
			cout << "fclose (or FCLOSE):       close        �ر���һ��open���ļ�" << endl;
			cout << "fwrite (or FWRITE):       fwrite 1000  д��1000�ֽ�" << endl;
			cout << "flseek (or FLSEEK):       fwrite 1000  ��ȡ1000�ֽ�" << endl;
			cout << "fread (or FREAD):         fread 1000   ��дָ���Ϊ1000" << endl;
			cout << "help (or HELP):           help         ����" << endl;
			cout << "copyfile (or COPYFILE):   copyfile     ���������ļ�" << endl;
		}
		else if (strncmp(com, "copyfile", 6) == 0 || strncmp(com, "COPYFILE", 6) == 0) {//�ļ�����̡�
			File_In_Mydisk();
		}
		else if (com[0] == '\0') {//�س�
			//cout << "�س�" << endl;
			continue;
		}
		else {
			cout << "Plese check the command, and import again." << endl;
		}
		memset(com, 0, COMNUM);
	}
	
	return 0;
}

void Fformat() {
	superblock.SuperBlockSet(512, 32768, 128, 40, 32749, 40);
	//512kbһ��
	//32768��
	//128kbһInode
	//40��Inode
	//32749δʹ��
	//40δʹ��
	
	SuperBlock_Change();

	bits.reset();
	for (int i = 0; i < 19; i++) {
		bits.set(i);
	}
	memcpy(buff, &bits, BLOCKSIZE);
	IOwrite(11*BLOCKSIZE);
	bits.reset();
	for (int i = 1; i < 8; i++) {
		memcpy(buff, &bits, BLOCKSIZE);
		IOwrite((11 + i)*BLOCKSIZE);
	}
	cout << "Format diskette completely." << endl;
}

void Mkdir(char *name) {
	int error_type = Judge_Name(name, 1);
	if (error_type == NAME_TOO_LONG) {
		cout << "The name is too long,please import again." << endl;
		return;
	}
	else if (error_type == NO_INODE) {
		cout << "There is no space to creat a new one." << endl;
		return;
	}
	else if (error_type == FATHER_IS_NOT_ROOT) {
		cout << "The name's first character must be '/',please check name." << endl;
		return;
	}
	else if (error_type == IS_EXIST) {
		cout << "The file or catelogue is exist." << endl;
		return;
	}
	else if (error_type == NO_FATHER_FIND) {
		cout << "Please check the location." << endl;
		return;
	}
	buff_zero();

	IOread((40 - superblock.Inode_unuse_num) / 4 * BLOCKSIZE + BLOCKSIZE);;

	Inode inode;
	inode.kind = 1;
	inode.creat_time = int(time(0));
	inode.amend_time = int(time(0));
	inode.read_time = int(time(0));
	inode.size = 0;
	inode.loc_num = 0;
	memcpy(inode.name, name, 16);
	inode.loc_num = { 0 };

	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 0], &inode.kind, 4);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 4], &inode.creat_time, 4);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 8], &inode.amend_time, 4);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 12], &inode.read_time, 4);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 16], &inode.size, 4);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 20], &inode.loc_num, 4);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 24], &inode.name, 16);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 40], &inode.location, 88);

	IOwrite((40 - superblock.Inode_unuse_num) / 4 * BLOCKSIZE + BLOCKSIZE);

	superblock.Inode_unuse_num--;
	SuperBlock_Change();

	cout << "Mkdir success." << endl;
}

void IOread(int offset) {
	FILE *fd;

	fd = fopen(DISNAME, "rb");
	fseek(fd, offset, 0);
	fread(buff, BLOCKSIZE, 1, fd);
	fclose(fd);
}

void IOwrite(int offset) {
	FILE *fd;
	fd = fopen(DISNAME, "rb+");
	fseek(fd, offset, 0);
	fwrite(buff, BLOCKSIZE, 1, fd);
	fclose(fd);
}

void buff_zero() {
	for (int i = 0; i < BLOCKSIZE; i++) {
		buff[i] = '\0';
	}
}

void Ls() {
	Inode inode;
	time_t t;

	IOread(0);
	memcpy(&superblock, buff, 512);
	cout.setf(ios::left);
	cout << setw(20) << "����������С��" << setw(6) << superblock.block_size << setw(2) << "kb" << endl;
	cout << setw(20) << "��������������" << setw(6) << superblock.block_num << setw(2) << "��" << endl;
	cout << setw(20) << "����INODE��С��" << setw(6) << superblock.Inode_size << setw(2) << "kb" << endl;
	cout << setw(20) << "����INODE������" << setw(6) << superblock.Inode_num << setw(2) << "��" << endl;
	cout << setw(20) << "����δʹ����������" << setw(6) << superblock.block_unuse_num << setw(2) << "��" << endl;
	cout << setw(20) << "����δʹ��INODE����" << setw(6) << superblock.Inode_unuse_num << setw(2) << "��" << endl;

	for (int i = 0; i < (40 - superblock.Inode_unuse_num); i++) {
		IOread(BLOCKSIZE + i * INODESIZE);

		memcpy(&inode, buff, INODESIZE);

		cout << inode.name;
		for (int j = 0; j < 30 - int(strlen(inode.name)); j++)
			cout << "-";
		if (inode.kind == 1)cout << "Ŀ¼";
		else if (inode.kind == 2)cout << "С���ļ�";
		else if (inode.kind == 3)cout << "�����ļ�";
		else if (inode.kind == 4)cout << "�����ļ�";
		else cout << "����";
		cout << "----" << inode.size << "B";
		t = time_t(inode.creat_time);
		char* dt = ctime(&t);
		cout << "----" << dt;
	}
	return;
}

void SuperBlock_Change() {
	buff_zero();

	memcpy(&buff[0], &superblock.block_size, 4);
	memcpy(&buff[4], &superblock.block_num, 4);
	memcpy(&buff[8], &superblock.Inode_size, 4);
	memcpy(&buff[12], &superblock.Inode_num, 4);
	memcpy(&buff[16], &superblock.block_unuse_num, 4);
	memcpy(&buff[20], &superblock.Inode_unuse_num, 4);
	memcpy(&buff[24], &superblock.filler, 448);

	IOwrite(0);
}

void Fcreat(char *name) {
	int error_type = Judge_Name(name, 2);
	if (error_type == NAME_TOO_LONG) {
		cout << "The name is too long,please import again." << endl;
		return;
	}
	else if (error_type == NO_INODE) {
		cout << "There is no space to creat a new one." << endl;
		return;
	}
	else if (error_type == FATHER_IS_NOT_ROOT) {
		cout << "The name's first character must be '/',please check name." << endl;
		return;
	}
	else if (error_type == IS_EXIST) {
		cout << "The file or catelogue is exist." << endl;
		return;
	}
	else if (error_type == NO_FATHER_FIND) {
		cout << "Please check the location." << endl;
		return;
	}
	buff_zero();

	IOread((40 - superblock.Inode_unuse_num) / 4 * BLOCKSIZE + BLOCKSIZE);

	Inode inode;
	inode.kind = 2;
	inode.creat_time = int(time(0));
	inode.amend_time = int(time(0));
	inode.read_time = int(time(0));
	inode.size = 0;
	inode.loc_num = 0;
	memcpy(inode.name, name, 16);
	inode.loc_num = { 0 };

	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 0], &inode.kind, 4);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 4], &inode.creat_time, 4);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 8], &inode.amend_time, 4);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 12], &inode.read_time, 4);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 16], &inode.size, 4);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 20], &inode.loc_num, 4);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 24], &inode.name, 16);
	memcpy(&buff[(40 - superblock.Inode_unuse_num) % 4 * INODESIZE + 40], &inode.location, 88);

	IOwrite((40 - superblock.Inode_unuse_num) / 4 * BLOCKSIZE + BLOCKSIZE);

	superblock.Inode_unuse_num--;
	SuperBlock_Change();

	cout << "Fcreat success." << endl;
}

int Judge_Name(char *name, int kind) {
	char name_from_inode[16];
	int kind_from_inode;
	if (strlen(name) > 16) {
		return NAME_TOO_LONG;
	}

	if (superblock.Inode_unuse_num <= 0) {
		return NO_INODE;
	}

	if (name[0] != '/') {
		return FATHER_IS_NOT_ROOT;
	}

	int i, j;
	for (i = 0; i < (40 - superblock.Inode_unuse_num) / 4 + 1; i++) {
		IOread(i*BLOCKSIZE + BLOCKSIZE);
		if (i == (40 - superblock.Inode_unuse_num) / 4) {
			for (j = 0; j < (40 - superblock.Inode_unuse_num) % 4; j++) {
				memcpy(name_from_inode, &buff[j*INODESIZE + 24], 16);
				memcpy(&kind_from_inode, &buff[j*INODESIZE], 4);
				if (strncmp(name, name_from_inode, 16) == 0 && kind_from_inode == kind) {
					return IS_EXIST;
				}
			}
		}
		else {
			for (j = 0; j < 4; j++) {
				memcpy(name_from_inode, &buff[j*INODESIZE + 24], 16);
				memcpy(&kind_from_inode, &buff[j*INODESIZE], 4);
				if (strncmp(name, name_from_inode, 16) == 0 && kind_from_inode == kind) {
					return IS_EXIST;
				}
			}
		}
	}

	int count = 0;
	for (i = 16; i > 0; i--) {
		if (name[i] == '/'&&i != 0)break;
		else count++;
	}
	if (count == 16)return 0;//��Ŀ¼

	char father_name[16];
	for (i = 0; i < 16; i++) {
		father_name[i] = '\0';
	}
	memcpy(father_name, name, 16 - count);
	
	for (i = 0; i < (40 - superblock.Inode_unuse_num) / 4 + 1; i++) {
		IOread(i*BLOCKSIZE + BLOCKSIZE);
		if (i == (40 - superblock.Inode_unuse_num) / 4) {
			for (j = 0; j < (40 - superblock.Inode_unuse_num) % 4; j++) {
				memcpy(name_from_inode, &buff[j*INODESIZE + 24], 16);
				memcpy(&kind_from_inode, &buff[j*INODESIZE], 4);
				if (strncmp(father_name, name_from_inode, 16 - count - 1) == 0 && kind_from_inode == 1) {
					return 0;
				}
			}
		}
		else {
			for (j = 0; j < 4; j++) {
				memcpy(name_from_inode, &buff[j*INODESIZE + 24], 16);
				memcpy(&kind_from_inode, &buff[j*INODESIZE], 4);
				if (strncmp(name, name_from_inode, 16 - count - 1) == 0 && kind_from_inode == 1) {
					return 0;
				}
			}
		}
	}
	return NO_FATHER_FIND;
}

void Fdelete(char *name) {
	Inode inode;
	int i, j;
	for (i = 0; i < 40 - superblock.Inode_unuse_num; i++) {
		IOread(i*INODESIZE + BLOCKSIZE);
		memcpy(&inode, buff, INODESIZE);
		if (strncmp(name, inode.name, 16) == 0 && inode.kind >= 2) {
			if (inode.kind == 2) {//С���ļ�
				cout << "If you want to delete " << name << ".Please import 'y'." << endl;
				char com;
				com = getchar();
				getchar();
				if (com != 'y'&&com != 'Y') {
					cout << "Fdelete cancel." << endl;
					return;
				}
				else {
					for (j = 0; j < 6; j++) {
						if (inode.location[j] != 0) {
							Block_Set_Num(inode.location[j], 0);
						}
					}
					DeleteInode(i);
					superblock.Inode_unuse_num++;
					SuperBlock_Change();
					cout << "Fdelete completely." << endl;
					return;
				}
			}
			else if (inode.kind == 3) {//�����ļ�
				int location_buff[6 + 128 + 128] = { 0 };//��һ����СΪ262��int����յ�ַ
				for (i = 0; i < 6; i++) {
					location_buff[i] = inode.location[i];
				}
				if (inode.location[6] != 0) {
					IOread(inode.location[6] * BLOCKSIZE);
					memcpy(&location_buff[6], buff, BLOCKSIZE);
				}
				if (inode.location[7] != 0) {
					IOread(inode.location[7] * BLOCKSIZE);
					memcpy(&location_buff[6 + 128], buff, BLOCKSIZE);
				}
				cout << "If you want to delete " << name << ".Please import 'y'." << endl;
				char com;
				com = getchar();
				getchar();
				if (com != 'y'&&com != 'Y') {
					cout << "Fdelete cancel." << endl;
					return;
				}
				else {
					for (j = 0; j < 262; j++) {
						if (location_buff[j] != 0) {
							Block_Set_Num(location_buff[j], 0);
						}
					}
					DeleteInode(i);
					superblock.Inode_unuse_num++;
					SuperBlock_Change();
					cout << "Fdelete completely." << endl;
					return;
				}
			}
			else if (inode.kind == 4) {
				int block_buff[128];
				int block;
				cout << "If you want to delete " << name << ".Please import 'y'." << endl;
				char com;
				com = getchar();
				getchar();
				if (com != 'y'&&com != 'Y') {
					cout << "Fdelete cancel." << endl;
					return;
				}
				else {
					for (j = 0; j < 6; j++) {
						if (inode.location[j] != 0) {
							Block_Set_Num(inode.location[j], 0);
						}
					}
					IOread(inode.location[6] * BLOCKSIZE);
					memcpy(block_buff, buff, BLOCKSIZE);
					for (j = 0; j < 128; j++) {
						Block_Set_Num(block_buff[j], 0);
					}
					Block_Set_Num(inode.location[6], 0);
					IOread(inode.location[7] * BLOCKSIZE);
					memcpy(block_buff, buff, BLOCKSIZE);
					for (j = 0; j < 128; j++) {
						Block_Set_Num(block_buff[j], 0);
					}
					Block_Set_Num(inode.location[7], 0);
					
					IOread(inode.location[8] * BLOCKSIZE);
					memcpy(block_buff, buff, BLOCKSIZE);
					for (j = 0; j < 128; j++) {
						IOread(block_buff[j] * BLOCKSIZE);
						for (int k = 0; k < 128; k++) {//����
							memcpy(&block, &buff[k * 4], 4);
							if (block != 0) {
								Block_Set_Num(block, 0);
							}
						}
						Block_Set_Num(block_buff[j], 0);//һ��
					}
					Block_Set_Num(inode.location[8], 0);

					IOread(inode.location[8] * BLOCKSIZE);
					memcpy(block_buff, buff, BLOCKSIZE);
					for (j = 0; j < 128; j++) {
						IOread(block_buff[j] * BLOCKSIZE);
						for (int k = 0; k < 128; k++) {//����
							memcpy(&block, &buff[k * 4], 4);
							if (block != 0) {
								Block_Set_Num(block, 0);
							}
						}
						Block_Set_Num(block_buff[j], 0);//һ��
					}
					Block_Set_Num(inode.location[9], 0);

					DeleteInode(i);
					superblock.Inode_unuse_num++;
					SuperBlock_Change();
					cout << "Fdelete completely." << endl;
					return;
				}
			}
		}
	}
	cout << "No file to delete." << endl;
}

void DeleteInode(int inode_num) {
	IOread(inode_num / 4 * BLOCKSIZE + BLOCKSIZE);
	for (int i = inode_num % 4; i < 3; i++) {
		memcpy(&buff[i*INODESIZE], &buff[(i + 1)*INODESIZE], INODESIZE);
	}
	IOwrite(inode_num / 4 * BLOCKSIZE + BLOCKSIZE);

	for (int i = inode_num / 4 + 1; i < 10; i++) {
		IOread(i*BLOCKSIZE + BLOCKSIZE);
		IOwrite(i*BLOCKSIZE + BLOCKSIZE - INODESIZE);
	}
	return;
}

void Remove(char *name) {
	char name_from_inode[16];
	int kind_from_inode;
	int i, j;
	for (i = 0; i < (40 - superblock.Inode_unuse_num) / 4 + 1; i++) {
		IOread(i*BLOCKSIZE + BLOCKSIZE);
		if (i == (40 - superblock.Inode_unuse_num) / 4) {
			for (j = 0; j < (40 - superblock.Inode_unuse_num) % 4; j++) {
				memcpy(name_from_inode, &buff[j*INODESIZE + 24], 16);
				memcpy(&kind_from_inode, &buff[j*INODESIZE], 4);
				if (strncmp(name, name_from_inode, 16) == 0 && kind_from_inode == 1) {
					cout << "If you want to delete " << name << ".Please import 'y'" << endl;
					char com;
					com = getchar();
					getchar();
					if (com != 'y'&&com != 'Y') {
						cout << "Fdelete cancel." << endl;
						return;
					}
					else {
						DeleteInode(i * 4 + j);
						superblock.Inode_unuse_num++;
						SuperBlock_Change();
						cout << "Fdelete (" << name << ") completely." << endl;
						DeleteChild(name);
						return;
					}
				}
			}
		}
		else {
			for (j = 0; j < 4; j++) {
				memcpy(name_from_inode, &buff[j*INODESIZE + 24], 16);
				memcpy(&kind_from_inode, &buff[j*INODESIZE], 4);
				if (strncmp(name, name_from_inode, 16) == 0 && kind_from_inode == 1) {
					cout << "If you want to delete " << name << ".Please import 'y'." << endl;
					char com;
					com = getchar();
					getchar();
					if (com != 'y'&&com != 'Y') {
						cout << "Fdelete cancel." << endl;
						return;
					}
					else {
						DeleteInode(i * 4 + j);
						superblock.Inode_unuse_num++;
						SuperBlock_Change();
						cout << "Fdelete (" << name << ") completely." << endl;
						DeleteChild(name);
						return;
					}
				}
			}
		}
	}
	cout << "No catelogue to delete." << endl;
}

void DeleteChild(char *name) {
	Inode inode;
	int i, j;
	int num = 40 - superblock.Inode_unuse_num;
	int loc = 0;
	for (i = 0; i < num; i++, loc++) {
		IOread(loc*INODESIZE + BLOCKSIZE);
		memcpy(&inode, buff, INODESIZE);
		if (strncmp(name, inode.name, strlen(name)) == 0 && inode.kind >= 2 && strlen(inode.name) != strlen(name)) {
			if (inode.kind == 2) {//С���ļ�
				for (j = 0; j < 6; j++) {
					if (inode.location[j] != 0) {
						Block_Set_Num(inode.location[j], 0);
					}
				}
				DeleteInode(loc);
				superblock.Inode_unuse_num++;
				SuperBlock_Change();
				loc--;
				cout << "Fdelete (" << inode.name << ") completely." << endl;
			}
			else if (inode.kind == 3) {//�����ļ�
				int location_buff[6 + 128 + 128] = { 0 };//��һ����СΪ262��int����յ�ַ
				for (i = 0; i < 6; i++) {
					location_buff[i] = inode.location[i];
				}
				if (inode.location[6] != 0) {
					IOread(inode.location[6] * BLOCKSIZE);
					memcpy(&location_buff[6], buff, BLOCKSIZE);
				}
				if (inode.location[7] != 0) {
					IOread(inode.location[7] * BLOCKSIZE);
					memcpy(&location_buff[6 + 128], buff, BLOCKSIZE);
				}

				for (j = 0; j < 262; j++) {
					if (location_buff[j] != 0) {
						Block_Set_Num(location_buff[j], 0);
					}
				}
				DeleteInode(loc);
				superblock.Inode_unuse_num++;
				SuperBlock_Change();
				loc--;
				cout << "Fdelete (" << inode.name << ") completely." << endl;
			}
			else if (inode.kind == 4) {//�����ļ�
				int block_buff[128];
				int block;
				for (j = 0; j < 6; j++) {
					if (inode.location[j] != 0) {
						Block_Set_Num(inode.location[j], 0);
					}
				}
				IOread(inode.location[6] * BLOCKSIZE);
				memcpy(block_buff, buff, BLOCKSIZE);
				for (j = 0; j < 128; j++) {
					Block_Set_Num(block_buff[j], 0);
				}
				Block_Set_Num(inode.location[6], 0);
				IOread(inode.location[7] * BLOCKSIZE);
				memcpy(block_buff, buff, BLOCKSIZE);
				for (j = 0; j < 128; j++) {
					Block_Set_Num(block_buff[j], 0);
				}
				Block_Set_Num(inode.location[7], 0);

				IOread(inode.location[8] * BLOCKSIZE);
				memcpy(block_buff, buff, BLOCKSIZE);
				for (j = 0; j < 128; j++) {
					IOread(block_buff[j] * BLOCKSIZE);
					for (int k = 0; k < 128; k++) {//����
						memcpy(&block, &buff[k * 4], 4);
						if (block != 0) {
							Block_Set_Num(block, 0);
						}
					}
					Block_Set_Num(block_buff[j], 0);//һ��
				}
				Block_Set_Num(inode.location[8], 0);

				IOread(inode.location[8] * BLOCKSIZE);
				memcpy(block_buff, buff, BLOCKSIZE);
				for (j = 0; j < 128; j++) {
					IOread(block_buff[j] * BLOCKSIZE);
					for (int k = 0; k < 128; k++) {//����
						memcpy(&block, &buff[k * 4], 4);
						if (block != 0) {
							Block_Set_Num(block, 0);
						}
					}
					Block_Set_Num(block_buff[j], 0);//һ��
				}
				Block_Set_Num(inode.location[9], 0);

				DeleteInode(i);
				superblock.Inode_unuse_num++;
				SuperBlock_Change();
				cout << "Fdelete completely." << endl;
				return;
			}
			
		}
		else if (strncmp(name, inode.name, strlen(name)) == 0 && inode.kind == 1) {//��Ŀ¼
			DeleteInode(i);
			superblock.Inode_unuse_num++;
			SuperBlock_Change();
			cout << "Fdelete (" << inode.name << ") completely." << endl;
		}
	}
	return;
}

void Fopen(char *name) {
	if (open > 0) {
		cout << "One file has opened." << endl;
		return;
	}
	char name_from_inode[16];
	int kind_form_inode;
	for (int i = 0; i < 40 - superblock.Inode_unuse_num; i++) {
		IOread(i*INODESIZE + BLOCKSIZE);
		memcpy(name_from_inode, &buff[24], 16);
		memcpy(&kind_form_inode, &buff[0], 4);
		if (strncmp(name, name_from_inode, 16) == 0 && kind_form_inode == 2) {
			open = i * INODESIZE + BLOCKSIZE;
			cout << "Fopen " << name << " seccuss." << endl;
			return;
		}
	}
	cout << "There is no file named '" << name << "'." << endl;
	return;
}

void Fwrite(int count) {
	IOread(open);

	Inode inode;
	memcpy(&inode, buff, INODESIZE);

	if ((inode.size < seek + count) && (superblock.block_unuse_num - ((seek + count) / BLOCKSIZE - seek / BLOCKSIZE) < 0)) {
		cout << "There is no space to read data." << endl;
		return;
	}
	if (seek + count > (6 + 128 * 2) * BLOCKSIZE || inode.size > (6 + 128 * 2) * BLOCKSIZE) {
		inode.kind = 4;
	}
	else if (seek + count > 6 * BLOCKSIZE || inode.size > 6 * BLOCKSIZE) {
		inode.kind = 3;
	}
	//cout << inode.kind << endl;
	int i;
	buff_zero();
	inode.size = seek;
	int p = 0;//ָ��writebuff��дλ��
	if (inode.kind == 2) {//С���ļ�
		//���seek������
		if (seek % BLOCKSIZE == 0) {
			i = seek / BLOCKSIZE;
		}
		else {
			i = seek / BLOCKSIZE + 1;
		}
		for (; i < 6; i++) {
			if (inode.location[i] != 0) {
				Block_Set_Num(inode.location[i], 0);
				inode.location[i] = 0;
				inode.loc_num--;
			}
		}

		//��ʼд�ļ�
		for (;;) {
			if (inode.size % BLOCKSIZE == 0) {
				inode.loc_num++;
				inode.location[inode.loc_num - 1] = Serch_Block();
				Block_Set_Num(inode.location[inode.loc_num - 1], 1);
				if (count < BLOCKSIZE) {
					memcpy(buff, &writebuff[p], count);
					memset(&buff[count], 0, BLOCKSIZE - count);
					IOwrite(inode.location[inode.loc_num - 1] * BLOCKSIZE);
					inode.size += count;
					count = 0;
					p = count;
				}
				else {
					memcpy(buff, &writebuff[p], BLOCKSIZE);
					IOwrite(inode.location[inode.loc_num - 1] * BLOCKSIZE);
					inode.size += BLOCKSIZE;
					count -= BLOCKSIZE;
					p += BLOCKSIZE;
				}
			}
			else {
				IOread(inode.location[inode.loc_num - 1] * BLOCKSIZE);
				if (count <= BLOCKSIZE - inode.size % BLOCKSIZE) {
					memcpy(&buff[inode.size % BLOCKSIZE], &writebuff[p], count);
					memset(&buff[inode.size % BLOCKSIZE + count], 0, BLOCKSIZE - inode.size % BLOCKSIZE - count);
					IOwrite(inode.location[inode.loc_num - 1] * BLOCKSIZE);
					inode.size += count;
					count = 0;
				}
				else {
					memcpy(&buff[inode.size % BLOCKSIZE], &writebuff[p], BLOCKSIZE - inode.size % BLOCKSIZE);
					IOwrite(inode.location[inode.loc_num - 1] * BLOCKSIZE);
					count -= BLOCKSIZE - inode.size % BLOCKSIZE;
					p += BLOCKSIZE - inode.size % BLOCKSIZE;
					inode.size += BLOCKSIZE - inode.size % BLOCKSIZE;
				}
			}
			if (count == 0)break;
		}
		IOread(open);
		memcpy(buff, &inode, INODESIZE);
		IOwrite(open);
	}
	else if (inode.kind == 3) {//�����ļ�//���ڴ����ļ���˵��ֻ����ǰ�ѵ�ַ�����ڴ��У���ز�����С���ļ�һ��
		int location_buff[6 + 128 + 128] = { 0 };//��һ����СΪ262��int����յ�ַ
		int block;
		for (i = 0; i < 6; i++) {
			location_buff[i] = inode.location[i];
		}
		if (inode.location[6] != 0) {
			IOread(inode.location[6] * BLOCKSIZE);
			memcpy(&location_buff[6], buff, BLOCKSIZE);
		}
		else {
			inode.location[6] = Serch_Block();
			Block_Set_Num(inode.location[6], 1);
			block = Serch_Block();
			Block_Set_Num(block, 1);
			inode.loc_num++;
			buff_zero();
			memcpy(buff, &block, 4);
			IOwrite(inode.location[6] * BLOCKSIZE);
		}
		if (inode.location[7] != 0) {
			IOread(inode.location[7] * BLOCKSIZE);
			memcpy(&location_buff[6 + 128], buff, BLOCKSIZE);
		}
		else {
			inode.location[7] = Serch_Block();
			Block_Set_Num(inode.location[7], 1);
			block = Serch_Block();
			Block_Set_Num(block, 1);
			inode.loc_num++;
			buff_zero();
			memcpy(buff, &block, 4);
			IOwrite(inode.location[7] * BLOCKSIZE);
		}


		//��ʼ�ļ�����
		//���seek������
		if (seek % BLOCKSIZE == 0) {
			i = seek / BLOCKSIZE;
		}
		else {
			i = seek / BLOCKSIZE + 1;
		}
		for (; i < 262; i++) {
			if (location_buff[i] != 0) {
				Block_Set_Num(location_buff[i], 0);
				location_buff[i] = 0;
				inode.loc_num--;
			}
		}
		
		for (;;) {
			if (inode.size % BLOCKSIZE == 0) {
				inode.loc_num++;
				location_buff[inode.loc_num - 1] = Serch_Block();
				Block_Set_Num(location_buff[inode.loc_num - 1], 1);
				if (count < BLOCKSIZE) {
					memcpy(buff, &writebuff[p], count);
					memset(&buff[count], 0, BLOCKSIZE - count);
					IOwrite(location_buff[inode.loc_num - 1] * BLOCKSIZE);
					inode.size += count;
					count = 0;
					p = count;
				}
				else {
					memcpy(buff, &writebuff[p], BLOCKSIZE);
					IOwrite(location_buff[inode.loc_num - 1] * BLOCKSIZE);
					inode.size += BLOCKSIZE;
					count -= BLOCKSIZE;
					p += BLOCKSIZE;
				}
			}
			else {
				IOread(location_buff[inode.loc_num - 1] * BLOCKSIZE);
				if (count <= BLOCKSIZE - inode.size % BLOCKSIZE) {
					memcpy(&buff[inode.size % BLOCKSIZE], &writebuff[p], count);
					memset(&buff[inode.size % BLOCKSIZE + count], 0, BLOCKSIZE - inode.size % BLOCKSIZE - count);
					IOwrite(location_buff[inode.loc_num - 1] * BLOCKSIZE);
					inode.size += count;
					count = 0;
				}
				else {
					memcpy(&buff[inode.size % BLOCKSIZE], &writebuff[p], BLOCKSIZE - inode.size % BLOCKSIZE);
					IOwrite(location_buff[inode.loc_num - 1] * BLOCKSIZE);
					count -= BLOCKSIZE - inode.size % BLOCKSIZE;
					p += BLOCKSIZE - inode.size % BLOCKSIZE;
					inode.size += BLOCKSIZE - inode.size % BLOCKSIZE;
				}
			}
			if (count == 0)break;
		}
		//���µ�ַ��
		memcpy(&inode.location, location_buff, 24);
		memcpy(buff, &location_buff[6], BLOCKSIZE);
		IOwrite(inode.location[6] * BLOCKSIZE);
		memcpy(buff, &location_buff[6 + 128], BLOCKSIZE);
		IOwrite(inode.location[7] * BLOCKSIZE);

		IOread(open);
		memcpy(buff, &inode, INODESIZE);
		IOwrite(open);
	}
	else if (inode.kind == 4) {//�����ļ�
		int block;
		int block_buff[128];
		if (inode.location[8] == 0) {
			inode.location[8] = Serch_Block();
			Block_Set_Num(inode.location[8], 1);
			for (i = 0; i < 128; i++) {
				block_buff[i] = Serch_Block();
				Block_Set_Num(block_buff[i], 1);
				buff_zero();
				IOwrite(block_buff[i] * BLOCKSIZE);
			}
			memcpy(&buff, &block_buff, BLOCKSIZE);
			IOwrite(inode.location[8] * BLOCKSIZE);
		}
		IOread(inode.location[8] * BLOCKSIZE);
		memcpy(block_buff, buff, BLOCKSIZE);
		if (inode.location[9] == 0) {
			inode.location[9] = Serch_Block();
			Block_Set_Num(inode.location[9], 1);
			for (i = 0; i < 128; i++) {
				block_buff[i] = Serch_Block();
				Block_Set_Num(block_buff[i], 1);
				buff_zero();
				IOwrite(block_buff[i] * BLOCKSIZE);
			}
			memcpy(buff, block_buff, BLOCKSIZE);
			IOwrite(inode.location[9] * BLOCKSIZE);
		}
		
		//��ʼд��
		for (;;) {
			if (inode.size % BLOCKSIZE == 0) {
				inode.loc_num++;
				block = Serch_Block();
				Block_Set_Num(block, 1);
				if (count < BLOCKSIZE) {
					memcpy(buff, &writebuff[p], count);
					memset(&buff[count], 0, BLOCKSIZE - count);
					IOwrite(block * BLOCKSIZE);
					inode.size += count;
					count = 0;
					p = count;
				}
				else {
					memcpy(buff, &writebuff[p], BLOCKSIZE);
					IOwrite(block * BLOCKSIZE);
					inode.size += BLOCKSIZE;
					count -= BLOCKSIZE;
					p += BLOCKSIZE;
				}
				Loc_Change(inode, block);
			}
			else {
				block = Inode_Loc_Num(inode);
				IOread(block * BLOCKSIZE);
				if (count <= BLOCKSIZE - inode.size % BLOCKSIZE) {
					memcpy(&buff[inode.size % BLOCKSIZE], &writebuff[p], count);
					memset(&buff[inode.size % BLOCKSIZE + count], 0, BLOCKSIZE - inode.size % BLOCKSIZE - count);
					IOwrite(block * BLOCKSIZE);
					inode.size += count;
					count = 0;
				}
				else {
					memcpy(&buff[inode.size % BLOCKSIZE], &writebuff[p], BLOCKSIZE - inode.size % BLOCKSIZE);
					IOwrite(block * BLOCKSIZE);
					count -= BLOCKSIZE - inode.size % BLOCKSIZE;
					p += BLOCKSIZE - inode.size % BLOCKSIZE;
					inode.size += BLOCKSIZE - inode.size % BLOCKSIZE;
				}
			}
			if (count == 0)break;
		}
	}
	SuperBlock_Change();
	seek = 0;
}

void Loc_Change(Inode inode,int block) {
	int i;
	int block_buff[128];
	if (inode.loc_num < 6) {
		inode.location[inode.loc_num - 1] = block;
		IOread(open);
		memcpy(buff, &inode, INODESIZE);
		IOwrite(open);
	}
	else if (inode.loc_num < 6 + 128) {
		i = inode.loc_num - 6 - 1;
		IOread(inode.location[6] * BLOCKSIZE);
		memcpy(&buff[i], &block, 4);
		IOwrite(inode.location[6] * BLOCKSIZE);
	}
	else if (inode.loc_num < 6 + 2 * 128) {
		i = inode.loc_num - (6 + 128) - 1;
		IOread(inode.location[7] * BLOCKSIZE);
		memcpy(&buff[i], &block, 4);
		IOwrite(inode.location[7] * BLOCKSIZE);
	}
	else if (inode.loc_num < 6 + 128 * 2 + 128 * 128) {
		i = inode.loc_num - (6 + 128 + 128) - 1;
		IOread(inode.location[8] * BLOCKSIZE);
		memcpy(block_buff, buff, BLOCKSIZE);

		IOread(block_buff[i / 128] * BLOCKSIZE);
		block_buff[i % 128] = block;
		memcpy(buff, block_buff, BLOCKSIZE);
		IOwrite(block_buff[i / 128] * BLOCKSIZE);

		IOread(open);
		memcpy(&buff, &inode, INODESIZE);
		IOwrite(open);
	}
	else {
		i = inode.loc_num - (6 + 128 + 128 + 128 * 128) - 1;
		IOread(inode.location[9] * BLOCKSIZE);
		memcpy(block_buff, buff, BLOCKSIZE);

		IOread(block_buff[i / 128] * BLOCKSIZE);
		block_buff[i % 128] = block;
		memcpy(buff, block_buff, BLOCKSIZE);
		IOwrite(block_buff[i / 128] * BLOCKSIZE);

		IOread(open);
		memcpy(&buff, &inode, INODESIZE);
		IOwrite(open);
	}
}
int Inode_Loc_Num(Inode inode) {
	int i;
	int block;
	int block_buff[128];
	if (inode.loc_num < 6) {
		block = inode.location[inode.loc_num - 1];
	}
	else if (inode.loc_num < 6 + 128) {
		i = inode.loc_num - 6 - 1;
		IOread(inode.location[6] * BLOCKSIZE);
		memcpy(&block, &buff[i], 4);
	}
	else if (inode.loc_num < 6 + 2 * 128) {
		i = inode.loc_num - (6 + 128);
		IOread(inode.location[7] * BLOCKSIZE);
		memcpy(&block, &buff[i - 1], 4);
	}
	else if (inode.loc_num < 6 + 128 * 2 + 128 * 128) {
		i = inode.loc_num - (6 + 128 + 128) - 1;
		IOread(inode.location[8] * BLOCKSIZE);
		memcpy(&block_buff[0], buff, BLOCKSIZE);
		IOread(block_buff[i / 128] * BLOCKSIZE);
		memcpy(&block, &block_buff[i % 128 - 1], 4);
	}
	else {
		i = inode.loc_num - (6 + 128 + 128 + 128 * 128) - 1;
		IOread(inode.location[9] * BLOCKSIZE);
		memcpy(&block_buff[0], buff, BLOCKSIZE);
		IOread(block_buff[i / 128] * BLOCKSIZE);
		memcpy(&block, &block_buff[i % 128 - 1], 4);
	}
	return block;
}

int Serch_Block() {
	int i, j;
	for (i = 0; i < 8; i++) {
		IOread((11 + i)*BLOCKSIZE);
		memcpy(&bits, buff, BLOCKSIZE);
		for (j = 0; j < 4096; j++) {
			if (!bits.test(j)) {
				return i * BLOCKSIZE + j;
			}
		}
	}
	return 0;
}

void Block_Set_Num(int loc, int num) {
	IOread(loc / 4096 * BLOCKSIZE + 11 * BLOCKSIZE);
	memcpy(&bits, buff, BLOCKSIZE);
	if (num == 0) {
		bits.reset(loc % 4096);
		superblock.block_unuse_num++;
	}
	else if (num == 1) {
		bits.set(loc % 4096);
		superblock.block_unuse_num--;
	}
	memcpy(buff, &bits, BLOCKSIZE);
	IOwrite(loc / 4096 * BLOCKSIZE + 11 * BLOCKSIZE);
}

void Fread(int count) {
	Inode inode;
	IOread(open);
	memcpy(&inode, buff, INODESIZE);
	int i;
	int p = 0;//��ȡ��������
	int first_block = seek / BLOCKSIZE;
	if (inode.kind == 2) {//С���ļ�
		i = first_block;
		for (; i < 6; i++) {
			IOread(inode.location[i] * BLOCKSIZE);
			if (i == first_block) {//��һ��
				if (count < BLOCKSIZE - seek % BLOCKSIZE) {//��ȡ��count
					memcpy(&readbuff[p], &buff[seek], count);
					p += count;
					count = 0;
				}
				else if (inode.size - seek < count) {//��ȡ���ļ�ĩβ
					memcpy(&readbuff[p], &buff[seek], inode.size - seek);
					p += inode.size - seek;
					count = 0;
				}
				else {//��ȡ�������β
					memcpy(&readbuff[p], &buff[seek], BLOCKSIZE - seek % BLOCKSIZE);
					count -= BLOCKSIZE - seek % BLOCKSIZE;
					p += BLOCKSIZE - seek % BLOCKSIZE;
					seek += BLOCKSIZE - seek % BLOCKSIZE;
				}
			}
			else if (inode.location[i + 1] == 0) {//���һ��
				if (inode.size - seek > count) {//��ȡ��count
					memcpy(&readbuff[p], &buff[0], count);
					p += count;
					count = 0;
				}
				else {//��ȡ���ļ�ĩβ
					memcpy(&readbuff[p], &buff[0], inode.size - seek);
					p += inode.size - seek;
					count = 0;
				}
			}
			else {//�м��
				memcpy(&readbuff[p], buff, BLOCKSIZE);
				p += BLOCKSIZE;
				count -= BLOCKSIZE;
			}
			if (count == 0)break;
		}
		cout << "read (" << p << ") byte:" << endl;
		for (int j = 0; j < p; j++) {
			cout << readbuff[j];
		}
		cout << endl;
		memset(readbuff, 0, 1024);
		seek = 0;
	}
	else if (inode.kind == 3) {//�����ļ�
		int location_buff[6 + 128 + 128] = { 0 };//��һ����СΪ262��int����յ�ַ
		for (i = 0; i < 6; i++) {
			location_buff[i] = inode.location[i];
		}
		if (inode.location[6] != 0) {
			IOread(inode.location[6] * BLOCKSIZE);
			memcpy(&location_buff[6], buff, BLOCKSIZE);
		}
		if (inode.location[7] != 0) {
			IOread(inode.location[7] * BLOCKSIZE);
			memcpy(&location_buff[6 + 128], buff, BLOCKSIZE);
		}

		i = first_block;
		for (; i < 262; i++) {
			IOread(inode.location[i] * BLOCKSIZE);
			if (i == first_block) {//��һ��
				if (count < BLOCKSIZE - seek % BLOCKSIZE) {//��ȡ��count
					memcpy(&readbuff[p], &buff[seek], count);
					p += count;
					count = 0;
				}
				else if (inode.size - seek < count) {//��ȡ���ļ�ĩβ
					memcpy(&readbuff[p], &buff[seek], inode.size - seek);
					p += inode.size - seek;
					count = 0;
				}
				else {//��ȡ�������β
					memcpy(&readbuff[p], &buff[seek], BLOCKSIZE - seek % BLOCKSIZE);
					count -= BLOCKSIZE - seek % BLOCKSIZE;
					p += BLOCKSIZE - seek % BLOCKSIZE;
					seek += BLOCKSIZE - seek % BLOCKSIZE;
				}
			}
			else if (inode.location[i + 1] == 0) {//���һ��
				if (inode.size - seek > count) {//��ȡ��count
					memcpy(&readbuff[p], &buff[0], count);
					p += count;
					count = 0;
				}
				else {//��ȡ���ļ�ĩβ
					memcpy(&readbuff[p], &buff[0], inode.size - seek);
					p += inode.size - seek;
					count = 0;
				}
			}
			else {//�м��
				memcpy(&readbuff[p], buff, BLOCKSIZE);
				p += BLOCKSIZE;
				count -= BLOCKSIZE;
			}
			if (count == 0)break;
		}
		cout << "read (" << p << ") byte:" << endl;
		for (int j = 0; j < p; j++) {
			cout << readbuff[j];
		}
		cout << endl;
		memset(readbuff, 0, 1024);
		seek = 0;
	}
	else if (inode.kind == 4) {
		cout << "Sorry, I had not solove this part." << endl;
	}
}

void File_In_Mydisk() {
	FILE *fd;
	int i;

	char name1[17] = "/home/texts/t";
	char name2[17] = "/home/photos/p";
	char name3[17] = "/home/reports/r";

	Fopen(name1);
	fd = fopen("text.txt", "rb");
	seek = 0;
	for (i = 0;; i++) {
		seek = i * 1024;
		memset(writebuff, 0, 1024);
		fseek(fd, i * 1024, 0);
		int ret = fread(writebuff, 1, 1024, fd);
		Fwrite(ret);
		if (feof(fd))break;
	}
	open = 0;
	fclose(fd);
	cout << "copy (" << name1 << ") seccuss." << endl;

	
	Fopen(name2);
	fd = fopen("photo.PNG", "rb");
	seek = 0;
	for (i = 0;; i++) {
		seek = i * 1024;
		memset(writebuff, 0, 1024);
		fseek(fd, i * 1024, 0);
		int ret = fread(writebuff, 1, 1024, fd);
		Fwrite(ret);
		if (feof(fd))break;
	}
	open = 0;
	fclose(fd);
	cout << "copy (" << name2 << ") seccuss." << endl;

	Fopen(name3);
	fd = fopen("report.pdf", "rb");
	seek = 0;
	for (i = 0;; i++) {
		seek = i * 1024;
		memset(writebuff, 0, 1024);
		fseek(fd, i * 1024, 0);
		int ret = fread(writebuff, 1, 1024, fd);
		Fwrite(ret);
		if (feof(fd))break;
	}
	open = 0;
	fclose(fd);
	cout << "copy (" << name3 << ") seccuss." << endl;
}