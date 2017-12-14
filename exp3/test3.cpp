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

int syn;  //��ŵ��ʵ�����
int p;
int count;

char ch;
int sum;  //�����������ֵ�ֵ
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
        cout << "<��������>��<��ʶ��>=<�޷�������>" << token << endl;
        Scanner();
        if (syn == 16) {
            cout << "<��ֵ���>��<��ʶ��>=<���ʽ>" << token << endl;
            Scanner();
            if (syn == 11) {
                cout << "<�޷�������>��<����>{<����>}" << sum << endl;
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
        cout << " <����˵��>��Var <��ʶ��>{��<��ʶ��>}" << token << endl;
        Scanner();
        while (Constan_Defined()) {
            Scanner();
            if (syn == 23) {
                cout << "�ֺ�" << token << endl;
                return true;
            } else if (syn == 26) {
                cout << "����" << token << endl;
                Scanner();
                continue;
            }
            cout << "����˵������" << endl;
        }
    }
    return false;
}

bool Variable_Defined() {
    if (syn == 10) {
        cout << "��������" << token << endl;
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
        cout << "<����˵��>��Var <��ʶ��>{��<��ʶ��>}" << token << endl;
        Scanner();
        while (Variable_Defined()) {
            Scanner();
            if (syn == 23) {
                cout << "�ֺ�" << token << endl;
                return true;
            } else if (syn == 26) {
                cout << "����" << token << endl;
                Scanner();
                continue;
            }
        }
    }
    return false;
}

bool Condition() {
    printf("����\n");
    printf("����Expression����\n\n");
    Expression();
    if (syn == 17 || syn == 18 || syn == 19 || syn == 20 || syn == 21 ||
        syn == 22) {
          printf("%s\n",token);
          printf("��ϵ�����\n\n");
        Scanner();
    } else {
        printf("��ϵ���������\n\n");
        return false;
    }
    printf("����Expression����\n\n");
    Expression();
    return true;
}

bool Expression() {
    printf("<���ʽ>��[+|-]<��>{<�ӷ������><��>}\n");
    do {
        if (syn == 12 || syn == 13) {
          printf("%s\n",token);
          printf("<�ӷ������>��+|-\n");
            Scanner();
            printf("����term����\n\n");
            term();
        } else {
          printf("����term����\n\n");
            term();
        }
    } while (syn == 12 || syn == 13);
    return true;
}

bool term() {
    printf("<��>��<����>{<�˷������><����>}\n");
    while (Factor()) {
        if (syn == 14 || syn == 15) {
          printf("%s\n",token);
          printf("<�˷������>��* |/\n");
            Scanner();

        } else {
            return true;
        }
    }
    return false;
}

bool Factor() {
    printf("����factor\n");
    // Scanner();
    if (syn == 10) {
      printf("��ʶ��\n");
      printf("%s\n",token );
        Scanner();  //����
        return true;
    } else if (syn == 11) {
      printf("�޷�������\n");
      printf("%d\n",sum );
        Scanner();
        return true;
    } else if (syn == 24) {
      printf("���������ţ����ʽ��������\n");
      printf("%s\n",token );
        Scanner();
        Expression();
        if (syn == 25) {
          printf("������\n");
          printf("%s\n",token );
            Scanner();
            return true;
        } else {
            // cout << "����û��)" << endl;
            return false;
        }
    } else {
        printf("\')\' ERROR!\n");
        return false;
    }
    return false;
}

bool Assign() {
    printf("<��ֵ���>��<��ʶ��>=<���ʽ>\n\n");
    printf("��ʶ��%s\n",token);
    Scanner();
    if (syn == 16) {
        printf("��ֵ���=\n\n");
        Scanner();
        printf("����Expression����\n\n");
        Expression();
        return true;
    } else {
      printf("ȱ��=�Ų��Ǹ�ֵ���\n\n");
      return false;
    }
}

bool yucu() {
  printf("�������\n");
  printf("%s\n",token);
    Scanner();
    while (Statement()) {
        if (syn == 23) {
          printf("��������еķָ��\n");
          printf("%s\n",token);
            Scanner();
            if (syn == 9) {
                break;
            }
        }
    }
    if (syn == 9) {
      printf("<�������>��begin <���>{;<���>} end\n");
      printf("%s\n",token);
        Scanner();
        return true;
    } else {
      printf("��end\n\n");
        return false;
    }
    if (syn == 0) {
      printf("�������\n");
      printf("%s\n",token);
    }
}

bool Conditional_s() {
    if (syn == 2) {
        printf("if\n");
        printf("����ʽ��<�������>��if <����> then <���><E>\n");
        Scanner();
        printf("����Condition����\n\n");
        Condition();
        if (syn == 3) {
            printf("then\n");
            Scanner();
            printf("����Statement����\n\n");
            Statement();
            if (syn == 4) {
              printf("����ʽ��<E>��else<���>\n");
                Scanner();
                printf("����Statement����\n\n");
                Statement();
            } else {
                return true;
            }
        } else {
            printf("ȱ�� then\n\n");
            return false;
        }
    }
    return false;
}

bool DoWhile() {
  printf("<��ѭ�����>��while <����> do <���>\n");
  printf("%s\n",token);
    // cout << "<��ѭ�����>��while <����> do <���>" << token << endl;
    Scanner();
    printf("����Condition����\n\n");
    Condition();
    if (syn == 6) {
      printf("ѭ��do\n");
        Scanner();
        printf("����Statement����\n\n");
        Statement();
        return true;
    } else{
      return false;
    }
}

bool Statement() {
    if (syn == 10) {
      printf("<��ֵ���>\n" );
        Assign();
        return true;
    } else if (syn == 5) {
      printf("<ѭ�����>\n" );
        DoWhile();
        return true;
    } else if (syn == 1) {
      printf("<�������>\n" );
        yucu();
        return true;
    } else if (syn == 2) {
      printf("<�������>\n" );
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
      printf("�ļ��򿪴���\n");
      exit(1);
  }

  do {
        ch=fgetc(fp);
        program[p++] = ch;
  } while (ch != '#');
  program[p]='\0';

    printf("Դ����:\n%s\n\n�﷨������ʼ��\n",program);

    p = 0;
    printf("<����>��[<����˵��>][<����˵��>]<���>");

    Constdefine();
    Vardefine();
    do {
        Scanner();
        Statement();
    } while (syn != 0);
    cout << "������ɣ��ر��ļ�" << endl;
    fclose(fp);
    return 0;
}

