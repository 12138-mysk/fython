#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <cstdio>

using namespace std;

enum DfaState{
	Initail, 	// 初始状态
	Id,			// Id状态
	Id_int1,	// int 状态1
	Id_int2,	// int 状态2
	Id_int3,	// int 状态3
	Int, 		// int 状态
	Identifier,	// 标识符状态
	IntLiteral,	// 数字字面量状态
	GT,			// > 状态
	GE,			// >= 状态
	Eq,			// = 状态
	Oper,		// '+' '-' '*' '/' 运算符
};

struct Token {
	DfaState type;
	string text;
};

struct ListNode {
	Token token;
	ListNode *next;
};

DfaState initToken(Token &, char);
ListNode *addToken(ListNode *, Token &);

void output(ListNode *head) {
	ListNode *data = head->next;
	while (data) {
		cout << setw(11);

		switch (data->token.type) {
			case Identifier :
				cout << "Identifier ";
				break;
			case IntLiteral:
				cout << "IntLiteral "; 
				break;
			case GT :
				cout << "GT "; 
				break;
			case GE : 
				cout << "GE ";
				break;
			case Int :
				cout << "Int";
				break;
			case Eq :
				cout << "Eq";
				break;
			case Oper :
				cout << "Oper";
				break;
			default :
				cout << "未定义" << endl;
				break;
		}

		cout << setw(11) << data->token.text << endl;
		data = data->next;
	}
	return ;
}

int main(int argc, char const *argv[]) {
	DfaState state = Initail;
	FILE *fp = fopen("1.txt", "r");
	// fstream file;
	// file.open("1.txt", ios::in);
	
	Token token;
	char ch;
	ListNode *head = new ListNode;
	head->next = nullptr;
	ListNode *temp = head;

	while (~fscanf(fp, "%c", &ch)) {
		switch (state) {
			case Initail :
				state = initToken(token, ch);
				break;
			case Id :
				if (isalpha(ch) || isdigit(ch)) {
					token.text += ch;
				} else {
					// goto add;
					// goto init;
					temp = addToken(temp, token);
					state = initToken(token, ch);
				}
				break;
			case IntLiteral :
				if (isdigit(ch)) {
					token.text += ch;
				} else {
					// goto add;
					// goto init;
					temp = addToken(temp, token);
					state = initToken(token, ch);
				}
				break;
			case Id_int1 : 
				if (ch == 'n') {
					state = Id_int2;
					token.text += ch;
				} else if (isalpha(ch) || isdigit(ch)) {
					state = Id;			// 切换回Id状态
					token.text += ch;
				} else {
					// goto init;
					state = initToken(token, ch);
				}
				break;
			case Id_int2 :
				if (ch == 't') {
					state = Id_int3;
					token.text += ch;
				} else if (isalpha(ch) || isdigit(ch)) {
					state = Id;			// 切换回Id状态
					token.text += ch;
				} else {
					// goto init;
					state = initToken(token, ch);
				}
				break;
			case Id_int3 :
				if (ch == ' ') {
					token.type = Int;
					// goto add;
					temp = addToken(temp, token);
				} else {
					state = Id;
				}
				// goto init;
				state = initToken(token, ch);
				break;
			case GT : 
				if (ch == '=') {
					token.type = GE;
					state = GE;
					token.text += ch;
				} else {
					temp = addToken(temp, token);
					state = initToken(token, ch);
				}
				break;
			case GE :
				temp = addToken(temp, token);
				state = initToken(token, ch);
				break;
			case Eq :
				temp = addToken(temp, token);
				state = initToken(token, ch);
				break;
			case Oper :
				temp = addToken(temp, token);
				state = initToken(token, ch);
				break;
			default :
				cout << "未定义标识符" << endl;
				break;
		}
	}
	if (state != Initail) temp = addToken(temp, token);
	output(head);

	// file.close();
	fclose(fp);
	return 0;
}

ListNode *addToken(ListNode *temp, Token &token) {
	ListNode *node = new ListNode;
	node->token.type = token.type;
	node->token.text = token.text;
	node->next = nullptr;

	temp->next = node;
	temp = node;
	token.text.clear();
	return temp;
}

DfaState initToken(Token &token, char ch) {
	DfaState newState = Initail;

	if (isalpha(ch)) {	
		if (ch == 'i') 
			newState = Id_int1;
		else 
			newState = Id;

		token.type = Identifier;
	} else if (isdigit(ch)) {
		newState = IntLiteral;
		token.type = IntLiteral;
	} else if (ch == '>'){
		newState = GT;
		token.type = GT;
	} else if (ch == '=') {
		newState = Eq;
		token.type = Eq;
	} else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
		newState = Oper;
		token.type = Oper;
	} else {
		// cout << "未定义标识符" << endl;
		return newState;
	}
	token.text += ch;
	return newState;
}