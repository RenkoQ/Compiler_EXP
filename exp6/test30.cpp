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

int syn;  //存放单词的类型
int p;   //用于遍历程序源代码

char ch;  //用于读取程序源代码
int sum;  //用来保存数字的值
char program[1000], token[10];
//关键字
const char *rwtab[9] = {"begin", "if",    "then", "else", "while",
                        "do",    "Const", "Var",  "end"};
int m;

char* lexicalType[27];//词法类型
struct symbol{
	char name[10];
	int value;
};//符号表项
symbol symbolTable[100];//符号表

int q;//用于依次往符号表中添加项目

char id[10];//用于存储id

char temp[100][10]={"t1","t2","t3","t4","t5"};//中间变量

int w;//用于依次生成中间变量

bool lookup(char *id)
{
	int i;
	for(i=0;symbolTable[i].name[0]!='\0';i++){
		if(strcmp(id,symbolTable[i].name)==0)
		    return true;
	}
	return false;
}
// 1~9对应关键字，10对应标识符，11对应无符号整数，
// 12~22对应运算符 ，23~26对应分界符
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

//输出单词符号二元式<词类表示，单词的属性值>
void lexicalPrint()
{
    if(syn==11)//无符号整数类型属性值为其value
        printf("  %s:<%d,%d>\n",lexicalType[syn],syn,sum);
    else//其余类型属性值为其单词序列
        printf("  %s:<%d,%s>\n",lexicalType[syn],syn,token);
}

bool Const_Define()
{
    //匹配标识符
    if (syn == 10){
      // cout << "<常量定义>→<标识符>=<无符号整数>" << token << endl;
        if (!lookup(token)) {
            strcpy(symbolTable[q].name, token);
            Scanner();
            //匹配等号
            if (syn == 16){
              // cout << "<赋值语句>→<标识符>=<表达式>" << token << endl;
                Scanner();
                //匹配无符号整数
                if (syn == 11){
                  // cout << "<无符号整数>→<数字>{<数字>}" << sum << endl;
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
//         cout << " <变量说明>→Var <标识符>{，<标识符>}" << token << endl;
//         Scanner();
//         while (Const_Define()) {
//             Scanner();
//             if (syn == 23) {
//                 cout << "分号" << token << endl;
//                 return true;
//             } else if (syn == 26) {
//                 cout << "逗号" << token << endl;
//                 Scanner();
//                 continue;
//             }
//             cout << "常量说明错误" << endl;
//         }
//     }
//     return false;
// }
bool Constdes()  //常量说明
{
    if (syn == 7)  //匹配Const关键字
    {
      //printf("产生式：<G>→<常量说明>\n");
    	//printf("产生式：<常量说明>→Const <常量定义><A>；\n");
        Scanner();
        if (Const_Define())  //匹配<常量定义>
        {
            Scanner();
            while (syn == 26)  //匹配逗号
            {
              //printf("产生式：<A>→，<常量定义><A> \n");
                Scanner();
                if (Const_Define())  //匹配<常量定义>
                {
                    Scanner();
                    continue;
                } else {
                  printf("ERROR: lose Const_Define in Constdes\n");
                    return false;
                }
            }
            if (syn == 23)  //匹配分号
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
//         cout << "<变量说明>→Var <标识符>{，<标识符>}" << token << endl;
//         Scanner();
//         while (Variable_Defined()) {
//             Scanner();
//             if (syn == 23) {
//                 cout << "分号" << token << endl;
//                 return true;
//             } else if (syn == 26) {
//                 cout << "逗号" << token << endl;
//                 Scanner();
//                 continue;
//             }
//         }
//     }
//     return false;
// }
bool Vardes()//指针不会读到下一个
{
	if(syn==8)//匹配Var关键字
    {
    	//printf("产生式：<H>→<变量说明>\n");
    	//printf("产生式：<变量说明>→Var <标识符><B>；\n");
        Scanner();
        //匹配标识符
        if(syn==10){
        	if(!lookup(token)){
	        	strcpy(symbolTable[q].name,token);
	        	printf("  add Identifier to symbolTable name %s\n",symbolTable[q].name);
	        	q++;
	            Scanner();
	            while(syn==26)//匹配逗号
	            {
					        //printf("产生式：<B>→，<标识符><B> \n");
	            	  Scanner();
                  //匹配标识符
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
	            //printf("产生式：<B>→ε\n");
              //匹配分号
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
    	//printf("产生式：<H>→ε\n");
    	return false;
    }
}

bool Condition()//指针会读到下一个
{
	printf("产生式：<条件>→<表达式><关系运算符><表达式>\n");
	if(Expression())//匹配<表达式>
	{
    //匹配<关系运算符>
		if(syn==17||syn==18||syn==19||syn==20||syn==21||syn==22){
			Scanner();
      //匹配<表达式>
			if(Expression()){
				return true;
			}else{
        printf("ERROR: lose Expression in Condition\n");
				return false;
      }
		}else{
			  printf("ERROR: lose 关系运算符 in Condition\n");
        return false;
		}
	}else
	    return false;
}

// bool Expression() {
//     printf("<表达式>→[+|-]<项>{<加法运算符><项>}\n");
//     do {
//         if (syn == 12 || syn == 13) {
//           printf("%s\n",token);
//           printf("<加法运算符>→+|-\n");
//             Scanner();
//             printf("调用term函数\n\n");
//             term();
//         } else {
//           printf("调用term函数\n\n");
//             term();
//         }
//     } while (syn == 12 || syn == 13);
//     return true;
// }
char * Expression()
{
  // printf("<表达式>→[+|-]<项>{<加法运算符><项>}\n");
	char lastid[10],op[10];
  int i;
	char str[50]="产生式：<表达式>→";
  //匹配+
	if(syn==12){
		for(i=0;str[i]!='\0';i++);
	  str[i]='+';
		Scanner();
	}else if(syn==13)//匹配-
	{
		for(i=0;str[i]!='\0';i++);
	    str[i]='-';
		Scanner();
	}
  //匹配select集
	if(syn==10||syn==11||syn==24){
		//printf("%s<项><C>\n",str);
		term();//匹配<项>
		strcpy(lastid,id);
		while(syn==12||syn==13)//匹配<加法运算符>
    {
        strcpy(op,token);
		    //printf("产生式：<C>→<加法运算符><项><C>\n");
			  Scanner();
        //匹配<项>
			  if(term()){
				      printf("  %s:=%s%s%s\n",temp[w],lastid,op,id);
				      strcpy(lastid,temp[w]);
				      w++;
              continue;
			  }else{
				      printf("错误：缺少项\n");
              return NULL;
		    }
     }
    //printf("产生式：<C>→ε\n");
    strcpy(id,lastid);
		return id;
  }else{
      return NULL;
  }
}

// bool term() {
//     printf("<项>→<因子>{<乘法运算符><因子>}\n");
//     while (Factor()) {
//         if (syn == 14 || syn == 15) {
//           printf("%s\n",token);
//           printf("<乘法运算符>→* |/\n");
//             Scanner();
//
//         } else {
//             return true;
//         }
//     }
//     return false;
// }

//指针会读到下一个 select={syn=10,11,24}
char *term()
{
  // printf("<项>→<因子>{<乘法运算符><因子>}\n");
	char lastid[10],op[10];
	if(syn==10||syn==11||syn==24)//匹配select
	{
		//printf("产生式：<项>→<因子><D>\n");
		Factor();
		strcpy(lastid,id);//保存id标识符
		Scanner();
		while(syn==14||syn==15)//匹配项<乘法运算符>
		{
			strcpy(op,token);
			//printf("产生式：<D>→<乘法运算符><因子><D>\n");
			Scanner();
			if(Factor())//匹配<因子>
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
		//printf("产生式：<D>→ε\n");
		strcpy(id,lastid);
		return id;
	}
	else
	    return NULL;
}

// bool Factor() {
//     printf("因数factor\n");
//     // Scanner();
//     if (syn == 10) {
//       printf("标识符\n");
//       printf("%s\n",token );
//         Scanner();  //特殊
//         return true;
//     } else if (syn == 11) {
//       printf("无符号数字\n");
//       printf("%d\n",sum );
//         Scanner();
//         return true;
//     } else if (syn == 24) {
//       printf("遇到左括号，表达式分析函数\n");
//       printf("%s\n",token );
//         Scanner();
//         Expression();
//         if (syn == 25) {
//           printf("右括号\n");
//           printf("%s\n",token );
//             Scanner();
//             return true;
//         } else {
//             // cout << "错误：没有)" << endl;
//             return false;
//         }
//     } else {
//         printf("\')\' ERROR!\n");
//         return false;
//     }
//     return false;
// }

char * Factor()//指针不会读到下一个 select={syn=10,11,24}
{
	strcpy(id,token);
    //匹配标识符
    if(syn==10){
        return id;
    }else if(syn==11)//匹配无符号整数
    {
        return id;
    }else if(syn==24)//匹配左括号
    {
        Scanner();
        //匹配<表达式>
        if(Expression()){
          //匹配右括号
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
//     printf("<赋值语句>→<标识符>=<表达式>\n\n");
//     printf("标识符%s\n",token);
//     Scanner();
//     if (syn == 16) {
//         printf("赋值语句=\n\n");
//         Scanner();
//         printf("调用Expression函数\n\n");
//         Expression();
//         return true;
//     } else {
//       printf("缺少=号不是赋值语句\n\n");
//       return false;
//     }
// }
bool Assign()
{
  // printf("<赋值语句>→<标识符>=<表达式>\n\n");
	char lastid[10];
    //匹配标识符
    if(syn==10){
		    if(lookup(token)){
		        strcpy(lastid,token);
			      //printf("产生式：<赋值语句>→<标识符>＝<表达式>;\n");
			      Scanner();
            //匹配等号
			      if(syn==16){
				      Scanner();
              //匹配表达式
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
			printf("ERROR：no var define in Assign%s\n",token);
			return false;
		}
	}else{
    return false;
  }
}

// bool yucu() {
//   printf("复合语句\n");
//   printf("%s\n",token);
//     Scanner();
//     while (Statement()) {
//         if (syn == 23) {
//           printf("复合语句中的分割符\n");
//           printf("%s\n",token);
//             Scanner();
//             if (syn == 9) {
//                 break;
//             }
//         }
//     }
//     if (syn == 9) {
//       printf("<复合语句>→begin <语句>{;<语句>} end\n");
//       printf("%s\n",token);
//         Scanner();
//         return true;
//     } else {
//       printf("少end\n\n");
//         return false;
//     }
//     if (syn == 0) {
//       printf("复合语句\n");
//       printf("%s\n",token);
//     }
// }

bool yucu()//指针会读到下一个
{
	if(syn==1)//匹配begin
	{
		//printf("产生式：<复合语句>→begin <语句><F>end\n");
		Scanner();
		if(Statement())//匹配<语句>
		{
			while(syn==23)//匹配分号
		    {
		    	//printf("产生式：<F>→；<语句><F>\n");
			    Scanner();
			    if(Statement())//匹配<语句>
			    {
				    continue;
			    }else{
            printf("ERROR: lose Statement after ; in yucu\n");
    				return false;
    			}
		    }
		    //printf("产生式：<F>→ε\n");
		    if(syn==9)//匹配end
			{
				Scanner();
				return true;
			}else{
              printf("ERROR: lose end in yucu\n");
            	printf("错误：缺少end\n");
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
//         printf("产生式：<条件语句>→if <条件> then <语句><E>\n");
//         Scanner();
//         printf("调用Condition函数\n\n");
//         Condition();
//         if (syn == 3) {
//             printf("then\n");
//             Scanner();
//             printf("调用Statement函数\n\n");
//             Statement();
//             if (syn == 4) {
//               printf("产生式：<E>→else<语句>\n");
//                 Scanner();
//                 printf("调用Statement函数\n\n");
//                 Statement();
//             } else {
//                 return true;
//             }
//         } else {
//             printf("缺少 then\n\n");
//             return false;
//         }
//     }
//     return false;
// }

bool Conditional_s() {
    if (syn == 2)
    {
        printf("产生式：<条件语句>→if <条件> then <语句><E>\n");
        Scanner();
        if (Condition())  //<条件>
        {
            if (syn == 3)  //then
            {
                Scanner();
                if (Statement())  //<语句>
                {
                    if (syn == 4)  //else
                    {
                        printf("产生式：<E>→else<语句>\n");
                        Scanner();
                        if (Statement())  //匹配<语句>
                        {
                            return true;
                        } else {
                          printf("ERROR: lose Statement after else in Conditional_s\n");
                            return false;
                        }
                    }
                    printf("产生式：<E>→ε\n");
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
//   printf("<当循环语句>→while <条件> do <语句>\n");
//   printf("%s\n",token);
//     // cout << "<当循环语句>→while <条件> do <语句>" << token << endl;
//     Scanner();
//     printf("调用Condition函数\n\n");
//     Condition();
//     if (syn == 6) {
//       printf("循环do\n");
//         Scanner();
//         printf("调用Statement函数\n\n");
//         Statement();
//         return true;
//     } else{
//       return false;
//     }
// }

bool DoWhile()
{
   //匹配while
    if (syn == 5) {
        printf("产生式：<当循环语句>→while <条件> do <语句>\n");
        Scanner();
        if (Condition())  //匹配<条件>
        {
            if (syn == 6)  //匹配do
            {
                Scanner();
                if (Statement())  //匹配<语句>
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

// int main() {
//   FILE *fp;
//   p=0;
//
//   if( (fp=fopen("test3.txt","r"))==NULL )
//   {
//       printf("文件打开错误！\n");
//       exit(1);
//   }
//
//   do {
//         ch=fgetc(fp);
//         program[p++] = ch;
//   } while (ch != '#');
//   program[p]='\0';
//
//     printf("源程序:\n%s\n\n语法分析开始：\n",program);
//
//     p = 0;
//     printf("<程序>→[<常量说明>][<变量说明>]<语句>");
//
//     Constdes();
//     Vardefine();
//     do {
//         Scanner();
//         Statement();
//     } while (syn != 0);
//     cout << "分析完成，关闭文件" << endl;
//     fclose(fp);
//     return 0;
// }

int main() {
    p = 0;
    initwordType();
    printf("输入词法分析串以#作为结束\n");
    do {
        ch = getchar();
        program[p++] = ch;
    } while (ch != '#');
    p = 0;
    q = 0;
    w = 0;
    for(int i=0;program[i]!='\0'&&program[i]!='\n';i++)
        printf("%c",program[i]);//输出第一行代码
    printf("\n");
    Scanner();
    Constdes();
    Scanner();
    Vardes();
    Scanner();
    Statement();
}

// int main()
// {
//   FILE *fp;
// 	p=0;
// 	initwordType();
//     // printf("输入词法分析串以#作为结束\n");
//     if( (fp=fopen("test3.txt","r"))==NULL ) {
//   		printf("文件打开错误！\n");
//   		exit(1);
//   	}
//
//     do{
//         ch=fgetc(fp);
//         program[p++]=ch;
//     }while(ch!='#');
//     program[p]='\0';
// 	   p=0;
// 	    q=0;
// 	     w=0;
// 	    for(int i=0;program[i]!='\0'&&program[i]!='\n';i++)
// 	         printf("%c",program[i]);//输出第一行代码
//       printf("\n");
// 	     Scanner();
// 	     Constdes();
// 	     Scanner();
// 	     Vardes();
//        Scanner();
//        Statement();
// }
