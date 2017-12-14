#include <stdio.h>
#include <string.h>
#include <cstdlib> 

int syn,p,sum;  
//syn为单词种类编码；p为字符数组下标(遍历)；sum存储数字

//扫描遍历输入的字符串
void scaner(char prog[],char token[],char ch,char *rwtab[])
{
    int m,n;
    sum=0;
    for(m=0;m<10;m++)
    {
		token[m]= NULL;  
		//token存放构成单词符号的字符串
	}

	ch=prog[p++]; 
 	m=0;

	//遇到空格换行制表符时
    while( (ch==' ') || (ch=='\n') || (ch=='\t') )
        ch=prog[p++];    
        //判断字母
		if(((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A')))  
    	{
        	while(((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A'))||((ch>='0')&&(ch<='9')))
        	{
            	token[m++]=ch;
            	ch=prog[p++];
        	} 
        	p--;  //回退
        	syn=100;
        	//判断该串是否与关键字相同
        	for(n=0;n<17;n++)
        	{
            	if(strcmp(token,rwtab[n])==0)  
            	{
                	syn=n+1;
                	break;
            	}  //为关键字
        	}
    	}

		//判断数字 
    	else if((ch>='0')&&(ch<='9'))  
    	{
        	while((ch>='0')&&(ch<='9'))
        	{
            	sum=sum*10+ch-'0';
            	ch=prog[p++];
        	}  

        	p--;  //回退
        	syn=111; 
    	}
    	//不是字母数字关键字 
    	else
    	{
        	switch(ch)
        	{
        	case '<':
            	token[m++]=ch;
            	ch=prog[p++];
            	if(ch=='=')
            	{
                	syn=48; //<= 
                	token[m++]=ch;
            	}

            	else
            	{
                	syn=46; //<
                	p--;
            	}
            	break;

        	case '>':
            	token[m++]=ch;
            	ch=prog[p++];
            	if(ch=='=')
            	{
                	syn=50;  //>=
                	token[m++]=ch;
            	}

            	else
            	{
                	syn=49;  //>
                	p--;
            	}
            	break;

        	case '?':
            	syn=51;  //?
            	token[m++]=ch;
            	break;

        	case '+':
            	token[m++]=ch;
            	ch=prog[p++];
            	if(ch=='+')
            	{
                	syn=53;  //++
                	token[m++]=ch;
            	}

            	else
            	{
                	syn=40;  //+
                	p--;
            	}
            	break;

        	case '-':
            	token[m++]=ch;
            	ch=prog[p++];
            	if(ch=='-')
            	{
                	syn=54;  //--
                	token[m++]=ch;
            	}

            	else
            	{
               		syn=41;  //-
                	p--;
            	}
            	break;

        	case '*':
            	syn=42;  //*
            	token[m++]=ch;
            	break;

        	case '/':
            	syn=43;  ///
            	token[m++]=ch;
            	break;

        	case ':':
            	syn=44;  //:
            	token[m++]=ch;
            	break;

        	case '.':
            	syn=45;  //.
            	token[m++]=ch;
            	break;

        	case '=':
            	token[m++]=ch;
            	ch=prog[p++];
            	if(ch=='=')
            	{
                	syn=52;  //==
                	token[m++]=ch;
            	}

            	else
            	{
                	syn=25; //=
                	p--;
            	}
            	break;

        	case '!':
            	token[m++]=ch;
            	ch=prog[p++];
            	if(ch=='=')
            	{
                	syn=47;  //!=
                	token[m++]=ch;
            	}
            	else
            	{
                	syn=31;  //!
                	p--;
            	}
            	break;

        	case '(':
            	syn=27;  //(
            	token[m++]=ch;
            	break;

        	case ')':
            	syn=28;  //)
            	token[m++]=ch;
            	break;

        	case '{':
            	syn=29;  //{
            	token[m++]=ch;
            	break;

        	case '}':
            	syn=30;  //}
            	token[m++]=ch;
            	break;

        	case '[':
            	syn=32;  //[
            	token[m++]=ch;
            	break;

        	case ']':
            	syn=33;  //]
            	token[m++]=ch;
            	break;

        	case ';':
            	syn=26;  //;
            	token[m++]=ch;
            	break;

        	case '%':
            	syn=34;  //%
            	token[m++]=ch;
            	break;

        	case '&':
            	syn=35;  //&
            	token[m++]=ch;
            	break;

        	case '#':
           		syn=37;  //#
            	token[m++]=ch;
            	break;

        	case ',':
            	syn=38;  //,
            	token[m++]=ch;
            	break;

        	case '"':
            	syn=39; //"
            	token[m++]=ch;
            	break;

        	case '\0':
            	syn=0;
            	break;

        	default:
            	syn=-1;
            	break;
        	}
    	}
    	token[m++]='\0';
}



int main(void)
{
    char *rwtab[17]={"include","if","for","while","do","break","return","else","switch","case","void","int","double","long","char","struct","exit"};  //关键字
    FILE *fp;
    char prog[2000],token[10],ch; 
    p=0;

    if( (fp=fopen("ex.txt","r"))==NULL )
    {
        printf("文件打开错误！\n");
        exit(1);
    }

    do{
        ch=fgetc(fp);
        prog[p++]=ch;
    }while(ch!=EOF);  //将文件中的字符串存入prog[]
    prog[p]='\0';  //因为单个字符读取，加入'\0'防止存入非法数据 

    printf("源程序:\n%s\n\n词法分析：\n",prog);

    p=0;
    do{
            scaner(prog,token,ch,rwtab);
            switch(syn)
            {
                case 111:
                    printf("( %-10d%-8d )\n",syn,sum);  //数字或数字串，输出"(11,数字)"的二元式
                    break;

                case -1:
                    break;

                case 0:
                    return 0;

                default:
                    printf("( %-10d%-8s )\n",syn,token);
                    break;
            }
        }while(1);  //未结束,继续扫描

    fclose(fp);

    return 0;
 }


