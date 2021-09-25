#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "../../include/qisr.h"
#include "../../include/msp_cmn.h"
#include "../../include/msp_errors.h"

#define SAMPLE_RATE_16K     (16000)
#define SAMPLE_RATE_8K      (8000)
#define MAX_GRAMMARID_LEN   (32)
#define MAX_PARAMS_LEN      (1024)
#define BUF_SIZE 1024*2048
#define FILE_TEMP "file.txt"
//------------------------------------------------
/*
服务器：
	套接字 socket
	绑定IP bind
	监听 listen
	等待连接 accept
	recv/send read/write
	close
	
	*/
//TCP
#include <stdio.h>
#include <unistd.h>//read write close
#include <string.h>//bzero
#include <stdlib.h>//molloc
#include <sys/types.h>//open
#include <sys/stat.h>//open
#include <fcntl.h>//open
#include <sys/socket.h>
#include <arpa/inet.h>//htons
#include <netinet/in.h>//inet_ntoa
#include <pthread.h>
#define BUF_SIZE 1024*2048

//------------------------------------------------

const char * ASR_RES_PATH        = "fo|res/asr/common.jet"; //离线语法识别资源路径
const char * GRM_BUILD_PATH      = "res/asr/GrmBuilld"; //构建离线语法识别网络生成数据保存路径
const char * GRM_FILE            = "call.bnf"; //构建离线识别语法网络所用的语法文件
const char * LEX_NAME            = "contact"; //更新离线识别语法的contact槽（语法文件为此示例中使用的call.bnf）

typedef struct _UserData {
	int     build_fini; //标识语法构建是否完成
	int     update_fini; //标识更新词典是否完成
	int     errcode; //记录语法构建或更新词典回调错误码
	char    grammar_id[MAX_GRAMMARID_LEN]; //保存语法构建返回的语法ID
}UserData;


//const char *get_audio_file(void); //选择进行离线语法识别的语音文件
int build_grammar(UserData *udata); //构建离线识别语法网络
int update_lexicon(UserData *udata); //更新离线识别语法词典
int run_asr(UserData *udata); //进行离线语法识别
/*
const char* get_audio_file(void)
{
	/*
	//calloc(1,96044);

	while(1)
	{
		printf("等待音频数据!\n");
		//接收socket音频数据    recv    read
		ret = recv(ca_sockfd,recvdbuf, BUF_SIZE,0);//flags
		if(ret < 0)
		{
			printf("接收失败\n");
			return 0;
		}

	一次接不完就循环接

	打开文件 （新建该文件，并且清空文件数据）


	将接收的缓冲区数据写入到文件

	关闭文件

	return "wav/1.wav"

	}
	
	return "wav/1.wav"
}
*/
int build_grm_cb(int ecode, const char *info, void *udata)
{
	UserData *grm_data = (UserData *)udata;

	if (NULL != grm_data) {
		grm_data->build_fini = 1;
		grm_data->errcode = ecode;
	}

	if (MSP_SUCCESS == ecode && NULL != info) {
		printf("构建语法成功！ 语法ID:%s\n", info);
		if (NULL != grm_data)
			snprintf(grm_data->grammar_id, MAX_GRAMMARID_LEN - 1, info);
	}
	else
		printf("构建语法失败！%d\n", ecode);

	return 0;
}

int build_grammar(UserData *udata)
{
	FILE *grm_file                           = NULL;
	char *grm_content                        = NULL;
	unsigned int grm_cnt_len                 = 0;
	char grm_build_params[MAX_PARAMS_LEN]    = {NULL};
	int ret = 0;

	grm_file = fopen(GRM_FILE, "rb");	
	if(NULL == grm_file) {
		printf("打开\"%s\"文件失败！[%s]\n", GRM_FILE, strerror(errno));
		return -1; 
	}

	fseek(grm_file, 0, SEEK_END);
	grm_cnt_len = ftell(grm_file);
	fseek(grm_file, 0, SEEK_SET);

	grm_content = (char *)malloc(grm_cnt_len + 1);
	if (NULL == grm_content)
	{
		printf("内存分配失败!\n");
		fclose(grm_file);
		grm_file = NULL;
		return -1;
	}
	fread((void*)grm_content, 1, grm_cnt_len, grm_file);
	grm_content[grm_cnt_len] = '\0';
	fclose(grm_file);
	grm_file = NULL;

	snprintf(grm_build_params, MAX_PARAMS_LEN - 1, 
		"engine_type = local, \
		asr_res_path = %s, sample_rate = %d, \
		grm_build_path = %s, ",
		ASR_RES_PATH,
		SAMPLE_RATE_16K,
		GRM_BUILD_PATH
		);
		
	printf("---------------------!");
	ret = QISRBuildGrammar("bnf", grm_content, grm_cnt_len, grm_build_params, build_grm_cb, udata);

	free(grm_content);
	grm_content = NULL;

	return ret;
}

int update_lex_cb(int ecode, const char *info, void *udata)
{
	UserData *lex_data = (UserData *)udata;

	if (NULL != lex_data) {
		lex_data->update_fini = 1;
		lex_data->errcode = ecode;
	}

	if (MSP_SUCCESS == ecode)
		printf("更新词典成功！\n");
	else
		printf("更新词典失败！%d\n", ecode);

	return 0;
}

int update_lexicon(UserData *udata)
{
	const char *lex_content                   = "丁伟\n黄辣椒";
	unsigned int lex_cnt_len                  = strlen(lex_content);
	char update_lex_params[MAX_PARAMS_LEN]    = {NULL}; 

	snprintf(update_lex_params, MAX_PARAMS_LEN - 1, 
		"engine_type = local, text_encoding = UTF-8, \
		asr_res_path = %s, sample_rate = %d, \
		grm_build_path = %s, grammar_list = %s, ",
		ASR_RES_PATH,
		SAMPLE_RATE_16K,
		GRM_BUILD_PATH,
		udata->grammar_id);
	return QISRUpdateLexicon(LEX_NAME, lex_content, lex_cnt_len, update_lex_params, update_lex_cb, udata);
}

int run_asr(UserData *udata)
{
	char asr_params[MAX_PARAMS_LEN]    = {NULL};
	const char *rec_rslt               = NULL;
	const char *session_id             = NULL;
	const char *asr_audiof             = NULL;
	FILE *f_pcm                        = NULL;
	char *pcm_data                     = NULL;
	long pcm_count                     = 0;
	long pcm_size                      = 0;
	int last_audio                     = 0;
	int aud_stat                       = MSP_AUDIO_SAMPLE_CONTINUE;
	int ep_status                      = MSP_EP_LOOKING_FOR_SPEECH;
	int rec_status                     = MSP_REC_STATUS_INCOMPLETE;
	int rss_status                     = MSP_REC_STATUS_INCOMPLETE;
	int errcode                        = -1;

	//asr_audiof = get_audio_file();
	f_pcm = fopen("1.wav", "rb");
	if (NULL == f_pcm) {
		printf("打开\"%s\"失败！[%s]\n", f_pcm, strerror(errno));
		goto run_error;
	}
	printf("打开文件成功\n");
	fseek(f_pcm, 0, SEEK_END);
	pcm_size = ftell(f_pcm);
	fseek(f_pcm, 0, SEEK_SET);
	pcm_data = (char *)malloc(pcm_size);
	if (NULL == pcm_data)
		goto run_error;
	fread((void *)pcm_data, pcm_size, 1, f_pcm);
	fclose(f_pcm);
	f_pcm = NULL;

	//离线语法识别参数设置
	snprintf(asr_params, MAX_PARAMS_LEN - 1, 
		"engine_type = local, \
		asr_res_path = %s, sample_rate = %d, \
		grm_build_path = %s, local_grammar = %s, \
		result_type = xml, result_encoding = UTF-8, ",
		ASR_RES_PATH,
		SAMPLE_RATE_16K,
		GRM_BUILD_PATH,
		udata->grammar_id
		);
	session_id = QISRSessionBegin(NULL, asr_params, &errcode);
	if (NULL == session_id)
		goto run_error;
	printf("开始识别...\n");

	while (1) {
		unsigned int len = 6400;

		if (pcm_size < 12800) {
			len = pcm_size;
			last_audio = 1;
		}

		aud_stat = MSP_AUDIO_SAMPLE_CONTINUE;

		if (0 == pcm_count)
			aud_stat = MSP_AUDIO_SAMPLE_FIRST;

		if (len <= 0)
			break;

		printf(">");
		fflush(stdout);
		errcode = QISRAudioWrite(session_id, (const void *)&pcm_data[pcm_count], len, aud_stat, &ep_status, &rec_status);
		if (MSP_SUCCESS != errcode)
			goto run_error;

		pcm_count += (long)len;
		pcm_size -= (long)len;

		//检测到音频结束
		if (MSP_EP_AFTER_SPEECH == ep_status)
			break;

		usleep(150 * 1000); //模拟人说话时间间隙
	}
	//主动点击音频结束
	QISRAudioWrite(session_id, (const void *)NULL, 0, MSP_AUDIO_SAMPLE_LAST, &ep_status, &rec_status);

	free(pcm_data);
	pcm_data = NULL;

	//获取识别结果
	while (MSP_REC_STATUS_COMPLETE != rss_status && MSP_SUCCESS == errcode) {
		rec_rslt = QISRGetResult(session_id, &rss_status, 0, &errcode);
		usleep(150 * 1000);
	}
	printf("\n识别结束：\n");
	printf("=============================================================\n");
	if (NULL != rec_rslt)
	{
		printf("%s\n", rec_rslt);
		//***************************************

		//发送结果到客户端    send  wirte
		//将缓冲区数据socket发送到服务器    send/write
		//写入文件实用
		
		int file = open(FILE_TEMP,O_RDWR|O_CREAT|O_TRUNC,0664);
		if(file < 0)
		{
			printf("open failed\n");
			return 0;
		}
		int ret = write(file,rec_rslt,BUF_SIZE);
		printf("写入文件%d的数据\n",ret);
		
		goto run_exit;
	}
	else
	{
		printf("没有识别结果！\n");
		char buf[]="id=\"9999\"";    //自己补充一个随意ID，做识别没结果或者出错的
		//发送结果到客户端    send  wirte
		int file = open(FILE_TEMP,O_RDWR|O_CREAT|O_TRUNC,0664);
		if(file < 0)
		{
			printf("open failed\n");
			return 0;
		}
		int ret = write(file,rec_rslt,BUF_SIZE);
		printf("写入文件%d的数据\n",ret);
		close(file);
		goto run_exit;
	}
		
	printf("=============================================================\n");
    printf("退出函数\n");
	goto run_exit;

run_error:
	if (NULL != pcm_data) {
		free(pcm_data);
		pcm_data = NULL;
	}
	if (NULL != f_pcm) {
		fclose(f_pcm);
		f_pcm = NULL;
	}
run_exit:
	QISRSessionEnd(session_id, NULL);
	return errcode;
}

int main_yuyin()  //
{
	const char *login_config    = "appid = 11c4f6b1"; //登录参数
	UserData asr_data; 
	int ret = 0 ;
	char c;

	ret = MSPLogin(NULL, NULL, login_config); //第一个参数为用户名，第二个参数为密码，传NULL即可，第三个参数是登录参数
	if (MSP_SUCCESS != ret) {
		printf("登录失败：%d\n", ret);
		goto exit;
	}

	memset(&asr_data, 0, sizeof(UserData));
	printf("构建离线识别语法网络...\n");
	ret = build_grammar(&asr_data);  //第一次使用某语法进行识别，需要先构建语法网络，获取语法ID，之后使用此语法进行识别，无需再次构建
	if (MSP_SUCCESS != ret) {
		printf("构建语法调用失败！\n");
		goto exit;
	}
	while (1 != asr_data.build_fini)
		usleep(300 * 1000);
	if (MSP_SUCCESS != asr_data.errcode)
		goto exit;
	printf("离线识别语法网络构建完成，开始识别...\n");	


  //*****************************************
  //tcp_init();


	
	
	ret = run_asr(&asr_data);
	if (MSP_SUCCESS != ret) {
	printf("离线语法识别出错: %d \n", ret);
	goto exit;
	}
	
	
	
exit:
	MSPLogout();
	printf("请按任意键退出...\n");
	getchar();
	return 0;
}

int main(int argc,char const *argv[])
{
	if(argc != 2)
	{
		printf("请输入 ./XXX + <port>");
		return 0;
	}
	//1.创建套接字
	int sockfd = socket(AF_INET , SOCK_STREAM, 0);//TCP
	if(socket<0)
	{
		printf("创建套接字失败\n");
		return 0;
	}
	//2. 绑定IP
	struct sockaddr_in saddr;//本端 服务器端
	struct sockaddr_in caddr;//对端 客户端
	bzero(&saddr,sizeof(saddr));
	bzero(&saddr,sizeof(caddr));
	
	saddr.sin_family = AF_INET;//IPv4地址族
	saddr.sin_port = htons(atoi(argv[1]));  //atoi是将字符串转换为整型
	saddr.sin_addr.s_addr = INADDR_ANY;//所有人都可以访问，动态获取
	int bindd = bind(sockfd, (struct sockaddr *)&saddr, sizeof(struct sockaddr));
	if(bindd < 0)
	{
		printf("绑定失败\n");
		return 0;
	}
	//3. 监听 listen
	int lis = listen(sockfd,5);
	if(lis < 0)
	{
		printf("监听失败\n");
		return 0;
	}
	//4. 等待连接 accept
	socklen_t len = sizeof(caddr);
	printf("等待客户端连接...\n");
	int ca_sockfd = accept(sockfd, (struct sockaddr *)&caddr, &len);
	if(ca_sockfd < 0)
	{
		printf("连接失败\n");
		return 0;
	}
	printf("IP:%s 连接成功\n",inet_ntoa(caddr.sin_addr));
		
	//5. recv/send read/write
	
	
	//------传输一般消息
	char buf[50] = {0};  //存储数据缓冲区
	ssize_t ret = 0; //返回值
	int choose = 0;
	//----------------
	
	while(1)
	{
		printf("准备接收：\n");
		ret = recv(ca_sockfd,buf, 50, 0);
		if(ret < 0)
		{
			printf("接收失败\n");
			return 0;
		}
		buf[strlen(buf)]='\0';
		choose = atoi(buf);
		printf("分辨接收的类型是：%d\n",choose);
		bzero(buf,50);
		
		switch(choose)
		{
			case 1:
			{
				ret = recv(ca_sockfd,buf, 50, 0);
				if(ret < 0)
				{
					printf("接收失败\n");
					return 0;
				}
				printf("接收IP:%s 发送的消息：%s",inet_ntoa(caddr.sin_addr),buf);
				printf("接收完成\n");
				bzero(buf,50);
				break;
			}
			case 2:
			{
				printf("请输入要接收文件的相对路径和名字：\n");
				//gets(buf);
				//printf("%s\n",buf);
				//fgets(buf,sizeof(buf),stdin);
				
				//buf[strlen(buf)-1]='\0';
				scanf("%s",buf);
				printf("%s：%d\n",buf,strlen(buf));
				//-----------传输文件
				char *recvdbuf = calloc(1,BUF_SIZE);
				int file_d = open(buf,O_RDWR | O_CREAT,0644);
				if(file_d < 0)
				{
					printf("open failed\n");
					return 0;
				}
				int file_ret = 0;
				long int i= 0;
				bzero(buf,50);
				fflush(stdin);
				//---------------
				
				long int length = 0;
				printf("请输入要接收的文件大小：\n");
				scanf("%ld",&length);
				
				while(1)
				{
					bzero(recvdbuf,BUF_SIZE);
					i+=file_ret;
					printf("i = %ld\n",i);
					if(i == length)
					{
						printf("接收IP:%s 发送的%ld个字节\n",inet_ntoa(caddr.sin_addr),i);
						printf("接收完成\n");
						close(file_d);
						break;
					}
					ret = recv(ca_sockfd,recvdbuf, BUF_SIZE,0);//flags
					if(ret < 0)
					{
						printf("接收失败\n");
						return 0;
					}
					file_ret = write(file_d,recvdbuf,ret);
					printf("写入了%d的数据\n",file_ret);
					
				}
				free(recvdbuf);
				break;
			}
			case 3:
			{
				char *sendbuf= malloc(BUF_SIZE);
				char *recvdbuf= malloc(BUF_SIZE);
				int file_ret = 0;
				int ret = 0;
				char *id = NULL;
				printf("等待音频数据!\n");
				//接收socket音频数据    recv    read
				ret = recv(ca_sockfd,recvdbuf, 96044,MSG_WAITALL);//flags
				if(ret < 0)
				{
					printf("接收失败\n");
					return 0;
				}
				printf("接收成功%d的数据\n",ret);
				//打开文件 （新建该文件，并且清空文件数据）
				int file_d = open("1.wav",O_RDWR|O_CREAT|O_TRUNC,0664);
				if(file_d < 0)
				{
					printf("open failed\n");
					return 0;
				}
				//将接收的缓冲区数据写入到文件
				file_ret =write(file_d,recvdbuf,BUF_SIZE);
				//关闭文件
				close(file_d);
				//解析文件：
				main_yuyin();
				printf("解析音频文件成功\n");
				//发送解析结果给客户端
				printf("确定发送解析文件:输入 1\n");
				int mycheck = 0;
				scanf("%d",&mycheck);
				if(mycheck == 1)
				{
					//发送文件
					int file = open(FILE_TEMP,O_RDONLY,0664);
					if(file < 0)
					{
						printf("open failed\n");
						return 0;
					}
					
					ret = read(file,sendbuf,BUF_SIZE);
					file_ret = send(ca_sockfd,sendbuf,ret,MSG_WAITALL);
					
					printf("发送了%d的数据\n",file_ret);
					close(file);
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
	
	
	//6. 关闭套接字
	close(ca_sockfd);
	close(sockfd);
	return 0;
}