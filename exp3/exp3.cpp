#include "stdio.h"
#include "string.h"                 /*定义字符串库函数*/
#include "conio.h"                  /*提供有关屏幕窗口操作函数*/
#include "ctype.h"                  /*分类函数*/
#include <cstdlib>
char prog[80]={'\0'},token[8];                     /*存放构成单词符号的字符串*/
char ch;
int  syn,                           /*存放单词字符的种别码*/
n,kk,sum,                           /*存放整数型单词*/
m,p;  /*p是缓冲区prog的指针，m是token的指针*/
void expression();
char *rwtab[6]={"begin","if","then","while","do","end"};
void scaner()
{
    m=0; sum=0;
    for (n=0;n<8;n++)token[n]='\0';
    ch=prog[p++];
    while (ch==' ')ch=prog[p++];
    if (isalpha(ch))    /*ch为字母字符*/
    {
        while (isalpha(ch)||isdigit(ch))    /*ch 为字母字符或者数字字符*/
        {
            token[m++]=ch; ch=prog[p++];
        }
        token[m++]='\0';
        ch=prog[p--];
        syn=10;
        for (n=0;n<6;n++)
            if (strcmp(token,rwtab[n])==0)    /*字符串的比较*/
            {
                syn=n+1;
                break;
            }
    }
    else
        if (isdigit(ch))    /*ch是数字字符*/
        {
            while (isdigit(ch))    /*ch是数字字符*/
            {
                sum=sum*10+ch-'0'; ch=prog[p++];
            }
            ch=prog[p--];
            syn=11;
        }
        else
            switch (ch)
            {
            case'<':
                m=0;  token[m++]=ch;
                ch=prog[p++];
                if (ch=='>')
                {   syn=21;
                    token[m++]=ch;
                }
                else if (ch=='=')
                {   syn=22;  token[m++]=ch;
                }
                else
                {
					syn=20; ch=prog[p--];
                }
                break;
            case'>':
                m=0;
                token[m++]=ch;
                ch=prog[p++];
                if (ch=='=')
                {
 					syn=24;  token[m++]=ch;
                }
                else
                {
					syn=23;  ch=prog[p--];
                }
                break;
            case':':
                m=0; token[m++]=ch;
                ch=prog[p++];
                if (ch=='=')
                { syn=18; token[m++]=ch;
                }
                else
                { syn=17; ch=prog[p--];
                }
                break;
            case'+':
                syn=13;  token[0]=ch;
                break;
            case'-':
                syn=14;  token[0]=ch;
                break;
            case'*':
                syn=15;  token[0]=ch;
                break;
            case'/':
                syn=16;  token[0]=ch;
                break;
            case'=':
                syn=25;  token[0]=ch;
                break;
            case';':
                syn=26;  token[0]=ch;
                break;
            case'(':
                syn=27;  token[0]=ch;
                break;
            case')':
                syn=28;  token[0]=ch;
                break;
            case'#':
                syn=0;  token[0]=ch;
                break;
            default:
                syn=-1;
            }
}

void factor()
{
  printf("因子\n");
  if (syn==10) {
      printf("%s\n",token );
      scaner();
  }else if (syn==11) {
      printf("无符号数字%d\n",sum );
      scaner();
  }else if (syn==27) {
      scaner();
		  printf("遇到左括号，表达式分析函数\n\n");
		  expression();
      if (syn==28) {
          scaner();     //有)
          printf("右括号\n");
      }else{
          printf("\')\' ERROR!\n");
      }
  }else{
      printf("expressions ERROR!\n");  kk=1;
  }
  return;
}

void term()
{
  printf("<项>→<因子>{<乘法运算符><因子>}\n");
    factor();
    while (syn==15 || syn==16)  //有* , /
    {
    	printf("运算符%s\n",token);
        scaner();

        printf("%s\n",token);
		    printf("调用factor函数\n\n");
		    factor();
    }
    return;
}

void expression()  //表达式分析函数
{
  printf("<表达式>→[+|-]<项>{<加法运算符><项>}\n");
    term();
    while (syn==13 || syn==14)   //有+ , -
    {
        printf("运算符%s\n",token);
		scaner();

        printf("%s\n",token);
		    printf("调用term函数\n\n");
		    term();
    }
    return;
}

void statement()  //语句分析函数
{
  printf("<赋值语句>→<标识符>=<表达式>\n");
  printf("%s\n",token);
    if (syn==10)  //是否标识符
    {
        scaner();
        if (syn==18)  //有:=
        {
            scaner();
			      printf(":=\n");
			      printf("调用表达式分析函数\n\n");
			      expression();
        }else{
            printf("\":=\" ERROR!\n");
            kk=1;
        }
    }else{
        printf("statement ERROR!\n");
        kk=1;
    }
    return;
}

void yucu()  //语句串分析
{
  printf("复合语句分析\n");
  printf("%s\n",token);
    statement();
    while (syn==26)  //有;
    {
         scaner();
         printf(";\n");
		     printf("调用语句分析函数\n\n");
		     statement();
    }
    return;
}

void lrparser()
{
  //begin开头
    if (syn==1){
      scaner();
		  printf("<复合语句>→begin <语句>{;<语句>} end\n");
		  printf("begin\n");
      printf("调用语句串分析函数\n\n");
		  yucu();
      if (syn==6){
           scaner();
           if (syn==0 && (kk==0)){
             printf("\n\n语法分析结果：\n");
             printf("success");
           }else{
             if (kk!=1){
               printf("The lack of \"end\",ERROR!\n");
             }
             kk=1;
           }
      }
   }else{
      printf("The lack of \"begin\",ERROR!\n");
      kk=1;
   }
    return;
}

int main(void)
{
    FILE *fp;
    kk=0;  p=0;

    if( (fp=fopen("ex3.txt","r"))==NULL )
    {
        printf("文件打开错误！\n");
        exit(1);
    }

    do{
        ch=fgetc(fp);
        prog[p++]=ch;
    }while(ch!=EOF);  //将文件中的字符串存入prog[]
    prog[p]='\0';  //因为单个字符读取，加入'\0'防止存入非法数据

    printf("源程序:\n%s\n\n语法分析开始：\n",prog);

    p=0;
    scaner();
    lrparser();

    fclose(fp);

    return 0;
 }
