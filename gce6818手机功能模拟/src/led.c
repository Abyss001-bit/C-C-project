#include "led.h"

/*************************************************
*主函数-实现led的控制
*************************************************/
int my_led(int is_led)
{
	//打开led驱动
	int led_fd = open("/dev/Led",O_RDONLY);
	if(led_fd < 0)
	{
		printf("open led failed!\n");
		return 0;
	}
	
	if(is_led == 1)
	{
		
			ioctl(led_fd,LED1,0);
			ioctl(led_fd,LED2,0);
			ioctl(led_fd,LED3,0);
			ioctl(led_fd,LED4,0);
		
	}
	if(is_led == 0)
	{
	
		
			ioctl(led_fd,LED1,1);
			ioctl(led_fd,LED2,1);
			ioctl(led_fd,LED3,1);
			ioctl(led_fd,LED4,1);
		
	}
	//关闭文件描述符
	close(led_fd);
	
	return 0;
}