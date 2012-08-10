
#include  "CykService.nsmap"
#include  "soapH.h"

/**//////////////////////////////////////////////////////////////////////////
///宏与全局变量的定义
#define  BACKLOG (100)  
#define  MAX_THR (10)   
#define  MAX_QUEUE (1000)


pthread_mutex_t 	queue_cs;                        	//队列锁
pthread_cond_t  	queue_cv;                       	//条件变量
SOAP_SOCKET     	queue[MAX_QUEUE];   		//数组队列
int                       	head =0, tail =0;          	//队列头队列尾初始化         
/**///////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
void * process_queue(void *);        	//线程入口函数
int  enqueue(SOAP_SOCKET);  		//入队列函数
SOAP_SOCKET dequeue(void);         	//出队列函数



/**///////////////////////////////////////////////////////////////////////////
//线程入口函数
void * process_queue(void * soap)
{
	struct soap * tsoap = (struct soap *)soap;
	
	for(;;)
	{
		tsoap->socket = dequeue();
		if (!soap_valid_socket(tsoap->socket))
		{
			break;
		}
		soap_serve(tsoap);
		soap_destroy(tsoap);
		soap_end(tsoap);
	}
	return NULL;
}

//入队列操作
int enqueue(SOAP_SOCKET sock)
{
	int status = SOAP_OK;
	int next;
	
	pthread_mutex_lock(&queue_cs);
	next = tail +1;
	
	if (next >= MAX_QUEUE) 
		next = 0;
	
	if (next == head) 
		status = SOAP_EOM;
	else
	{
		queue[tail] =sock;
		tail = next;
	}
	pthread_cond_signal(&queue_cv);
	pthread_mutex_unlock(&queue_cs);
	return status;
}

//出队列操作
SOAP_SOCKET dequeue()
{
	SOAP_SOCKET sock;
	
	pthread_mutex_lock(&queue_cs);
	while (head == tail )
	{
		pthread_cond_wait(&queue_cv,&queue_cs);
	}
	
	sock = queue[head++];
	
	if (head >= MAX_QUEUE)
	{
		head =0;
	}
	pthread_mutex_unlock(&queue_cs);
	return sock;
}


/**///////////////////////////具体服务方法////////////////////////////////////////
//加法的实现
int ns__add(struct soap *soap, int a, int b, int *result)
{
	*result = a + b;
	return SOAP_OK;
} 
//减法的实现
int ns__sub(struct soap *soap, int a, int b, int *result)
{ 
	*result = a - b;
	return SOAP_OK;
} 
//乘法的实现
int ns__mul(struct soap *soap, int a, int b, int *result)
{ 
	*result = a * b;
	return SOAP_OK;
} 
//除法的实现
int ns__div(struct soap *soap, int a, int b, int *result)
{ 
	if (b)
		*result = a / b;
	else
	{
		char *s = (char*)soap_malloc(soap, 1024);
		//sprintf(s, "Can't">http://tempuri.org/">Can't divide %f by %f", a, b);
		sprintf(s, "error1");
		return soap_sender_fault(soap, "Division by zero", s);
	}
	return SOAP_OK;
} 
//乘方的实现
int ns__pow(struct soap *soap, int a, int b, int *result)
{ 
	*result = pow(a, b);
	if (soap_errno == EDOM) /**//* soap_errno 和errorno类似, 但是和widnows兼容 */
	{ 
		char *s = (char*)soap_malloc(soap, 1024);
		sprintf(s, "Can't take the power of %f to  %f", a, b);
		// sprintf(s, "Can't">http://tempuri.org/">Can't take power of %f to %f", a, b);
		return soap_sender_fault(soap, "Power function domain error", s);
	}
	return SOAP_OK;
} 
int ns__hello(struct soap *soap,char **p)
{
	*p= (char *)soap_malloc(soap, sizeof("hello11!")+1);//要返回指针的指针的话，那最终的指针所指向的空间是要用    soap_malloc在服务端分配的
	strcpy(*p, "hello11!");//为分配的空间附上需要的值。
	return SOAP_OK;
}

/**///////////////////////////////////////////////////////////////////////////////////////////////////////
//主函数
int gsoap_server_main(char* ip, int port)
{
	struct soap ServerSoap;
	struct soap * soap_thr[MAX_THR];
	pthread_t tid[MAX_THR];
	int i;
	SOAP_SOCKET m,s;
	
	
	//初始话运行时环境
	soap_init(&ServerSoap);
	
	//锁和条件变量初始化
	pthread_mutex_init(&queue_cs,NULL);
	pthread_cond_init(&queue_cv,NULL);
	
	//绑定服务端口
	m = soap_bind(&ServerSoap, ip, port, BACKLOG);
	
	//循环直至服务套接字合法
	while (!soap_valid_socket(m))
	{
		fprintf(stderr,"Bind port error! \n");
		m = soap_bind(&ServerSoap,NULL,port,BACKLOG);
	}
	fprintf(stderr,"socket connection successful %d \n",m);

	//生成服务线程
	for(i = 0; i <MAX_THR; i++)
	{
		soap_thr[i] = soap_copy(&ServerSoap);
		fprintf(stderr,"Starting thread %d \n",i);
		pthread_create(&tid[i],NULL,(void*(*)(void*))process_queue,(void*)soap_thr[i]);
	}

	for(;;)
	{
		//接受客户端的连接
		s = soap_accept(&ServerSoap);
	
		if (!soap_valid_socket(s)) 
		{
			if (ServerSoap.errnum) 
			{
				soap_print_fault(&ServerSoap,stderr);
				continue;
			}
			else
			{
				fprintf(stderr,"Server timed out \n");
				break;
			}
		}
		
		//客户端的IP地址
		fprintf(stderr,"Accepted connection from IP=%d.%d.%d.%d socket=%d \n", ((ServerSoap.ip)>>24)&0xFF,((ServerSoap.ip)>>16)&0xFF,((ServerSoap.ip)>>8)&0xFF,(ServerSoap.ip)&0xFF, (ServerSoap.socket));

		//请求的套接字进入队列，如果队列已满则循环等待
		while(enqueue(s) == SOAP_EOM)
			sleep(1000);
	}
	
	//服务结束后的清理工作
	for(i = 0; i < MAX_THR; i++)
	{
		while (enqueue(SOAP_INVALID_SOCKET) == SOAP_EOM) 
		{
			sleep(1000);
		}
	}
	
	for(i=0; i< MAX_THR; i++)
	{
		fprintf(stderr,"Waiting for thread %d to terminate ..\n",i);
		pthread_join(tid[i],NULL);
		fprintf(stderr,"terminated \n");
		soap_done(soap_thr[i]);
		free(soap_thr[i]);
	}
	
	pthread_mutex_destroy(&queue_cs);
	pthread_cond_destroy(&queue_cv);


	//分离运行时的环境
	soap_done(&ServerSoap);
	return 0;
}

#if 0 /* 做成可执行文件方式*/
int main(int argc, char* argv[])
{
	gsoap_server_main("192.168.10.196", 10000); /* 指定ip 方式，也可传入参数NULL 默认为本机ip*/
	return 0;
}
#endif

