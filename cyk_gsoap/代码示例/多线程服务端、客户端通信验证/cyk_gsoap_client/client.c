#include <stdio.h>
#include "soapH.h"
#include "CykService.nsmap"

int main(int argc, char* argv[])
{
	struct soap asoap;
	int a=0, b=0;
	int result=0;
	char* aaa=NULL;

	soap_init(&asoap);
	//soap_set_namespaces(&asoap, namespaces);


	soap_init(&asoap);
	//soap_set_namespaces(&asoap, namespaces);

	if(SOAP_OK == soap_call_ns1__hello(&asoap, "http://192.168.10.196:10001", NULL, &aaa))
		printf("ok\n");
	else
		printf("failed\n");

	printf("--------> %s\n", aaa);
	

	a = 100;
	b = 200;
	soap_call_ns1__add(&asoap, "http://192.168.10.196:10001", NULL, a, b, &result);
	printf("%d+%d=%d\n",a,b,result);

	a = 300;
	b = 200;
	soap_call_ns1__sub(&asoap, "http://192.168.10.196:10001", NULL, a, b, &result);
	printf("%d-%d=%d\n",a,b,result);

	a = 100;
	b = 200;
	soap_call_ns1__mul(&asoap, "http://192.168.10.196:10001", NULL, a, b, &result);
	printf("%d*%d=%d\n",a,b,result);

	a = 200;
	b = 0;
	soap_call_ns1__div(&asoap, "http://192.168.10.196:10001", NULL, a, b, &result);
	printf("%d/%d=%d\n",a,b,result);
	
	soap_destroy(&asoap);
	soap_end(&asoap);
	soap_done(&asoap);
	
	return 0;
}










