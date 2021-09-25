#include "video.h"

void video_play()
{
	printf("成功打开视频播放器！\n");
	show_bmp(0,0,"./black/welcome.bmp");
	system("mplayer -quiet -loop 0 -x 400 -y 240 -Zoom ./avi/mn.avi  &");

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


	struct node *list = list_init("mn.avi");						//初始化双向循环链表
	char *path = "avi";										//mp3为音乐目录文件名
	DIR *dp = opendir(path);
	struct dirent *p;
	
	while(p = readdir(dp))										//遍历目录文件
	{
		if(p->d_type == DT_REG)
		{
			if(strstr(p->d_name,".mp3")) 						//判断是否为.mp3文件
			{
				struct node *new = newnode(p->d_name); 			//创建新节点
				addnode(new,list);								//插入新节点
			}				
		}
	}
	
	char buf[20];												//用于存放拼接后的字符串
	struct node *head = list->next;
	
	
	while(1)
	{
		int x=0,y=0;
		get_xy(&x,&y);
		if(x>0&&x<100)
		{
			system("killall -9 mplayer");
			system("mplayer  -vf scale=800:480  ./avi/mn.avi &"); 
			
		}
		if(x>700&&x<800)
		{
			system("killall -9 mplayer");
			system("clear");
			break;
		}
	}
	
	
}
