#include "stdio.h"
#include "string.h"                 /*�����ַ����⺯��*/
#include "conio.h"                  /*�ṩ�й���Ļ���ڲ�������*/
#include "ctype.h"                  /*���ຯ��*/
#include <cstdlib>
char prog[80]={'\0'},token[8];                     /*��Ź��ɵ��ʷ��ŵ��ַ���*/
char ch;
int  syn,                           /*��ŵ����ַ����ֱ���*/
n,kk,sum,                           /*��������͵���*/
m,p;  /*p�ǻ�����prog��ָ�룬m��token��ָ��*/
void expression();
char *rwtab[6]={"begin","if","then","while","do","end"};
void scaner()
{
    m=0; sum=0;
    for (n=0;n<8;n++)token[n]='\0';
    ch=prog[p++];
    while (ch==' ')ch=prog[p++];
    if (isalpha(ch))    /*chΪ��ĸ�ַ�*/
    {
        while (isalpha(ch)||isdigit(ch))    /*ch Ϊ��ĸ�ַ����������ַ�*/
        {
            token[m++]=ch; ch=prog[p++];
        }
        token[m++]='\0';
        ch=prog[p--];
        syn=10;
        for (n=0;n<6;n++)
            if (strcmp(token,rwtab[n])==0)    /*�ַ����ıȽ�*/
            {
                syn=n+1;
                break;
            }
    }
    else
        if (isdigit(ch))    /*ch�������ַ�*/
        {
            while (isdigit(ch))    /*ch�������ַ�*/
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
  printf("����\n");
  if (syn==10) {
      printf("%s\n",token );
      scaner();
  }else if (syn==11) {
      printf("�޷�������%d\n",sum );
      scaner();
  }else if (syn==27) {
      scaner();
		  printf("���������ţ����ʽ��������\n\n");
		  expression();
      if (syn==28) {
          scaner();     //��)
          printf("������\n");
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
  printf("<��>��<����>{<�˷������><����>}\n");
    factor();
    while (syn==15 || syn==16)  //��* , /
    {
    	printf("�����%s\n",token);
        scaner();
        
        printf("%s\n",token);
		    printf("����factor����\n\n");
		    factor();
    }
    return;
}

void expression()  //���ʽ��������
{
  printf("<���ʽ>��[+|-]<��>{<�ӷ������><��>}\n");
    term();
    while (syn==13 || syn==14)   //��+ , -
    {
        printf("�����%s\n",token);
		scaner();
        
        printf("%s\n",token);
		    printf("����term����\n\n");
		    term();
    }
    return;
}
void statement()  //����������
{
  printf("<��ֵ���>��<��ʶ��>=<���ʽ>\n");
  printf("%s\n",token);
    if (syn==10)  //�Ƿ��ʶ��
    {
        scaner();
        if (syn==18)  //��:=
        {
            scaner();
			printf(":=\n");
			printf("���ñ��ʽ��������\n\n");
			expression();
        }
        else
        {
            printf("\":=\" ERROR!\n");
            kk=1;
        } }
    else
    {   printf("statement ERROR!\n");
        kk=1;
    }
    return;
}

void yucu()  //��䴮����
{
  printf("����������\n");
  printf("%s\n",token);
    statement();
    while (syn==26)  //��;
    {
         scaner();
         printf(";\n");
		     printf("��������������\n\n");
		     statement();
    }
    return;
}
void lrparser()
{
  //begin��ͷ
    if (syn==1){
      scaner();
		  printf("<�������>��begin <���>{;<���>} end\n");
		  printf("begin\n");
      printf("������䴮��������\n\n");
		  yucu();
      if (syn==6){
           scaner();
           if (syn==0 && (kk==0)){
             printf("\n\n�﷨���������\n");
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
        printf("�ļ��򿪴���\n");
        exit(1);
    }

    do{
        ch=fgetc(fp);
        prog[p++]=ch;
    }while(ch!=EOF);  //���ļ��е��ַ�������prog[]
    prog[p]='\0';  //��Ϊ�����ַ���ȡ������'\0'��ֹ����Ƿ�����

    printf("Դ����:\n%s\n\n�﷨������ʼ��\n",prog);

    p=0;
    scaner();
    lrparser();

    fclose(fp);

    return 0;
 }

