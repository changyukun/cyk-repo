#include "soapH.h"
#include "HELLO.nsmap" //ע�ⲻ��.h,����.nsmap������ԭ����add.h���ɵġ�


//���������������soap��ֻ��Ҫ�Ķ�IP ��ַ�Ͷ˿ڡ�
int main(int argc, char **argv)
{	
	struct soap soap;
	int i;
	int m, s; // master and slave sockets
	soap_init(&soap);
	m = soap_bind(&soap, "192.168.1.3", 10000, 100); // ����ip ���˿�
	if(m<0)
	{
		soap_print_fault(&soap, stderr);
		exit(-1);
	}
	else
	{
		fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
		for(i=1;;i++)
		{
			s=soap_accept(&soap);
			if(s<0)
			{
				soap_print_fault(&soap, stderr);
				break;
			}

			fprintf(stderr, "%d: accepted connection from IP=%d.%d.%d.%d socket=%d", i, (soap.ip >> 24)&0xFF, (soap.ip >> 16)&0xFF, (soap.ip >> 8)&0xFF, soap.ip&0xFF, s);
			if(soap_serve(&soap)!=SOAP_OK)  //Process RPC request
				soap_print_fault(&soap, stderr);//print error
				
			fprintf(stderr, "request served\n");
			
			soap_destroy(&soap);//clean up class instances
			soap_end(&soap);//clean up everything and close socket
		}
	}

	soap_done(&soap);//close master socket and detach environment
	return 0;
}


//��һ��������struct soap.....����أ����������add.h�ж���Ĳ���
int HELLO__hello(struct soap *soap,char **p)
{
	*p= (char *)soap_malloc(soap, sizeof("hello11!")+1);//Ҫ����ָ���ָ��Ļ��������յ�ָ����ָ��Ŀռ���Ҫ��    soap_malloc�ڷ���˷����
	strcpy(*p, "hello11!");//Ϊ����Ŀռ丽����Ҫ��ֵ��
	return SOAP_OK;
}


//����һ������ָ�룬���Բ���Ҫ����ռ�
int ns__add(struct soap *soap, int num1, int num2, int* sum)
{
	*sum=num1+num2;
	return SOAP_OK;
}











