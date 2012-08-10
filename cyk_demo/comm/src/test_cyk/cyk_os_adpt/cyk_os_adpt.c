/*****************************************************************************
File Name   :  cyk_os_adpt.c

Description :  

     Author   :  changyukun

*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif



/*========== 头文件==============================================================================================*/
#include "cyk_os_adpt.h"
#include "cyk_os_adpt_list.h"

#ifdef OS_LINUX

#include <stdio.h>
#include <time.h>
#include <sys/syscall.h>
	
#elif defined(OS_LINUX_ST)

#include <stdio.h>
#include <time.h>

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif






/*========== 外部变量引用=======================================================================================*/






/*========== 宏定义==============================================================================================*/


#ifdef OS_LINUX

	#define HL_TASK_LIST_HEAD_INIT do\
	{\
		if(task_list_head_inited==0)\
		{\
			INIT_LIST_HEAD(&task_list_head);\
			task_list_head_inited = 1;\
		}\
	}while(0)

	#define HL_MQ_LIST_HEAD_INIT do\
	{\
		if(mq_list_head_inited==0)\
		{\
			INIT_LIST_HEAD(&mq_list_head);\
			mq_list_head_inited = 1;\
		}\
	}while(0)
	
#elif defined(OS_LINUX_ST)

	#define HL_TASK_LIST_HEAD_INIT do\
	{\
		if(task_list_head_inited==0)\
		{\
			INIT_LIST_HEAD(&task_list_head);\
			task_list_head_inited = 1;\
		}\
	}while(0)

	#define HL_MQ_LIST_HEAD_INIT do\
	{\
		if(mq_list_head_inited==0)\
		{\
			INIT_LIST_HEAD(&mq_list_head);\
			mq_list_head_inited = 1;\
		}\
	}while(0)

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif




/*========== 数据结构定义=======================================================================================*/






/*========== 变量定义============================================================================================*/
#ifdef OS_LINUX

	static struct list_head 	task_list_head;
	static struct list_head 	mq_list_head;
	static int			task_list_head_inited = 0;
	static int			mq_list_head_inited = 0;
	static int 			task_count = 0;
	
#elif defined(OS_LINUX_ST)

	static struct list_head 	task_list_head;
	static struct list_head 	mq_list_head;
	static int			task_list_head_inited = 0;
	static int			mq_list_head_inited = 0;
	static int 			task_count = 0;
	static int 			ClkPerSec = 0;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif




/*========== 函数实现============================================================================================*/

//====================pthread function===========================================================================================
//-------------------------------------------------------------------------------------------------
static int find_from_id(hl_task_t *tid,hl_task_elemet_t **pelt)
{
/*
	参数:
		1、

	返回:
		1、0:成功  other:失败
		
	说明:
		1、
*/
#ifdef OS_LINUX

	hl_task_t _tid=0;
	hl_task_elemet_t *_pelt=NULL;

	HL_TASK_LIST_HEAD_INIT;

	if(tid == NULL || pelt == NULL)
		return -1;
	
	_tid = *tid;

	list_for_each_entry(_pelt, &task_list_head, stNode)
	{
		if (_pelt->tid == _tid)
		{
			*pelt=_pelt;
			return 0;
		}
	}

	return -2;

#elif defined(OS_LINUX_ST)

	hl_task_t _tid=0;
	hl_task_elemet_t *_pelt=NULL;

	HL_TASK_LIST_HEAD_INIT;

	if(tid == NULL || pelt == NULL)
		return -1;
	
	_tid = *tid;

	list_for_each_entry(_pelt, &task_list_head, stNode)
	{
		if (_pelt->tid == _tid)
		{
			*pelt=_pelt;
			return 0;
		}
	}

	return -2;
	
#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
static int find_from_name(char *tname,hl_task_elemet_t **pelt)
{
/*
	参数:
		1、

	返回:
		1、0:成功  other:失败
		
	说明:
		1、
*/
#ifdef OS_LINUX

	char* _tid=NULL;
	hl_task_elemet_t *_pelt=NULL;

	HL_TASK_LIST_HEAD_INIT;
	
	if(tname == NULL || pelt == NULL)
		return -1;
	
	_tid = tname;

	list_for_each_entry(_pelt, &task_list_head, stNode)
	{
		if(!strcmp(_pelt->task_name,_tid))
		{
			*pelt=_pelt;
			return 0;
		}
	}

	return -2;

#elif defined(OS_LINUX_ST)

	char* _tid=NULL;
	hl_task_elemet_t *_pelt=NULL;

	HL_TASK_LIST_HEAD_INIT;
	
	if(tname == NULL || pelt == NULL)
		return -1;
	
	_tid = tname;

	list_for_each_entry(_pelt, &task_list_head, stNode)
	{
		if(!strcmp(_pelt->task_name,_tid))
		{
			*pelt=_pelt;
			return 0;
		}
	}

	return -2;
	
#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
int hl_task_create(void(*Function)(void* Param), void* Param,hl_task_t** Task,int Priority,const char* Name,int StackSize,void* pStack, void* priv)
{
/*
	参数:
		1、Function		: [in] 线程函数
		2、Param		: [in] 对应线程函数的参数
		3、Task		: [in/out] 用于保存线程ID 
		4、Priority		: [in] 线程的优先级别
		5、Name		: [in] 线程的名字
		6、StackSize	: [in] 线程使用的堆栈大小( 单位字节)
		7、pStack		: [in] 线程使用的堆栈指针
		8、priv			: [in/out] 私有数据，根据不同的操作系统可能有特殊的参数、或者需要返回特殊的信息

	返回:
		1、0:成功  other:失败
		
	说明:
		1、
*/
#ifdef OS_LINUX

	int _err=0;
	hl_task_elemet_t *_pelt;
	hl_task_t *_ptid;
	pthread_attr_t _attr;
	struct sched_param  _sparam;

	HL_TASK_LIST_HEAD_INIT;

	printf("hl_task_create: %s !\n",Name);
	pthread_attr_init(&_attr);
	pthread_attr_setschedpolicy(&_attr,SCHED_RR);

	//min = 1 ; max = 99
	if (Priority < sched_get_priority_min(SCHED_RR))
		Priority = sched_get_priority_min(SCHED_RR);

	if (Priority > sched_get_priority_max(SCHED_RR))
		Priority = sched_get_priority_max(SCHED_RR);

	_sparam.sched_priority = Priority;
	pthread_attr_setschedparam(&_attr,&_sparam);

	if(StackSize>0)
	{
		if(StackSize<PTHREAD_STACK_MIN)
			StackSize = PTHREAD_STACK_MIN;

		_err = pthread_attr_setstacksize(&_attr, StackSize);
		if(_err != 0)
		{
			hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		}
	}

	if(pStack != NULL)
	{
		_err = pthread_attr_setstackaddr(&_attr, pStack);
		if(_err != 0)
		{
			hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		}
	}

	*Task = (hl_task_t *)hl_malloc(sizeof(hl_task_t));
	
	_err = pthread_create(*Task,&_attr,(void* (*)(void*))Function,(void *)Param);
	if(_err != 0)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		pthread_attr_destroy(&_attr);
		hl_free(*Task);
		return _err;
	}

	pthread_attr_destroy(&_attr);
	
	task_count++;
	_ptid=*Task;
	pthread_detach(*_ptid);
	
	_pelt=(hl_task_elemet_t*)hl_malloc(sizeof(hl_task_elemet_t));
	if(_pelt == NULL)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		hl_free(*Task);
		return -1;
	}
	memset(_pelt,0,sizeof(hl_task_elemet_t));

	if(NULL == Name)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		hl_free(*Task);
		hl_free(_pelt);
		return -1; 
	}
	strcpy(_pelt->task_name,Name);
	_pelt->tid=*_ptid;
	pthread_cond_init(&_pelt->cond,NULL);
	pthread_mutex_init(&_pelt->mutex,NULL);

	list_add_tail(&_pelt->stNode, &task_list_head);

	return 0;

#elif defined(OS_LINUX_ST)

	ST_ErrorCode_t ErrCode = ST_NO_ERROR;
	hl_task_elemet_t *_pelt;
	hl_task_t *_ptid;

	HL_TASK_LIST_HEAD_INIT;

	if(Function==NULL || StackSize<=0 || Task==NULL ||Priority<1 || Priority>99)
		return -1;

	ErrCode = STOS_TaskCreate(Function, Param, (STOS_Partition_t *)NULL, StackSize, &pStack, NULL, Task, NULL, Priority, Name, (STOS_TaskFlags_t)0);
	
	if(ErrCode != ST_NO_ERROR || (*Task) == NULL)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	task_count++;
	_ptid=*Task;
	
	_pelt=(hl_task_elemet_t*)hl_malloc(sizeof(hl_task_elemet_t));
	if(_pelt == NULL)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}
	memset(_pelt,0,sizeof(hl_task_elemet_t));

	if(NULL == Name)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		hl_free(_pelt);
		return -1; 
	}
	strcpy(_pelt->task_name,Name);
	_pelt->tid=*_ptid;

	list_add_tail(&_pelt->stNode, &task_list_head);

	return 0;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
int hl_task_delete(hl_task_t* Task)
{
/*
	参数:
		1、Task	: [in] 线程ID 

	返回:
		1、0:成功  other:失败
		
	说明:
		1、
*/
#ifdef OS_LINUX

	int _err=0;
	hl_task_t _tid=0;
	hl_task_elemet_t *_pelt=NULL;

	HL_TASK_LIST_HEAD_INIT;
	
	if(NULL == Task)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}
	
	_tid=*Task;
	if (pthread_cancel(_tid))
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}
	task_count--;
	
	_err=find_from_id(&_tid,&_pelt);
	if(_err != 0)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	list_del(&_pelt->stNode);
	
	pthread_cond_destroy(&_pelt->cond);
	pthread_mutex_destroy(&_pelt->mutex);
	hl_free(_pelt);

	hl_free(Task);

	return 0;

#elif defined(OS_LINUX_ST)

	int _err=0;
	hl_task_t _tid=0;
	hl_task_elemet_t *_pelt=NULL;

	HL_TASK_LIST_HEAD_INIT;

	if(Task==NULL)
		return -1;

	_tid=*Task;

	STOS_TaskDelete(Task, NULL, NULL, NULL);

	task_count--;
	
	_err=find_from_id(&_tid,&_pelt);
	if(_err != 0)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	list_del(&_pelt->stNode);
	hl_free(_pelt);

	return 0;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
hl_task_t  hl_task_selfid(void)
{
/*
	参数:
		1、
		
	返回:
		1、返回调用线程的ID 
		
	说明:
		1、
*/
#ifdef OS_LINUX

	hl_task_t _tid=0;

	HL_TASK_LIST_HEAD_INIT;
	
	_tid = pthread_self();
	return _tid;

#elif defined(OS_LINUX_ST)


#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
hl_task_t hl_task_id(char *tname)
{
/*
	参数:
		1、tname	: [in] 线程名字
		
	返回:
		1、成功返回线程ID, 失败返回0
		
	说明:
		1、
*/
#ifdef OS_LINUX

	int _err=0;
	hl_task_elemet_t *_pelt=NULL;

	HL_TASK_LIST_HEAD_INIT;
	
	_err=find_from_name(tname,&_pelt);
	if(_err != 0)
		return 0;
	
	return (hl_task_t)_pelt->tid;

#elif defined(OS_LINUX_ST)

	int _err=0;
	hl_task_elemet_t *_pelt=NULL;

	HL_TASK_LIST_HEAD_INIT;
	
	_err=find_from_name(tname,&_pelt);
	if(_err != 0)
		return 0;
	
	return (hl_task_t)_pelt->tid;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
 }

//-------------------------------------------------------------------------------------------------
hl_pid_t get_tid(void)
{
/*
	参数:
		1、
		
	返回:
		1、
		
	说明:
		1、获取线程的tid ( 同一进程下的多个线程具有相同的pid，但具有不同的tid，如用linux 下
			命令ps -eo %cpu,tid,time 即可看到同一个进程下的每个线程的tid，从而实现了将代码中用
			pthread_create 创建的线程与ps 、top 命令等看到的各个线程实现一一对应)
*/

#ifdef OS_LINUX

	return syscall(SYS_gettid);

#elif defined(OS_LINUX_ST)


#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif 
}

//-------------------------------------------------------------------------------------------------
hl_pid_t get_pid(void)
{
/*
	参数:
		1、
		
	返回:
		1、
		
	说明:
		1、获取线程的pid ( 即获得所属于进程的pid )
*/
#ifdef OS_LINUX

	return getpid();

#elif defined(OS_LINUX_ST)


#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif 
}

//-------------------------------------------------------------------------------------------------
char*  hl_task_name(hl_task_t *tid)
{
/*
	参数:
		1、tid	: [in] 线程ID
		
	返回:
		1、成功返回线程名字符串, 失败返回NULL
		
	说明:
		1、
*/
#ifdef OS_LINUX

	int _err=0;
	hl_task_elemet_t *_pelt=NULL;

	HL_TASK_LIST_HEAD_INIT;
	
	_err=find_from_id(tid,&_pelt);
	if(_err != 0)
		return NULL;

	return (char*)_pelt->task_name;

#elif defined(OS_LINUX_ST)

	int _err=0;
	hl_task_elemet_t *_pelt=NULL;

	HL_TASK_LIST_HEAD_INIT;
	
	_err=find_from_id(tid,&_pelt);
	if(_err != 0)
		return NULL;

	return (char*)_pelt->task_name;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
int  hl_task_suspendself(hl_task_t *tid)
{
/*
	参数:
		1、tid	: [in] 线程ID

	返回:
		1、0:成功  other:失败
		
	说明:
		1、
*/
#ifdef OS_LINUX

	int _err=0;
	hl_task_t _tid=0;
	hl_task_elemet_t *_pelt=NULL;

	HL_TASK_LIST_HEAD_INIT;
	
	if(NULL != tid)
		_tid=*tid;
	else
		_tid=hl_task_selfid();
	
	_err=find_from_id(&_tid,&_pelt);
	if(_err != 0)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}
	 
	_err=pthread_mutex_lock(&_pelt->mutex);
   
	if(0 != _err)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}
	
	_err= pthread_cond_wait(&_pelt->cond,&_pelt->mutex);
	if(0 != _err)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}
    
	_err=pthread_mutex_unlock(&_pelt->mutex);
	if(0 != _err)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	return 0;

#elif defined(OS_LINUX_ST)

	return 0;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
int  hl_task_resume(hl_task_t *tid)
{
/*
	参数:
		1、tid	: [in] 线程ID

	返回:
		1、0:成功  other:失败
		
	说明:
		1、
*/
#ifdef OS_LINUX

	int _err=0;
	hl_task_t _tid=0;
	hl_task_elemet_t *_pelt=NULL;

	HL_TASK_LIST_HEAD_INIT;

	if(NULL == tid)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}
	
	_tid=*tid;
	
	_err=find_from_id(&_tid,&_pelt);
	if(_err != 0)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}
	 
	_err=pthread_mutex_lock(&_pelt->mutex);
	if(0 != _err)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}
	
	_err= pthread_cond_signal(&_pelt->cond);
	if(0 != _err)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	_err=pthread_mutex_unlock(&_pelt->mutex);
	if(0 != _err)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	return 0;

#elif defined(OS_LINUX_ST)

	HL_TASK_LIST_HEAD_INIT;

	if(NULL == tid)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}
	
	STOS_TaskResume(tid);
	return 0;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
int hl_task_getcount(void)
{
/*
	参数:
		1、

	返回:
		1、0:成功  other:失败
		
	说明:
		1、获取线程数量( 由函数hl_task_create 创建的线程数量)
*/
#ifdef OS_LINUX

	HL_TASK_LIST_HEAD_INIT;
	return task_count;

#elif defined(OS_LINUX_ST)

	HL_TASK_LIST_HEAD_INIT;
	return task_count;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
void hl_task_print()
{
/*
	参数:
		1、

	返回:
		1、
		
	说明:
		1、打印所有线程信息( 由函数hl_task_create 创建的线程数量)
*/
#ifdef OS_LINUX

	hl_task_elemet_t *last=NULL;
	int _index=0;

	HL_TASK_LIST_HEAD_INIT;
	
	printf("\n\tAll Thread: %d \n\n",task_count);

	list_for_each_entry(last, &task_list_head, stNode)
	{
		printf("\ttask name:%d --> %s,tid-->%d \n",++_index,last->task_name,(int)(last->tid));
	}

#elif defined(OS_LINUX_ST)

	hl_task_elemet_t *last=NULL;
	int _index=0;

	HL_TASK_LIST_HEAD_INIT;
	
	printf("\n\tAll Thread: %d \n\n",task_count);

	list_for_each_entry(last, &task_list_head, stNode)
	{
		printf("\ttask name:%d --> %s,tid-->%d \n",++_index,last->task_name,last->tid);
	}

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}






//====================semahore function==========================================================================================

hl_sem_t * hl_sem_create( int value)
{
/*
	参数:
		1、value	: 传入信号量的初始值

	返回:
		1、返回信号量指针
		
	说明:
		1、创建一个信号量
*/
#ifdef OS_LINUX

	hl_sem_t *_sem=NULL;
	
	_sem = (hl_sem_t*)hl_malloc(sizeof(hl_sem_t));
	if(NULL == _sem)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return (hl_sem_t*)NULL;
	}

	if(SEM_VALUE_MAX < value)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return (hl_sem_t*)NULL;
	}
	
	if(sem_init(_sem,0,value) != 0)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return (hl_sem_t*)NULL;
	}

	return (hl_sem_t *)_sem;

#elif defined(OS_LINUX_ST)

	hl_sem_t *_sem=NULL;
	
	_sem = (hl_sem_t*)hl_malloc(sizeof(hl_sem_t));
	if(NULL == _sem)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return (hl_sem_t*)NULL;
	}

	if(SEM_VALUE_MAX < value)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return (hl_sem_t*)NULL;
	}
	
	if(sem_init(_sem,0,value) != 0)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return (hl_sem_t*)NULL;
	}

	return (hl_sem_t *)_sem;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}


//-------------------------------------------------------------------------------------------------
int hl_sem_delete( hl_sem_t *sem_p)
{
/*
	参数:
		1、sem_p	: 传入信号量的指针

	返回:
		1、0:成功  other:失败
		
	说明:
		1、删除一个信号量
*/
#ifdef OS_LINUX

	if(NULL == sem_p)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	if(sem_destroy(sem_p) != 0)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	hl_free(sem_p);
		
	return 0;

#elif defined(OS_LINUX_ST)

	if(NULL == sem_p)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	if(sem_destroy(sem_p) != 0)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	hl_free(sem_p);
		
	return 0;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
int hl_sem_signal(hl_sem_t * sem_p)
{
/*
	参数:
		1、sem_p	: 传入信号量的指针

	返回:
		1、0:成功  other:失败
		
	说明:
		1、发送一个信号量
*/
#ifdef OS_LINUX

	if(sem_p == NULL)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	return sem_post(sem_p);

#elif defined(OS_LINUX_ST)

	if(sem_p == NULL)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	return sem_post(sem_p);

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
int hl_sem_wait(hl_sem_t * sem_p)
{
/*
	参数:
		1、sem_p	: 传入信号量的指针

	返回:
		1、0:成功  other:失败
		
	说明:
		1、等待一个信号量
*/
#ifdef OS_LINUX

	if(sem_p == NULL)
	{
		hl_task_t tid = hl_task_selfid();
		hl_printf(("Error : %s -->%s -->line=%d [ID=%d]\n",__FILE__,__FUNCTION__,__LINE__,(int)tid));
		return -1;
	}

	return sem_wait(sem_p);

#elif defined(OS_LINUX_ST)

	if(sem_p == NULL)
	{
		hl_task_t tid = hl_task_selfid();
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	return sem_wait(sem_p);

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
int hl_sem_waittimeout(hl_sem_t * sem_p, hl_clock_t * millisecond_p)
{
/*
	参数:
		1、sem_p		: 传入信号量的指针
		2、millisecond_p	: 传入超时时间

	返回:
		1、0:成功  other:失败
		
	说明:
		1、超时等待一个信号量
*/
#ifdef OS_LINUX

	struct timespec _ts;
	unsigned long _sec=0,_msec=0;
			
	if(NULL == sem_p)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}
	
	if((hl_clock_t*)OS_TIMEOUT_INFINITY == millisecond_p)
	{
		return hl_sem_wait(sem_p);
	}

	if((hl_clock_t*)OS_TIMEOUT_IMMEDIATE == millisecond_p)
	{
		return sem_trywait(sem_p);
	}

	if(clock_gettime(CLOCK_REALTIME, &_ts) == -1)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	_msec =	*millisecond_p;
	_sec=_msec/1000; //get second count;
	_msec = _msec%1000;
	
	_ts.tv_sec +=  _sec;
	_ts.tv_nsec += (_msec*1000000);

	return sem_timedwait(sem_p,&_ts);

#elif defined(OS_LINUX_ST)

	struct timespec _ts;
	unsigned long _sec=0,_msec=0;
			
	if(NULL == sem_p)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}
	
	if((hl_clock_t*)OS_TIMEOUT_INFINITY == millisecond_p)
	{
		return hl_sem_wait(sem_p);
	}

	if((hl_clock_t*)OS_TIMEOUT_IMMEDIATE == millisecond_p)
	{
		return sem_trywait(sem_p);
	}

	if(clock_gettime(CLOCK_REALTIME, &_ts) == -1)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	_msec =	*millisecond_p;
	_sec=_msec/1000; //get second count;
	_msec = _msec%1000;
	
	_ts.tv_sec +=  _sec;
	_ts.tv_nsec += (_msec*1000000);

	return sem_timedwait(sem_p,&_ts);

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
int hl_sem_getvalue(hl_sem_t *sem_p, int *value)
{
/*
	参数:
		1、sem_p	: 传入信号量的指针
		2、value	: 用于返回信号量值

	返回:
		1、0:成功  other:失败
		
	说明:
		1、获取信号量值
*/
#ifdef OS_LINUX

	if(sem_p == NULL )
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	return sem_getvalue(sem_p,value);

#elif defined(OS_LINUX_ST)

	if(sem_p == NULL )
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	return sem_getvalue(sem_p,value);

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}






//====================mutex function=============================================================================================

int hl_mutex_create(hl_mutex_t* mt_p)
{
/*
	参数:
		1、mt_p	: 用于返回互斥锁

	返回:
		1、0:成功  other:失败
		
	说明:
		1、创建一个互斥锁
*/
#ifdef OS_LINUX

	/*
		初始化已经定义的互斥锁成功返回0
		如果mt_p==NULL 返回非零
		使用默认属性初始化锁
	*/
	int _err=0;
	pthread_mutexattr_t mutex_attr;

	pthread_mutexattr_init( &mutex_attr );
	//PTHREAD_MUTEX_TIMED_NP ,PTHREAD_MUTEX_RECURSIVE_NP ,PTHREAD_MUTEX_ERRORCHECK_NP ,PTHREAD_MUTEX_ERRORCHECK_NP
	pthread_mutexattr_settype( &mutex_attr,PTHREAD_MUTEX_RECURSIVE_NP );

	_err=pthread_mutex_init(mt_p,&mutex_attr);
	pthread_mutexattr_destroy(&mutex_attr);

	return _err;

#elif defined(OS_LINUX_ST)

	/*
		初始化已经定义的互斥锁成功返回0
		如果mt_p==NULL 返回非零
		使用默认属性初始化锁
	*/
	int _err=0;
	pthread_mutexattr_t mutex_attr;

	pthread_mutexattr_init( &mutex_attr );
	//PTHREAD_MUTEX_TIMED_NP ,PTHREAD_MUTEX_RECURSIVE_NP ,PTHREAD_MUTEX_ERRORCHECK_NP ,PTHREAD_MUTEX_ERRORCHECK_NP
	pthread_mutexattr_settype( &mutex_attr,PTHREAD_MUTEX_RECURSIVE_NP );

	_err=pthread_mutex_init(mt_p,&mutex_attr);
	pthread_mutexattr_destroy(&mutex_attr);

	return _err;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
int hl_mutex_destroy(hl_mutex_t *mt_p)
{
/*
	参数:
		1、mt_p	: 传入互斥锁指针

	返回:
		1、0:成功  other:失败
		
	说明:
		1、注销互斥锁
*/
#ifdef OS_LINUX

	int _error=0;

	if(!(_error=pthread_mutex_destroy(mt_p)))
		hl_free(mt_p);
	else
		return _error;

#elif defined(OS_LINUX_ST)

	int _error=0;

	if(!(_error=pthread_mutex_destroy(mt_p)))
		hl_free(mt_p);
	else
		return _error;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
int hl_mutex_lock(hl_mutex_t *mt_p)
{
/*
	参数:
		1、mt_p	: 传入互斥锁指针

	返回:
		1、0:成功  other:失败
		
	说明:
		1、锁定
*/
#ifdef OS_LINUX

    	return pthread_mutex_lock(mt_p);

#elif defined(OS_LINUX_ST)

	return pthread_mutex_lock(mt_p);

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
int hl_mutex_unlock(hl_mutex_t *mt_p)
{
/*
	参数:
		1、mt_p	: 传入互斥锁指针

	返回:
		1、0:成功  other:失败
		
	说明:
		1、解锁
*/
#ifdef OS_LINUX

    	return pthread_mutex_unlock(mt_p);

#elif defined(OS_LINUX_ST)

	return pthread_mutex_unlock(mt_p);

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}






//====================message function===========================================================================================

//-------------------------------------------------------------------------------------------------
static int find_from_mqt(hl_mq_t * tid,hl_mq_elemet_t **pelt)
{
/*
	参数:
		1、

	返回:
		1、0:成功  other:失败
		
	说明:
		1、
*/
#ifdef OS_LINUX

	hl_mq_t _tid=0;
	hl_mq_elemet_t *_pelt=NULL;

	HL_MQ_LIST_HEAD_INIT;

	if(tid == NULL || pelt == NULL)
		return -1;
	
	_tid = *tid;

	list_for_each_entry(_pelt, &mq_list_head, stNode)
	{
		if (_pelt->mq_t == _tid)
		{
			*pelt=_pelt;
			return 0;
		}
	}

	return -2;

#elif defined(OS_LINUX_ST)

	hl_mq_elemet_t *_pelt=NULL;

	HL_MQ_LIST_HEAD_INIT;

	if(tid == NULL || pelt == NULL)
		return -1;

	list_for_each_entry(_pelt, &mq_list_head, stNode)
	{
		if (_pelt->priv == (void*)tid)
		{
			*pelt=_pelt;
			return 0;
		}
	}

	return -2;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
hl_mq_t* hl_mq_create(hl_size_t element_size, unsigned int no_elements)
{
/*
	参数:
		1、element_size	: 传入每个消息体的大小( 字节)
		2、no_elements	: 传入消息体的个数

	返回:
		1、NULL:失败, 成功返回消息队列的句柄
		
	说明:
		1、
*/
#ifdef OS_LINUX

	#define FILE_MODE 0600

	static unsigned int s_count=0;
	struct mq_attr attr={0};
	hl_mq_elemet_t * mq_element=NULL;

	if(element_size<=0 || no_elements<=0)
		return NULL;

	HL_MQ_LIST_HEAD_INIT;

	if((mq_element = (hl_mq_elemet_t*)hl_malloc(sizeof(hl_mq_elemet_t))) == NULL)
		return NULL;

	memset(mq_element, 0, sizeof(hl_mq_elemet_t));

	sprintf(mq_element->mq_name,"/hualumq%d",s_count++);
	
	attr.mq_maxmsg=no_elements;
	attr.mq_msgsize=element_size;

again:
	mq_element->mq_t = mq_open(mq_element->mq_name, O_RDWR | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG, &attr);
	if((hl_mq_t)-1 == mq_element->mq_t)
	{
		if(errno==EEXIST)/* 消息队列已经存在*/
		{
			mq_unlink(mq_element->mq_name);
			goto again;
		}
		
		hl_free(mq_element);
		return NULL;
	}

	list_add_tail(&mq_element->stNode, &mq_list_head);
	return &(mq_element->mq_t);

#elif defined(OS_LINUX_ST)

	static unsigned int s_count=0;
	hl_mq_elemet_t * mq_element=NULL;

	if(element_size<=0 || no_elements<=0)
		return NULL;

	HL_MQ_LIST_HEAD_INIT;

	if((mq_element = (hl_mq_elemet_t*)hl_malloc(sizeof(hl_mq_elemet_t))) == NULL)
		return NULL;

	memset(mq_element, 0, sizeof(hl_mq_elemet_t));
	sprintf(mq_element->mq_name,"/hualumq%d",s_count++);

	mq_element->priv = (void*)STOS_MessageQueueCreate(element_size, no_elements);
	if(mq_element->priv == NULL)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		hl_free(mq_element);
		return NULL;
	}
	
	list_add_tail(&mq_element->stNode, &mq_list_head);
	return (hl_mq_t*)(mq_element->priv);

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------
int hl_mq_delete(hl_mq_t * mq_p)
{
/*
	参数:
		1、mq_p	: 传入消息队列的句柄

	返回:
		1、0:成功  other:失败
		
	说明:
		1、注销一个消息队列
*/
#ifdef OS_LINUX

	hl_mq_elemet_t *pelt=NULL;

	HL_MQ_LIST_HEAD_INIT;

	if(0!=find_from_mqt(mq_p, &pelt))
		return -1;

	mq_close(*mq_p);
	mq_unlink(pelt->mq_name);

	list_del(&pelt->stNode);
	hl_free(pelt);

	return 0;

#elif defined(OS_LINUX_ST)

	ST_ErrorCode_t ErrCode = ST_NO_ERROR;
	hl_mq_elemet_t *pelt=NULL;

	HL_MQ_LIST_HEAD_INIT;

	if(0!=find_from_mqt(mq_p, &pelt))
		return -1;

	ErrCode = STOS_MessageQueueDelete(mq_p);
	if(ST_NO_ERROR != ErrCode)
	{
		hl_printf(("Error : %s -->%s -->line=%d\n",__FILE__,__FUNCTION__,__LINE__));
		return -1;
	}

	list_del(&pelt->stNode);
	hl_free(pelt);

	return 0;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------	
int hl_mq_send(hl_mq_t * mq_p, void * msg_p, hl_size_t msg_size)
{
/*
	参数:
		1、mq_p	: 传入消息队列的句柄
		2、msg_p	: 传入待发送的消息数据
		3、msg_size	: 传入消息数据的大小( 字节)

	返回:
		1、0:成功  other:失败
		
	说明:
		1、发送一个消息
*/
#ifdef OS_LINUX

	struct mq_attr attr;
	int err=0;

	if((err = mq_getattr(*mq_p, &attr)) != 0)
		return -1;
	
	if((msg_size != attr.mq_msgsize) && (msg_size < attr.mq_msgsize))
		return -1;
	
	if((err = mq_send(*mq_p, (char*)msg_p, msg_size, 0)) == -1)
		return -1;

	return 0;

#elif defined(OS_LINUX_ST)

	void* message = NULL;

	if((mq_p == NULL)||(NULL == msg_p) || msg_size<=0)
		return -1;

	message = (void*)STOS_MessageQueueClaimTimeout(mq_p,TIMEOUT_IMMEDIATE);
	if(NULL == message)
		return -1;

	memcpy(message, msg_p, msg_size);

	STOS_MessageQueueSend(mq_p, (void *)message);

	return 0;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------		
int hl_mq_receive (hl_mq_t* mq_p, void * msg_p, hl_size_t msg_size)
{
/*
	参数:
		1、mq_p	: 传入消息队列的句柄
		2、msg_p	: 用于返回消息数据的buffer
		3、msg_size	: 传入用于返回消息数据buffer 的大小( 字节)

	返回:
		1、0:成功  other:失败
		
	说明:
		1、阻塞接收一个消息
*/
#ifdef OS_LINUX

	struct mq_attr attr;
	int err=0;

	if((err = mq_getattr(*mq_p, &attr)) != 0)
		return -1;

	if((msg_size != attr.mq_msgsize) && (msg_size < attr.mq_msgsize))
		return -1;
	
	if((err = mq_receive(*mq_p, (char*)msg_p, msg_size, 0)) < 0)
		return -1;

	return err;

#elif defined(OS_LINUX_ST)

	void* message = NULL;

	if((mq_p == NULL)||(NULL == msg_p) || msg_size<=0)
		return -1;

	message = (void*)STOS_MessageQueueReceive(mq_p);
	if(NULL == message)
		return -1;
	
	memcpy(msg_p, message, msg_size);
	STOS_MessageQueueRelease(mq_p, message);
	return 0;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------	
int hl_mq_receivetimeout(hl_mq_t* mq_p , void * msg_p, hl_size_t msg_size, int millisecond_p)
{
/*
	参数:
		1、mq_p		: 传入消息队列的句柄
		2、msg_p		: 用于返回消息数据的buffer
		3、msg_size		: 传入用于返回消息数据buffer 的大小( 字节)
		4、millisecond_p	: 传入超时时间( 单位: 毫秒 OS_TIMEOUT_INFINITY : 无限等待 OS_TIMEOUT_IMMEDIATE : 不等待)

	返回:
		1、0:成功  other:失败
		
	说明:
		1、超时接收一个消息
*/
#ifdef OS_LINUX

	struct timespec _ts;
	struct mq_attr attr;
	hl_clock_t _total_msec=0;
	int err=0;
	unsigned long _sec=0,_msec=0;

	if(OS_TIMEOUT_INFINITY == millisecond_p)
		return hl_mq_receive(mq_p, msg_p, msg_size);

	if((err = mq_getattr(*mq_p, &attr)) != 0)
		return -1;

	if((msg_size != attr.mq_msgsize) && (msg_size < attr.mq_msgsize))
		return -1;

	if(OS_TIMEOUT_IMMEDIATE == millisecond_p)
		_total_msec = 0;
	else
		_total_msec = millisecond_p;

	if(clock_gettime(CLOCK_REALTIME, &_ts) == -1)
		return -1;

	_msec =	_total_msec;
	_sec=_msec/1000;
	_msec = _msec%1000;
	
	_ts.tv_sec +=  _sec;
	_ts.tv_nsec += (_msec*1000000);

	if((err = mq_timedreceive(*mq_p, (char*)msg_p, msg_size, 0, &_ts)) < 0)
		return -1;
	
	return err;

#elif defined(OS_LINUX_ST)

	void* message = NULL;
	STOS_Clock_t clock_time;

	if(OS_TIMEOUT_INFINITY == millisecond_p)
	{
		message = (void*)STOS_MessageQueueReceiveTimeout(mq_p, TIMEOUT_INFINITY);
		if(message == NULL)
			return -1;
	}
	else if(OS_TIMEOUT_IMMEDIATE == millisecond_p)
	{
		message = (void*)STOS_MessageQueueReceiveTimeout(mq_p, TIMEOUT_IMMEDIATE);
		if(message == NULL)
			return -1;
	}
	else
	{
		clock_time = STOS_time_plus(STOS_time_now(), (unsigned int)(millisecond_p*((ST_GetClocksPerSecond())/1000)));
		message = (void*)STOS_MessageQueueReceiveTimeout(mq_p, (STOS_Clock_t *)&clock_time);
		if(message == NULL)
			return -1;
	}
	
	memcpy(msg_p, message, msg_size);
	STOS_MessageQueueRelease(mq_p, message);
	return 0;

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}








//====================memory function===========================================================================================

//-------------------------------------------------------------------------------------------------	
void* hl_malloc(int size)
{
/*
	参数:
		1、size	 : 传入待申请内存空间的大小

	返回:
		1、NULL : 失败  otehr : 成功
		
	说明:
		1、申请内存
*/
#ifdef OS_LINUX

	if(size<=0)
		return NULL;
	
	return (void*)malloc(size);

#elif defined(OS_LINUX_ST)

	if(size<=0)
		return NULL;
	
	return (void*)malloc(size);

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}

//-------------------------------------------------------------------------------------------------	
void hl_free(void* pbuf)
{
/*
	参数:
		1、pbuf	 : 传入待释放指针

	返回:
		1、
		
	说明:
		1、释放内存
*/

#ifdef OS_LINUX

	free(pbuf);

#elif defined(OS_LINUX_ST)

	free(pbuf);

#elif defined(OS_ECOS)


#elif defined(OS_OS21)


#endif
}























#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif 


