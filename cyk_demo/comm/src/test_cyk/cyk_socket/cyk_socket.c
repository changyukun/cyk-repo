
/*****************************************************************************
File Name   :  test_socket.c

Description :  测试socket 进程通信( 分别将server 端、client 端做成两个执行文件进行测试)

     Author   :  changyukun

*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif



/*========== 头文件==============================================================================================*/
#include <stdio.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/un.h>  
#define UNIX_DOMAIN "/tmp/UNIX.domain"  






/*========== 外部变量引用=======================================================================================*/






/*========== 宏定义==============================================================================================*/






/*========== 数据结构定义======================================================================================*/






/*========== 变量定义===========================================================================================*/
			






/*========== 函数实现===========================================================================================*/

/* --- 服务器端*/
int main_server(void)   
{   
	socklen_t clt_addr_len;   
	int listen_fd;   
	int com_fd;   
	int ret;   
	int i;   
	static char recv_buf[2048];    
	int len;   
	
	struct sockaddr_un clt_addr;   
	struct sockaddr_un srv_addr;   
	
	listen_fd=socket(PF_UNIX,SOCK_STREAM,0);   
	if(listen_fd<0)   
	{   
		perror("cannot create communication socket");   
		return 1;   
	}     

	//set server addr_param   
	srv_addr.sun_family=AF_UNIX;   
	strncpy(srv_addr.sun_path,UNIX_DOMAIN,sizeof(srv_addr.sun_path)-1);   
	unlink(UNIX_DOMAIN);   
	
	//bind sockfd & addr   
	ret=bind(listen_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));   
	if(ret==-1)   
	{   
		perror("cannot bind server socket");   
		close(listen_fd);   
		unlink(UNIX_DOMAIN);   
		return 1;   
	}  
	
	//listen sockfd    
	ret=listen(listen_fd,1);   
	if(ret==-1)   
	{   
		perror("cannot listen the client connect request");   
		close(listen_fd);   
		unlink(UNIX_DOMAIN);   
		return 1;   
	}   

	while(1)
	{
		//have connect request use accept   
		printf("read to accept .......\n");
		len=sizeof(clt_addr);   
		com_fd=accept(listen_fd,(struct sockaddr*)&clt_addr,&len);   
		if(com_fd<0)   
		{   
			perror("cannot accept client connect request");   
			close(listen_fd);   
			unlink(UNIX_DOMAIN);   
			return 1;   
		}   
		
		//read and printf sent client info     
		while(1)
		{   
			memset(recv_buf,0,2048);   
			int num=read(com_fd,recv_buf,sizeof(recv_buf));   
			if(num>0)
			{
				if (strcmp(recv_buf, "end")==0)
					break;
				
				//printf("recv data (len=%d) : %s\n",num,recv_buf);   
				printf("recv data (len=%d) : \n",num); 
			}
			else
			{
				usleep(5000);
			}
		}   
		
		close(com_fd);   
	}
	
	close(listen_fd);   
	unlink(UNIX_DOMAIN);
	return 0;   
} 


/* --- 客户端*/
int main_client(void)   
{   
	int connect_fd;   
	int ret;   
	char snd_buf[1024];   
	int i;   
	static struct sockaddr_un srv_addr;   
	
	//creat unix socket   
	connect_fd=socket(PF_UNIX,SOCK_STREAM,0);   
	if(connect_fd<0)   
	{   
		perror("cannot create communication socket");   
		return 1;   
	}   
	
	srv_addr.sun_family=AF_UNIX;   
	strcpy(srv_addr.sun_path,UNIX_DOMAIN);   
	
	//connect server   
	ret=connect(connect_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));   
	if(ret==-1)   
	{   
		perror("cannot connect to the server");   
		close(connect_fd);   
		return 1;   
	}   
	memset(snd_buf,0,1024);   
	strcpy(snd_buf,"message from client  changyukun");   
	
	//send info server   
	for(i=0;i<4;i++)   
	{
		write(connect_fd,snd_buf,sizeof(snd_buf));   
	}

	sleep(5);
	printf(" client end --->\n");
	memset(snd_buf,0,1024);   
	strcpy(snd_buf,"end");   
	write(connect_fd,snd_buf,sizeof(snd_buf)); 
	
	close(connect_fd);   



	/* 进行第二次连接*/
	connect_fd=socket(PF_UNIX,SOCK_STREAM,0);  
	srv_addr.sun_family=AF_UNIX;   
	strcpy(srv_addr.sun_path,UNIX_DOMAIN);   
	
	//connect server   
	ret=connect(connect_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));   
	if(ret==-1)   
	{   
		perror("cannot connect to the server");   
		close(connect_fd);   
		return 1;   
	}   
	memset(snd_buf,0,1024);   
	strcpy(snd_buf,"message from client  changyukun1111");   
	
	//send info server   
	for(i=0;i<4;i++)   
	{
		write(connect_fd,snd_buf,sizeof(snd_buf));   
	}
	
	close(connect_fd);

	return 0;   
}









#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


