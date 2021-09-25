#include "get_xy.h"
/******************************************************
*功能：获取一次x、y坐标
*参数：*x    --->横坐标
       *y    --->纵坐标
	  
*返回值：无
********************************************************/
int get_xy(int *x,int *y)
{
	//1.打开触摸屏
	int event_fd = open(EVENT_PATHNAME,O_RDWR);
	if(event_fd < 0)
	{
		printf("open event failed!\n");
		return -1;
	}
	//定义关键结构体的变量
	struct input_event touch_event;
	bzero(&touch_event,sizeof(touch_event));
	while(1)
	{
		read(event_fd,&touch_event,sizeof(touch_event));
		if(EV_ABS == touch_event.type && ABS_X == touch_event.code)
		{
			//*x = touch_event.value*0.78;//1024
			*x = touch_event.value;
		}
		if(EV_ABS == touch_event.type && ABS_Y == touch_event.code)
		{
			//*y = touch_event.value*0.8;//600
			*y = touch_event.value;
		}
		if(EV_KEY == touch_event.type && BTN_TOUCH == touch_event.code && 1 == touch_event.value)//1：按下 0：松开
		{
			break;	
		}
	}
	close(event_fd);
	return 0;
}

/******************************************************
*功能：滑屏
*参数：无
	  
*返回值：1--->向左滑   2---->向右滑   3----->向上滑
********************************************************/
int get_two_xy(void)
{
	//1.打开触摸屏
	int event_fd = open(EVENT_PATHNAME,O_RDWR);
	if(event_fd < 0)
	{
		printf("open event failed!\n");
		return -1;
	}
	//定义关键结构体的变量
	struct input_event touch_event;
	bzero(&touch_event,sizeof(touch_event));
	int x1,x2,y1,y2=-1;
	while(1)
	{
		while(1)
		{
			read(event_fd,&touch_event,sizeof(touch_event));
			if(EV_ABS == touch_event.type && ABS_X == touch_event.code)
			{
				//x1 = touch_event.value*0.78;//1024
				x1 = touch_event.value;
			}
			if(EV_ABS == touch_event.type && ABS_Y == touch_event.code)
			{
				//y1 = touch_event.value*0.8;//600
				y1 = touch_event.value;
			}
			if(x1!=0 && y1!=0)
			{
				break;	
			}
		}
		while(1)
		{
			read(event_fd,&touch_event,sizeof(touch_event));
			if(EV_ABS == touch_event.type && ABS_X == touch_event.code)
			{
				//x2 = touch_event.value*0.78;//1024
				x2 = touch_event.value;
			}
			if(EV_ABS == touch_event.type && ABS_Y == touch_event.code)
			{
				//y2 = touch_event.value*0.8;//600
				y2 = touch_event.value;
			}
			if(EV_KEY == touch_event.type && BTN_TOUCH == touch_event.code && 0 == touch_event.value)//1：按下 0：松开
			{
				break;	
			}
		}
		break;
	}
	close(event_fd);
	if(x1-x2>5)//向左滑
	{
		return 1;
	}
	if(x2-x1>5)//向右滑
	{
		return 2;
	}
	if(y1-y2>8 && x1-x2<5 || x2-x1<5)//向上滑
	{
		return 3;
	}
	return 0;
}
