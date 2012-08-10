#include "soapH.h"
#include "HELLO.nsmap" //注意不是.h,而是.nsmap这是由原来的add.h生成的。


//这个函数就是启动soap，只需要改动IP 地址和端口。
int main(int argc, char **argv)
{	
	struct soap soap;
	int i;
	int m, s; // master and slave sockets
	soap_init(&soap);
	m = soap_bind(&soap, "192.168.1.3", 10000, 100); // 本机ip 及端口
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


//第一个参数是struct soap.....，规矩，后面跟着是add.h中定义的参数
int HELLO__hello(struct soap *soap,char **p)
{
	*p= (char *)soap_malloc(soap, sizeof("hello11!")+1);//要返回指针的指针的话，那最终的指针所指向的空间是要用    soap_malloc在服务端分配的
	strcpy(*p, "hello11!");//为分配的空间附上需要的值。
	return SOAP_OK;
}


//返回一个数字指针，所以不需要分配空间
int ns__add(struct soap *soap, int num1, int num2, int* sum)
{
	*sum=num1+num2;
	return SOAP_OK;
}











