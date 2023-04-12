#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "list.h"

Nlist* s_pListTail=NULL;
//定义全局变量 分别为字段链表表头,尾指针和当前所在字段
static Nlist* s_pListHead=NULL;
static Nlist* s_pNowList=NULL;
static char target[50];
//判断文件是否为ini文件
int ifIsIni(char *filename)
{
	char *namesk=filename;
	for(int i=0;i<4;i++)
	{
		namesk++;
		if(!*namesk)
		{
			printf("文件类型不匹配或者文件名有误\n");
			return -1;
		}
	}
	while(*namesk)
	{
		namesk++;
		filename++;
	}
	int cmp=strncmp(filename,".ini",4);
	return cmp;
	
}

//对字符串进行去空格,去注释处理
int process(char *buf)
{
	char *ptr=buf;
	while(*ptr)
	{
		if(*ptr==' ')
		{
			char *cos=ptr;
			while(*cos)
			{
				*cos=*(cos+1);
				cos++;
			}
		}
		ptr++;
	}
	ptr=buf;
	while(*ptr)
	{
		if(*ptr==';'||*ptr=='#')
		{
			*ptr='\0';
			return 0;
		}
		ptr++;
	}
	return 0;
}

//将字符串处理后插入到链表中
int addNode(char *buf)
{
	char *token;
	char section[50]={0};
	char key[100]={0};
	char value[300]={0};
	if(*buf=='[')
	{
		buf++;
		token=strtok(buf,"]");
		strncpy(section,token,50);
		N newNode=createListNode(section);
		if(s_pListHead==NULL)
		{
			s_pListHead=newNode;
			
		}
		else
		{
			insrtSectionToTail(s_pListTail,newNode);
		}
		s_pNowList=newNode;
		s_pListTail=newNode;
	}
	else 
	{
		memset(key,0,100);
		memset(value,0,300);
		token=strtok(buf,"=");
		strncpy(key,token,50);
		token=strtok(NULL,"=");
		if(token!=NULL)strncpy(value,token,50);
		K newKey=createKeyNode(key,value);
		if(s_pNowList->keyHead==NULL)
		{
			s_pNowList->keyHead=newKey;
		}
		else
		{
			insrtKeyToTail(s_pNowList->keyTail,newKey);
		}
		s_pNowList->keyTail=newKey;
	}
}

//读取文件
int readFile(FILE *fp)
{
	char buf[500];
	char *str=NULL;
	while(!feof(fp))
	//while(1)
	{
		memset(buf,0,500);
		fscanf(fp,"%[^\n]\n",buf);
		if(*buf=='\0')
		{
			fseek(fp,SEEK_CUR,1);
			continue;
		}
		//str=fgets(buf,500,fp);
		//printf("%s",buf);
		//if(str==NULL) return 0;
		process(buf);
		addNode(buf);

	}
	return 0;
}

//选择2对应的功能函数
void function2()
{
	N temp=NULL;
	char section[50];
	while(1)
	{
		printf("请输入字段信息\n");
		scanf("%s",section);
		temp=findSection(s_pListHead,section);
		if(temp==NULL)
		{
			printf("查询字段失败，请检查输入是否正确\n");
			continue;
		}
		displaySection(temp);
		break;
	}
	
}

//选择3对应功能函数
void function3()
{
	N temp=s_pListHead;
	char section[50];
	while(1)
	{
		s_pListHead==temp;
		printf("请输入字段信息\n");
		scanf("%s",section);
		s_pListHead=removeSection(s_pListHead,section);
		if(s_pListHead==NULL)
		{
			printf("字段输入有误\n");
			continue;
		}
		update(s_pListHead,target);
		break;
	}
}

//选择4对应的功能函数
void function4()
{
	K rt=NULL;
	N temp=NULL;
	char section[50];
	char key[100];
	while(1)
	{
		printf("请输入字段信息\n");
		scanf("%s",section);
		temp=findSection(s_pListHead,section);
		if(temp==NULL)
		{
			printf("查询字段失败，请检查输入是否正确\n");
			continue;
		}
		
		//rt=temp->keyHead;
		if(temp->keyHead==NULL)
		{
			printf("当前字段无key\n");
			break;
		}
		while(1)
		{
			//temp->keyHead=rt;
			printf("请输入需要删除的key\n");
			scanf("%s",key);
			if(!strcmp(temp->keyHead->key,key))
			{
				rt=temp->keyHead;
				temp->keyHead=temp->keyHead->next;
				free(rt);
				break;
			}
			temp->keyTail=removeKey(temp->keyHead,temp->keyTail,key);
			if(temp->keyTail==NULL)
			{
				printf("key输入有误\n");
				continue;
			}
			update(s_pListHead,target);
			break;
		}
		break;
	}
}

//选择5对应函数功能
void function5()
{
	int rt;
	N temp=NULL;
	char section[50];
	char key[100];
	while(1)
	{
		printf("请输入字段信息\n");
		scanf("%s",section);
		temp=findSection(s_pListHead,section);
		if(temp==NULL)
		{
			printf("查询字段失败，请检查输入是否正确\n");
			continue;
		}
		
		while(1)
		{
			printf("请输入需要修改的key\n");
			scanf("%s",key);
			rt=changeKeyValue(temp->keyHead,key);
			if(rt==-1)
			{
				printf("字段输入有误\n");
				continue;
			}
			update(s_pListHead,target);
			break;
		}
		break;
	}
}

//选择6对应的功能函数
void function6()
{
	N rt=NULL;
	char section[50];
	while(1)
	{
		printf("请输入需要添加字段\n");
		scanf("%s",section);
		rt=findSection(s_pListHead,section);
		if(rt!=NULL)
		{
			printf("当前字段已存在\n");
			continue;
		}
		N newNode=createListNode(section);
		if(s_pListHead==NULL)
		{
		s_pListHead=newNode;
		}
		else
		{
		insrtSectionToTail(s_pListTail,newNode);
		
		}
		s_pListTail=newNode;
		printf("添加字段成功\n");
		update(s_pListHead,target);
		break;
	}

}

//选择7对应功能函数
void function7()
{
	N temp=NULL;
	char section[50];
	char key[100];
	char value[300];
	while(1)
	{
		printf("请输入字段信息\n");
		scanf("%s",section);
		temp=findSection(s_pListHead,section);
		if(temp==NULL)
		{
			printf("查询字段失败，请检查输入是否正确\n");
			continue;
		}
		
		while(1)
		{
			printf("请输入需要添加的key\n");
			scanf("%s",key);
			if(!findKey(temp->keyHead,key))
			{
				printf("当前key值已存在，请重新输入\n");
				continue;
			}
			printf("请输入对应value\n");
			scanf("%s",value);
			K newKey=createKeyNode(key,value);
			if(temp->keyHead==NULL)
			{
				temp->keyHead=newKey;
			}
			else
			{
				insrtKeyToTail(temp->keyTail,newKey);
			}
			temp->keyTail=newKey;
			update(s_pListHead,target);
			break;
		}
		break;
	}
}
//界面
void menu()
{
	int choose;
	char section[50];
	char key[100];
	printf("************请输入你的选择****************\n");
	printf("1.显示全部字段信息\n");
	printf("2.显示选择字段信息\n");
	printf("3.删除某字段\n");
	printf("4.删除某字段下的key\n");
	printf("5.改变某字段下的key-value值\n");
	printf("6.添加字段\n");
	printf("7.添加字段下key\n");
	printf("0.退出程序\n");
	printf("------------------------------------------\n");
	scanf("%d",&choose);
	switch(choose)
	{
		default:break;
		case 1:display_all(s_pListHead);break;
		case 2:function2();break;
		case 3:function3();break;
		case 4:function4();break;
		case 5:function5();break;
		case 6:function6();break;
		case 7:function7();break;
		case 0:clean_all(s_pListHead);exit(0);
		
	}
	
}

int main(int argc,char *argv[])
{
	FILE * fp;
	strncpy(target,argv[1],50);
	if(ifIsIni(target))
	{
		printf("文件类型不匹配或者文件名有误\n");
		return -1;
	}
	
	
		//clean_all(s_pListHead);
		fp=fopen(target,"r");
		if(fp==NULL)
		{
			printf("读文件打开失败，请检查文件名或路径是否正确\n");
			return -1;
		}
		readFile(fp);
		fclose(fp);
		//printf("读取文件完成\n");
	while(1)
	{	
		menu();
	}

	return 0;
}
