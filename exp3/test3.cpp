#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

void Scanner();
bool Statement();
bool Condition();
bool DoWhile();
bool Expression();
bool term();
bool Factor();
bool Conditional_s();
bool Assign();
bool yucu();

int syn;  //存放单词的类型
int p;
int count;

char ch;
int sum;  //用来保存数字的值
char program[1000], token[10];
const char *rwtab[9] = {"begin", "if",    "then", "else", "while",
                        "do",    "Const", "Var",  "end"};
int m;

bool isDigital(char ch) {
    if (ch <= '9' && ch >= '0')
        return true;
    else
        return false;
}

bool isAlpha(char ch) {
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        return true;
    else
        return false;
}

void Scanner() {
    int num = 0;
    for (m = 0; m < 10; m++) {
        token[m] = '\0';
    }
    m = 0;
    ch = program[p++];

    while (ch == ' ' || ch == '\n' || ch == '\t') {
        ch = program[p++];
    }
    if (isAlpha(ch)) {
        do {
            token[m++] = ch;
            ch = program[p++];
        } while (isAlpha(ch) || isDigital(ch));
        p--;
        syn = 10;
        token[m++] = '\0';
        for (int n = 0; n <= 8; n++) {
            if (strcmp(token, rwtab[n]) == 0) {
                syn = n + 1;
                break;
            }
        }
        return;
    } else if (isDigital(ch)) {
        sum = 0;
        while (isDigital(ch)) {
            sum = sum * 10 + ch - '0';
            ch = program[p++];
        }
        p--;
        syn = 11;
        if (isAlpha(ch)) syn = -1;
        return;
    } else {
        token[0] = ch;
        switch (ch) {
            case '<':
                ch = program[p++];
                if (ch == '>') {
                    syn = 22;
                    token[1] = ch;
                } else if (ch == '=') {
                    syn = 18;
                    token[1] = ch;
                } else {
                    syn = 19;
                    p--;
                }
                break;
            case '>':
                ch = program[p++];
                if (ch == '=') {
                    syn = 21;
                    token[1] = ch;
                } else {
                    syn = 20;
                    p--;
                }
                break;
            case '=':
                ch = program[p++];
                if (ch == '=') {
                    syn = 17;
                    token[1] = ch;
                } else {
                    syn = 16;
                    p--;
                }
                break;
            case '+':
                syn = 12;
                break;
            case '-':
                syn = 13;
                break;
            case '*':
                syn = 14;
                break;
            case '/':
                syn = 15;
                break;
            case ';':
                syn = 23;
                break;
            case '(':
                syn = 24;
                break;
            case ')':
                syn = 25;
                break;
            case ',':
                syn = 26;
                break;
            case '#':
                syn = 0;
                break;
            default:
                syn = -1;
                break;
        }
        return;
    }
}

bool Constan_Defined() {
    if (syn == 10) {
        cout << "<常量定义>→<标识符>=<无符号整数>" << token << endl;
        Scanner();
        if (syn == 16) {
            cout << "<赋值语句>→<标识符>=<表达式>" << token << endl;
            Scanner();
            if (syn == 11) {
                cout << "<无符号整数>→<数字>{<数字>}" << sum << endl;
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}

bool Constdefine() {
    Scanner();
    if (syn == 7) {
        cout << " <变量说明>→Var <标识符>{，<标识符>}" << token << endl;
        Scanner();
        while (Constan_Defined()) {
            Scanner();
            if (syn == 23) {
                cout << "分号" << token << endl;
                return true;
            } else if (syn == 26) {
                cout << "逗号" << token << endl;
                Scanner();
                continue;
            }
            cout << "常量说明错误" << endl;
        }
    }
    return false;
}

bool Variable_Defined() {
    if (syn == 10) {
        cout << "变量定义" << token << endl;
        return true;
    } else
        return false;
}

bool Vardefine() {
    if (syn == 10 || syn == 2 || syn == 5 || syn == 1 || syn == 0) {
        return true;
    }
    Scanner();
    if (syn == 8) {
        cout << "<变量说明>→Var <标识符>{，<标识符>}" << token << endl;
        Scanner();
        while (Variable_Defined()) {
            Scanner();
            if (syn == 23) {
                cout << "分号" << token << endl;
                return true;
            } else if (syn == 26) {
                cout << "逗号" << token << endl;
                Scanner();
                continue;
            }
        }
    }
    return false;
}

bool Condition() {
    printf("条件\n");
    printf("调用Expression函数\n\n");
    Expression();
    if (syn == 17 || syn == 18 || syn == 19 || syn == 20 || syn == 21 ||
        syn == 22) {
          printf("%s\n",token);
          printf("关系运算符\n\n");
        Scanner();
    } else {
        printf("关系运算符错误\n\n");
        return false;
    }
    printf("调用Expression函数\n\n");
    Expression();
    return true;
}

bool Expression() {
    printf("<表达式>→[+|-]<项>{<加法运算符><项>}\n");
    do {
        if (syn == 12 || syn == 13) {
          printf("%s\n",token);
          printf("<加法运算符>→+|-\n");
            Scanner();
            printf("调用term函数\n\n");
            term();
        } else {
          printf("调用term函数\n\n");
            term();
        }
    } while (syn == 12 || syn == 13);
    return true;
}

bool term() {
    printf("<项>→<因子>{<乘法运算符><因子>}\n");
    while (Factor()) {
        if (syn == 14 || syn == 15) {
          printf("%s\n",token);
          printf("<乘法运算符>→* |/\n");
            Scanner();

        } else {
            return true;
        }
    }
    return false;
}

bool Factor() {
    printf("因数factor\n");
    // Scanner();
    if (syn == 10) {
      printf("标识符\n");
      printf("%s\n",token );
        Scanner();  //特殊
        return true;
    } else if (syn == 11) {
      printf("无符号数字\n");
      printf("%d\n",sum );
        Scanner();
        return true;
    } else if (syn == 24) {
      printf("遇到左括号，表达式分析函数\n");
      printf("%s\n",token );
        Scanner();
        Expression();
        if (syn == 25) {
          printf("右括号\n");
          printf("%s\n",token );
            Scanner();
            return true;
        } else {
            // cout << "错误：没有)" << endl;
            return false;
        }
    } else {
        printf("\')\' ERROR!\n");
        return false;
    }
    return false;
}

bool Assign() {
    printf("<赋值语句>→<标识符>=<表达式>\n\n");
    printf("标识符%s\n",token);
    Scanner();
    if (syn == 16) {
        printf("赋值语句=\n\n");
        Scanner();
        printf("调用Expression函数\n\n");
        Expression();
        return true;
    } else {
      printf("缺少=号不是赋值语句\n\n");
      return false;
    }
}

bool yucu() {
  printf("复合语句\n");
  printf("%s\n",token);
    Scanner();
    while (Statement()) {
        if (syn == 23) {
          printf("复合语句中的分割符\n");
          printf("%s\n",token);
            Scanner();
            if (syn == 9) {
                break;
            }
        }
    }
    if (syn == 9) {
      printf("<复合语句>→begin <语句>{;<语句>} end\n");
      printf("%s\n",token);
        Scanner();
        return true;
    } else {
      printf("少end\n\n");
        return false;
    }
    if (syn == 0) {
      printf("复合语句\n");
      printf("%s\n",token);
    }
}

bool Conditional_s() {
    if (syn == 2) {
        printf("if\n");
        printf("产生式：<条件语句>→if <条件> then <语句><E>\n");
        Scanner();
        printf("调用Condition函数\n\n");
        Condition();
        if (syn == 3) {
            printf("then\n");
            Scanner();
            printf("调用Statement函数\n\n");
            Statement();
            if (syn == 4) {
              printf("产生式：<E>→else<语句>\n");
                Scanner();
                printf("调用Statement函数\n\n");
                Statement();
            } else {
                return true;
            }
        } else {
            printf("缺少 then\n\n");
            return false;
        }
    }
    return false;
}

bool DoWhile() {
  printf("<当循环语句>→while <条件> do <语句>\n");
  printf("%s\n",token);
    // cout << "<当循环语句>→while <条件> do <语句>" << token << endl;
    Scanner();
    printf("调用Condition函数\n\n");
    Condition();
    if (syn == 6) {
      printf("循环do\n");
        Scanner();
        printf("调用Statement函数\n\n");
        Statement();
        return true;
    } else{
      return false;
    }
}

bool Statement() {
    if (syn == 10) {
      printf("<赋值语句>\n" );
        Assign();
        return true;
    } else if (syn == 5) {
      printf("<循环语句>\n" );
        DoWhile();
        return true;
    } else if (syn == 1) {
      printf("<复合语句>\n" );
        yucu();
        return true;
    } else if (syn == 2) {
      printf("<条件语句>\n" );
        Conditional_s();
        return true;
    } else {
        return false;
    }
}

int main() {
  FILE *fp;
  p=0;

  if( (fp=fopen("test3.txt","r"))==NULL )
  {
      printf("文件打开错误！\n");
      exit(1);
  }

  do {
        ch=fgetc(fp);
        program[p++] = ch;
  } while (ch != '#');
  program[p]='\0';

    printf("源程序:\n%s\n\n语法分析开始：\n",program);

    p = 0;
    printf("<程序>→[<常量说明>][<变量说明>]<语句>");

    Constdefine();
    Vardefine();
    do {
        Scanner();
        Statement();
    } while (syn != 0);
    cout << "分析完成，关闭文件" << endl;
    fclose(fp);
    return 0;
}

