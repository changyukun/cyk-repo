
obj-m := my_module.o
my_module-objs := cyk_module.o

#KERNELDIR :=/opt/prj/st7167/linux-sh4-2.6.23.17_stm23_A26
KERNELDIR :=/lib/modules/$(shell uname -r)/build


default:
	make -C $(KERNELDIR) M=$(shell pwd) modules

clean:
	make -C $(KERNELDIR) M=$(shell pwd) clean

