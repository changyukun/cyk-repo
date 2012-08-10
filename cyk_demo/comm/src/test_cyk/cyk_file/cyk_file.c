/*****************************************************************************
File Name   :  hl_file.c

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
#define DIR_MAX_LEN 	255






/*========== 数据结构定义=======================================================================================*/








/*========== 变量定义============================================================================================*/







/*========== 函数实现============================================================================================*/
//-------------------------------------------------------------------------------------------------
int hl_file_op(unsigned char type, char*FullPathName, char* filename, void* privateData)
{
/*
	参数:
		1、type			: 	0: 打开路径
						  	1: 关闭路径
						  	2: 读取打开的路径中的子目录或文件名，由参数filename 返回
						  	3: 判断传入的FullPathName 路径是否为一个目录
						  	4: 获得传入目录的上级目录，由参数FullPathName 返回，参数filename 返回子目录或文件的名字
						  	5: 根据传入的路径参数创建一个目录( 由FullPathName 指定要创建的目录)
						  	6: 删除传入的目录( 由FullPathName 指定要删除的目录)
						  	7: 删除传入的文件( 由FullPathName 指定要删除文件的全路径)
						  	8: 修改文件属性，文件全路径由参数FullPathName 传入，添加的属性由参数private 传入，0777 为全属性
		2、FullPathName	: 
		3、filename		: 

	返回:
		1、0:成功 other:失败
	
	说明:
		1、
*/
	
	int				result = 0;
	int				found = 0;
	static DIR* 		dirp = NULL;
	DIR* 			dirp_temp = NULL;
	struct dirent*		direntp;

	switch(type)
	{
		case 0:	/* 打开路径*/
			{
				if(dirp!=NULL)
				{
					closedir(dirp);
					dirp = NULL;
				}
				dirp = opendir(FullPathName);
				if(dirp==NULL) 
					result = -1;
			}
			break;
			
		case 1:	/* 关闭路径*/
			{
				closedir(dirp);
				dirp = NULL;
			}
			break;
			
		case 2:	/* 读取路径，返回子目录或文件名*/
			{
				while((direntp = readdir(dirp))!=NULL)
				{
					if((strcmp(direntp->d_name,".")!=0) && (strcmp(direntp->d_name,"..")!=0))
					{
						if(NULL != filename) 
							strcpy(filename,direntp->d_name); 
						found = 1;
						break;
					}
					else
					{
						if((strcmp(direntp->d_name,".")!=0) || (strcmp(direntp->d_name,"..")!=0))
							continue;
						else
						{
							found = 0;
							break;
						}
					}
				}
				
				if(!found)
					result = -1;
			}
			break;
			
		case 3:	/* 判断是否为一个真正的目录*/
			{
				dirp_temp = opendir(FullPathName);
				if(dirp_temp==NULL) 
					result = -1;
				else 
					closedir(dirp_temp);
			}
			break;

		case 4:	/* 获得传入目录的父目录*/
			{
				int j,k;
				for (j=strlen((char *)FullPathName);j!=0;j--)
				{
					if(FullPathName[j]=='/') 
						break;
				}
				if (j==0) 
				{
					result = -1;
					break;
				}
				if (FullPathName[j]=='/') { k=j; j=strlen((char *)FullPathName); }
				else
				{
					result = -1;
					break;
				}
				if(filename != NULL)
				{
					strncpy(filename,(char *)&FullPathName[k+1],(j-k-1));
					filename[(j-k-1)]=0;
				}
				FullPathName[k]=0;
			}
			break;

		case 5:	/* 根据传入的路径参数创建一个目录*/
			{
				#if 1
					/* 逐级分析目录并进行创建*/
					int j,k;
					char FullDir[DIR_MAX_LEN] = {0};
					char SubDir[DIR_MAX_LEN] = {0};

					sprintf(FullDir,"%s",FullPathName);
					while(1)
					{
						for (j=strlen((char *)FullDir);j!=0;j--)
						{
							if(FullDir[j]=='/') 
								break;
						}
						if (j==0) 
						{
							result = -1;
							break;
						}
						if (FullDir[j]=='/') { k=j; j=strlen((char *)FullDir); }
						else
						{
							result = -1;
							break;
						}
						strncpy(SubDir,(char *)&FullDir[k+1],(j-k-1));
						SubDir[(j-k-1)]=0;
						FullDir[k]=0;

						dirp_temp = opendir(FullDir);
						if(dirp_temp==NULL) 
							continue;
						else 
						{
							closedir(dirp_temp);
							sprintf(FullDir,"%s/%s",FullDir,SubDir);
							if(mkdir(FullDir,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) != 0)
								return -1;
						}

						sprintf(FullDir,"%s",FullPathName);	
						dirp_temp = opendir(FullDir);
						if(dirp_temp != NULL)
						{
							closedir(dirp_temp);
							break;
						}
					}
				#else
					/* 不分析目录直接进行创建*/
					if(mkdir(FullPathName,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) != 0)
						result = -1;
				#endif
			}
			break;

		case 6:
			{
				if(0!=rmdir(FullPathName)) /* 删除传入的路径*/
					result = -1;
			}
			break;

		case 7:/* 删除文件*/
			if(0 == access(FullPathName,F_OK))
			{
				if(0!=remove(FullPathName))	
					result = -1;
			}
			break;

		case 8:/* 修改文件属性*/
			{
				/*
					S_ISUID 04000 文件的(set user-id on execution) 位
					S_ISGID 02000 文件的(set group-id on execution) 位
					S_ISVTX 01000 文件的sticky位
					S_IRUSR (S_IREAD) 00400 文件所有者具可读取权限
					S_IWUSR (S_IWRITE) 00200 文件所有者具可写入权限
					S_IXUSR (S_IEXEC) 00100 文件所有者具可执行权限
					S_IRGRP 00040 用户组具可读取权限
					S_IWGRP 00020 用户组具可写入权限
					S_IXGRP 00010 用户组具可执行权限
					S_IROTH 00004 其他用户具可读取权限
					S_IWOTH 00002 其他用户具可写入权限
					S_IXOTH 00001 其他用户具可执行权限
				*/
				int mod = (int)privateData;
				chmod(FullPathName, mod);
			}
			break;
			
		default:
			result = -1;
			break;
	}

	return result;
}





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif 


