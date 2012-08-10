
#include  "CykService.nsmap"
#include  "soapH.h"

/**//////////////////////////////////////////////////////////////////////////
///����ȫ�ֱ����Ķ���
#define  BACKLOG (100)  
#define  MAX_THR (10)   
#define  MAX_QUEUE (1000)


pthread_mutex_t 	queue_cs;                        	//������
pthread_cond_t  	queue_cv;                       	//��������
SOAP_SOCKET     	queue[MAX_QUEUE];   		//�������
int                       	head =0, tail =0;          	//����ͷ����β��ʼ��         
/**///////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
void * process_queue(void *);        	//�߳���ں���
int  enqueue(SOAP_SOCKET);  		//����к���
SOAP_SOCKET dequeue(void);         	//�����к���



/**///////////////////////////////////////////////////////////////////////////
//�߳���ں���
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

//����в���
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

//�����в���
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


/**///////////////////////////������񷽷�////////////////////////////////////////
//�ӷ���ʵ��
int ns__add(struct soap *soap, int a, int b, int *result)
{
	*result = a + b;
	return SOAP_OK;
} 
//������ʵ��
int ns__sub(struct soap *soap, int a, int b, int *result)
{ 
	*result = a - b;
	return SOAP_OK;
} 
//�˷���ʵ��
int ns__mul(struct soap *soap, int a, int b, int *result)
{ 
	*result = a * b;
	return SOAP_OK;
} 
//������ʵ��
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
//�˷���ʵ��
int ns__pow(struct soap *soap, int a, int b, int *result)
{ 
	*result = pow(a, b);
	if (soap_errno == EDOM) /**//* soap_errno ��errorno����, ���Ǻ�widnows���� */
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
	*p= (char *)soap_malloc(soap, sizeof("hello11!")+1);//Ҫ����ָ���ָ��Ļ��������յ�ָ����ָ��Ŀռ���Ҫ��    soap_malloc�ڷ���˷����
	strcpy(*p, "hello11!");//Ϊ����Ŀռ丽����Ҫ��ֵ��
	return SOAP_OK;
}

/**///////////////////////////////////////////////////////////////////////////////////////////////////////
//������
int gsoap_server_main(char* ip, int port)
{
	struct soap ServerSoap;
	struct soap * soap_thr[MAX_THR];
	pthread_t tid[MAX_THR];
	int i;
	SOAP_SOCKET m,s;
	
	
	//��ʼ������ʱ����
	soap_init(&ServerSoap);
	
	//��������������ʼ��
	pthread_mutex_init(&queue_cs,NULL);
	pthread_cond_init(&queue_cv,NULL);
	
	//�󶨷���˿�
	m = soap_bind(&ServerSoap, ip, port, BACKLOG);
	
	//ѭ��ֱ�������׽��ֺϷ�
	while (!soap_valid_socket(m))
	{
		fprintf(stderr,"Bind port error! \n");
		m = soap_bind(&ServerSoap,NULL,port,BACKLOG);
	}
	fprintf(stderr,"socket connection successful %d \n",m);

	//���ɷ����߳�
	for(i = 0; i <MAX_THR; i++)
	{
		soap_thr[i] = soap_copy(&ServerSoap);
		fprintf(stderr,"Starting thread %d \n",i);
		pthread_create(&tid[i],NULL,(void*(*)(void*))process_queue,(void*)soap_thr[i]);
	}

	for(;;)
	{
		//���ܿͻ��˵�����
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
		
		//�ͻ��˵�IP��ַ
		fprintf(stderr,"Accepted connection from IP=%d.%d.%d.%d socket=%d \n", ((ServerSoap.ip)>>24)&0xFF,((ServerSoap.ip)>>16)&0xFF,((ServerSoap.ip)>>8)&0xFF,(ServerSoap.ip)&0xFF, (ServerSoap.socket));

		//������׽��ֽ�����У��������������ѭ���ȴ�
		while(enqueue(s) == SOAP_EOM)
			sleep(1000);
	}
	
	//����������������
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


	//��������ʱ�Ļ���
	soap_done(&ServerSoap);
	return 0;
}

#if 0 /* ���ɿ�ִ���ļ���ʽ*/
int main(int argc, char* argv[])
{
	gsoap_server_main("192.168.10.196", 10000); /* ָ��ip ��ʽ��Ҳ�ɴ������NULL Ĭ��Ϊ����ip*/
	return 0;
}
#endif

