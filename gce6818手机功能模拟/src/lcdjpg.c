#include <stdio.h>   	
#include <fcntl.h>		 	 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "lcdjpg.h"
#include "jpeglib.h"
#include <sys/stat.h>

//#include "api_v4l2.h"

static char g_color_buf[FB_SIZE]={0};

static int  g_fb_fd;
static int *g_pfb_memory;

/* video_chat.c »­ÖÐ»­ÏÔÊ¾µÄ×ø±ê */
volatile int g_jpg_in_jpg_x;
volatile int g_jpg_in_jpg_y;

unsigned long file_size_get(const char *pfile_path)
{
	unsigned long filesize = -1;	
	struct stat statbuff;
	
	if(stat(pfile_path, &statbuff) < 0)
	{
		return filesize;
	}
	else
	{
		filesize = statbuff.st_size;
	}
	
	return filesize;
}
//LCD»­µã
void lcd_draw_point(unsigned int x,unsigned int y, unsigned int color)
{
	*(g_pfb_memory+y*800+x)=color;
}

int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path,char *pjpg_buf,unsigned int jpg_buf_size,unsigned int jpg_half)  
{
	//³õÊ¼»¯LCD
	g_fb_fd = open("/dev/fb0", O_RDWR);
	
	if(g_fb_fd<0)
	{
			printf("open lcd error\n");
			return -1;
	}

	g_pfb_memory  = (int *)mmap(	NULL, 				//Ó³ÉäÇøµÄ¿ªÊ¼µØÖ·£¬ÉèÖÃÎªNULLÊ±±íÊ¾ÓÉÏµÍ³¾ö¶¨Ó³ÉäÇøµÄÆðÊ¼µØÖ·
									FB_SIZE, 				//Ó³ÉäÇøµÄ³¤¶È
									PROT_READ|PROT_WRITE, 	//ÄÚÈÝ¿ÉÒÔ±»¶ÁÈ¡ºÍÐ´Èë
									MAP_SHARED,				//¹²ÏíÄÚ´æ
									g_fb_fd, 				//ÓÐÐ§µÄÎÄ¼þÃèÊö´Ê
									0						//±»Ó³Éä¶ÔÏóÄÚÈÝµÄÆðµã
								);
	
	/*¶¨Òå½âÂë¶ÔÏó£¬´íÎó´¦Àí¶ÔÏó*/
	struct 	jpeg_decompress_struct 	cinfo;
	struct 	jpeg_error_mgr 			jerr;	
	
	char *pcolor_buf = g_color_buf;
	char 	*pjpg;
	
	unsigned int 	i=0;
	unsigned int	color =0;
	unsigned int	count =0;
	
	unsigned int 	x_s = x;
	unsigned int 	x_e ;	
	unsigned int 	y_e ;
	
			 int	jpg_fd;
	unsigned int 	jpg_size;
	
	unsigned int 	jpg_width;
	unsigned int 	jpg_height;
	

	if(pjpg_path!=NULL)
	{
		/* ÉêÇëjpg×ÊÔ´£¬È¨ÏÞ¿É¶Á¿ÉÐ´ */	
		jpg_fd=open(pjpg_path,O_RDWR);
		
		if(jpg_fd == -1)
		{
		   printf("open %s error\n",pjpg_path);
		   
		   return -1;	
		}	
		
		/* »ñÈ¡jpgÎÄ¼þµÄ´óÐ¡ */
		jpg_size=file_size_get(pjpg_path);	

		/* ÎªjpgÎÄ¼þÉêÇëÄÚ´æ¿Õ¼ä */	
		pjpg = malloc(jpg_size);

		/* ¶ÁÈ¡jpgÎÄ¼þËùÓÐÄÚÈÝµ½ÄÚ´æ */		
		read(jpg_fd,pjpg,jpg_size);
	}
	else
	{
		jpg_size = jpg_buf_size;
		
		pjpg = pjpg_buf;
	}

	/*×¢²á³ö´í´¦Àí*/
	cinfo.err = jpeg_std_error(&jerr);

	/*´´½¨½âÂë*/
	jpeg_create_decompress(&cinfo);

	/*Ö±½Ó½âÂëÄÚ´æÊý¾Ý*/		
	jpeg_mem_src(&cinfo,pjpg,jpg_size);
	
	/*¶ÁÎÄ¼þÍ·*/
	jpeg_read_header(&cinfo, TRUE);

	/*¿ªÊ¼½âÂë*/
	jpeg_start_decompress(&cinfo);		
	if(jpg_half)
	{
		x_e	= x_s+(cinfo.output_width/2);
		y_e	= y  +(cinfo.output_height/2);		
		
		/*¶Á½âÂëÊý¾Ý*/
		while(cinfo.output_scanline < cinfo.output_height)
		{		
			pcolor_buf = g_color_buf;
			
			/* ¶ÁÈ¡jpgÒ»ÐÐµÄrgbÖµ */
			jpeg_read_scanlines(&cinfo,(JSAMPARRAY)&pcolor_buf,1);			
			
			/* ÔÙ¶ÁÈ¡jpgÒ»ÐÐµÄrgbÖµ */
			jpeg_read_scanlines(&cinfo,(JSAMPARRAY)&pcolor_buf,1);

			for(i=0; i<(cinfo.output_width/2); i++)
			{
				/* »ñÈ¡rgbÖµ */
				color = 		*(pcolor_buf+2);
				color = color | *(pcolor_buf+1)<<8;
				color = color | *(pcolor_buf)<<16;
				
				/* ÏÔÊ¾ÏñËØµã */
				lcd_draw_point(x,y,color);			
				pcolor_buf +=6;			
				x++;
			}		
			/* »»ÐÐ */
			y++;						
			x = x_s;			
		}
	}
	else
	{
		x_e	= x_s+cinfo.output_width;
		y_e	= y  +cinfo.output_height;	

		/*¶Á½âÂëÊý¾Ý*/
		while(cinfo.output_scanline < cinfo.output_height )
		{		
			pcolor_buf = g_color_buf;
			
			/* ¶ÁÈ¡jpgÒ»ÐÐµÄrgbÖµ */
			jpeg_read_scanlines(&cinfo,(JSAMPARRAY)&pcolor_buf,1);
			
			for(i=0; i<cinfo.output_width; i++)
			{
				/* »ñÈ¡rgbÖµ */
				color = 		*(pcolor_buf+2);
				color = color | *(pcolor_buf+1)<<8;
				color = color | *(pcolor_buf)<<16;
				
				/* ÏÔÊ¾ÏñËØµã */
				lcd_draw_point(x,y,color);				
				pcolor_buf +=3;				
				x++;
			}			
			/* »»ÐÐ */
			y++;						
			x = x_s;			
		}		
	}
	
	/*½âÂëÍê³É*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	if(pjpg_path!=NULL)
	{
		/* ¹Ø±ÕjpgÎÄ¼þ */
		close(jpg_fd);	
		
		/* ÊÍ·ÅjpgÎÄ¼þÄÚ´æ¿Õ¼ä */
		free(pjpg);		
	}
	//LCD¹Ø±Õ
	/* È¡ÏûÄÚ´æÓ³Éä */
	munmap(g_pfb_memory, FB_SIZE);
	
	/* ¹Ø±ÕLCDÉè±¸ */
	close(g_fb_fd);
	return 0;
}

