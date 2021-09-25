#include "stdio.h"
#include "font.h"

int main(void)
{
	char String[100] = "重庆粤嵌";
	Init_Font();   //打开字库

	/*
	Clean_Area(0,  //x坐标起始点
               0,  //y坐标起始点
			   800, //绘制的宽度
			   480,//绘制的高度
			   0x00FFFFFF); //往屏幕指定区域填充颜色	

	Display_characterX(100,          //x坐标起始点
                       100,          //y坐标起始点
					   String,   //GB2312 中文字符串
					   0xFF00 ,     //字体颜色值
					   4); 
	*/
	Clean_Area(0,0,800,480,0x00FFFFFF);
	Display_characterX(100,100,String,0xFF,4); 
	
	UnInit_Font(); 	//关闭字库
	return 0;
}