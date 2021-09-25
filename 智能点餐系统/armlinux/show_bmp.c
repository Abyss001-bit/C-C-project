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
	//打开bmp
	int bmp_fd = open("./1.bmp",O_RDWR);
	if(bmp_fd < 0)
	{
		perror("open bmp failed");
		return -1;
	}
	//偏移54字节
	lseek(bmp_fd,54,SEEK_SET);
	//读取图片数据
	char bmp_buf[800*480*3];
	read(bmp_fd,bmp_buf,800*480*3);
	
	//bmp_buf[0] -->b
	//bmp_buf[1] -->g
	//bmp_buf[2] -->b

	//给像素点的字节(argb)赋值
	char lcd_buf[800*480*4];
	int i,j;
	for(i=0,j=0;i<800*480*4;i+=4,j+=3)
	{
		lcd_buf[i+0] = bmp_buf[j+0];   		//b
		lcd_buf[i+1] = bmp_buf[j+1];   		//g
		lcd_buf[i+2] = bmp_buf[j+2];   	    //r
		lcd_buf[i+3] = 0;   		//a
	}
	
	//将颜色数据写入lcd
	write(lcd_fd,lcd_buf,800*480*4);
	
	//关闭lcd和bmp
	close(lcd_fd);
	close(bmp_fd);
	
	return 0;
}