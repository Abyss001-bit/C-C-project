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

int *p;   //内存指针
int lcd_fd;  //显示屏
int ts_fd;   //触摸屏
int allsum=0;  //全局变量实现一次下单后的总价格 

struct caid
{
	char *name; //菜名
    int num; //份数
	int price; //单价
	int sum; //份数*单价
};

struct caid caidan[] = {
	//自己命名的菜品名字   ---附加，可以把菜名显示在对应菜的图片上  ×
{"绝望鸭脖",0,35,0},
{"大力牛排",0,39,0},
{"串烤五花",0,45,0},
{"冰镇毛肚",0,12,0},
{"香线扇贝",0,36,0},
{"大杂烩肉",0,24,0},
{"凉拌辣椒",0,29,0},
{"麻辣丸子",0,30,0},
{"跺椒猪头",0,32,0},
};

//========内存映射,初始化/dev/fb0 显示屏==================
void lcd_init(void)
{
	//打开LCD
	lcd_fd = open("/dev/fb0",O_RDWR);
	if(lcd_fd < 0)
	{
		perror("open lcd failed");
		return;
	}
	//内存映射
	p = mmap(NULL,800*480*4,PROT_READ | PROT_WRITE,MAP_SHARED,lcd_fd,0);
	if(p == NULL)   //映射失败
	{
		perror("mmap() faield");
		return;
	}
}

//========初始化/dev/input/event0 触摸屏==================
//初始化触摸屏
void ts_init(void)
{
	ts_fd = open("/dev/input/event0",O_RDWR);
	if(ts_fd < 0)
	{
		perror("open ts failed");
		return;
	}
}
//========获取坐标==================
void get_pos(int *x,int *y)
{
	//读取触摸屏数据
	struct input_event buf;  //触摸屏结构体变量
	while(1)
	{
		read(ts_fd,&buf,sizeof(buf));  //(100,200)
		if(buf.type == EV_ABS)		//判断是触摸屏事件
		{
			if(buf.code == ABS_X)   		//X轴
			{
				*x = buf.value;
				
			}
			else if(buf.code == ABS_Y)		//Y轴
			{
				*y = buf.value;
				
			}
		}
		//判断手指松开
		if(buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value == 0)
			break;
	}
}

//========显示bmp图片==================
//显示bmp图片
void show_bmp(char *bmp_name)
{
	//打开bmp
	int bmp_fd = open(bmp_name,O_RDWR);
	if(bmp_fd < 0)
	{
		perror("open bmp failed");
		return;
	}
	//偏移54字节
	lseek(bmp_fd,54,SEEK_SET);
	//读取图片数据
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

//========播放video广告=================
//播放广告
void video()
{
	//播放广告
	system("mplayer -quiet -loop 0  -geometry 16:17 -x \
	207 -y 342 -Zoom food.avi  &");  
	sleep(1);
	system("clear");
}

//========实现int类型转换为字符串类型==================
#define MAX 100
//int 转 string
char *simple_itoa(int value)
{
	char *string = malloc(MAX);  //动态分配内存
	int i = 0, j = 0;	
	int len = 0;

	while (value != 0) {  
	char c = value % 10 + '0';  //每一位转成字符
	string[i++] = c;  // 字符放进字符串(字符数组)
	value /= 10;
	}

	string[i] = '\0';
	len = strlen(string);

	for(j=0; j<len/2; j++) {//字符串倒序
	char temp = string[j];
	string[j] = string[len-j-1];
	string[len-j-1] = temp;
	}
	return string;  //返回字符串
}
/*
1、实现bmp图片进行菜品的显示  √
2、可以正常使用触摸屏进行点餐（增加、减少菜品，显示数量）   ---数组可加减数量，显示数量 √
3、可以正常播放广告（指定位置，指定大小，循环播放） √
4、点击“下单”之后，显示购物车的菜品单价，数量，菜品总价以及所有菜品总价钱，可退回继续点餐  ---√
5、可以选择支付方式  √
6、支付成功后打印小票，以文件的形式（菜品单价、数量、单个菜品总价以及所有菜品总价）√
7、附加功能（加分项）
	× 主页显示：进入界面颜色文字
	....................
*/

//========使用Clean_Area()、Display_characterX()函数=================
//========实现清空显示份数显示=================
void show_Aarea(){
	/*
	int   Clean_Area(int X,  //x坐标起始点
                 int Y,  //y坐标起始点
				 int width, //绘制的宽度
				 int height,//绘制的高度
				 unsigned long color); //往屏幕指定区域填充颜色
	*/
	//得到显示数据部分全白
	
	/*
	int   Display_characterX(unsigned int x,          //x坐标起始点
                         unsigned int y,          //y坐标起始点
						 unsigned char *string,   //GB2312 中文字符串
						 unsigned int color ,     //字体颜色值
						 int size);               //字体放大倍数 1~8
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


//显示购物车的菜品单价，数量，菜品总价以及所有菜品总价钱===================
void caidan_display()
{
	int i;
	unsigned int j=0;
	const int k = 45;
	unsigned char str[20];
	unsigned char *p; 
	Display_characterX(0,j,"===欢迎光临,点餐愉快===",0x00000000,2);
	Display_characterX(0,j+=k,"菜名 数量 单价 总价",0x00000000,2);
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
	Display_characterX(0,j+=k,"共计",0x00000000,2);
	Display_characterX(100,j,simple_itoa(allsum),0x00000000,2);
	Display_characterX(0,j+=k,"===以上是您的点餐===",0x00000000,2);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++


void CancelOrder()
{
	allsum = 0;//取消后总价格清零
	//实现点单取消后继续点单--------
	while(1)
	{
		//显示主界面
		show_bmp("menu.bmp");   
		//播放广告
		video();
		//获取加减“号”位置实现点餐加减
		get_area();
	}
}

void myprintf()
{
	//打印文件结束  
	//终端上
	int i;
	printf("===欢迎光临,点餐愉快===\n");
	printf("菜名 数量 单价 总价\n");
	for(i = 0;i<9;i++)
	{
		if(caidan[i].num != 0)
		{
			printf("%8s  %d %d %d\n",caidan[i].name,caidan[i].num,caidan[i].price,caidan[i].sum);
		}
	}
	printf("总计%d\t\n",allsum);
	printf("===以上是您的点餐===\n");
	printf("OK！\n");
}

void filePrint()
{
	//进入文件 O_TRUNC:文件清零
	int fd_w = open(MYFILE,O_RDWR|O_CREAT|O_TRUNC , 0664);
	if(fd_w == -1)
	{
		printf("open error\n");
		return;
	}
	unsigned char str[20]={"\0"};  //初始化为空字符串
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
	unsigned char zongjia[] = {"总价\t"};
	len = sprintf(str,"总价\t %d\n",allsum);
	write(fd_w, p,len);
	write(fd_w,"===以上是您的点餐==='0x0A'",20);
	close(fd_w);
	printf("file print is OK!\n");
}

void MyOff()
{
	int i;
	//复位-------------------一次点餐结束,复位置零各个全局变量----------------
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
	//扫码支付后点击打印小票--------------
	//先在屏幕上输出小票信息之后再打印进文件--------------
	if(x>322 && x<672 && y>498&&y<594)
	{
		//打印文件结束  
		//终端上
		myprintf();
		
		//打印进文件
		filePrint();
		
		sleep(5);
		//复位
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
	//下单后显示
	while(1){
		//跳转页面		下单后跳转到显示购物车界面-------------------------------------------
		system("killall  -SIGSTOP  mplayer");
		Clean_Area(0,0,800,480,0x00FFFFFF);
		show_bmp("ack.bmp");
		caidan_display();
		get_pos(&x,&y);	
		//购物车界面get_pos()---------------------
		if(x > 850 && x < 1024 && y > 0 && y < 300)
		{//点击确认---------------
			//跳转到支付界面
			//system("clear");
			//跳转页面		
			Clean_Area(0,0,800,480,0x00FFFFFF);
			show_bmp("zhifu.bmp");
			display_xiaopiao();
		}
		//下单界面点击取消,实现返回点餐界面,已经点了的菜品不取消------
		else if(x > 850 && x < 1024 && y > 300 && y < 480)
		{
			//取消
			system("clear");		
			//跳转页面		
			CancelOrder();
		}
		else 
		{
			;
		}
	}	
}

//得到加减区域值
void get_area()
{
	int x,y;
	while(1)
	{
		show_Aarea();
		get_pos(&x,&y);
		//图片横着计算
		//a[0]区域
		if(x > (314-3) && x < (352+3) && y > (154-2) && y < (192+2))//a[0] -
		{
			//对应数组值减1
			if(caidan[0].num != 0)
			{
				caidan[0].num -=1;
			}
		}
		if(x > (491-3) && x < (529+3) && y > 154 && y < 192)//a[0] +
		{
			//对应数组值加1
			caidan[0].num +=1;
		}
		
		//a[1]区域
		if(x > (546-3) && x < (584+3) && y > 157 && y < 195)//a[1] -
		{
			//对应数组值减1
			if(caidan[1].num != 0)
			{
				caidan[1].num -=1;
			}
		}
		if(x > (733-3) && x < (771+3) && y > 154 && y < 192)//a[1] +
		{
			//对应数组值加1
				caidan[1].num +=1;
		}
		
		
		//a[2]区域
		if(x > (784-3) && x < (822+3) && y > 154 && y < 192)//a[2] -
		{
			//对应数组值减1
			if(caidan[2].num != 0)
			{
				caidan[2].num -=1;
			}
		}
		if(x > (981-3) && x < 1025 && y > 151 && y < 189)//a[2] +
		{
			//对应数组值加1
				caidan[2].num +=1;
		}
		
		
		//a[3]区域
		if(x > (314-3) && x < (352+3) && y > 357 && y < 395)//a[3] -
		{
			//对应数组值减1
			if(caidan[3].num != 0)
			{
				caidan[3].num -=1;
			}
			
		}
		if(x > (495-3) && x < (533+3) && y > 358 && y < 396)//a[3] +
		{
			//对应数组值加1
				caidan[3].num +=1;
				
		}
		
		
		//a[4]区域
		if(x > (545-3) && x < (583+3) && y > 360 && y < 398)//a[4] -
		{
			//对应数组值减1
			if(caidan[4].num != 0)
			{
				caidan[4].num -=1;
			}
			
		}
		if(x > (730-3) && x < (768+3) && y > 360 && y < 398)//a[4] +
		{
			//对应数组值加1
				caidan[4].num +=1;
		}
		
		
		//a[5]区域
		if(x > (784-3) && x < (822+3) && y > 363 && y < 401)//a[5] -
		{
			//对应数组值减1
			if(caidan[5].num != 0)
			{
				caidan[5].num -=1;
			}
		}
		if(x > (976-3) && x < 1025 && y > 360 && y < 398)//a[5] +
		{   //1022
			//对应数组值加1
				caidan[5].num +=1;
		}
		
		
		//a[6]区域
		if(x > (317-3) && x < (355+3) && y > 554 && y < 592)//a[6] -
		{
			//对应数组值减1
			if(caidan[6].num != 0)
			{
				caidan[6].num -=1;
			}
		}
		if(x > (495-3) && x < (533+3) && y > 556 && y < 594)//a[6] +
		{
			//对应数组值加1
				caidan[6].num +=1;
		}

		//a[7]区域
		if(x > (546-3) && x < (584+3) && y > 558 && y < 596)//a[7] -
		{
			//对应数组值减1
			if(caidan[7].num != 0)
			{
				caidan[7].num -=1;
			}
		}
		if(x > (733-3) && x < (771+3) && y > 560 && y < 598)//a[7] +
		{
			//对应数组值加1
				caidan[7].num +=1;
		}
		
		
		//a[8]区域
		if(x > (784-3) && x < (822+3) && y > 555 && y < 593)//a[8] -
		{
			//对应数组值减1
			if(caidan[8].num != 0)
			{
				caidan[8].num -=1;
			}
		}
		if(x > (981-3) && x < 1025 && y > 554 && y < 592)//a[8] +
		{
			//对应数组值加1
				caidan[8].num +=1;
				//printf("第一个区域的值a[8] = %d \n",caidan[8].num);
		}
	

		//下单：
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
	
	//初始化lcd
	lcd_init();
	//初始化触摸屏
	ts_init();
	//初始化字库
	Init_Font();
	//显示主界面
	show_bmp("menu.bmp");   
	//播放广告
	video();
	//获取加减“号”位置实现点餐加减
	get_area();
	//关闭lcd
	close(lcd_fd);
	//关闭ts
	close(ts_fd);
	//关闭字库
	UnInit_Font();
	return 0;
}