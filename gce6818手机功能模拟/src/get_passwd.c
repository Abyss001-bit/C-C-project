#include "get_passwd.h"

unsigned char password[6]={'2','0','2','1','0','7'};//初始密码
unsigned char login[6];//存储输入的密码
unsigned int passwd_count = 3; //计数
unsigned int check_flag = 0;
unsigned int off_flag = 0;
unsigned int re_in = 0;

int input_passwd(int pass_unmber)
{
	int ts_x,ts_y;//触摸屏坐标
	
	short login_x[6]={180,250,330,400,480,550};//隐藏显示密码图片x轴的坐标
	get_xy(&ts_x,&ts_y);
	
	
	if((ts_x>170 && ts_x<310) && (ts_y>145 && ts_y<216))
	{
		login[pass_unmber-1]='1';
		show_bmp(login_x[pass_unmber-1],62,"./pic/xx.bmp");
	}
	if((ts_x>320 && ts_x<464) && (ts_y>145 && ts_y<216))
	{
		login[pass_unmber-1]='2';
		show_bmp(login_x[pass_unmber-1],62,"./pic/xx.bmp");
	}
	if((ts_x>470 && ts_x<600) && (ts_y>145 && ts_y<216))
	{
		login[pass_unmber-1]='3';
		show_bmp(login_x[pass_unmber-1],62,"./pic/xx.bmp");
	}
	if((ts_x>170 && ts_x<310) && (ts_y>224 && ts_y<296))
	{
		login[pass_unmber-1]='4';
		show_bmp(login_x[pass_unmber-1],62,"./pic/xx.bmp");
	}
	if((ts_x>320 && ts_x<464) && (ts_y>224 && ts_y<296))
	{
		login[pass_unmber-1]='5';
		show_bmp(login_x[pass_unmber-1],62,"./pic/xx.bmp");
	}
	if((ts_x>470 && ts_x<600) && (ts_y>224 && ts_y<296))
	{
		login[pass_unmber-1]='6';
		show_bmp(login_x[pass_unmber-1],62,"./pic/xx.bmp");
	}
	if((ts_x>170 && ts_x<310) && (ts_y>300 && ts_y<378))
	{
		login[pass_unmber-1]='7';
		show_bmp(login_x[pass_unmber-1],62,"./pic/xx.bmp");
	}
	if((ts_x>320 && ts_x<464) && (ts_y>300 && ts_y<378))
	{
		login[pass_unmber-1]='8';
		show_bmp(login_x[pass_unmber-1],62,"./pic/xx.bmp");
	}
	if((ts_x>470 && ts_x<600) && (ts_y>300 && ts_y<378))
	{
		login[pass_unmber-1]='9';
		show_bmp(login_x[pass_unmber-1],62,"./pic/xx.bmp");
	}
	
	if((ts_x>320 && ts_x<470) && (ts_y>382 && ts_y<457))
	{
		login[pass_unmber-1]='0';
		show_bmp(login_x[pass_unmber-1],62,"./pic/xx.bmp");
	}
	
	if((ts_x>170 && ts_x<310) && (ts_y>382 && ts_y<457))
	{
		//清除
		if((pass_unmber-1) != 0)
		{
			off_flag =1;
			show_bmp(login_x[pass_unmber-2],62,"./pic/xxx.bmp");
		}
		
	}
	
	if((ts_x>470 && ts_x<600) && (ts_y>382 && ts_y<457))
	{
		//确认
		check_flag = 1;
		passwd_count--;
	}
	
	return 0;
}
int get_passwd(void)
{
	while(1)
	{
		int d=0;
		show_bmp(0,0,"./black/password.bmp");
		//密码的输入
		while(++d)
		{
			if(re_in == 1)
			{
				re_in = 0;//重置re_in
				passwd_count = 3;//密码输入次数重置
			}
			
			if(off_flag == 1)
			{
				--d;
				--d;
				off_flag=0;
			}
			
			input_passwd(d);
			
			if(d>6 && check_flag == 1)
			{
				if(passwd_count == 0)
				{
					printf("out\n");
					show_bmp(190,140,"./pic/failout.bmp");
					re_in = 1; //失败3此次过
					sleep(1);
					return -1;
				}//无次数了
				else{
					break;
				}
			}	
			
		}
		check_flag =0;
		
		if(strcmp(password,login) == 0)
		{	
			show_bmp(190,140,"./pic/ok.bmp");	
			sleep(1);
			re_in = 1;//成功登录过
			break;
		}
		else
		{
			show_bmp(190,140,"./pic/wrong.bmp");	
			sleep(1);
		}
	}
	sleep(1);
	return 0;
}