/*****************************************************************************
File Name   :  cyk_module.c

Description :  此文件用于测试ko 的编译及生成过程( 调试过程中如看不到打印可通过dmesg 命令查看)

     Author   :  changyukun

*****************************************************************************/





/*========== 头文件==============================================================================================*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/proc_fs.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#include <asm/errno.h>






/*========== 外部变量引用=======================================================================================*/





/*========== 函数声明============================================================================================*/
int cyk_open(struct inode *node, struct file *filp);
ssize_t cyk_read(struct file *filp, char *buffer, size_t size, loff_t *offset);
ssize_t cyk_write(struct file * file,const char *buffer,size_t length,loff_t *offset);





/*========== 宏定义==============================================================================================*/






/*========== 数据结构定义=======================================================================================*/






/*========== 变量定义============================================================================================*/

static unsigned int major = 0;

static struct file_operations cyk_fops = 
{
	open	: cyk_open,
	read		: cyk_read,  
	write	: cyk_write,   
};






/*========== 函数实现============================================================================================*/

//-------------------------------------------------------------------------------------------------
int cyk_open(struct inode* node, struct file* filp)
{
	printk("-------> cyk_open\n");
	return 0;
}

//-------------------------------------------------------------------------------------------------
ssize_t cyk_read(struct file* filp, char* buffer, size_t size, loff_t* offset)
{
	printk("-------> cyk_read\n");
	return 0;
}

//-------------------------------------------------------------------------------------------------
ssize_t cyk_write(struct file* file, const char* buffer, size_t length, loff_t* offset)
{
	printk("-------> cyk_write\n");
	return 0;
}

//-------------------------------------------------------------------------------------------------
static int __init cyk_init_module(void)
{
	major = register_chrdev(major, "cyk_ioctrl", &cyk_fops);
	printk("--cyk_ioctrl------> major=%d\n", major);

	return 0;
}

//-------------------------------------------------------------------------------------------------
static void __exit cyk_cleanup_module(void)
{
	unregister_chrdev(major, "cyk_ioctrl");
}



module_init(cyk_init_module);
module_exit(cyk_cleanup_module);

MODULE_LICENSE("CYK");  
MODULE_AUTHOR("changyukun");


