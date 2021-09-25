#include "music.h"

void music_play()
{
	printf("成功打开音乐播放器！\n");
	show_bmp(0,0,"./black/music_play.bmp");//显示音乐背景图片
	//system("madplay  mp3/*.mp3 -z &");  					//列表随机播放
	system("madplay  mp3/*.mp3 -r &");  						//列表循环播放
	
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


	struct node *list = list_init("1.mp3");						//初始化双向循环链表
	char *path = "mp3";										//mp3为音乐目录文件名
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
	
	int flag=0;
	int dir_flag = 0;
	while(1)
	{
		int x=0,y=0;
		get_xy(&x,&y);
		if((x>364&&x<435)&&(y>358&&y<431)&&flag==0)//暂停
		{
			x=0,y=0;
			show_bmp(0,0,"./black/music.bmp");//显示音乐背景图片 》图
			system("killall -STOP madplay &");
			flag = 1;
		}
		if((x>364&&x<435)&&(y>358&&y<431)&&flag==1)//继续
		{
			x=0,y=0;
			show_bmp(0,0,"./black/music_play.bmp");//显示音乐背景图片  ||图
			system("killall -CONT madplay &");
			flag = 0;
		}
		if((x>199&&x<255)&&(y>375&&y<432))//点击了上一首
		{
			x=0,y=0;
			system("killall -9 madplay");
			show_bmp(0,0,"./black/music_play.bmp");//显示音乐背景图片  ||图
			head = head->prev;
			bzero(buf,20);
			sprintf(buf,"madplay %s/%s &",path,head->data);
			system(buf);
		}
		if((x>544&&x<600)&&(y>357&&y<431))//点击了下一首
		{
			x=0,y=0;
			system("killall -9 madplay");
			show_bmp(0,0,"./black/music_play.bmp");//显示音乐背景图片  ||图
			head = head->next;
			bzero(buf,20);
			sprintf(buf,"madplay %s/%s &",path,head->data);
			system(buf);
		}
		if((x>703&&x<750)&&(y>381&&y<431))//点击了退出
		{
			x=0,y=0;
			system("killall -9 madplay");
			printf("成功退出音乐播放器！\n");
			break;
		}
		if((x>64&&x<110)&&(y>383&&y<420) && dir_flag == 0 && flag == 0)
		{
			x=0,y=0;
			show_bmp(0,0,"./black/music_play.bmp");//显示音乐背景图片
			dirsearch();//显示目录
			dir_flag = 1;
			
			//没点暂停，在播放中,显示||图
		}
		if((x>64&&x<110)&&(y>383&&y<420) && dir_flag == 0 && flag == 1)
		{
			x=0,y=0;
			show_bmp(0,0,"./black/music.bmp");//显示音乐背景图片
			dirsearch();//显示目录
			dir_flag = 1;
			
			//点了暂停，故显示 》图
			
		}
		
		if((x>64&&x<110)&&(y>383&&y<420) && dir_flag == 1 && flag == 0)
		{
			x=0,y=0;
			show_bmp(0,0,"./black/music_play.bmp");//显示音乐背景图片
			dir_flag = 0;
			
			//没点暂停，在播放中,显示||图
		}
		if((x>64&&x<110)&&(y>383&&y<420) && dir_flag == 1 && flag == 1)
		{
			x=0,y=0;
			show_bmp(0,0,"./black/music.bmp");//显示音乐背景图片
			dir_flag = 0;
			
			//点了暂停，故显示 》图
			
		}
		
	}
}
	
