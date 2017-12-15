#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

// void Scanner();
// bool Statement();
// bool Condition();
// bool DoWhile();
// bool Expression();
// bool term();
// bool Factor();
// bool Conditional_s();
// bool Assign();
// bool yucu();

char * term();
char * Expression();
char * Factor();
bool Statement();

int syn;  //��ŵ��ʵ�����
int p;   //���ڱ�������Դ����

char ch;  //���ڶ�ȡ����Դ����
int sum;  //�����������ֵ�ֵ
char program[1000], token[10];
//�ؼ���
const char *rwtab[9] = {"begin", "if",    "then", "else", "while",
                        "do",    "Const", "Var",  "end"};
int m;

char* lexicalType[27];//�ʷ�����
struct symbol{
	char name[10];
	int value;
};//���ű���
symbol symbolTable[100];//���ű�

int q;//�������������ű��������Ŀ

char id[10];//���ڴ洢id

char temp[100][10]={"t1","t2","t3","t4","t5"};//�м����

int w;//�������������м����

bool lookup(char *id)
{
	int i;
	for(i=0;symbolTable[i].name[0]!='\0';i++){
		if(strcmp(id,symbolTable[i].name)==0)
		    return true;
	}
	return false;
}
// 1~9��Ӧ�ؼ��֣�10��Ӧ��ʶ����11��Ӧ�޷���������
// 12~22��Ӧ����� ��23~26��Ӧ�ֽ��
void initwordType()
{
	int i;
	for(i=1;i<=9;i++)
	    lexicalType[i]="ReservedWord";
    lexicalType[10]="Identifier";
    lexicalType[11]="Number";
	for(i=12;i<=22;i++)
	    lexicalType[i]="Operator";
	for(i=23;i<=26;i++)
	    lexicalType[i]="Delimiter";
}

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

//������ʷ��Ŷ�Ԫʽ<�����ʾ�����ʵ�����ֵ>
void lexicalPrint()
{
    if(syn==11)//�޷���������������ֵΪ��value
        printf("  %s:<%d,%d>\n",lexicalType[syn],syn,sum);
    else//������������ֵΪ�䵥������
        printf("  %s:<%d,%s>\n",lexicalType[syn],syn,token);
}

bool Const_Define()
{
    //ƥ���ʶ��
    if (syn == 10){
      // cout << "<��������>��<��ʶ��>=<�޷�������>" << token << endl;
        if (!lookup(token)) {
            strcpy(symbolTable[q].name, token);
            Scanner();
            //ƥ��Ⱥ�
            if (syn == 16){
              // cout << "<��ֵ���>��<��ʶ��>=<���ʽ>" << token << endl;
                Scanner();
                //ƥ���޷�������
                if (syn == 11){
                  // cout << "<�޷�������>��<����>{<����>}" << sum << endl;
                    symbolTable[q].value = sum;
                    q++;
                    return true;
                } else {
                    printf("ERROR: no Number in Const_Define\n");
                    return false;
                }
            } else {
                printf("ERROR: no = in Const_Define\n");
                return false;
            }
        } else {
            printf("ERROR: Duplicate Definition in Const_Define\n");
            return false;
        }
    } else
        return false;
}

// bool Constdes() {
//     Scanner();
//     if (syn == 7) {
//         cout << " <����˵��>��Var <��ʶ��>{��<��ʶ��>}" << token << endl;
//         Scanner();
//         while (Const_Define()) {
//             Scanner();
//             if (syn == 23) {
//                 cout << "�ֺ�" << token << endl;
//                 return true;
//             } else if (syn == 26) {
//                 cout << "����" << token << endl;
//                 Scanner();
//                 continue;
//             }
//             cout << "����˵������" << endl;
//         }
//     }
//     return false;
// }
bool Constdes()  //����˵��
{
    if (syn == 7)  //ƥ��Const�ؼ���
    {
      //printf("����ʽ��<G>��<����˵��>\n");
    	//printf("����ʽ��<����˵��>��Const <��������><A>��\n");
        Scanner();
        if (Const_Define())  //ƥ��<��������>
        {
            Scanner();
            while (syn == 26)  //ƥ�䶺��
            {
              //printf("����ʽ��<A>����<��������><A> \n");
                Scanner();
                if (Const_Define())  //ƥ��<��������>
                {
                    Scanner();
                    continue;
                } else {
                  printf("ERROR: lose Const_Define in Constdes\n");
                    return false;
                }
            }
            if (syn == 23)  //ƥ��ֺ�
            {
                return true;
            } else {
              printf("ERROR: lose ; in Constdes\n");
                return false;
            }
        } else {
            printf("ERROR: lose Const_Define in Constdes\n");
            return false;
        }
    } else {
        return false;
    }
}

// bool Vardefine() {
//     if (syn == 10 || syn == 2 || syn == 5 || syn == 1 || syn == 0) {
//         return true;
//     }
//     Scanner();
//     if (syn == 8) {
//         cout << "<����˵��>��Var <��ʶ��>{��<��ʶ��>}" << token << endl;
//         Scanner();
//         while (Variable_Defined()) {
//             Scanner();
//             if (syn == 23) {
//                 cout << "�ֺ�" << token << endl;
//                 return true;
//             } else if (syn == 26) {
//                 cout << "����" << token << endl;
//                 Scanner();
//                 continue;
//             }
//         }
//     }
//     return false;
// }
bool Vardes()//ָ�벻�������һ��
{
	if(syn==8)//ƥ��Var�ؼ���
    {
    	//printf("����ʽ��<H>��<����˵��>\n");
    	//printf("����ʽ��<����˵��>��Var <��ʶ��><B>��\n");
        Scanner();
        //ƥ���ʶ��
        if(syn==10){
        	if(!lookup(token)){
	        	strcpy(symbolTable[q].name,token);
	        	printf("  add Identifier to symbolTable name %s\n",symbolTable[q].name);
	        	q++;
	            Scanner();
	            while(syn==26)//ƥ�䶺��
	            {
					        //printf("����ʽ��<B>����<��ʶ��><B> \n");
	            	  Scanner();
                  //ƥ���ʶ��
				          if(syn==10){
				    	      strcpy(symbolTable[q].name,token);
	        	        printf("  add Identifier to symbolTable name %s\n",symbolTable[q].name);
	        	        q++;
					          Scanner();
					          continue;
                 }else{
                    printf("ERROR: no Identifier in Vardes\n");
	    				      return false;
	    			     }
	            }
	            //printf("����ʽ��<B>����\n");
              //ƥ��ֺ�
	            if(syn==23){
					       return true;
				      }else{
                printf("ERROR: no ; in Vardes\n");
				   	     return false;
	            }
	        }else{
            printf("ERROR: Duplicate Definition in Vardes::%s\n",token);
				    return false;
	        }
        }else{
          printf("ERROR: Duplicate Definition in Vardes::%s\n",token);
			       return false;
        }
    }else{
    	//printf("����ʽ��<H>����\n");
    	return false;
    }
}

bool Condition()//ָ��������һ��
{
	printf("����ʽ��<����>��<���ʽ><��ϵ�����><���ʽ>\n");
	if(Expression())//ƥ��<���ʽ>
	{
    //ƥ��<��ϵ�����>
		if(syn==17||syn==18||syn==19||syn==20||syn==21||syn==22){
			Scanner();
      //ƥ��<���ʽ>
			if(Expression()){
				return true;
			}else{
        printf("ERROR: lose Expression in Condition\n");
				return false;
      }
		}else{
			  printf("ERROR: lose ��ϵ����� in Condition\n");
        return false;
		}
	}else
	    return false;
}

// bool Expression() {
//     printf("<���ʽ>��[+|-]<��>{<�ӷ������><��>}\n");
//     do {
//         if (syn == 12 || syn == 13) {
//           printf("%s\n",token);
//           printf("<�ӷ������>��+|-\n");
//             Scanner();
//             printf("����term����\n\n");
//             term();
//         } else {
//           printf("����term����\n\n");
//             term();
//         }
//     } while (syn == 12 || syn == 13);
//     return true;
// }
char * Expression()
{
  // printf("<���ʽ>��[+|-]<��>{<�ӷ������><��>}\n");
	char lastid[10],op[10];
  int i;
	char str[50]="����ʽ��<���ʽ>��";
  //ƥ��+
	if(syn==12){
		for(i=0;str[i]!='\0';i++);
	  str[i]='+';
		Scanner();
	}else if(syn==13)//ƥ��-
	{
		for(i=0;str[i]!='\0';i++);
	    str[i]='-';
		Scanner();
	}
  //ƥ��select��
	if(syn==10||syn==11||syn==24){
		//printf("%s<��><C>\n",str);
		term();//ƥ��<��>
		strcpy(lastid,id);
		while(syn==12||syn==13)//ƥ��<�ӷ������>
    {
        strcpy(op,token);
		    //printf("����ʽ��<C>��<�ӷ������><��><C>\n");
			  Scanner();
        //ƥ��<��>
			  if(term()){
				      printf("  %s:=%s%s%s\n",temp[w],lastid,op,id);
				      strcpy(lastid,temp[w]);
				      w++;
              continue;
			  }else{
				      printf("����ȱ����\n");
              return NULL;
		    }
     }
    //printf("����ʽ��<C>����\n");
    strcpy(id,lastid);
		return id;
  }else{
      return NULL;
  }
}

// bool term() {
//     printf("<��>��<����>{<�˷������><����>}\n");
//     while (Factor()) {
//         if (syn == 14 || syn == 15) {
//           printf("%s\n",token);
//           printf("<�˷������>��* |/\n");
//             Scanner();
//
//         } else {
//             return true;
//         }
//     }
//     return false;
// }

//ָ��������һ�� select={syn=10,11,24}
char *term()
{
  // printf("<��>��<����>{<�˷������><����>}\n");
	char lastid[10],op[10];
	if(syn==10||syn==11||syn==24)//ƥ��select
	{
		//printf("����ʽ��<��>��<����><D>\n");
		Factor();
		strcpy(lastid,id);//����id��ʶ��
		Scanner();
		while(syn==14||syn==15)//ƥ����<�˷������>
		{
			strcpy(op,token);
			//printf("����ʽ��<D>��<�˷������><����><D>\n");
			Scanner();
			if(Factor())//ƥ��<����>
			{
				printf("  %s:=%s%s%s\n",temp[w],lastid,op,id);
				strcpy(lastid,temp[w]);
				w++;
				Scanner();
				continue;
			}
			else
			{
				printf("ERROR: lose Factor in term\n");
				return NULL;
			}
		}
		//printf("����ʽ��<D>����\n");
		strcpy(id,lastid);
		return id;
	}
	else
	    return NULL;
}

// bool Factor() {
//     printf("����factor\n");
//     // Scanner();
//     if (syn == 10) {
//       printf("��ʶ��\n");
//       printf("%s\n",token );
//         Scanner();  //����
//         return true;
//     } else if (syn == 11) {
//       printf("�޷�������\n");
//       printf("%d\n",sum );
//         Scanner();
//         return true;
//     } else if (syn == 24) {
//       printf("���������ţ����ʽ��������\n");
//       printf("%s\n",token );
//         Scanner();
//         Expression();
//         if (syn == 25) {
//           printf("������\n");
//           printf("%s\n",token );
//             Scanner();
//             return true;
//         } else {
//             // cout << "����û��)" << endl;
//             return false;
//         }
//     } else {
//         printf("\')\' ERROR!\n");
//         return false;
//     }
//     return false;
// }

char * Factor()//ָ�벻�������һ�� select={syn=10,11,24}
{
	strcpy(id,token);
    //ƥ���ʶ��
    if(syn==10){
        return id;
    }else if(syn==11)//ƥ���޷�������
    {
        return id;
    }else if(syn==24)//ƥ��������
    {
        Scanner();
        //ƥ��<���ʽ>
        if(Expression()){
          //ƥ��������
        	if(syn==25){
	        	return id;
	        }else
	        {
        		printf("ERROR: lose ) in Factor\n");
				    return NULL;
        	}
        }else{
          printf("ERROR: lose Expression in Factor\n");
        	return NULL;
        }
    }else
		return NULL;
}

// bool Assign() {
//     printf("<��ֵ���>��<��ʶ��>=<���ʽ>\n\n");
//     printf("��ʶ��%s\n",token);
//     Scanner();
//     if (syn == 16) {
//         printf("��ֵ���=\n\n");
//         Scanner();
//         printf("����Expression����\n\n");
//         Expression();
//         return true;
//     } else {
//       printf("ȱ��=�Ų��Ǹ�ֵ���\n\n");
//       return false;
//     }
// }
bool Assign()
{
  // printf("<��ֵ���>��<��ʶ��>=<���ʽ>\n\n");
	char lastid[10];
    //ƥ���ʶ��
    if(syn==10){
		    if(lookup(token)){
		        strcpy(lastid,token);
			      //printf("����ʽ��<��ֵ���>��<��ʶ��>��<���ʽ>;\n");
			      Scanner();
            //ƥ��Ⱥ�
			      if(syn==16){
				      Scanner();
              //ƥ����ʽ
				      if(Expression()){
					           printf("  %s:=%s\n",lastid,id);
					           return true;
				      }else{
					           printf("ERROR: lose Expression in Assign\n");
					           return false;
				      }
			      }else{
                printf("ERROR: lose = in Assign\n");
				        return false;
			     }
		    }else{
			printf("ERROR��no var define in Assign%s\n",token);
			return false;
		}
	}else{
    return false;
  }
}

// bool yucu() {
//   printf("�������\n");
//   printf("%s\n",token);
//     Scanner();
//     while (Statement()) {
//         if (syn == 23) {
//           printf("��������еķָ��\n");
//           printf("%s\n",token);
//             Scanner();
//             if (syn == 9) {
//                 break;
//             }
//         }
//     }
//     if (syn == 9) {
//       printf("<�������>��begin <���>{;<���>} end\n");
//       printf("%s\n",token);
//         Scanner();
//         return true;
//     } else {
//       printf("��end\n\n");
//         return false;
//     }
//     if (syn == 0) {
//       printf("�������\n");
//       printf("%s\n",token);
//     }
// }

bool yucu()//ָ��������һ��
{
	if(syn==1)//ƥ��begin
	{
		//printf("����ʽ��<�������>��begin <���><F>end\n");
		Scanner();
		if(Statement())//ƥ��<���>
		{
			while(syn==23)//ƥ��ֺ�
		    {
		    	//printf("����ʽ��<F>����<���><F>\n");
			    Scanner();
			    if(Statement())//ƥ��<���>
			    {
				    continue;
			    }else{
            printf("ERROR: lose Statement after ; in yucu\n");
    				return false;
    			}
		    }
		    //printf("����ʽ��<F>����\n");
		    if(syn==9)//ƥ��end
			{
				Scanner();
				return true;
			}else{
              printf("ERROR: lose end in yucu\n");
            	printf("����ȱ��end\n");
            	return false;
      }
		}else{
      printf("ERROR: lose Statement after begin in yucu\n");
      return false;
		}
	}else{
    return false;
  }
}

// bool Conditional_s() {
//     if (syn == 2) {
//         printf("if\n");
//         printf("����ʽ��<�������>��if <����> then <���><E>\n");
//         Scanner();
//         printf("����Condition����\n\n");
//         Condition();
//         if (syn == 3) {
//             printf("then\n");
//             Scanner();
//             printf("����Statement����\n\n");
//             Statement();
//             if (syn == 4) {
//               printf("����ʽ��<E>��else<���>\n");
//                 Scanner();
//                 printf("����Statement����\n\n");
//                 Statement();
//             } else {
//                 return true;
//             }
//         } else {
//             printf("ȱ�� then\n\n");
//             return false;
//         }
//     }
//     return false;
// }

bool Conditional_s() {
    if (syn == 2)
    {
        printf("����ʽ��<�������>��if <����> then <���><E>\n");
        Scanner();
        if (Condition())  //<����>
        {
            if (syn == 3)  //then
            {
                Scanner();
                if (Statement())  //<���>
                {
                    if (syn == 4)  //else
                    {
                        printf("����ʽ��<E>��else<���>\n");
                        Scanner();
                        if (Statement())  //ƥ��<���>
                        {
                            return true;
                        } else {
                          printf("ERROR: lose Statement after else in Conditional_s\n");
                            return false;
                        }
                    }
                    printf("����ʽ��<E>����\n");
                    return true;
                } else {
                  printf("ERROR: lose Statement after then in Conditional_s\n");
                    return false;
                }

            } else {
              printf("ERROR: lose then in Conditional_s\n");
                return false;
            }
        } else {
          printf("ERROR: lose Condition after if in Conditional_s\n");
            return false;
        }
    } else{
        return false;
    }
}

// bool DoWhile() {
//   printf("<��ѭ�����>��while <����> do <���>\n");
//   printf("%s\n",token);
//     // cout << "<��ѭ�����>��while <����> do <���>" << token << endl;
//     Scanner();
//     printf("����Condition����\n\n");
//     Condition();
//     if (syn == 6) {
//       printf("ѭ��do\n");
//         Scanner();
//         printf("����Statement����\n\n");
//         Statement();
//         return true;
//     } else{
//       return false;
//     }
// }

bool DoWhile()
{
   //ƥ��while
    if (syn == 5) {
        printf("����ʽ��<��ѭ�����>��while <����> do <���>\n");
        Scanner();
        if (Condition())  //ƥ��<����>
        {
            if (syn == 6)  //ƥ��do
            {
                Scanner();
                if (Statement())  //ƥ��<���>
                {
                    return true;
                } else {
                  printf("ERROR: lose Statement after do in DoWhile\n");
                    return false;
                }
            } else {
                printf("ERROR: lose do in DoWhile\n");
                return false;
            }
        } else {
            printf("ERROR: lose Condition in DoWhile\n");
            return false;
        }
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
    p = 0;
    initwordType();
    printf("����ʷ���������#��Ϊ����\n");
    do {
        ch = getchar();
        program[p++] = ch;
    } while (ch != '#');
    p = 0;
    q = 0;
    w = 0;
    for(int i=0;program[i]!='\0'&&program[i]!='\n';i++)
        printf("%c",program[i]);//�����һ�д���
    printf("\n");
    Scanner();
    Constdes();
    Scanner();
    Vardes();
    Scanner();
    Statement();
}
