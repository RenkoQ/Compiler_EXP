#include <stdio.h>
#include <string.h>
#include <cstdlib> 

int syn,p,sum;  
//synΪ����������룻pΪ�ַ������±�(����)��sum�洢����

//ɨ�����������ַ���
void scaner(char prog[],char token[],char ch,char *rwtab[])
{
    int m,n;
    sum=0;
    for(m=0;m<10;m++)
    {
		token[m]= NULL;  
		//token��Ź��ɵ��ʷ��ŵ��ַ���
	}

	ch=prog[p++]; 
 	m=0;

	//�����ո����Ʊ��ʱ
    while( (ch==' ') || (ch=='\n') || (ch=='\t') )
        ch=prog[p++];    
        //�ж���ĸ
		if(((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A')))  
    	{
        	while(((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A'))||((ch>='0')&&(ch<='9')))
        	{
            	token[m++]=ch;
            	ch=prog[p++];
        	} 
        	p--;  //����
        	syn=100;
        	//�жϸô��Ƿ���ؼ�����ͬ
        	for(n=0;n<17;n++)
        	{
            	if(strcmp(token,rwtab[n])==0)  
            	{
                	syn=n+1;
                	break;
            	}  //Ϊ�ؼ���
        	}
    	}

		//�ж����� 
    	else if((ch>='0')&&(ch<='9'))  
    	{
        	while((ch>='0')&&(ch<='9'))
        	{
            	sum=sum*10+ch-'0';
            	ch=prog[p++];
        	}  

        	p--;  //����
        	syn=111; 
    	}
    	//������ĸ���ֹؼ��� 
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
    char *rwtab[17]={"include","if","for","while","do","break","return","else","switch","case","void","int","double","long","char","struct","exit"};  //�ؼ���
    FILE *fp;
    char prog[2000],token[10],ch; 
    p=0;

    if( (fp=fopen("ex.txt","r"))==NULL )
    {
        printf("�ļ��򿪴���\n");
        exit(1);
    }

    do{
        ch=fgetc(fp);
        prog[p++]=ch;
    }while(ch!=EOF);  //���ļ��е��ַ�������prog[]
    prog[p]='\0';  //��Ϊ�����ַ���ȡ������'\0'��ֹ����Ƿ����� 

    printf("Դ����:\n%s\n\n�ʷ�������\n",prog);

    p=0;
    do{
            scaner(prog,token,ch,rwtab);
            switch(syn)
            {
                case 111:
                    printf("( %-10d%-8d )\n",syn,sum);  //���ֻ����ִ������"(11,����)"�Ķ�Ԫʽ
                    break;

                case -1:
                    break;

                case 0:
                    return 0;

                default:
                    printf("( %-10d%-8s )\n",syn,token);
                    break;
            }
        }while(1);  //δ����,����ɨ��

    fclose(fp);

    return 0;
 }


