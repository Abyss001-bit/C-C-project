#include "weather.h"

int fd = -1;
//串口初始化
int uart_init(int fd)
{
    //①保存原先串口配置     
    struct termios old_cfg,new_cfg;
    bzero(&old_cfg,sizeof(old_cfg));
    bzero(&new_cfg,sizeof(new_cfg));

    //②激活选项
    new_cfg.c_cflag  |=  CLOCAL | CREAD; 
    cfmakeraw(&new_cfg); 
    //③设置波特率
    cfsetispeed(&new_cfg, B115200); 
    cfsetospeed(&new_cfg, B115200);
    //④设置字符大小
    new_cfg.c_cflag &= ~CSIZE; /* 用数据位掩码清空数据位设置 */
    new_cfg.c_cflag |= CS8;
    //⑤设置奇偶校验位
    new_cfg.c_cflag &= ~PARENB;
    //⑥设置停止位
    new_cfg.c_cflag &= ~CSTOPB; /* 将停止位设置为一个比特 */
    //⑦设置最少字符和等待时间
    new_cfg.c_cc[VTIME]= 0;
    new_cfg.c_cc[VMIN] = 1;
    //⑧清除串口缓冲
    tcflush(fd, TCIFLUSH);
    //⑨激活配置
    tcsetattr(fd,TCSANOW,&new_cfg);
}
/*
//接收消息
void *dingwenjie(void *arg)
{
    char buf[6]={0};
    while(1)//串口发送消息
    {
        bzero(buf,6); 
        read(fd,buf,sizeof(buf));
        printf("\n\n接收的数据是:%s\n",buf);
		Display_characterX(200,200,&buf,0x00000000,2);
        //⑧清除串口缓冲
        tcflush(fd, TCIFLUSH);
    }
}
*/

int weather(void)
{
	Init_Font();
	Clean_Area(0,0,800,480,0x00ffffff);
    //1.打开串口
    fd = open("/dev/ttySAC1",O_RDWR);
    if(fd < 0)
    {
        printf("open COM2 failed!\n");
    }
    printf("打开 COM2 成功!\n");
    //初始化串口
    uart_init(fd);
    printf("初始化串口 COM2 成功!\n");

    //pthread_t pid;
    //pthread_create(&pid,NULL,dingwenjie,NULL);
	/*
    char buf[20]={0};
    while(1)//串口发送消息
    {
        bzero(buf,20); 
        printf("请输入您要发送的数据:\n");
        fgets(buf,20,stdin);//stdin 就是键盘
        write(fd,buf,strlen(buf));
        //⑧清除串口缓冲
        tcflush(fd, TCIFLUSH);
        sleep(0.5);
    }
	*/
	Display_characterX(30,10,"weather and wet as it :",0x00E8E7E5,4);
	char buf[6]={0};
    while(1)//串口发送消息
    {
		Clean_Area(200,200,300,100,0x00FFFFFF);
        bzero(buf,6); 
        read(fd,buf,sizeof(buf));
        printf("\n\n接收的数据是:%s\n",buf);
		Display_characterX(200,200,&buf,0x00000000,5);
        //⑧清除串口缓冲
        tcflush(fd, TCIFLUSH);
		int x=0;
		int y=0;
		get_xy(&x,&y);
		if((x>750&&x<800)&&(y>430&&y<480))//退出登录--------break;
		{
			break;
		}
		if((get_two_xy()) == 3) //上滑
					break;
    }
    close(fd);
	UnInit_Font();
    return 0;
}


