#include "music.h"

void music_play()
{
	printf("�ɹ������ֲ�������\n");
	show_bmp(0,0,"./black/music_play.bmp");//��ʾ���ֱ���ͼƬ
	//system("madplay  mp3/*.mp3 -z &");  					//�б��������
	system("madplay  mp3/*.mp3 -r &");  						//�б�ѭ������
	
	//���ýڵ�
	struct node
	{
		char *data;
		struct node *next;
		struct node *prev;
	};
	
	//��ʼ������
	struct node *list_init(char *newdata)
	{
		struct node *head = malloc(sizeof(struct node));
		head->data = newdata;
		head->next = head;
		head->prev = head;
		return head;
	}
	
	//�����½ڵ�
	struct node *newnode(char *newdata)
	{
		struct node *new = malloc(sizeof(struct node));
		new->data = newdata;
		new->next = NULL;
		new->prev = NULL;
	}
	
	//�����½ڵ�
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


	struct node *list = list_init("1.mp3");						//��ʼ��˫��ѭ������
	char *path = "mp3";										//mp3Ϊ����Ŀ¼�ļ���
	DIR *dp = opendir(path);
	struct dirent *p;
	
	while(p = readdir(dp))										//����Ŀ¼�ļ�
	{
		if(p->d_type == DT_REG)
		{
			if(strstr(p->d_name,".mp3")) 						//�ж��Ƿ�Ϊ.mp3�ļ�
			{
				struct node *new = newnode(p->d_name); 			//�����½ڵ�
				addnode(new,list);								//�����½ڵ�
			}				
		}
	}
	
	char buf[20];												//���ڴ��ƴ�Ӻ���ַ���
	struct node *head = list->next;
	
	int flag=0;
	int dir_flag = 0;
	while(1)
	{
		int x=0,y=0;
		get_xy(&x,&y);
		if((x>364&&x<435)&&(y>358&&y<431)&&flag==0)//��ͣ
		{
			x=0,y=0;
			show_bmp(0,0,"./black/music.bmp");//��ʾ���ֱ���ͼƬ ��ͼ
			system("killall -STOP madplay &");
			flag = 1;
		}
		if((x>364&&x<435)&&(y>358&&y<431)&&flag==1)//����
		{
			x=0,y=0;
			show_bmp(0,0,"./black/music_play.bmp");//��ʾ���ֱ���ͼƬ  ||ͼ
			system("killall -CONT madplay &");
			flag = 0;
		}
		if((x>199&&x<255)&&(y>375&&y<432))//�������һ��
		{
			x=0,y=0;
			system("killall -9 madplay");
			show_bmp(0,0,"./black/music_play.bmp");//��ʾ���ֱ���ͼƬ  ||ͼ
			head = head->prev;
			bzero(buf,20);
			sprintf(buf,"madplay %s/%s &",path,head->data);
			system(buf);
		}
		if((x>544&&x<600)&&(y>357&&y<431))//�������һ��
		{
			x=0,y=0;
			system("killall -9 madplay");
			show_bmp(0,0,"./black/music_play.bmp");//��ʾ���ֱ���ͼƬ  ||ͼ
			head = head->next;
			bzero(buf,20);
			sprintf(buf,"madplay %s/%s &",path,head->data);
			system(buf);
		}
		if((x>703&&x<750)&&(y>381&&y<431))//������˳�
		{
			x=0,y=0;
			system("killall -9 madplay");
			printf("�ɹ��˳����ֲ�������\n");
			break;
		}
		if((x>64&&x<110)&&(y>383&&y<420) && dir_flag == 0 && flag == 0)
		{
			x=0,y=0;
			show_bmp(0,0,"./black/music_play.bmp");//��ʾ���ֱ���ͼƬ
			dirsearch();//��ʾĿ¼
			dir_flag = 1;
			
			//û����ͣ���ڲ�����,��ʾ||ͼ
		}
		if((x>64&&x<110)&&(y>383&&y<420) && dir_flag == 0 && flag == 1)
		{
			x=0,y=0;
			show_bmp(0,0,"./black/music.bmp");//��ʾ���ֱ���ͼƬ
			dirsearch();//��ʾĿ¼
			dir_flag = 1;
			
			//������ͣ������ʾ ��ͼ
			
		}
		
		if((x>64&&x<110)&&(y>383&&y<420) && dir_flag == 1 && flag == 0)
		{
			x=0,y=0;
			show_bmp(0,0,"./black/music_play.bmp");//��ʾ���ֱ���ͼƬ
			dir_flag = 0;
			
			//û����ͣ���ڲ�����,��ʾ||ͼ
		}
		if((x>64&&x<110)&&(y>383&&y<420) && dir_flag == 1 && flag == 1)
		{
			x=0,y=0;
			show_bmp(0,0,"./black/music.bmp");//��ʾ���ֱ���ͼƬ
			dir_flag = 0;
			
			//������ͣ������ʾ ��ͼ
			
		}
		
	}
}
	
