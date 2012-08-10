#include <stdio.h>
#include "soapH.h"
#include "Service.nsmap"

int main(int argc, char* argv[])
{
	struct soap asoap;
	struct _ns2__hello ns2__hello;
	struct _ns2__helloResponse  ns2__helloResponse;

	soap_init(&asoap);
	//soap_set_namespaces(&asoap, namespaces);

	if(SOAP_OK == soap_call___ns1__hello(&asoap, "http://192.168.1.3:10000", NULL, &ns2__hello, &ns2__helloResponse))
		printf("ok\n");
	else
		printf("failed\n");

	printf("--------> %s\n", ns2__helloResponse.p);

	soap_destroy(&asoap);
	soap_end(&asoap);
	soap_done(&asoap);
	
	return 0;
}










