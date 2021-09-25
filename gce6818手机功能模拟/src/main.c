#include <stdio.h>
#include <stdlib.h>
#include "first_index.h"
#include "main_index.h"
#include "myfont.h"
#include "manag_dir.h"
#include "camera.h"
#include "tcpclinet.h"
#include <pthread.h>
#include "weather.h"


int main(int argc, char const *argv[])
{
	
	if(argc != 2)
	{
		printf("请输入 ./XXX + <port>");
		return 0;
	}
	char * PORT =(char *)argv[1];
	Init_Font();
	while(1)
	{
		first_index();
		while(1)
		{
			
			int ret = get_passwd();
			if(ret < 0)
			{
				break;
			}
			
			while(1)
			{
				int x=0;
				int y=0;
				main_index();
				get_xy(&x,&y);
				if((x>95&&x<153)&&(y>85&&y<133))
				{
					music_play();
				}
				
				if((x>338&&x<454)&&(y>78&&y<139))
				{
					//天气监测
					weather();
				}
				
				if((x>642&&x<705)&&(y>83&&y<139))
				{
					//设置
				}
				
				if((x>93&&x<159)&&(y>221&&y<275))
				{
					//文件管理
					 manag_dir();
				}
				
				if((x>645&&x<702)&&(y>208&&y<277))
				{
					void_main(PORT);
					//语音转写
				}
				
				if((x>92&&x<156)&&(y>352&&y<410))//点击了图库的小图标
				{
					camera();//相机
				}
				
				if((x>367&&x<433)&&(y>355&&y<409))//点击了图库的小图标
				{
					jpg_tk();
				}
				
				
				if((x>642&&x<706)&&(y>363&&y<411))//退出登录--------break;
				{
					//视频播放器
					video_play();
				}
				
				if((x>750&&x<800)&&(y>430&&y<480))//退出登录--------break;
				{
					break;
				}
				
				if((get_two_xy()) == 3) //上滑
					break;
				
			}
			break;
		}
		
	}
	UnInit_Font();
	return 0;
}