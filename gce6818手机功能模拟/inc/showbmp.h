#ifndef __SHOWBMP_H__
#define __SHOWBMP_H__

//头文件定义
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define WIGHT 800
#define HIGHT 480
#define LCD_MAP_SIZE WIGHT*HIGHT*4
//函数声明
/********************
 * 参数：xx    --->显示起始位置横坐标
 *       yy    --->显示起始位置纵坐标
 *       path  ---->显示的图片bmp路径
 * 返回值：无
 * ****************************/
int show_bmp(int xx,int yy,char *path);
#endif