#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main(void)  
{
	//打开LCD
	int lcd_fd = open("/dev/fb0",O_RDWR);
	if(lcd_fd < 0)
	{
		perror("open lcd failed");
		return -1;
	}
	//给像素点的字节(argb)赋值
	char lcd_buf[800*480*4];
	
	int i;
	for(i=0;i<800*480*4;i+=4)
	{
		lcd_buf[i+0] = 0;   		//b
		lcd_buf[i+1] = 0;   		//g
		lcd_buf[i+2] = 255;   	    //r
		lcd_buf[i+3] = 0;   		//a
	}
	
	//将颜色数据写入lcd
	write(lcd_fd,lcd_buf,800*480*4);
	
	//关闭lcd
	close(lcd_fd);
	
	return 0;
}