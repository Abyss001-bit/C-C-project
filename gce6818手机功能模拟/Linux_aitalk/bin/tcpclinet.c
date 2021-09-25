/*
客户端：
	socket
	服务器IP
	connect
	recv/send read/write
	close
	
	*/
//TCP

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

#define BUF_SIZE 1024*2048

int main(int argc,char const *argv[])
{
	if(argc != 2)
	{
		printf("请输入 ./XXX + <port>");
		return 0;
	}
	
	//1.创建套接字
	int sockfd = socket(AF_INET , SOCK_STREAM, 0);  //TCP
	if(socket<0)
	{
		printf("创建套接字失败\n");
		return 0;
	}
	//2. 连接对方IP
	struct sockaddr_in saddr;//对端 服务器端
	bzero(&saddr,sizeof(saddr));
	saddr.sin_family = AF_INET;//IPv4地址族
	saddr.sin_port = htons(atoi(argv[1]));  //atoi是将字符串转换为整型
	saddr.sin_addr.s_addr = inet_addr("192.168.72.132");//服务器地址
	
	
	
	//3. 连接服务器 connect
	socklen_t len = sizeof(saddr);
	printf("等待服务端连接...\n");
	int con = connect(sockfd,  (struct sockaddr *)&saddr, len);
	if(con < 0)
	{
		printf("连接失败\n");
		return 0;
	}
	
	
	
	//4. recv/send read/write
	//------传输一般消息
	char buf[50] = {0};  //存储数据缓冲区
	ssize_t ret = 0; //返回值
	//----------------

	//-----------选择传输方式
	int choose = 0;
	//-----------------
	
	while(1)
	{
		//fflush(stdin);
		printf("请选择发送实时消息还是文件：消息:1、文件:2、语音:3\n");
		//scanf("%s",buf);
		//printf("%s：%d\n",buf,strlen(buf));
		fgets(buf,sizeof(buf),stdin);
		buf[strlen(buf)]='\0';
		ret = send(sockfd,buf, sizeof(buf), 0);
		choose = atoi(buf);
		switch(choose)
		{
			case 1:
			{
				printf("请输入要传的消息:\n");
				//fflush(stdin);
				fgets(buf,sizeof(buf),stdin);
				ret = send(sockfd,buf, sizeof(buf), 0);
				if(ret < 0)
				{
					printf("发送失败\n");
					return 0;
				}
				bzero(buf,sizeof(buf));
				printf("数据发送成功\n");
				break;
			}
			case 2:
			{
				printf("请输入要传输文件的相对路径和名字：\n");
				//fflush(stdin);
				//gets(buf);
				fgets(buf,sizeof(buf),stdin);
				buf[strlen(buf)-1]='\0';
				printf("%s：%d\n",buf,strlen(buf));
				//---------------传文件------
				char *sendbuf = calloc(1,BUF_SIZE);
				int file_d = open(buf,O_RDONLY);
				if(file_d < 0)
				{
					printf("open failed\n");
					return 0;
				}
				bzero(buf,50);
				
				
				//传输文件长度
				//lseek函数成功的话,返回的是从文件开始到文件此时的偏移量之间的大小
				long int length = 0;
				length = lseek(file_d,0,SEEK_END);
				printf("%ld\n",length);//输出这个文件的大小
				lseek(file_d,0,SEEK_SET);
				//---------------------------------------
				
				int file_ret = 0;
				bzero(buf,50);
				//---------------
				while(1)
				{
					file_ret = read(file_d,sendbuf,BUF_SIZE);
					ret = send(sockfd,sendbuf,file_ret,0);
					printf("发送了%d的数据\n",file_ret);
					if(file_ret < BUF_SIZE)
					{
						//printf("发送了%d的数据\n",file_ret);
						printf("发送完成\n");
						close(file_d);
						break;
					}
					if(ret < 0)
					{
						printf("发送失败\n");
						return 0;
					}
				}
				free(sendbuf);
				break;
			}
			case 3:
			{
				int flag = 0;
				char *p=NULL;
				char *tmp=NULL;
				int id=0;
				//录音
				printf("请输入任意一个字符并回车!,请在3s内说完\n");
				scanf("%d",&flag);
				system("arecord -d3 -c1 -r16000 -twav -fS16_LE 1.wav");
				
				//将录好的音频发送到服务器进行识别
				//----------------open打开音频文件
				int file_d = open("1.wav",O_RDONLY);
				if(file_d < 0)
				{
					printf("open failed\n");
					return 0;
				}
				//read读取数据到缓冲区     malloc(96044);
				char *sendbuf= calloc(1,BUF_SIZE);
				char *recvdbuf= calloc(1,BUF_SIZE);
				int file_ret = 0;
				int ret = 0;
				
				file_ret =read(file_d,sendbuf,BUF_SIZE);
				
				//将缓冲区数据socket发送到服务器    send/write
				ret = send(sockfd,sendbuf,file_ret,0);
				if(ret < 0)
				{
					printf("发送失败\n");
					return 0;
				}
				printf("发送了%d的数据\n",ret);
				//清除缓冲区数据
				//bzero(senddbuf,BUF_SIZE);
				//接收服务器发回来的识别结果
				//recv   /  read
				//bzero(recvdbuf,BUF_SIZE);
				printf("准备接收id串\n");
				printf("确定接收解析文件:输入 1\n");
				int mycheck = 0;
				scanf("%d",&mycheck);
				if(mycheck == 1)
				{
					ret = read(sockfd,recvdbuf, BUF_SIZE);
					if(ret < 0)
					{
						printf("接收失败\n");
						return 0;
					}
					printf("接收成功\n");
				}
				//语音解析结果接收成功--------------------
				//解析数据得到唯一ID
				sleep(2);
				printf("接收数据为：%s\n",recvdbuf);
				
				if((p = strstr(recvdbuf,"id=")) !=NULL)
				{
					strtok(p,"\"");//切割字符串以 “ ”为分隔符，得到“id=”
					tmp = strtok(NULL,"\"");//再次切割
					id = atoi(tmp);
					printf("id = %d\n",id);
				}
				
				if(id == 1)
				{
					//system("aplay 1.wav");//自己可以录音唱一首
					system("madplay mp3/*.mp3 -r &");
					//printf("aplay 1.wav\n");
				}
				if(id == 2)
				{
					//system("aplay 1.wav");//自己可以录音唱一首
					system("killall -9 madplay");
					//printf("aplay 1.wav\n");
				}
				if(id == 9999)//识别出错
				{
					printf("对不起,这句我还没学会\n");
				}
				free(sendbuf);
				free(recvdbuf);
				break;
			}
			default:
			{
				printf("暂时无此选项\n");
				break;
			}
		}
		
	}
	//5. 关闭套接字
	close(sockfd);
	return 0;
}

