#ifndef __GET_XY_H__
#define __GET_XY_H__

//头文件定义
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <linux/input.h>
//宏定义
#define EVENT_PATHNAME "/dev/input/event0"

//结构体定义

//函数声明
int get_xy(int *x,int *y);
int get_two_xy(void);
#endif