
/*****************************************************************************
File Name   :  test_comm_a_2_c++.cpp

Description :  

     Author   :  changyukun

*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif



/*========== ͷ�ļ�==============================================================================================*/
#include <stdio.h>
#include <stdlib.h>







/*========== �ⲿ��������=======================================================================================*/






/*========== �궨��==============================================================================================*/






/*========== ���ݽṹ����======================================================================================*/
class Comm_a_CwnApp2
{
public:
	Comm_a_CwnApp2();
	int InitInstance2();

};





/*========== ��������===========================================================================================*/
Comm_a_CwnApp2 comm_a_theApp2;			






/*========== ����ʵ��===========================================================================================*/
//-------------------------------------------------------------------------------------------------
Comm_a_CwnApp2::Comm_a_CwnApp2()
{
	printf("-------> Comm_a_CwnApp2::Comm_a_CwnApp2() \n");
}
//-------------------------------------------------------------------------------------------------
int Comm_a_CwnApp2::InitInstance2()
{
	printf("-------> Comm_a_CwnApp2::InitInstance2() \n");
	return 0;
}




//-------------------------------------------------------------------------------------------------
void comm_a_2_init_cpp(void)
{
	printf("********** comm_a_2_init_cpp *********************\n");
	comm_a_theApp2.InitInstance2();
}







#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

