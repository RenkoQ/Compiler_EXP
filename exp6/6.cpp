#include<stdio.h>
#include<string.h>
#include<iostream>
#include<stdlib.h>
using namespace std;

//四元式表的结构
struct {
	char result[12];
	char ag1[12];
	char op[12];
	char ag2[12];
} quad;

char prog[80],token[12];
char ch;
int syn,p,m=0,n,sum=0,kk;    //p是缓冲区prog的指针，m是token的指针
char *rwtab[6]= {"begin","if","then","while","do","end"};
void scaner();
char *factor(void);
char *term(void);
char *expression_r(void);
int yucu();
void emit(char *result,char *ag1,char *op,char *ag2);
char *newtemp();
int statement();
int k=0;

// 生成一个三地址语句送到四元式表中
void emit(char *result,char *ag1,char *op,char *ag2) {
	strcpy(quad.result,result);
	strcpy(quad.ag1,ag1);
	strcpy(quad.op,op);
	strcpy(quad.ag2,ag2);

	cout<<quad.result<<"="<<quad.ag1<<quad.op<<quad.ag2<<endl;
}

//回送一个新的临时变量名，临时变量名产生的顺序为T1，T2
char *newtemp() {
	char *p;
	char m[12];
	p=(char *)malloc(12);
	k++;
	itoa(k,m,10);
	strcpy(p+1,m);
	p[0]='t';
	return (p);
}

void scaner() {
	for(n=0; n<8; n++) token[n]=NULL;
	ch=prog[p++];
	while(ch==' ') {
		ch=prog[p];
		p++;
	}
	if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')) {
		m=0;
		while((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')) {
			token[m++]=ch;
			ch=prog[p++];
		}
		token[m++]='\0';
		p--;
		syn=10;
		for(n=0; n<6; n++)
			if(strcmp(token,rwtab[n])==0) {
				syn=n+1;
				break;
			}
	} else if((ch>='0'&&ch<='9')) {
		{
			sum=0;
			while((ch>='0'&&ch<='9')) {
				sum=sum*10+ch-'0';
				ch=prog[p++];
			}
		}
		p--;
		syn=11;
		if(sum>32767)
			syn=-1;
	} else switch(ch) {
			case'<':
				m=0;
				token[m++]=ch;
				ch=prog[p++];
				if(ch=='>') {
					syn=21;
					token[m++]=ch;
				} else if(ch=='=') {
					syn=22;
					token[m++]=ch;
				} else {
					syn=23;
					p--;
				}
				break;
			case'>':
				m=0;
				token[m++]=ch;
				ch=prog[p++];
				if(ch=='=') {
					syn=24;
					token[m++]=ch;
				} else {
					syn=20;
					p--;
				}
				break;
			case':':
				m=0;
				token[m++]=ch;
				ch=prog[p++];
				if(ch=='=') {
					syn=18;
					token[m++]=ch;
				} else {
					syn=17;
					p--;
				}
				break;
			case'*':
				syn=13;
				token[0]=ch;
				break;
			case'/':
				syn=14;
				token[0]=ch;
				break;
			case'+':
				syn=15;
				token[0]=ch;
				break;
			case'-':
				syn=16;
				token[0]=ch;
				break;
			case'=':
				syn=25;
				token[0]=ch;
				break;
			case';':
				syn=26;
				token[0]=ch;
				break;
			case'(':
				syn=27;
				token[0]=ch;
				break;
			case')':
				syn=28;
				token[0]=ch;
				break;
			case'#':
				syn=0;
				token[0]=ch;
				break;
			default:
				syn=-1;
				break;
		}
}

//在原来语法分析的基础上插入相应的语义动作：将输入串翻译成四元式序列
//对表达式、赋值语句进行翻译
int lrparser() {
	int schain=0;
	kk=0;
	if(syn==1) {
		scaner();
		schain=yucu();
		if(syn==6) {
			scaner();
			if(syn==0 && (kk==0))
				cout<<"success!"<<endl;
		}

		else {
			if(kk!=1)
				cout<<"缺end!"<<endl;
			kk=1;
		}
	} else {
		cout<<"缺begin!"<<endl;
		kk=1;
	}

	return(schain);
}

int yucu() {
	int schain=0;
	schain=statement();
	while(syn==26) {
		scaner();
		schain=statement();
	}
	return(schain);
}

int statement() {
	char *eplace,*tt;
	eplace=(char *)malloc(12);
	tt=(char *)malloc(12);
	int schain=0;
	switch(syn) {
		case 10:
			strcpy(tt,token);
			scaner();
			if(syn==18) {
				scaner();
				strcpy(eplace,expression_r());
				emit(tt,eplace,"","");
				schain=0;
			} else {
				cout<<"缺少赋值符!"<<endl;
				kk=1;
			}
			return(schain);
			break;
	}
	return(schain);
}

char *expression_r(void) {
	char *tp,*ep2,*eplace,*tt;
	tp=(char *)malloc(12);
	ep2=(char *)malloc(12);
	eplace=(char *)malloc(12);
	tt =(char *)malloc(12);
	strcpy(eplace,term ());          //调用term分析产生表达式计算的第一项eplace

	while((syn==15)||(syn==16)) {
		if(syn==15)strcpy(tt,"+");
		else strcpy(tt,"-");
		scaner();
		strcpy(ep2,term());        //调用term分析产生表达式计算的第二项ep2
		strcpy(tp,newtemp());    //调用newtemp产生临时变量tp存储计算结果
		emit(tp,eplace,tt,ep2);    //生成四元式送入四元式表
		strcpy(eplace,tp);
	}
	return(eplace);
}

char *term(void) {
	char *tp,*ep2,*eplace,*tt;
	tp=(char *)malloc(12);
	ep2=(char *)malloc(12);
	eplace=(char *)malloc(12);
	tt=(char *)malloc(12);
	strcpy(eplace,factor());

	while((syn==13)||(syn==14)) {
		if(syn==13)strcpy(tt,"*");
		else strcpy(tt,"/");
		scaner();
		strcpy(ep2,factor());      //调用factor分析产生表达式计算的第二项ep2
		strcpy(tp,newtemp());    //调用newtemp产生临时变量tp存储计算结果
		emit(tp,eplace,tt,ep2);    //生成四元式送入四元式表
		strcpy(eplace,tp);
	}
	return(eplace);
}

char *factor(void) {
	char *fplace;
	fplace=(char *)malloc(12);
	strcpy(fplace,"");
	if(syn==10) {
		strcpy(fplace,token);             //将标识符token的值赋给fplace
		scaner();
	} else if(syn==11) {
		itoa(sum,fplace,10);
		scaner();
	} else if(syn==27) {
		scaner();
		fplace=expression_r();             //调用expression分析返回表达式的值
		if(syn==28)
			scaner();
		else {
			cout<<"缺)错误!"<<endl;
			kk=1;
		}
	} else {
		cout<<"缺(错误!"<<endl;
		kk=1;
	}
	return(fplace);
}

int main(void) {
	FILE *fp;
	kk=0;
	p=0;

	cout<<"**********语义分析程序**********"<<endl;

	if( (fp=fopen("ex6.txt","r"))==NULL ) {
		printf("文件打开错误！\n");
		exit(1);
	}

	do {
		ch=fgetc(fp);
		prog[p++]=ch;
	} while(ch!='#');  //将文件中的字符串存入prog[]
	prog[p]='\0';  //因为单个字符读取，加入'\0'防止存入非法数据


	p=0;
	scaner();
	lrparser();

	fclose(fp);

	return 0;
}
