#include "stdio.h"
#include "font.h"

int main(void)
{
	char String[100] = "������Ƕ";
	Init_Font();   //���ֿ�

	/*
	Clean_Area(0,  //x������ʼ��
               0,  //y������ʼ��
			   800, //���ƵĿ��
			   480,//���Ƶĸ߶�
			   0x00FFFFFF); //����Ļָ�����������ɫ	

	Display_characterX(100,          //x������ʼ��
                       100,          //y������ʼ��
					   String,   //GB2312 �����ַ���
					   0xFF00 ,     //������ɫֵ
					   4); 
	*/
	Clean_Area(0,0,800,480,0x00FFFFFF);
	Display_characterX(100,100,String,0xFF,4); 
	
	UnInit_Font(); 	//�ر��ֿ�
	return 0;
}