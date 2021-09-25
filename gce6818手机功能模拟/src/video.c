#include "video.h"

void video_play()
{
	printf("�ɹ�����Ƶ��������\n");
	show_bmp(0,0,"./black/welcome.bmp");
	system("mplayer -quiet -loop 0 -x 400 -y 240 -Zoom ./avi/mn.avi  &");

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


	struct node *list = list_init("mn.avi");						//��ʼ��˫��ѭ������
	char *path = "avi";										//mp3Ϊ����Ŀ¼�ļ���
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
