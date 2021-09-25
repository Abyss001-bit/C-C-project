#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

int main(void)  
{
	//打开LCD
	int lcd_fd = open("/dev/fb0",O_RDWR);
	if(lcd_fd < 0)
	{
		perror("open lcd failed");
		return -1;
	}
	//内存映射
	int *p = mmap(NULL,800*480*4,PROT_READ | PROT_WRITE,MAP_SHARED,lcd_fd,0);
	if(p == NULL)   //映射失败
	{
		perror("mmap() faield");
		return -1;
	}
	/*
	*p = 0x00ff0000;
	*(p+1) = 0x00ff0000;
	
	*(p+800*200+100) = 0x00ff0000;
	*/
	
	int x,y;
	for(y=0;y<480;y++)
	{
		for(x=0;x<800;x++)
		{
			*(p+800*y + x) = rand()%16777215;  //花屏
		}
	}
	
	//关闭lcd
	close(lcd_fd);
	
	return 0;
}