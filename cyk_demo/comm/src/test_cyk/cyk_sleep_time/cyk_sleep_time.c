
/*****************************************************************************
File Name   :  test_sleep_time.c

Description :  测试如何实现一个线程循环周期相对稳定的周期时间，如线程每10  ms 跑一圈

     Author   :  changyukun

*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif



/*========== 头文件==============================================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>








/*========== 外部变量引用=======================================================================================*/






/*========== 宏定义==============================================================================================*/






/*========== 数据结构定义======================================================================================*/
typedef unsigned long long uint64_t;





/*========== 变量定义===========================================================================================*/
			






/*========== 函数实现===========================================================================================*/

//-------------------------------------------------------------------------------------------------
static uint64_t get_cur_time(void)
{
/*
	参数:
		1、
		
	返回:
		1、

	说明:
		1、用于获取系统的当前时间
*/
	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC,&ts)<0)
	{
		printf("clock_gettime() doesn't work: %s",strerror(errno));
	}
	return (ts.tv_sec*1000LL) + (ts.tv_nsec/1000000LL);

}

//-------------------------------------------------------------------------------------------------
static void sleepMs(int ms)
{
/*
	参数:
		1、
		
	返回:
		1、

	说明:
		1、sleep 功能
*/
	struct timespec ts;
	ts.tv_sec=0;
	ts.tv_nsec=ms*1000000LL;
	nanosleep(&ts,NULL);
}

//-------------------------------------------------------------------------------------------------
void pthread_fun_test(void* param)
{
#define TIME_INTERVALE	10

	uint64_t realtime=0, time1=0, time2=0;
	static uint64_t time_next=0;
	static uint64_t time_org=0;
	long long diff;

	time_org = get_cur_time();
		
	while(1)
	{
		time_next += TIME_INTERVALE;
		while(1)
		{
			realtime = get_cur_time() - time_org;
			diff = time_next -realtime;
			
			if(diff>0)
			{
				//sleepMs(((int)diff)/2);
				usleep((((int)diff)/2)*1000);
			}
			else
			{
				break;
			}
		}

		#if 1  // --- cyk add for test (打印测试两次执行的时间是否为TIME_INTERVALE ms)
		time1 = get_cur_time();
		diff = time1 - time2;
		time2 = time1;
		if(diff != TIME_INTERVALE)
			printf("-----pthread_fun_test------> %lld\n",diff);
		#endif 
		
	}
	
#undef TIME_INTERVALE
}








#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


