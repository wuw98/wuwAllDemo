#include "list.h"



//创建键值节点
K createKeyNode(char *key,char *value)
{
	K newNode=malloc(sizeof(Klist));
	newNode->next=NULL;
	strcpy(newNode->key,key);
	strcpy(newNode->value,value);
	return newNode;
}

//创建字段节点
N createListNode(char *section)
{
	N newNode=malloc(sizeof(Nlist));
	newNode->next=NULL;
	newNode->keyHead=NULL;
	strcpy(newNode->section,section);
	return newNode;
}
 
 
 //插入键到链表
void insrtKeyToTail(K head,K node)
{
	K ptr=head;

	while(ptr->next!=NULL)ptr=ptr->next;
	ptr->next=node;
}

 //插入字段到链表
void insrtSectionToTail(N head,N node)
{
	N ptr=head;
	while(ptr->next!=NULL)ptr=ptr->next;
	ptr->next=node;
}

//在字段中查找键值
int findKey(K head,char *key)
{
	K ptr=head;
	while(ptr!=NULL)
	{
		if(!strcmp(ptr->key,key))return 0;
		ptr=ptr->next;
	}
	return -1;
}
//从链表中移除某字段
N removeSection(N head,char *section)
{
	N ptr=head;
	if(!strcmp(head->section,section))
	{
		head=head->next;
		free(ptr);
		return head;
	}
	while(ptr!=NULL)
	{
		if(!strcmp(ptr->next->section,section))
		{
			N target=ptr->next;
			ptr->next=target->next;
			free(target);
			return head;
		}
		ptr=ptr->next;
	}
	return NULL;
}


//从链表中移除某键值
K removeKey(K head,char *key)
{
	K ptr=head;
	if(!strcmp(head->key,key))
	{
		head=head->next;
		free(ptr);
		return head;
	}
	while(ptr!=NULL)
	{
		if(!strcmp(ptr->next->key,key))
		{
			K target=ptr->next;
			ptr->next=target->next;
			free(target);
			return head;
		}
		ptr=ptr->next;
	}
	return NULL;
}

//改变键值
int changeKeyValue(K head,char *changeKey)
{
	K ptr=head;
	while(ptr!=NULL)
	{
		if(!strcmp(ptr->key,changeKey))
		{
			printf("请输入更新后的value值\n");
			scanf("%s",ptr->value);
			return 0;
		}
		ptr=ptr->next;	
	}
	return -1;
}

//查找字段
N findSection(N head,char *section)
{
	N ptr=head;
	while(ptr!=NULL)
	{
		if(!strcmp(ptr->section,section))
			return ptr;
		ptr=ptr->next;
	}
	return NULL;
}
//打印单个字段的内容
void displaySection(N node)
{
	K ptr=node->keyHead;
	printf("**************************************\n");
	printf("[%s]\n",node->section);
	while(ptr!=NULL)
	{
		printf("%s = %s\n",ptr->key,ptr->value);
		ptr=ptr->next;
	}
}

//打印函数
void display_all(N head)
{
	if(head==NULL)
	{
		printf("当前链表为空\n");
		return;
	}
	N ptr=head;
	while(ptr!=NULL)
	{
		//printf("222\n");
		displaySection(ptr);
		ptr=ptr->next;
	}
}

void update(N head,char *filename)
{
	FILE *fp=fopen(filename,"w+");
	N ptr=head;
	K try=NULL;
	while(ptr!=NULL)
	{
		try=ptr->keyHead;
		fprintf(fp,"[%s]\n",ptr->section);
		while(try!=NULL)
		{
			fprintf(fp,"%s=%s\n",try->key,try->value);
			try=try->next;
		}
		fprintf(fp,"\n");
		ptr=ptr->next;
	}
	fclose(fp);
}


void clean_all(N head)
{
	N ptr=head;
	N tempSec=NULL;
	K tempKey=NULL;
	while(ptr!=NULL)
	{
		while(ptr->keyHead!=NULL)
		{
			tempKey=ptr->keyHead;
			ptr->keyHead=ptr->keyHead->next;
			free(tempKey);
		}
		tempSec=ptr;
		ptr=ptr->next;
		free(tempSec);
	}
}