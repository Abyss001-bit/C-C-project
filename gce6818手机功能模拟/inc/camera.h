#ifndef  __CAMERA_H_
#define __CAMERA_H_

#include <stdio.h>
#include "lcdjpg.h"
#include "api_v4l2.h"
#include "showbmp.h"
#include "get_xy.h"
#include <pthread.h>
#include <unistd.h>//read write close
#include <string.h>//bzero
#include <stdlib.h>//molloc
#include <sys/types.h>//open
#include <sys/socket.h>
#include <arpa/inet.h>//htons
#include <netinet/in.h>//inet_ntoa
#include <strings.h>
#include "jpg_tk.h"
#include "led.h"
#include "music.h"

int camera(void);
int udp_init(void);

#endif