#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <linux/input.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include "font.h"
#define MYFILE  "cai_fp.txt"
void get_area();

int *p;   //�ڴ�ָ��
int lcd_fd;  //��ʾ��
int ts_fd;   //������
int allsum=0;  //ȫ�ֱ���ʵ��һ���µ�����ܼ۸� 

struct caid
{
	char *name; //����
    int num; //����
	int price; //����
	int sum; //����*����
};

struct caid caidan[] = {
	//�Լ������Ĳ�Ʒ����   ---���ӣ����԰Ѳ�����ʾ�ڶ�Ӧ�˵�ͼƬ��  ��
{"����Ѽ��",0,35,0},
{"����ţ��",0,39,0},
{"�����廨",0,45,0},
{"����ë��",0,12,0},
{"�����ȱ�",0,36,0},
{"���ӻ���",0,24,0},
{"��������",0,29,0},
{"��������",0,30,0},
{"�彷��ͷ",0,32,0},
};

//========�ڴ�ӳ��,��ʼ��/dev/fb0 ��ʾ��==================
void lcd_init(void)
{
	//��LCD
	lcd_fd = open("/dev/fb0",O_RDWR);
	if(lcd_fd < 0)
	{
		perror("open lcd failed");
		return;
	}
	//�ڴ�ӳ��
	p = mmap(NULL,800*480*4,PROT_READ | PROT_WRITE,MAP_SHARED,lcd_fd,0);
	if(p == NULL)   //ӳ��ʧ��
	{
		perror("mmap() faield");
		return;
	}
}

//========��ʼ��/dev/input/event0 ������==================
//��ʼ��������
void ts_init(void)
{
	ts_fd = open("/dev/input/event0",O_RDWR);
	if(ts_fd < 0)
	{
		perror("open ts failed");
		return;
	}
}
//========��ȡ����==================
void get_pos(int *x,int *y)
{
	//��ȡ����������
	struct input_event buf;  //�������ṹ�����
	while(1)
	{
		read(ts_fd,&buf,sizeof(buf));  //(100,200)
		if(buf.type == EV_ABS)		//�ж��Ǵ������¼�
		{
			if(buf.code == ABS_X)   		//X��
			{
				*x = buf.value;
				
			}
			else if(buf.code == ABS_Y)		//Y��
			{
				*y = buf.value;
				
			}
		}
		//�ж���ָ�ɿ�
		if(buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value == 0)
			break;
	}
}

//========��ʾbmpͼƬ==================
//��ʾbmpͼƬ
void show_bmp(char *bmp_name)
{
	//��bmp
	int bmp_fd = open(bmp_name,O_RDWR);
	if(bmp_fd < 0)
	{
		perror("open bmp failed");
		return;
	}
	//ƫ��54�ֽ�
	lseek(bmp_fd,54,SEEK_SET);
	//��ȡͼƬ����
	char bmp_buf[800*480*3];
	read(bmp_fd,bmp_buf,800*480*3);
	
	int x,y,color;
	char b,g,r;
	int i = 0;
	for(y=0;y<480;y++)
	{
		for(x=0;x<800;x++)
		{            
			b = bmp_buf[i++];
			g = bmp_buf[i++];
			r = bmp_buf[i++];
			color = b  |  g<<8 |  r<<16;
			*(p+800*(479-y) + x) = color;
		}
	}
	close(bmp_fd);
}

//========����video���=================
//���Ź��
void video()
{
	//���Ź��
	system("mplayer -quiet -loop 0  -geometry 16:17 -x \
	207 -y 342 -Zoom food.avi  &");  
	sleep(1);
	system("clear");
}

//========ʵ��int����ת��Ϊ�ַ�������==================
#define MAX 100
//int ת string
char *simple_itoa(int value)
{
	char *string = malloc(MAX);  //��̬�����ڴ�
	int i = 0, j = 0;	
	int len = 0;

	while (value != 0) {  
	char c = value % 10 + '0';  //ÿһλת���ַ�
	string[i++] = c;  // �ַ��Ž��ַ���(�ַ�����)
	value /= 10;
	}

	string[i] = '\0';
	len = strlen(string);

	for(j=0; j<len/2; j++) {//�ַ�������
	char temp = string[j];
	string[j] = string[len-j-1];
	string[len-j-1] = temp;
	}
	return string;  //�����ַ���
}
/*
1��ʵ��bmpͼƬ���в�Ʒ����ʾ  ��
2����������ʹ�ô��������е�ͣ����ӡ����ٲ�Ʒ����ʾ������   ---����ɼӼ���������ʾ���� ��
3�������������Ź�棨ָ��λ�ã�ָ����С��ѭ�����ţ� ��
4��������µ���֮����ʾ���ﳵ�Ĳ�Ʒ���ۣ���������Ʒ�ܼ��Լ����в�Ʒ�ܼ�Ǯ�����˻ؼ������  ---��
5������ѡ��֧����ʽ  ��
6��֧���ɹ����ӡСƱ�����ļ�����ʽ����Ʒ���ۡ�������������Ʒ�ܼ��Լ����в�Ʒ�ܼۣ���
7�����ӹ��ܣ��ӷ��
	�� ��ҳ��ʾ�����������ɫ����
	....................
*/

//========ʹ��Clean_Area()��Display_characterX()����=================
//========ʵ�������ʾ������ʾ=================
void show_Aarea(){
	/*
	int   Clean_Area(int X,  //x������ʼ��
                 int Y,  //y������ʼ��
				 int width, //���ƵĿ��
				 int height,//���Ƶĸ߶�
				 unsigned long color); //����Ļָ�����������ɫ
	*/
	//�õ���ʾ���ݲ���ȫ��
	
	/*
	int   Display_characterX(unsigned int x,          //x������ʼ��
                         unsigned int y,          //y������ʼ��
						 unsigned char *string,   //GB2312 �����ַ���
						 unsigned int color ,     //������ɫֵ
						 int size);               //����Ŵ��� 1~8
	*/			 
	Clean_Area(314,120,38,38,0x00FFFFFF);
	Display_characterX(312,120,simple_itoa(caidan[0].num),0x00000000,2);
	//system("clear");
	
	Clean_Area(500,120,38,38,0x00FFFFFF);
	Display_characterX(500,120,simple_itoa(caidan[1].num),0x00000000,2);
	//system("clear");
	
	Clean_Area(690,120,38,38,0x00FFFFFF);
	Display_characterX(690,120,simple_itoa(caidan[2].num),0x00000000,2);
	//system("clear");
	
	Clean_Area(314,280,38,38,0x00FFFFFF);
	Display_characterX(314,280,simple_itoa(caidan[3].num),0x00000000,2);
	//system("clear");
	
	Clean_Area(500,280,38,38,0x00FFFFFF);
	Display_characterX(500,280,simple_itoa(caidan[4].num),0x00000000,2);
	//system("clear");
	
	Clean_Area(690,280,38,38,0x00FFFFFF);
	Display_characterX(690,280,simple_itoa(caidan[5].num),0x00000000,2);
	//system("clear");
	
	Clean_Area(314,435,38,38,0x00FFFFFF);
	Display_characterX(314,435,simple_itoa(caidan[6].num),0x00000000,2);
	//system("clear");
	
	Clean_Area(500,435,38,38,0x00FFFFFF);
	Display_characterX(500,435,simple_itoa(caidan[7].num),0x00000000,2);
	//system("clear");
	
	Clean_Area(690,435,38,38,0x00FFFFFF);
	Display_characterX(690,435,simple_itoa(caidan[8].num),0x00000000,2);
	//system("clear");
	
}


//��ʾ���ﳵ�Ĳ�Ʒ���ۣ���������Ʒ�ܼ��Լ����в�Ʒ�ܼ�Ǯ===================
void caidan_display()
{
	int i;
	unsigned int j=0;
	const int k = 45;
	unsigned char str[20];
	unsigned char *p; 
	Display_characterX(0,j,"===��ӭ����,������===",0x00000000,2);
	Display_characterX(0,j+=k,"���� ���� ���� �ܼ�",0x00000000,2);
	for(i = 0;i<9;i++)
	{
		p = str;
		if(caidan[i].num != 0)
		{
			caidan[i].sum = caidan[i].num * caidan[i].price;
			sprintf(str,"%s %d %d %d",caidan[i].name,caidan[i].num,caidan[i].price,caidan[i].sum);
			allsum +=caidan[i].sum;
			Display_characterX(0,j+=k,str,0x00000000,2);
		}	
		p = NULL;
	}
	Display_characterX(0,j+=k,"����",0x00000000,2);
	Display_characterX(100,j,simple_itoa(allsum),0x00000000,2);
	Display_characterX(0,j+=k,"===���������ĵ��===",0x00000000,2);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++


void CancelOrder()
{
	allsum = 0;//ȡ�����ܼ۸�����
	//ʵ�ֵ㵥ȡ��������㵥--------
	while(1)
	{
		//��ʾ������
		show_bmp("menu.bmp");   
		//���Ź��
		video();
		//��ȡ�Ӽ����š�λ��ʵ�ֵ�ͼӼ�
		get_area();
	}
}

void myprintf()
{
	//��ӡ�ļ�����  
	//�ն���
	int i;
	printf("===��ӭ����,������===\n");
	printf("���� ���� ���� �ܼ�\n");
	for(i = 0;i<9;i++)
	{
		if(caidan[i].num != 0)
		{
			printf("%8s  %d %d %d\n",caidan[i].name,caidan[i].num,caidan[i].price,caidan[i].sum);
		}
	}
	printf("�ܼ�%d\t\n",allsum);
	printf("===���������ĵ��===\n");
	printf("OK��\n");
}

void filePrint()
{
	//�����ļ� O_TRUNC:�ļ�����
	int fd_w = open(MYFILE,O_RDWR|O_CREAT|O_TRUNC , 0664);
	if(fd_w == -1)
	{
		printf("open error\n");
		return;
	}
	unsigned char str[20]={"\0"};  //��ʼ��Ϊ���ַ���
	unsigned char *p = str;
	int len=0;
	int i;

	for(i = 0;i<9;i++)
	{
		p = str;
		if(caidan[i].num != 0)
		{
			len = sprintf(str,"%8s %d %d %d\n",caidan[i].name,caidan[i].num,caidan[i].price,caidan[i].sum);
			write(fd_w, p, len);
			memset(str, 0x00, sizeof (unsigned char) * 20); 
		}
		p = NULL;
	}
	p = str;
	unsigned char zongjia[] = {"�ܼ�\t"};
	len = sprintf(str,"�ܼ�\t %d\n",allsum);
	write(fd_w, p,len);
	write(fd_w,"===���������ĵ��==='0x0A'",20);
	close(fd_w);
	printf("file print is OK!\n");
}

void MyOff()
{
	int i;
	//��λ-------------------һ�ε�ͽ���,��λ�������ȫ�ֱ���----------------
	for(i = 0;i<9;i++)
	{
		caidan[i].num = 0;
		caidan[i].sum = 0;
		allsum = 0; 
	}
	CancelOrder();
}
void display_xiaopiao()
{
	int x,y;
	get_pos(&x,&y);
	//ɨ��֧��������ӡСƱ--------------
	//������Ļ�����СƱ��Ϣ֮���ٴ�ӡ���ļ�--------------
	if(x>322 && x<672 && y>498&&y<594)
	{
		//��ӡ�ļ�����  
		//�ն���
		myprintf();
		
		//��ӡ���ļ�
		filePrint();
		
		sleep(5);
		//��λ
		MyOff();
	}
	else
	{
		;
	}
}

void XiaDan()
{
	int x,y;
	//�µ�����ʾ
	while(1){
		//��תҳ��		�µ�����ת����ʾ���ﳵ����-------------------------------------------
		system("killall  -SIGSTOP  mplayer");
		Clean_Area(0,0,800,480,0x00FFFFFF);
		show_bmp("ack.bmp");
		caidan_display();
		get_pos(&x,&y);	
		//���ﳵ����get_pos()---------------------
		if(x > 850 && x < 1024 && y > 0 && y < 300)
		{//���ȷ��---------------
			//��ת��֧������
			//system("clear");
			//��תҳ��		
			Clean_Area(0,0,800,480,0x00FFFFFF);
			show_bmp("zhifu.bmp");
			display_xiaopiao();
		}
		//�µ�������ȡ��,ʵ�ַ��ص�ͽ���,�Ѿ����˵Ĳ�Ʒ��ȡ��------
		else if(x > 850 && x < 1024 && y > 300 && y < 480)
		{
			//ȡ��
			system("clear");		
			//��תҳ��		
			CancelOrder();
		}
		else 
		{
			;
		}
	}	
}

//�õ��Ӽ�����ֵ
void get_area()
{
	int x,y;
	while(1)
	{
		show_Aarea();
		get_pos(&x,&y);
		//ͼƬ���ż���
		//a[0]����
		if(x > (314-3) && x < (352+3) && y > (154-2) && y < (192+2))//a[0] -
		{
			//��Ӧ����ֵ��1
			if(caidan[0].num != 0)
			{
				caidan[0].num -=1;
			}
		}
		if(x > (491-3) && x < (529+3) && y > 154 && y < 192)//a[0] +
		{
			//��Ӧ����ֵ��1
			caidan[0].num +=1;
		}
		
		//a[1]����
		if(x > (546-3) && x < (584+3) && y > 157 && y < 195)//a[1] -
		{
			//��Ӧ����ֵ��1
			if(caidan[1].num != 0)
			{
				caidan[1].num -=1;
			}
		}
		if(x > (733-3) && x < (771+3) && y > 154 && y < 192)//a[1] +
		{
			//��Ӧ����ֵ��1
				caidan[1].num +=1;
		}
		
		
		//a[2]����
		if(x > (784-3) && x < (822+3) && y > 154 && y < 192)//a[2] -
		{
			//��Ӧ����ֵ��1
			if(caidan[2].num != 0)
			{
				caidan[2].num -=1;
			}
		}
		if(x > (981-3) && x < 1025 && y > 151 && y < 189)//a[2] +
		{
			//��Ӧ����ֵ��1
				caidan[2].num +=1;
		}
		
		
		//a[3]����
		if(x > (314-3) && x < (352+3) && y > 357 && y < 395)//a[3] -
		{
			//��Ӧ����ֵ��1
			if(caidan[3].num != 0)
			{
				caidan[3].num -=1;
			}
			
		}
		if(x > (495-3) && x < (533+3) && y > 358 && y < 396)//a[3] +
		{
			//��Ӧ����ֵ��1
				caidan[3].num +=1;
				
		}
		
		
		//a[4]����
		if(x > (545-3) && x < (583+3) && y > 360 && y < 398)//a[4] -
		{
			//��Ӧ����ֵ��1
			if(caidan[4].num != 0)
			{
				caidan[4].num -=1;
			}
			
		}
		if(x > (730-3) && x < (768+3) && y > 360 && y < 398)//a[4] +
		{
			//��Ӧ����ֵ��1
				caidan[4].num +=1;
		}
		
		
		//a[5]����
		if(x > (784-3) && x < (822+3) && y > 363 && y < 401)//a[5] -
		{
			//��Ӧ����ֵ��1
			if(caidan[5].num != 0)
			{
				caidan[5].num -=1;
			}
		}
		if(x > (976-3) && x < 1025 && y > 360 && y < 398)//a[5] +
		{   //1022
			//��Ӧ����ֵ��1
				caidan[5].num +=1;
		}
		
		
		//a[6]����
		if(x > (317-3) && x < (355+3) && y > 554 && y < 592)//a[6] -
		{
			//��Ӧ����ֵ��1
			if(caidan[6].num != 0)
			{
				caidan[6].num -=1;
			}
		}
		if(x > (495-3) && x < (533+3) && y > 556 && y < 594)//a[6] +
		{
			//��Ӧ����ֵ��1
				caidan[6].num +=1;
		}

		//a[7]����
		if(x > (546-3) && x < (584+3) && y > 558 && y < 596)//a[7] -
		{
			//��Ӧ����ֵ��1
			if(caidan[7].num != 0)
			{
				caidan[7].num -=1;
			}
		}
		if(x > (733-3) && x < (771+3) && y > 560 && y < 598)//a[7] +
		{
			//��Ӧ����ֵ��1
				caidan[7].num +=1;
		}
		
		
		//a[8]����
		if(x > (784-3) && x < (822+3) && y > 555 && y < 593)//a[8] -
		{
			//��Ӧ����ֵ��1
			if(caidan[8].num != 0)
			{
				caidan[8].num -=1;
			}
		}
		if(x > (981-3) && x < 1025 && y > 554 && y < 592)//a[8] +
		{
			//��Ӧ����ֵ��1
				caidan[8].num +=1;
				//printf("��һ�������ֵa[8] = %d \n",caidan[8].num);
		}
	

		//�µ���
		if(x > 26 && x < 275 && y > 475 && y < 602)
		{
			XiaDan();
		}
		
		else 
		{
			;
		}
	}
}


int main(void)
{
	
	//��ʼ��lcd
	lcd_init();
	//��ʼ��������
	ts_init();
	//��ʼ���ֿ�
	Init_Font();
	//��ʾ������
	show_bmp("menu.bmp");   
	//���Ź��
	video();
	//��ȡ�Ӽ����š�λ��ʵ�ֵ�ͼӼ�
	get_area();
	//�ر�lcd
	close(lcd_fd);
	//�ر�ts
	close(ts_fd);
	//�ر��ֿ�
	UnInit_Font();
	return 0;
}