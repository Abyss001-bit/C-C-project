#include "camera.h"

#define MAX_MSG_SIZE 256
#define SERVER_PORT 2234

#define GET_VIDEO		"GET_VIDEO"          //camera
#define ENTER_KUGOU	"ENTER_KUGOU"
#define	BACK		"BACK"
#define	CAMLED_ON		"LED_ON"
#define	CAMLED_OFF	"LED_OFF"
#define MUSIC_STOP_CONT  	"MUSIC_STOP_CONT"


int xx,yy=0;
int kk = 0;

struct sockaddr_in saddr;//本端 服务器端
struct sockaddr_in caddr;//对端 客户端
int clinet_length = sizeof(caddr);
int server_length = sizeof(saddr);

int sockfd = -1;

int recv_len = 0;
int send_len = 0;


//flag
int get_video = 0;
int get_video_count = 0;
int back = 0;
int music_stop_cont =0;
int music_stop_cont_count =0;
int enter_kugou = 0;
int enter_kugou_count = 0;

int led_on = 0;
int led_off = 0;

char message[MAX_MSG_SIZE];
char buf[10] = {0};  //存放一张照片
char buf_avi[10] = {0}; 


int udp_init(void);


void *jige(void *arg)
{
	while(1)
	{
		get_xy(&xx,&yy);
		xx=0,yy=0;
	}
}

void *jisao(void *arg)
{
	//实时获取对方想要查看监控的按钮是否按下
	while(1)
	{
		recv_len = recvfrom(sockfd, message, sizeof(message), 0,(struct sockaddr *)&caddr, &clinet_length);
		if(recv_len < 0)
		{
			printf("接收失败\n");
			return 0;
		}
		if(strncmp(message,GET_VIDEO,9) == 0) //双击手机camera实现拍照
		{
			get_video = 1;
			get_video_count++;
		}
		if(strncmp(message,BACK,9) == 0) //实现显示照片
		{
			back = 1;
		}
		if(strncmp(message,CAMLED_ON,9) == 0) //双击手机camera实现拍照
		{
			led_on =1;
		}
		
		if(strncmp(message,CAMLED_OFF,9) == 0) //双击手机camera实现拍照
		{
			led_off =1;
		}
		if(strncmp(message,ENTER_KUGOU,9) == 0) //双击手机camera实现拍照
		{
			enter_kugou =1;
			enter_kugou_count++;
		}
		if(strncmp(message,MUSIC_STOP_CONT,9) == 0) //双击手机camera实现拍照
		{
			music_stop_cont =1;
			music_stop_cont_count++;
		}
		printf("message：%s\n",message);
		bzero(message,sizeof(message));
	}
}



int camera(void)
{
	show_bmp(0,0,"./black/camera.bmp");
	udp_init();
	FrameBuffer video_buf;
	//1.打开摄像头
	linux_v4l2_device_init("/dev/video7");
	//2.开启摄像头开始工作
	linux_v4l2_start_capturing();

	pthread_t pid;//线程ID号
	pthread_create(&pid,NULL,jige,NULL);
	
	pthread_t pid1;//线程ID号
	pthread_create(&pid1,NULL,jisao,NULL);
	
	start:
	while(1)
	{
		//3.采集数据
		linux_v4l2_get_fream(&video_buf);
		
		//4.将数据显示在屏幕上
		lcd_draw_jpg(0,0,NULL,video_buf.buf,video_buf.length,0);
		
		//双击手机camera实现拍照
		if(get_video == 1)//只要上面的子线程有成功接收到获取信息的按钮，就把数据发过去
		{
			send_len = sendto(sockfd, video_buf.buf, video_buf.length, 0,(struct sockaddr *)&caddr, clinet_length);
			if(send_len < 0)
			{
				printf("failed\n");
				return 0;
			}
			if(get_video_count == 2)
			{
				sprintf(buf,"./jpg/p%d.jpg",kk);
				int zp_fd = open(buf,O_RDWR|O_CREAT,0644);
				if(zp_fd < 0)
				{
					return;
				}
				write(zp_fd,video_buf.buf,video_buf.length);
				
				close(zp_fd);
				kk++;
				xx = 0;
				yy = 0;
				printf("p.jpg is ok!\n");
				//拍照后重置双击实现拍照
				get_video_count = 0;
				//get_video = 0;
				lcd_draw_jpg(0,0,buf,NULL,0,0);
				sleep(1);
			}
		}
		
		if(get_video ==1)
		{
			if(led_on == 1)
			{
				my_led(1);
				led_on =0;
			}
			if(led_off == 1)
			{
				my_led(0);
				led_off =0;
			}
		}
		
		if(get_video == 1)
		{
			if(enter_kugou == 1)
			{
				system("madplay  mp3/*.mp3 -r &");  
				enter_kugou =0;
			}
			if(enter_kugou_count == 2)
			{
				system("killall -9 madplay");
				enter_kugou = 0;
				enter_kugou_count = 0;
			}
			if(music_stop_cont == 1)
			{
				system("killall -STOP madplay &");
				music_stop_cont = 0;
			}
			if(music_stop_cont_count == 2)
			{
				system("killall -CONT madplay &");
				music_stop_cont = 0;
				music_stop_cont_count = 0;
			}
		}
		//手机点击显示照片  再点一下back退出
		if(get_video == 1)
		{
			if(back == 1) //get_video == 1 && back == 1 实现显示照片
			{
				xx = 0;
				yy = 0;
				get_video = 0;
				back = 0;
				goto stop_out;
			}
		}
		
		//板子点击抓拍
		if((xx>689&&xx<781)&&(yy>190&&yy<285))  //抓拍
		{
				sprintf(buf,"./jpg/p%d.jpg",kk);
				int zp_fd = open(buf,O_RDWR|O_CREAT,0644);
				if(zp_fd < 0)
				{
					return;
				}
				write(zp_fd,video_buf.buf,video_buf.length);
				close(zp_fd);
				kk++;
				xx = 0;
				yy = 0;
				printf("p.jpg is ok!\n");
				//拍照后重置双击实现拍照
				get_video_count = 0;
				get_video = 0;
				lcd_draw_jpg(0,0,buf,NULL,0,0);
				sleep(1);
		}
		//板子点击实现显示照片
		if((xx>697&&xx<777)&&(yy>384&&yy<460))//浏览
		{
			xx = 0;
			yy = 0;
			goto stop_out;
		}
		
		if((xx>690&&xx<781)&&(yy>30&&yy<113))
		{
			xx = 0;
			yy = 0;
			goto out;
		}
	}
	stop_out:
	while(1)
	{
		lcd_draw_jpg(0,0,buf,NULL,0,0);
		if((get_two_xy()) == 3) //退出浏览
		{
			back = 0;
			goto start;
		}
	}
	out:
	//关闭摄像头
	linux_v4l2_stop_capturing();
	linux_v4l2_device_uinit();
	//线程退出
	pthread_cancel(pid);
	pthread_cancel(pid1);
	//4. 关闭套接字
	close(sockfd);	
	return 0;
}


int udp_init(void)
{
	//1.创建套接字
	sockfd = socket(AF_INET , SOCK_DGRAM, 0);
	if(socket<0)
	{
		printf("创建套接字失败\n");
		return 0;
	}
	//2. 绑定IP
	
	bzero(&saddr,sizeof(saddr));
	
	saddr.sin_family = AF_INET;//IPv4地址族
	saddr.sin_port = htons(SERVER_PORT);
	saddr.sin_addr.s_addr = INADDR_ANY;//所有人都可以访问，动态获取
	int bindd = bind(sockfd, (struct sockaddr *)&saddr, sizeof(struct sockaddr));
	if(bindd < 0)
	{
		printf("绑定失败\n");
		return 0;
	}
	
	return 0;
}
