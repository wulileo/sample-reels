#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cstdio>

#define SROUCEFILE "test1.c"
#define OUT1 "out1.txt"
#define OUT2 "out2.txt"

struct sig
{
	char name[20];
	bool type;//0为int，1为void
	bool fun;//0为变量，1为函数
};

char buff[10 * 1024];//源程序全存储
char buff_line[256];//源程序按 行 存储
char str_buff[1024][10];//全程序按 词 存储
int word_count;//词计数
int word_loc;//词指针

sig sig_arr[20];//符号表
int sig_count = 0;//符号表计数

struct code_4
{
	char op[4];
	char arg1[20];
	char arg2[20];
	char result[20];
	int num;//当前四元式标号
};

code_4 code_4_arr[1024];
int code_4_num = 0;//四元式数
int code_4_loc = 0;//四元式指针
int code_4_v_num = 0;//临时变量标号

int code_4_back_num = 0;//回填号
int while_start = 0;//while语句起始点

void Lex_Anal();
void Judge_Word(char *);
void Syn_Anal();

void StatementString();
void Statement();
void StatementType();
void FunctionStatement();
void VariableStatement();
void FormalParameter();
void ParameterList();
void Parameter();
void SentenceBlock();
void InternalStatement();
void InternalVariableStatement();
void SentenceString();
void Sentence();
void assignmentSentence();
void returnSentence();
void whileSentence();
void ifSentence();
void Expression();
void addExpression();
void item();
void factor();
void FTYPE();
void call();
void ActualParameter();
void ActualParameterList();


using namespace std;
int main()
{
	Syn_Anal();
}

void Lex_Anal() {
	//词法分析器
	remove(OUT1);
	int count = 0;
	int begin = 0;//记录开始位置
	int end = 0;  //记录结束位置
	ifstream file(SROUCEFILE);
	if (!file.is_open())
	{
		cout << "Error opening file";
		exit(1);
	}
	while (!file.eof())
	{
		file.getline(buff_line, 256);
		begin = 0;
		end = (int)strlen(buff_line);
		for (int i = 0; i < (int)strlen(buff_line); i++) {
			if (buff_line[i] == '/'&&buff_line[i + 1] == '/') {
				end = i;
				break;
			}
		}
		memcpy(buff + count, buff_line + begin, end - begin);
		count += (end - begin);
	}
	file.close();
	char *start_c;
	char *end_c;
	start_c = buff;
	end_c = buff;
	char word[20] = { 0 };
	for (;;) {
		end_c = start_c;
		if ((*start_c >= '0'&&*start_c <= '9')
			|| (*start_c >= 'a'&&*start_c <= 'z')
			|| (*start_c >= 'A'&&*start_c <= 'Z')) {
			int i = 0;
			while (*end_c != '('
				&& *end_c != ')'
				&& *end_c != ';'
				&& *end_c != '<'
				&& *end_c != '>'
				&& *end_c != '='
				&& *end_c != '+'
				&& *end_c != '-'
				&& *end_c != '*'
				&& *end_c != '/'
				&& *end_c != ','
				&& *end_c != '!'
				&& *end_c != '{'
				&& *end_c != '}'
				&& *end_c != ' '
				&& *end_c != '\t') {
				word[i] = *end_c;
				i++;
				end_c++;
			}
			Judge_Word(word);
			start_c = end_c;
		}
		else if (*start_c == '+'
			|| *start_c == '-'
			|| *start_c == '*'
			|| *start_c == '/'
			|| *start_c == '>'
			|| *start_c == '<'
			|| *start_c == ','
			|| *start_c == ';'
			|| *start_c == '='
			|| *start_c == '('
			|| *start_c == ')'
			|| *start_c == '{'
			|| *start_c == '}') {
			word[0] = *start_c;
			if (*(start_c + 1) == '=') {
				word[1] = *(start_c + 1);
				start_c = end_c + 2;
			}
			else {
				start_c = end_c + 1;
			}
			Judge_Word(word);
		}
		else {
			start_c++;
		}
		if (*start_c == '#') {
			break;
		}
		memset(word, 0, 20);
	}
	return;
}

void Judge_Word(char *word) {
	ofstream file(OUT1, ios::app);
	if (*word >= '0'&&*word <= '9') {
		cout << setw(20) << word << "  数字" << endl;
	}
	else if ((*word <= 'z' && *word >= 'a') || (*word <= 'Z' && *word >= 'A')) {
		if (strncmp(word, "int", 3) == 0
			|| strncmp(word, "void", 4) == 0
			|| strncmp(word, "if", 2) == 0
			|| strncmp(word, "else", 4) == 0
			|| strncmp(word, "while", 5) == 0
			|| strncmp(word, "return", 6) == 0) {
			cout << setw(20) << word << "  关键词" << endl;
		}
		else {
			cout << setw(20) << word << "  标识符" << endl;
		}
	}
	else if (*word == '=') {
		cout << setw(20) << word << "  赋值号" << endl;
	}
	else if (*word == ';') {
		cout << setw(20) << word << "  界符" << endl;
	}
	else if (*word == ',') {
		cout << setw(20) << word << "  分隔符" << endl;
	}
	else if (*word == '(') {
		cout << setw(20) << word << "  左括号" << endl;
	}
	else if (*word == ')') {
		cout << setw(20) << word << "  右括号" << endl;
	}
	else if (*word == '{') {
		cout << setw(20) << word << "  左大括号" << endl;
	}
	else if (*word == '}') {
		cout << setw(20) << word << "  右大括号" << endl;
	}
	else {
		cout << setw(20) << word << "  算符" << endl;
	}
	file << word << endl;
	file.close();
}

void Syn_Anal() {
	//语法分析器
	cout << "词法分析结果：" << endl;
	Lex_Anal();
	cout << "词法分析结果存放在：" << OUT1 << endl;

	//语法分析+语义分析+四元式产生
	ifstream file1(OUT1);
	if (!file1.is_open())
	{
		cout << "Error opening file";
		exit(1);
	}
	word_count = 0;
	while (!file1.eof())
	{
		file1.getline(str_buff[word_count], 10);
		word_count++;
	}
	file1.close();
	word_loc = 0;
	//寻找main函数作为入口
	code_4_num++;
	code_4_arr[code_4_loc].op[0] = 'j';
	code_4_arr[code_4_loc].num = code_4_num;
	code_4_arr[code_4_loc].arg1[0] = '-';
	code_4_arr[code_4_loc].arg2[0] = '-';
	code_4_loc++;
	//开始
	StatementString();
	if (code_4_arr[0].result[0] > '9' || code_4_arr[0].result[0] < '0') {
		cout << "缺少main函数入口" << endl;
		exit(0);
	}
	cout << "语法正确" << endl;
	cout << "四元式产生如下：" << endl;
	remove(OUT2);
	ofstream file2(OUT2, ios::app);
	for (int i = 0; i < code_4_num; i++) {
		cout << code_4_arr[i].num << "(" << code_4_arr[i].op << "," << code_4_arr[i].arg1 << "," << code_4_arr[i].arg2 << "," << code_4_arr[i].result << ")" << endl;
		file2 << code_4_arr[i].num << "(" << code_4_arr[i].op << "," << code_4_arr[i].arg1 << "," << code_4_arr[i].arg2 << "," << code_4_arr[i].result << ")" << endl;
	}
	cout << "四元式存放在：" << OUT2 << endl;
	file2.close();
}

void StatementString() {
	int i = 0;
	while (strncmp(str_buff[word_loc], "int", 3) == 0 || strncmp(str_buff[word_loc], "void", 4) == 0) {
		Statement();
		i++;
	}
	if (i == 0) {
		cout << "缺少int或void" << endl;
		exit(0);
	}
}

void Statement() {
	if (strncmp(str_buff[word_loc], "int", 3) == 0) {
		word_loc++;
		//写入字符表 变量
		int i;
		for (i = 0; i < sig_count; i++) {
			if (sig_arr[i].type == 0
				&& strncmp(sig_arr[i].name, str_buff[word_loc], strlen(str_buff[word_loc])) == 0) {
				break;
			}
		}
		if (i >= sig_count) {
			memcpy(&sig_arr[sig_count].name, str_buff[word_loc], strlen(str_buff[word_loc]));
			sig_arr[sig_count].type = 0;
			StatementType();
		}
		else {
			cout << "变量存在" << str_buff[word_loc] << word_loc << endl;
			exit(0);
		}
	}
	else if (strncmp(str_buff[word_loc], "void", 4) == 0) {
		word_loc++;
		//写入字符表 函数
		int i;
		for (i = 0; i < sig_count; i++) {
			if (sig_arr[i].type == 1
				&& strncmp(sig_arr[i].name, str_buff[word_loc], strlen(str_buff[word_loc])) == 0) {
				break;
			}
		}
		if (i >= sig_count) {
			if (strcmp(str_buff[word_loc], "main") == 0) {//找到main函数作为入口
				string s = to_string(code_4_num + 1);
				const char *p = s.data();
				memcpy(&code_4_arr[0].result, p, strlen(p));
			}
			memcpy(&sig_arr[sig_count].name, str_buff[word_loc], strlen(str_buff[word_loc]));
			sig_arr[sig_count].type = 1;
			word_loc++;
			FunctionStatement();
		}
		else {
			cout << "函数声明存在" << str_buff[word_loc] << word_loc << endl;
			exit(0);
		}
	}
	else {
		cout << "缺少标识符int或者void" << str_buff[word_loc] << word_loc << endl;
		exit(0);
	}
}

void StatementType() {
	word_loc++;
	if (strncmp(str_buff[word_loc], ";", 1) == 0) {
		VariableStatement();
	}
	else {
		FunctionStatement();
	}
}

void VariableStatement() {
	sig_arr[sig_count].fun = 0;//变量
	sig_count++;
	if (strncmp(str_buff[word_loc], ";", 1) == 0) {
		word_loc++;
	}
	else {
		cout << "缺少;" << str_buff[word_loc] << word_loc << endl;
		exit(0);
	}
}

void FunctionStatement() {
	sig_arr[sig_count].fun = 1;//函数
	sig_count++;
	if (strncmp(str_buff[word_loc], "(", 1) == 0) {
		word_loc++;
		FormalParameter();
		if (strncmp(str_buff[word_loc], ")", 1) == 0) {
			word_loc++;
		}
		else {
			cout << "缺少)" << str_buff[word_loc] << word_loc << endl;
			exit(0);
		}
		SentenceBlock();
	}
	else {
		cout << "缺少(" << str_buff[word_loc] << word_loc << endl;
		exit(0);
	}
}
void FormalParameter() {
	if (strncmp(str_buff[word_loc], ")", 1) != 0) {
		ParameterList();
	}
}

void ParameterList() {
	if (strncmp(str_buff[word_loc], "int", 3) == 0) {
		Parameter();
	}
	while (strncmp(str_buff[word_loc], ",", 1) == 0) {
		word_loc++;
		Parameter();
	}
}

void Parameter() {
	if (strncmp(str_buff[word_loc], "int", 3) == 0) {
		word_loc++;
		//更新参数列表
		int i;
		for (i = 0; i < sig_count; i++) {
			if (sig_arr[i].type == 0
				&& strncmp(sig_arr[i].name, str_buff[word_loc], strlen(str_buff[word_loc])) == 0) {
				break;
			}
		}
		if (i >= sig_count) {
			memcpy(&sig_arr[sig_count].name, str_buff[word_loc], strlen(str_buff[word_loc]));
			sig_arr[sig_count].type = 0;
			sig_arr[sig_count].fun = 0;
			sig_count++;
		}
		else {
			cout << "变量存在" <<word_loc<< str_buff[word_loc] << word_loc << endl;
			exit(0);
		}
		word_loc++;
	}
	else {
		cout << "缺少int" << str_buff[word_loc] << word_loc << endl;
		exit(0);
	}
}

void SentenceBlock() {
	if (strncmp(str_buff[word_loc], "{", 1) == 0) {
		word_loc++;
		InternalStatement();
		SentenceString();
		if (strncmp(str_buff[word_loc], "}", 1) == 0) {
			word_loc++;
		}
		else {
			cout << "缺少}" << str_buff[word_loc] << word_loc << endl;
			exit(0);
		}
	}
	else {
		cout << "缺少{" << str_buff[word_loc] << word_loc << endl;
		exit(0);
	}
}

void InternalStatement() {
	if (strncmp(str_buff[word_loc], "int", 3) == 0) {
		InternalVariableStatement();
	}
	while (strncmp(str_buff[word_loc], ";", 1) == 0) {
		word_loc++;
		InternalVariableStatement();
	}
}

void InternalVariableStatement() {
	if (strncmp(str_buff[word_loc], "int", 3) == 0) {
		word_loc++;
		//更新参数列表
		int i;
		for (i = 0; i < sig_count; i++) {
			if (sig_arr[i].type == 0
				&& strncmp(sig_arr[i].name, str_buff[word_loc], strlen(str_buff[word_loc])) == 0) {
				break;
			}
		}
		if (i >= sig_count) {
			memcpy(&sig_arr[sig_count].name, str_buff[word_loc], strlen(str_buff[word_loc]));
			sig_arr[sig_count].type = 0;
			sig_arr[sig_count].fun = 0;
			sig_count++;
		}
		else {
			cout << "变量存在" << str_buff[word_loc] << word_loc << endl;
			exit(0);
		}
		word_loc++;
		if (strncmp(str_buff[word_loc], ";", 1) != 0) {
			cout << "缺少;" << str_buff[word_loc] << word_loc << endl;
			exit(0);
		}
	}
}

void SentenceString() {
	while (strncmp(str_buff[word_loc], "}", 1) != 0) {
		Sentence();
	}
}

void Sentence() {
	if (strncmp(str_buff[word_loc], "if", 2) == 0) {
		word_loc++;
		ifSentence();
	}
	else if (strncmp(str_buff[word_loc], "while", 5) == 0) {
		word_loc++;
		whileSentence();
	}
	else if (strncmp(str_buff[word_loc], "return", 6) == 0) {
		word_loc++;
		returnSentence();
	}
	else {
		assignmentSentence();
	}
}

void ifSentence() {
	if (strncmp(str_buff[word_loc], "(", 1) == 0) {
		word_loc++;
		Expression();

		if (strncmp(str_buff[word_loc], ")", 1) == 0) {
			word_loc++;

			//回填
			string s = to_string(code_4_num + 2);
			const char *p = s.data();
			memcpy(&code_4_arr[code_4_back_num].result, p, strlen(p));

			code_4_num++;
			code_4_arr[code_4_loc].op[0] = 'j';
			code_4_arr[code_4_loc].num = code_4_num;
			code_4_arr[code_4_loc].arg1[0] = '-';
			code_4_arr[code_4_loc].arg2[0] = '-';
			code_4_back_num = code_4_loc;//此位置需要回填
			code_4_loc++;

			SentenceBlock();
		}
		else {
			cout << "缺少)" << str_buff[word_loc] << word_loc << endl;
			exit(0);
		}
	}
	else {
		cout << "缺少(" << str_buff[word_loc] << word_loc << endl;
		exit(0);
	}
	if (strncmp(str_buff[word_loc], "else", 4) == 0) {
		word_loc++;

		//回填
		string s = to_string(code_4_num + 1);
		const char *p = s.data();
		memcpy(&code_4_arr[code_4_back_num].result, p, strlen(p));

		SentenceBlock();
	}
	else {
		//回填
		string s = to_string(code_4_num + 1);
		const char *p = s.data();
		memcpy(&code_4_arr[code_4_back_num].result, p, strlen(p));
	}
}

void whileSentence() {
	if (strncmp(str_buff[word_loc], "(", 1) == 0) {
		word_loc++;
		while_start = code_4_num;
		Expression();

		if (strncmp(str_buff[word_loc], ")", 1) == 0) {
			word_loc++;

			//回填
			string s = to_string(code_4_num + 2);
			const char *p = s.data();
			memcpy(&code_4_arr[code_4_back_num].result, p, strlen(p));

			code_4_num++;
			code_4_arr[code_4_loc].op[0] = 'j';
			code_4_arr[code_4_loc].num = code_4_num;
			code_4_arr[code_4_loc].arg1[0] = '-';
			code_4_arr[code_4_loc].arg2[0] = '-';
			code_4_back_num = code_4_loc;//此位置需要回填
			code_4_loc++;

			SentenceBlock();

			//返回while起始位置
			code_4_num++;
			code_4_arr[code_4_loc].op[0] = 'j';
			code_4_arr[code_4_loc].num = code_4_num;
			code_4_arr[code_4_loc].arg1[0] = '-';
			code_4_arr[code_4_loc].arg2[0] = '-';
			s = to_string(while_start + 1);
			p = s.data();
			memcpy(&code_4_arr[code_4_loc].result, p, strlen(p));
			code_4_loc++;
		}
		else {
			cout << "缺少)" << str_buff[word_loc] << word_loc << endl;
			exit(0);
		}
	}
	else {
		cout << "缺少(" << str_buff[word_loc] << word_loc << endl;
		exit(0);
	}
	//回填
	string s = to_string(code_4_num + 1);
	const char *p = s.data();
	memcpy(&code_4_arr[code_4_back_num].result, p, strlen(p));
}

void returnSentence() {
	if (strncmp(str_buff[word_loc], ";", 1) != 0) {
		Expression();
		if (strncmp(str_buff[word_loc], ";", 1) != 0) {
			cout << "缺少;" << endl;
			exit(0);
		}
		else {
			code_4_num++;
			string s1 = 'T' + to_string(code_4_v_num);
			code_4_v_num++;
			const char *p = s1.data();
			memcpy(&code_4_arr[code_4_loc].result, p, strlen(p));
			code_4_arr[code_4_loc].op[0] = '=';
			code_4_arr[code_4_loc].arg2[0] = '-';
			memcpy(&code_4_arr[code_4_loc].result, p, strlen(p));
			code_4_arr[code_4_loc].num = code_4_num;
			if (code_4_arr[code_4_loc - 1].result[0] == 'T'
				&& (code_4_arr[code_4_loc].arg1[0] > '9' || code_4_arr[code_4_loc].arg1[0] < '0')
				&& (code_4_arr[code_4_loc].arg1[0] > 'Z' || code_4_arr[code_4_loc].arg1[0] < 'A')
				&& (code_4_arr[code_4_loc].arg1[0] > 'z' || code_4_arr[code_4_loc].arg1[0] < 'a')) {
				memset(&code_4_arr[code_4_loc].arg1, 0, strlen(code_4_arr[code_4_loc].arg1));
				memcpy(&code_4_arr[code_4_loc].arg1, code_4_arr[code_4_loc - 1].result, strlen(code_4_arr[code_4_loc - 1].result));
			}
			code_4_loc++;
		}
	}
	word_loc++;//return语句后的分号
}

void assignmentSentence() {//赋值语句
	int i;
	for (i = 0; i < sig_count; i++) {
		if (sig_arr[i].type == 0
			&& strncmp(sig_arr[i].name, str_buff[word_loc], strlen(str_buff[word_loc])) == 0) {
			break;
		}
	}
	if (i >= sig_count) {
		cout << "未定义变量" << str_buff[word_loc] << word_loc << endl;
		exit(0);
	}
	else {
		char left_v[20] = { 0 };
		memcpy(left_v, &str_buff[word_loc], strlen(str_buff[word_loc]));
		word_loc++;
		if (strncmp(str_buff[word_loc], "=", 1) == 0) {
			word_loc++;
			Expression();

			code_4_num++;
			code_4_arr[code_4_loc].op[0] = '=';
			code_4_arr[code_4_loc].arg2[0] = '-';
			memcpy(&code_4_arr[code_4_loc].result, left_v, strlen(left_v));
			code_4_arr[code_4_loc].num = code_4_num;
			if (code_4_arr[code_4_loc - 1].result[0] == 'T'
				&& (code_4_arr[code_4_loc].arg1[0] > '9' || code_4_arr[code_4_loc].arg1[0] < '0')
				&& (code_4_arr[code_4_loc].arg1[0] > 'Z' || code_4_arr[code_4_loc].arg1[0] < 'A')
				&& (code_4_arr[code_4_loc].arg1[0] > 'z' || code_4_arr[code_4_loc].arg1[0] < 'a')) {
				memset(&code_4_arr[code_4_loc].arg1, 0, strlen(code_4_arr[code_4_loc].arg1));
				memcpy(&code_4_arr[code_4_loc].arg1, code_4_arr[code_4_loc - 1].result, strlen(code_4_arr[code_4_loc - 1].result));
			}
			code_4_loc++;

			if (strncmp(str_buff[word_loc], ";", 1) == 0) {
				word_loc++;
			}
			else {
				cout << "缺少;" << str_buff[word_loc] << word_loc << endl;
				exit(0);
			}
		}
		else {
			cout << "缺少=" << str_buff[word_loc] << word_loc << endl;
			exit(0);
		}
	}
}

void Expression() {
	addExpression();
	while (
		strncmp(str_buff[word_loc], "<", 1) == 0
		||strncmp(str_buff[word_loc], "<=", 2) == 0
		||strncmp(str_buff[word_loc], ">", 1) == 0
		||strncmp(str_buff[word_loc], ">=", 2) == 0
		||strncmp(str_buff[word_loc], "==", 2) == 0
		||strncmp(str_buff[word_loc], "!=", 2) == 0
		) {

		int s_num = word_loc;

		word_loc++;
		addExpression();

		code_4_num++;
		code_4_arr[code_4_loc].op[0] = 'j';
		memcpy(&code_4_arr[code_4_loc].op[1], str_buff[s_num], strlen(str_buff[s_num]));
		memset(&code_4_arr[code_4_loc].arg1, 0, strlen(code_4_arr[code_4_loc].arg1));
		memset(&code_4_arr[code_4_loc].arg2, 0, strlen(code_4_arr[code_4_loc].arg2));
		if (strncmp(str_buff[s_num - 2], "*", 1) != 0
			&& strncmp(str_buff[s_num - 2], "/", 1) != 0
			&& strncmp(str_buff[s_num - 2], "+", 1) != 0
			&& strncmp(str_buff[s_num - 2], "-", 1) != 0
			&& strncmp(str_buff[s_num - 1], ")", 1) != 0) {
			memcpy(&code_4_arr[code_4_loc].arg1, str_buff[s_num - 1], strlen(str_buff[s_num - 1]));
		}
		else {
			memcpy(&code_4_arr[code_4_loc].arg1, &code_4_arr[code_4_loc - 1].result, strlen(code_4_arr[code_4_loc - 1].result));
		}
		if (strncmp(str_buff[s_num + 2], "*", 1) != 0
			&& strncmp(str_buff[s_num + 2], "/", 1) != 0
			&& strncmp(str_buff[s_num + 2], "+", 1) != 0
			&& strncmp(str_buff[s_num + 2], "-", 1) != 0
			&& strncmp(str_buff[s_num + 1], "(", 1) != 0) {
			memcpy(&code_4_arr[code_4_loc].arg2, str_buff[s_num + 1], strlen(str_buff[s_num + 1]));
		}
		else {
			memcpy(&code_4_arr[code_4_loc].arg2, &code_4_arr[code_4_loc - 1].result, strlen(code_4_arr[code_4_loc - 1].result));
		}
		code_4_arr[code_4_loc].num = code_4_num;
		code_4_back_num = code_4_loc;//此位置需要回填
		code_4_loc++;
	}
}

void addExpression() {
	item();
	while (strncmp(str_buff[word_loc], "+", 1) == 0 || strncmp(str_buff[word_loc], "-", 1) == 0) {
		int s_num = word_loc;

		word_loc++;
		item();

		code_4_num++;
		memcpy(&code_4_arr[code_4_loc].op, str_buff[s_num], strlen(str_buff[s_num]));
		string s1 = 'T' + to_string(code_4_v_num);
		code_4_v_num++;
		const char *p = s1.data();
		memcpy(&code_4_arr[code_4_loc].result, p, strlen(p));
		if (strncmp(str_buff[s_num - 2], "*", 1) != 0
			&& strncmp(str_buff[s_num - 2], "/", 1) != 0
			&& strncmp(str_buff[s_num - 1], ")", 1) != 0) {
			memcpy(&code_4_arr[code_4_loc].arg1, str_buff[s_num - 1], strlen(str_buff[s_num - 1]));
		}
		else {
			memcpy(&code_4_arr[code_4_loc].arg1, &code_4_arr[code_4_loc - 1].result, strlen(code_4_arr[code_4_loc - 1].result));
		}
		if (strncmp(str_buff[s_num + 2], "*", 1) != 0
			&& strncmp(str_buff[s_num + 2], "/", 1) != 0
			&& strncmp(str_buff[s_num + 1], "(", 1) != 0) {
			memcpy(&code_4_arr[code_4_loc].arg2, str_buff[s_num + 1], strlen(str_buff[s_num + 1]));
		}
		else {
			memcpy(&code_4_arr[code_4_loc].arg2, &code_4_arr[code_4_loc - 1].result, strlen(code_4_arr[code_4_loc - 1].result));
		}
		code_4_arr[code_4_loc].num = code_4_num;
		code_4_loc++;
	}
}

void item() {
	factor();
	while (strncmp(str_buff[word_loc], "*", 1) == 0 || strncmp(str_buff[word_loc], "/", 1) == 0) {
		int s_num = word_loc;

		word_loc++;
		factor();

		code_4_num++;
		memcpy(&code_4_arr[code_4_loc].op, str_buff[s_num], strlen(str_buff[s_num]));
		string s1 = 'T' + to_string(code_4_v_num);
		code_4_v_num++;
		const char *p = s1.data();
		memcpy(&code_4_arr[code_4_loc].result, p, strlen(p));
		if (strncmp(str_buff[s_num - 1], ")", 1) != 0) {
			memcpy(&code_4_arr[code_4_loc].arg1, str_buff[s_num - 1], strlen(str_buff[s_num - 1]));
		}
		else {
			memcpy(&code_4_arr[code_4_loc].arg1, &code_4_arr[code_4_loc - 1].result, strlen(code_4_arr[code_4_loc - 1].result));
		}
		if (strncmp(str_buff[s_num + 1], "(", 1) != 0) {
			memcpy(&code_4_arr[code_4_loc].arg2, str_buff[s_num + 1], strlen(str_buff[s_num + 1]));
		}
		else {
			memcpy(&code_4_arr[code_4_loc].arg2, &code_4_arr[code_4_loc + 1].result, strlen(code_4_arr[code_4_loc - 1].result));
		}
		code_4_arr[code_4_loc].num = code_4_num;
		code_4_loc++;
	}
}

void factor() {
	if (strncmp(str_buff[word_loc], "(", 1) == 0) {
		word_loc++;

		Expression();
		if (strncmp(str_buff[word_loc], ")", 1) == 0) {
			word_loc++;
		}
		else {
			cout << "缺少)" << str_buff[word_loc] << word_loc << endl;
			exit(0);
		}
	}
	else if (str_buff[word_loc][0] >= '0'&&str_buff[word_loc][0] <= '9') {
		memcpy(&code_4_arr[code_4_loc].arg1, &str_buff[word_loc], strlen(str_buff[word_loc]));
		word_loc++;
	}
	else if ((str_buff[word_loc][0] >= 'a'&&str_buff[word_loc][0] <= 'z' || str_buff[word_loc][0] >= 'A'&&str_buff[word_loc][0] <= 'Z')
		&& (strncmp(str_buff[word_loc], "int", 3) != 0 || strncmp(str_buff[word_loc], "void", 4) != 0)) {
		int i;
		for (i = 0; i < sig_count; i++) {
			if (sig_arr[i].type == 0
				&& strncmp(sig_arr[i].name, str_buff[word_loc], strlen(str_buff[word_loc])) == 0) {
				break;
			}
		}
		if (i >= sig_count) {
			cout << "未定义变量" << str_buff[word_loc] << word_loc << endl;
			exit(0);
		}
		else {
			if (sig_arr[i].fun == 0) {
				memcpy(&code_4_arr[code_4_loc].arg1, &str_buff[word_loc], strlen(str_buff[word_loc]));
				word_loc++;//变量
			}
			else {
				word_loc++;
				memcpy(&code_4_arr[code_4_loc].arg1, &str_buff[word_loc], strlen(str_buff[word_loc]));
				FTYPE();//函数
			}
		}
	}
	else {
		cout << str_buff[word_loc] << word_loc << str_buff[word_loc] << word_loc << endl;
		cout << "多余int或者void" << str_buff[word_loc] << word_loc << endl;
		exit(0);
	}
}

void FTYPE() {
	//匹配符号表
	if (strncmp(str_buff[word_loc], "(", 1) == 0) {
		call();
	}
}

void call() {
	if (strncmp(str_buff[word_loc], "(", 1) == 0) {
		word_loc++;
		ActualParameter();
		if (strncmp(str_buff[word_loc], ")", 1) == 0) {
			word_loc++;
		}
		else {
			cout << "缺少)" << str_buff[word_loc] << word_loc << endl;
			exit(0);
		}
	}
}

void ActualParameter() {
	if (strncmp(str_buff[word_loc], ")", 1) != 0) {
		ActualParameterList();
	}
}

void ActualParameterList() {
	if (strncmp(str_buff[word_loc], ")", 1) != 0) {
		Expression();
	}
	while (strncmp(str_buff[word_loc], ",", 1) == 0) {
		word_loc++;
		Expression();
	}
}