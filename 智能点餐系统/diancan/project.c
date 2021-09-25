/**==头文件=====================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/input.h>
#include <sys/mman.h>

/**=============================================*/
struct caid
{
	char *name;
    	char num;
	int price;
	int sum;
};

struct caid caidan[] = {
{"大盘鸡    ",0,35,0},
{"十三香虾  ",0,39,0},
{"爆炒腰花  ",0,45,0},
{"空心菜    ",0,12,0},
{"砂锅煲    ",0,36,0},
{"毛血旺    ",0,24,0},
{"麻辣鱼    ",0,29,0},
{"麻辣火锅  ",0,30,0},
{"跺椒鱼头  ",0,32,0},
{"爽口滑鸡  ",0,24,0},
{"蒜台炒肉  ",0,28,0},
{"锡纸花甲  ",0,39,0},
{"皮蛋瘦肉粥",0,3,0},
{"三鲜粉    ",0,12,0},
{"小笼包    ",0,6,0},
{"茶叶蛋    ",0,2,0},
};

/**==宏定义、结构体定义、全局变量定义===========*/
#define LCD_SIZE 800*480*4

/*bmp图片的文件头信息定义*/
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
#pragma pack(1)   			//取消字节对齐 
typedef struct {			//bmp图片文件头信息封装 
	// 位图文件头 
	u8  bit_file_type[2];	//位图文件类型:'BM'->0x4d42 
	u32 file_size;	  		//整个文件大小 
	u16 reserved1;	  		//保留 
	u16 reserved2;	  		//保留 
	u32 offset;		  		//文件头到位图数据之间的偏移量 

	// 位图信息头 
	u32 head_size;			//位图信息头长度 
	u32 width;		  		//位图宽度 
	u32 height;		  		//位图高度 
	u16 bit_planes;	  		//位图位面数 
	u16 bits_per_pixel; 	//每个像素的位数 
	u32 compression;		//压缩说明 
	u32 image_size;			//位图数据大小 
	u32 h_res;				//水平分辨率 
	u32 v_res;				//垂直分辨率 
	u32 color_palette;		//位图使用的颜色索引数 
	u32 vip_color;			//重要的颜色索引数目 

}bmp_head;
#pragma pack() 	//恢复字节对齐

struct TS_node{
	int ts_fd;
	int x;
	int y;
};

/**=============================================*/

/**==函数声明===================================*/
//线程执行函数
void *fun(void *arg);
//初始化lcd屏函数
char *LCD_Init(void);
//显示bmp图片函数
void Desplay(char *bmp_patpname,char *map_ptr,int x_offset,int y_offset);
//初始化触摸屏
void TS_Init(struct TS_node *p);
//获取触摸屏坐标
void get_xy(struct TS_node *p);

/**=============================================*/


/**==主函数=====================================*/
int main(void)
{
	//初始化lcd
	char *lcd_map = LCD_Init();
	Desplay("./pictrue/huanyin.bmp", lcd_map,0,0);
	sleep(2);
	struct TS_node *event_ptr = calloc(1,sizeof(struct TS_node));
	//初始化触摸屏
	TS_Init(event_ptr);
	
	//创建线程专门获取坐标
	pthread_t p_id;
	pthread_create(&p_id,NULL, fun,(void *)event_ptr);
	char i=0;//用于数组参数
	int pricesum;//合计
	while(1)
	{
		//printf("(%d,%d)\n",event_ptr->x,event_ptr->y);
	Desplay("./pictrue/gancai.bmp", lcd_map,0,0);
	Desplay("./pictrue/chuancai.bmp", lcd_map,400,0);
	Desplay("./pictrue/xiangcai.bmp", lcd_map,0,240);
	Desplay("./pictrue/zaocan.bmp", lcd_map,400,240);
	if(event_ptr->x>10 && event_ptr->x<390 && event_ptr->y>10 && event_ptr->y<230)//赣菜
	{
		event_ptr->x=0;event_ptr->y=0;
		Desplay("./pictrue/gancai_1.bmp", lcd_map,0,0);
		while(1)
		{
			if(event_ptr->x>0 && event_ptr->x<60 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=0;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>240 && event_ptr->x<300 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=0;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
				}
			if(event_ptr->x>300 && event_ptr->x<360 && event_ptr->y>180 && event_ptr->y<240)

			{
				i=1;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>540 && event_ptr->x<600 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=1;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
				}
			if(event_ptr->x>0 && event_ptr->x<60 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=2;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
				}
			if(event_ptr->x>240 && event_ptr->x<300 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=2;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
				}
		 	if(event_ptr->x>300 && event_ptr->x<360 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=3;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>540 && event_ptr->x<600 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=3;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);

				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>610 && event_ptr->x<795 && event_ptr->y>10 && event_ptr->y<200)
			{
				event_ptr->x=0;event_ptr->y=0;
				break;
			}
			if(event_ptr->x>610 && event_ptr->x<795 && event_ptr->y>260 && event_ptr->y<450)
			{
				printf("菜名          数量        价格        小计\n");

				for(i=0; i<16; i++)
				{
					if(caidan[i].num!=0)
					{
						printf("%s    %d           %d          %d 元\n",caidan[i].name,caidan[i].num,caidan[i].price,caidan[i].num*caidan[i].price);
						pricesum+=caidan[i].num*caidan[i].price;
						caidan[i].num=0;	
					}
				}

				printf("合计：%d元\n",pricesum);pricesum=0;
				event_ptr->x=0;event_ptr->y=0;
				break;
			}
		}
	}
	if(event_ptr->x>410 && event_ptr->x<790 && event_ptr->y>10 && event_ptr->y<230)
	{
		event_ptr->x=0;event_ptr->y=0;
		Desplay("./pictrue/chuancai_1.bmp", lcd_map,0,0);
		while(1)
		{
			if(event_ptr->x>0 && event_ptr->x<60 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=4;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>240 && event_ptr->x<300 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=4;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>300 && event_ptr->x<360 && event_ptr->y>180 && event_ptr->y<240)

			{
				i=5;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>540 && event_ptr->x<600 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=5;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>0 && event_ptr->x<60 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=6;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>240 && event_ptr->x<300 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=6;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
	 		}
			if(event_ptr->x>300 && event_ptr->x<360 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=7;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>540 && event_ptr->x<600 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=7;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			
			if(event_ptr->x>610 && event_ptr->x<795 && event_ptr->y>10 && event_ptr->y<200)
			{
				event_ptr->x=0;event_ptr->y=0;
				break;
			}
			if(event_ptr->x>610 && event_ptr->x<795 && event_ptr->y>260 && event_ptr->y<450)
			{
				printf("菜名          数量        价格        小计\n");

				for(i=0; i<16; i++)
				{
					if(caidan[i].num!=0)
					{
						printf("%s    %d           %d          %d 元\n",caidan[i].name,caidan[i].num,caidan[i].price,caidan[i].num*caidan[i].price);
						pricesum+=caidan[i].num*caidan[i].price;
						caidan[i].num=0;	
					}
				}

				printf("合计：%d元\n",pricesum);pricesum=0;
				event_ptr->x=0;event_ptr->y=0;
				break;
			}
		}
	}
	if(event_ptr->x>10 && event_ptr->x<390 && event_ptr->y>250 && event_ptr->y<470)
	{
		event_ptr->x=0;event_ptr->y=0;
		Desplay("./pictrue/xiangcai_1.bmp", lcd_map,0,0);
		while(1)
		{
			if(event_ptr->x>0 && event_ptr->x<60 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=8;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>240 && event_ptr->x<300 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=8;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>300 && event_ptr->x<360 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=9;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>540 && event_ptr->x<600 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=9;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>0 && event_ptr->x<60 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=10;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>240 && event_ptr->x<300 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=10;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>300 && event_ptr->x<360 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=11;
				if(caidan[i].num>0)caidan[i].num--;

				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>540 && event_ptr->x<600 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=11;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			
			if(event_ptr->x>610 && event_ptr->x<795 && event_ptr->y>10 && event_ptr->y<200)
			{
				event_ptr->x=0;event_ptr->y=0;
				break;
			}
			if(event_ptr->x>610 && event_ptr->x<795 && event_ptr->y>260 && event_ptr->y<450)
			{
				printf("菜名          数量        价格        小计\n");

				for(i=0; i<16; i++)
				{
					if(caidan[i].num!=0)
					{
						printf("%s    %d           %d          %d 元\n",caidan[i].name,caidan[i].num,caidan[i].price,caidan[i].num*caidan[i].price);
						pricesum+=caidan[i].num*caidan[i].price;
						caidan[i].num=0;	
					}
				}

				printf("合计：%d元\n",pricesum);pricesum=0;
				event_ptr->x=0;event_ptr->y=0;
				break;
			}		}
		}
	if(event_ptr->x>410 && event_ptr->x<790 && event_ptr->y>250 && event_ptr->y<470)
	{
		event_ptr->x=0;event_ptr->y=0;
		Desplay("./pictrue/zaocan_1.bmp", lcd_map,0,0);
		while(1)
		{
			if(event_ptr->x>0 && event_ptr->x<60 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=12;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;

			}
			if(event_ptr->x>240 && event_ptr->x<300 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=12;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>300 && event_ptr->x<360 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=13;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>540 && event_ptr->x<600 && event_ptr->y>180 && event_ptr->y<240)
			{
				i=13;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>0 && event_ptr->x<60 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=14;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>240 && event_ptr->x<300 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=14;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>300 && event_ptr->x<360 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=15;
				if(caidan[i].num>0)caidan[i].num--;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			if(event_ptr->x>540 && event_ptr->x<600 && event_ptr->y>420 && event_ptr->y<480)
			{
				i=15;
				caidan[i].num++;
				printf("%s    %d份\n",caidan[i].name,caidan[i].num);
				event_ptr->x=0;event_ptr->y=0;
			}
			
			if(event_ptr->x>610 && event_ptr->x<795 && event_ptr->y>10 && event_ptr->y<200)
			{
				event_ptr->x=0;event_ptr->y=0;
				break;
			}
			if(event_ptr->x>610 && event_ptr->x<795 && event_ptr->y>260 && event_ptr->y<450)
			{
				printf("菜名          数量        价格        小计\n");

				for(i=0; i<16; i++)
				{
					if(caidan[i].num!=0)
					{
						printf("%s    %d           %d          %d 元\n",caidan[i].name,caidan[i].num,caidan[i].price,caidan[i].num*caidan[i].price);
						pricesum+=caidan[i].num*caidan[i].price;
						caidan[i].num=0;	
					}
				}

				printf("合计：%d元\n",pricesum);pricesum=0;
				event_ptr->x=0;event_ptr->y=0;
				break;
			}
	
		}
	
	}
	}
	
}
/**=============================================*/








/**==函数定义区=================================*/
//线程执行函数
void *fun(void *arg)
{
	struct TS_node *p = (struct TS_node *)arg;
	while(1)
	{
		get_xy(p);
	}
}

//初始化lcd
char *LCD_Init(void)
{
	//找到并打开lcd设备文件
	int lcd_fd = open("/dev/fb0",O_RDWR);

	//判断打开成功否
	if(lcd_fd == -1)//失败
	{
		perror("打开dev/fb0失败");
		exit(-1);
	}
	
	/*开启映射*/
	char *map_ptr = mmap(NULL, LCD_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, lcd_fd, 0);//void *的指针可以转换为任意型
	//检测是否映射成功
	if(map_ptr == MAP_FAILED)
	{
		perror("映射失败");
		exit(-1);
	}
	
	return map_ptr;
}

//显示函数
void Desplay(char *bmp_patpname,char *map_ptr,int x_offset,int y_offset)
{
	/*向lcd文件中写入像素点数据*/
	//打开bmp图片
	int  bmp_fd = open(bmp_patpname, O_RDONLY);//完整路径 /mnt/hgfs/shear/day004/1.bmp
	if(bmp_fd == -1)
	{
		perror("打开bmp图片失败");
		exit(-1);
	}
	
	//读取图片头部信息
	bmp_head myhead;
	read(bmp_fd, &myhead, sizeof(bmp_head));
	
	//申请缓冲区
	char tmp_buf[myhead.width*myhead.height*3];
	//清空缓冲区
	bzero(tmp_buf,sizeof(tmp_buf));
	
	//读取图片数据
	read(bmp_fd, tmp_buf, sizeof(tmp_buf));
	//关闭图片文件
	close(bmp_fd);
	//将数据存入映射内存
	//将数据存入映射内存
	int i,j;
	for(i=0; i<myhead.height; i++)
		for(j=0; j<myhead.width; j++)
		{
			*(map_ptr+((i+y_offset)*800+j+x_offset)*4) = tmp_buf[((myhead.height-1-i)*myhead.width+j)*3];
			*(map_ptr+((i+y_offset)*800+j+x_offset)*4+1) = tmp_buf[((myhead.height-1-i)*myhead.width+j)*3+1];
			*(map_ptr+((i+y_offset)*800+j+x_offset)*4+2) = tmp_buf[((myhead.height-1-i)*myhead.width+j)*3+2];
			*(map_ptr+((i+y_offset)*800+j+x_offset)*4+3) = 0x00<<24;
		}
	
}

//初始化触摸屏
void TS_Init(struct TS_node *p)
{
	//打开触摸屏文件
	p->ts_fd = open("/dev/input/event0",O_RDONLY);
	if(p->ts_fd == -1)
	{
		perror("打开触摸屏失败");
		exit(-1);
	}
}

//获取触摸屏坐标
void get_xy(struct TS_node *p)
{
		
	//定义数据缓冲区
	struct input_event myevent;
	bool x_flag=false,y_flag=false;
	while(1)
	{
		//读取输入数据
		read(p->ts_fd, &myevent, sizeof(struct input_event));
		
		//判断事件类型
		if(myevent.type == EV_ABS)
		{
			//判断事件代号
			if(myevent.code == ABS_X)
			{
				if(!x_flag && !y_flag)
				{
					p->x=myevent.value;
					x_flag = true;
				}
			}
			if(myevent.code == ABS_Y)
			{
				if(x_flag && !y_flag)
				{
					p->y=myevent.value;
					y_flag = true;
				}
			}
		}
		if(x_flag && y_flag)
		{
			x_flag=false;
			y_flag=false;
			break;
		}
	}
}


/**=============================================*/
