#include "manag_dir.h"

void manag_dir()
{
	while(1)
	{
		int x=0,y=0;
		show_bmp(0,0,"./black/file.bmp");
		if((get_two_xy()) == 3) //上滑
					break;
		while(1)
		{
			get_xy(&x,&y);
			Clean_Area(0,0,800,480,0x00ffffff);
			if((x>80&&x<251) && (y>154 && y<300))
			{
				show_file("./jpg",".jpg");
				 x=0;
				 y=0;
			}
			if((x>321&&x<477)&& (y>145 && y<300))
			{
				show_file("./mp3",".mp3");
				x=0;
				 y=0;
			}
			if((x>548&&x<720)&& (y>146 && y<300))
			{
				show_file("./avi",".avi");
				x=0;
				 y=0;
			}
			if((x>750&&x<800)&&(y>430&&y<480))//退出登录--------break;
			{
				x=0;
				 y=0;
				break;
			}
		}
	}
	main_index();
}

void show_file(char *typebuf,char *filetypebuf)
{
	int Dir_flag = 0;
	//unsigned char file_name[10]={0};
	//1. open dir
	DIR* dirp = opendir(typebuf);
	if(dirp == NULL)
	{
		printf("open dir failed!\n");
		return;
	}
	while(1)
	{
		//readdir
		struct dirent *info = readdir(dirp);
		if(info == NULL)//finish reading
		{
			printf("reading finished\n");
			break;
		}
		//delete . ..
		if(info->d_name[0] == '.')
			continue;
		//
		if(info->d_type == DT_REG)
		{
			if(strcmp(&(info->d_name[strlen(info->d_name)-4]),filetypebuf)==0)
			{
				unsigned char *temp = NULL;
				temp = (char *)calloc(20,sizeof(char *));
				strcat(temp,info->d_name);
				strcat(temp,"\0");
				printf("%s\n",temp);
				Display_characterX(10,(50+Dir_flag*45),temp,0x00515151,3);//显示目录
				free(temp);
				
				Dir_flag++;
			}
		}
	}
	//printf("sum==%d\n",Dir_flag);
	
	//4. close dir
	closedir(dirp);
	
}