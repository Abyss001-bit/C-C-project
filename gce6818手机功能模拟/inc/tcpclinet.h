#ifndef  __TCPLINET_H_
#define __TCPLINET_H_

#include <stdio.h>
#include <unistd.h>//read write close
#include <string.h>//bzero
#include <stdlib.h>//molloc
#include <sys/types.h>//open
#include <sys/socket.h>
#include <sys/stat.h>//open
#include <fcntl.h>//open
#include <arpa/inet.h>//htons
#include <netinet/in.h>//inet_ntoa
#include <pthread.h>
#include "led.h"
#include "weather.h"


int void_main(char*argv);

#endif