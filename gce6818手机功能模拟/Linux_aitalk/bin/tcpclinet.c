/*
�ͻ��ˣ�
	socket
	������IP
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
		printf("������ ./XXX + <port>");
		return 0;
	}
	
	//1.�����׽���
	int sockfd = socket(AF_INET , SOCK_STREAM, 0);  //TCP
	if(socket<0)
	{
		printf("�����׽���ʧ��\n");
		return 0;
	}
	//2. ���ӶԷ�IP
	struct sockaddr_in saddr;//�Զ� ��������
	bzero(&saddr,sizeof(saddr));
	saddr.sin_family = AF_INET;//IPv4��ַ��
	saddr.sin_port = htons(atoi(argv[1]));  //atoi�ǽ��ַ���ת��Ϊ����
	saddr.sin_addr.s_addr = inet_addr("192.168.72.132");//��������ַ
	
	
	
	//3. ���ӷ����� connect
	socklen_t len = sizeof(saddr);
	printf("�ȴ����������...\n");
	int con = connect(sockfd,  (struct sockaddr *)&saddr, len);
	if(con < 0)
	{
		printf("����ʧ��\n");
		return 0;
	}
	
	
	
	//4. recv/send read/write
	//------����һ����Ϣ
	char buf[50] = {0};  //�洢���ݻ�����
	ssize_t ret = 0; //����ֵ
	//----------------

	//-----------ѡ���䷽ʽ
	int choose = 0;
	//-----------------
	
	while(1)
	{
		//fflush(stdin);
		printf("��ѡ����ʵʱ��Ϣ�����ļ�����Ϣ:1���ļ�:2������:3\n");
		//scanf("%s",buf);
		//printf("%s��%d\n",buf,strlen(buf));
		fgets(buf,sizeof(buf),stdin);
		buf[strlen(buf)]='\0';
		ret = send(sockfd,buf, sizeof(buf), 0);
		choose = atoi(buf);
		switch(choose)
		{
			case 1:
			{
				printf("������Ҫ������Ϣ:\n");
				//fflush(stdin);
				fgets(buf,sizeof(buf),stdin);
				ret = send(sockfd,buf, sizeof(buf), 0);
				if(ret < 0)
				{
					printf("����ʧ��\n");
					return 0;
				}
				bzero(buf,sizeof(buf));
				printf("���ݷ��ͳɹ�\n");
				break;
			}
			case 2:
			{
				printf("������Ҫ�����ļ������·�������֣�\n");
				//fflush(stdin);
				//gets(buf);
				fgets(buf,sizeof(buf),stdin);
				buf[strlen(buf)-1]='\0';
				printf("%s��%d\n",buf,strlen(buf));
				//---------------���ļ�------
				char *sendbuf = calloc(1,BUF_SIZE);
				int file_d = open(buf,O_RDONLY);
				if(file_d < 0)
				{
					printf("open failed\n");
					return 0;
				}
				bzero(buf,50);
				
				
				//�����ļ�����
				//lseek�����ɹ��Ļ�,���ص��Ǵ��ļ���ʼ���ļ���ʱ��ƫ����֮��Ĵ�С
				long int length = 0;
				length = lseek(file_d,0,SEEK_END);
				printf("%ld\n",length);//�������ļ��Ĵ�С
				lseek(file_d,0,SEEK_SET);
				//---------------------------------------
				
				int file_ret = 0;
				bzero(buf,50);
				//---------------
				while(1)
				{
					file_ret = read(file_d,sendbuf,BUF_SIZE);
					ret = send(sockfd,sendbuf,file_ret,0);
					printf("������%d������\n",file_ret);
					if(file_ret < BUF_SIZE)
					{
						//printf("������%d������\n",file_ret);
						printf("�������\n");
						close(file_d);
						break;
					}
					if(ret < 0)
					{
						printf("����ʧ��\n");
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
				//¼��
				printf("����������һ���ַ����س�!,����3s��˵��\n");
				scanf("%d",&flag);
				system("arecord -d3 -c1 -r16000 -twav -fS16_LE 1.wav");
				
				//��¼�õ���Ƶ���͵�����������ʶ��
				//----------------open����Ƶ�ļ�
				int file_d = open("1.wav",O_RDONLY);
				if(file_d < 0)
				{
					printf("open failed\n");
					return 0;
				}
				//read��ȡ���ݵ�������     malloc(96044);
				char *sendbuf= calloc(1,BUF_SIZE);
				char *recvdbuf= calloc(1,BUF_SIZE);
				int file_ret = 0;
				int ret = 0;
				
				file_ret =read(file_d,sendbuf,BUF_SIZE);
				
				//������������socket���͵�������    send/write
				ret = send(sockfd,sendbuf,file_ret,0);
				if(ret < 0)
				{
					printf("����ʧ��\n");
					return 0;
				}
				printf("������%d������\n",ret);
				//�������������
				//bzero(senddbuf,BUF_SIZE);
				//���շ�������������ʶ����
				//recv   /  read
				//bzero(recvdbuf,BUF_SIZE);
				printf("׼������id��\n");
				printf("ȷ�����ս����ļ�:���� 1\n");
				int mycheck = 0;
				scanf("%d",&mycheck);
				if(mycheck == 1)
				{
					ret = read(sockfd,recvdbuf, BUF_SIZE);
					if(ret < 0)
					{
						printf("����ʧ��\n");
						return 0;
					}
					printf("���ճɹ�\n");
				}
				//��������������ճɹ�--------------------
				//�������ݵõ�ΨһID
				sleep(2);
				printf("��������Ϊ��%s\n",recvdbuf);
				
				if((p = strstr(recvdbuf,"id=")) !=NULL)
				{
					strtok(p,"\"");//�и��ַ����� �� ��Ϊ�ָ������õ���id=��
					tmp = strtok(NULL,"\"");//�ٴ��и�
					id = atoi(tmp);
					printf("id = %d\n",id);
				}
				
				if(id == 1)
				{
					//system("aplay 1.wav");//�Լ�����¼����һ��
					system("madplay mp3/*.mp3 -r &");
					//printf("aplay 1.wav\n");
				}
				if(id == 2)
				{
					//system("aplay 1.wav");//�Լ�����¼����һ��
					system("killall -9 madplay");
					//printf("aplay 1.wav\n");
				}
				if(id == 9999)//ʶ�����
				{
					printf("�Բ���,����һ�ûѧ��\n");
				}
				free(sendbuf);
				free(recvdbuf);
				break;
			}
			default:
			{
				printf("��ʱ�޴�ѡ��\n");
				break;
			}
		}
		
	}
	//5. �ر��׽���
	close(sockfd);
	return 0;
}

