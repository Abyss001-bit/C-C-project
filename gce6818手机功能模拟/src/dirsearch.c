#include "dirsearch.h"

void dirsearch()
{
	int MusicDir_flag = 0;
	unsigned char Music_name[10]={0};
	//1. open dir
	DIR* dirp = opendir("./mp3");
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
			if(strcmp(&(info->d_name[strlen(info->d_name)-4]),".mp3")==0)
			{
				printf("name:%s\n",info->d_name);
				strcpy(&(Music_name[MusicDir_flag]),info->d_name);
				printf("%s\n",&(Music_name[MusicDir_flag]));
				
				Display_characterX(10,(50+MusicDir_flag*45),&(Music_name[MusicDir_flag]),0x00515151,3);//ÏÔÊ¾Ä¿Â¼
				MusicDir_flag++;
			}
		}
	}
	printf("sum==%d\n",MusicDir_flag);
	
	//4. close dir
	closedir(dirp);
}
