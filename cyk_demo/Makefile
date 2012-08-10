##################################################################
#####	File Name   : 
#####	Description :  
#####	    Author   :  changyukun
##################################################################


#=================================================================================
#=================================================================================
#=================================================================================
export CROSS = sh4-linux-
export CC = $(CROSS)gcc
export CXX = $(CROSS)g++
export AR := $(CROSS)ar
export AS := $(CROSS)as
export STRIP := $(CROSS)strip
export CPP = $(CC) -E
export OBJCOPY = $(CROSS)objcopy
export OBJDUMP = $(CROSS)objdump
export NM = $(CROSS)nm
export LD = $(CROSS)ld



#=================================================================================
#=================================================================================
#=================================================================================
export ROOT = $(shell pwd)
export ROOT_APP = $(ROOT)/app
export ROOT_COMM = $(ROOT)/comm
export ROOT_DRIVER = $(ROOT)/driver
export ROOT_IMG = $(ROOT)/img
export ROOT_MAKE = $(ROOT)/make
export ROOT_LIB = $(ROOT)/lib


	


			 

#=================================================================================		 
#=== project 相关设置===============================================================
#=================================================================================

include ./config

########指定本工程用户头文件所在的目录
CFG_PRJ_INC := 	-I$(ROOT_DRIVER)/include \
				-I$(ROOT_COMM)/include \
			       -I$(ROOT_APP)/include


########指定本工程用户库文件所在的目录
CFG_PRJ_LIB_DIR := 	-L$(ROOT_APP)/lib/ \
				       -L$(ROOT_COMM)/lib/ \
				       -L$(ROOT_DRIVER)/lib/ \
				       -L$(ROOT_LIB)/lib_platform/\
				       -L$(ROOT_LIB)/lib_hualu/ \
				       -L$(ROOT_LIB)/lib_comm/
				       
########指定本工程用户库文件
CFG_PRJ_LIB := 

########指定本工程用户添加的编译选项
CFG_PRJ_FLAGS := 




#=================================================================================
#=== 标准库相关设置============================================================
#=================================================================================
CFG_LIB_SDT 		:= -ldl -lpthread -lrt -lz -lm
CFG_FLAGS_SDT 		:= -o2 -fPIC -g -Wall -fno-strict-aliasing
CFG_FLAGS_SDT_EXE 	:= -g




#=================================================================================
#=================================================================================
#=================================================================================
CFG_INC    		:= $(CFG_PRJ_INC) $(HUALU_CONFIG_CINC_DIR)
CFG_LIB_DIR  	:= $(CFG_PRJ_LIB_DIR) $(HUALU_CONFIG_CLIB_DIR)
CFG_LIB          	:= $(CFG_PRJ_LIB) $(HUALU_CONFIG_CLIB) $(CFG_LIB_SDT)
CPPFLAGS	 	:= $(CFG_FLAGS_SDT) $(CFG_INC) $(CFG_PRJ_FLAGS) $(HUALU_CONFIG_CFLAG)
CFG_FLAGS_EXE	:= $(CFG_FLAGS_SDT_EXE)


export CFG_INC CPPFLAGS CFG_LIB CFG_LIB_DIR CFG_FLAGS_EXE



#==========================================
.PHONY: all versionupdate driver comm app stinstall okok
all: versionupdate driver comm app okok

clean: app_clean comm_clean driver_clean img_clean lib_clean



##========================================
driver:
	make -C $(ROOT_DRIVER)
driver_clean:
	make -C $(ROOT_DRIVER) clean
driver_lib:
	cp -rf $(ROOT_DRIVER)/lib/* $(ROOT_LIB)/lib_hualu/


##========================================
comm:
	make -C $(ROOT_COMM)
comm_clean:
	make -C $(ROOT_COMM) clean
comm_lib:
	cp -rf $(ROOT_COMM)/lib/* $(ROOT_LIB)/lib_hualu/


##========================================
app:
	make -C $(ROOT_APP)
app_clean:
	make -C $(ROOT_APP) clean
app_lib:
	cp -rf $(ROOT_APP)/lib/* $(ROOT_LIB)/lib_hualu/

##========================================
lib:  driver_lib comm_lib app_lib


##========================================
okok:
	echo compile ok ...


##========================================
lib_clean:
	rm -rf $(ROOT_DRIVER)/lib/*
	rm -rf $(ROOT_COMM)/lib/*
	rm -rf $(ROOT_APP)/lib/*
	rm -rf $(ROOT_LIB)/lib_hualu/*


##========================================
img_clean:
	rm -rf $(ROOT_IMG)/*.out
	rm -rf $(ROOT_IMG)/*.map


##========================================
versionupdate:
	rm -rf $(ROOT_APP)/src_exe/main.o














