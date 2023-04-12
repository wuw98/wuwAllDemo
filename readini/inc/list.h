#ifndef  __LIST_H
#define __LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//定义key--value结构体
typedef struct keylist
{
	char key[100];
	char value[300];
	struct keylist* next;
}Klist,*K;

//定义节点结构体
typedef struct nodelist
{
	char section[50];
	K keyHead;
	struct nodelist* next;
}Nlist,*N;

//创建键值节点
K createKeyNode(char *key,char *value);
//创建字段节点
N createListNode(char *section);
 //插入键到链表
void insrtKeyToTail(K head,K node);
 //插入字段到链表
void insrtSectionToTail(N head,N node);
//从链表中移除某字段
N removeSection(N head,char *section);
//从链表中移除某键值
K removeKey(K head,char *key);
//查找某键值
int findKey(K head,char *key);
//改变键值
int changeKeyValue(K head,char *changeKey);
//打印函数
void display_all(N head);
N findSection(N head,char *section);
void displaySection(N node);
void update(N head,char *filename);
void clean_all(N head);
#endif