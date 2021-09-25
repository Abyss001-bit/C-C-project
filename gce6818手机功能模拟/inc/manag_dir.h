#ifndef __MANAG_DIR_H__
#define __MANAG_DIR_H__

#include <string.h>
#include <strings.h>
#include <stdio.h>
#include "get_xy.h"
#include "showbmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>//opendir
#include <dirent.h>//opendir
#include <unistd.h>//close
#include <string.h>
#include <strings.h>
#include <malloc.h>
#include "myfont.h"
#include "main_index.h"

void manag_dir();
void show_file(char *typebuf,char *filetypebuf);
//´«²Î
/*
	filetype :  "./mp3"
	filetypebuf: ".mp3"
*/

#endif