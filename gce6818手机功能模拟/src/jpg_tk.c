#include "jpg_tk.h"

void jpg_tk(void)
{
	//设置节点
	struct node
	{
		char *data;
		struct node *next;
		struct node *prev;
	};
	
	//初始化链表
	struct node *list_init(char *newdata)
	{
		struct node *head = malloc(sizeof(struct node));
		head->data = newdata;
		head->next = head;
		head->prev = head;
		return head;
	}
	
	//创建新节点
	struct node *newnode(char *newdata)
	{
		struct node *new = malloc(sizeof(struct node));
		new->data = newdata;
		new->next = NULL;
		new->prev = NULL;
	}
	
	//加入新节点
	int addnode(struct node *new,struct node *list)
	{
		struct node *p = list;
		while(p->next != list)
		{
			p = p->next;
		}
		new->prev = list->prev;
		new->next = list;
		list->prev = new;
		new->prev->next = new;
	}


	struct node *list = list_init("1.jpg");						//初始化双向循环链表
	char *path = "jpg";										//mp3为音乐目录文件名
	DIR *dp = opendir(path);
	struct dirent *p;
	while(p = readdir(dp))										//遍历目录文件
	{
		if(p->d_type == DT_REG)
		{
			if(strstr(p->d_name,".jpg")) 						//判断是否为.mp3文件
			{
				struct node *new = newnode(p->d_name); 			//创建新节点
				addnode(new,list);								//插入新节点
			}				
		}
	}
	char buf[20];												//用于存放拼接后的字符串
	struct node *head = list->next;
	
	int x,y=-1;
	int ret = 0;
	sprintf(buf,"%s/%s",path,head->data);
	lcd_draw_jpg(0,0,buf,NULL,0,0);
	while(1)
	{
		ret = get_two_xy();
		if(ret == 1)//向左滑,上一张
		{
			head = head->prev;
			sprintf(buf,"%s/%s",path,head->data);
			lcd_draw_jpg(0,0,buf,NULL,0,0);
			ret=0;
		}
		if(ret == 2)//向右滑，下一张
		{
			head = head->next;
			sprintf(buf,"%s/%s",path,head->data);
			lcd_draw_jpg(0,0,buf,NULL,0,0);
			ret=0;
		}
		if(ret == 3)//向上滑,退出
		{
			break;
		}
	}
}