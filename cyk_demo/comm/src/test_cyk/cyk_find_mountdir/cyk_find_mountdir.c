
/*****************************************************************************
File Name   :  cyk_find_mountdir.c

Description :  

     Author   :  changyukun

*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif



/*========== 头文件==============================================================================================*/
#include <sys/statfs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "stdio.h"
#include <unistd.h>
#include <string.h>






/*========== 外部变量引用=======================================================================================*/






/*========== 宏定义==============================================================================================*/






/*========== 数据结构定义======================================================================================*/






/*========== 变量定义===========================================================================================*/
			






/*========== 函数实现===========================================================================================*/

//-------------------------------------------------------------------------------------------------
int findUdiskDir(char* uMountDir, int len, int* uNum)
{
/*
	参数:
		1、uMountDir	: 传入用于返回获得到目录的内存空间
		2、len			: 传入uMountDir buffer 的长度
		3、uNum		: 用于返回获得到的目录的个数
		
	返回:
		1、0:成功   1:传入的内存空间不足   -1:失败
		
	说明:
		1、获得到的两个目录之间用宏INTERVAL 定义的内容隔开
*/
	#define XXX_DATA_LEN 	1024
	#define CONDITION 		"/dev/sd"	
	#define INTERVAL			" "

	FILE* file = NULL;  
	char buf[XXX_DATA_LEN];
	char* p = NULL;
	int i = 0, num = 0, result = 0;

	if(NULL == uMountDir || len <= 0)
	{
		result = -1;
		goto out;
	}

	file = popen( "fdisk -l", "r" ); 

	if(file == NULL)
	{
		result = -1;
		goto out;
	}

	memset(uMountDir, 0, len);

	while(1)
	{
		memset(buf, 0, XXX_DATA_LEN);

		if(!(fgets(buf, XXX_DATA_LEN, file)))
			break;
		
		//printf("-----> %s",buf);

		if(strncmp(buf, CONDITION, strlen(CONDITION)) == 0)
		{
			i = 0;
			for(p = strtok(buf," \t\r\n"); p; p = strtok(NULL," \t\r\n"))
			{
				if(i == 0)
				{
					if(0 != access(p,F_OK))
						break;
				}
				else if(i == 2)
				{
					if(num == 0)
					{
						len = len -strlen(p);
						if(len > 0)
							sprintf(uMountDir, p);
						else
						{
							result = 1;
							goto out;
						}
					}
					else
					{
						len = len-strlen(p) - strlen(INTERVAL);
						if(len > 0)
						{
							strcat(uMountDir, INTERVAL);
							strcat(uMountDir, p);
						}
						else
						{
							result = 1;
							goto out;
						}
					}
					num++;
					//printf("----> %s\n",p);
				}
				i++;
			}
		}
	}
	pclose(file); 

out:

	if(NULL != uNum)
		*uNum = num;

	#undef  XXX_DATA_LEN
	#undef  CONDITION
	#undef  INTERVAL
	
	return result;
}










#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


