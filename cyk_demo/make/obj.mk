##################################################################
#####	File Name   : 
#####	Description :  
#####	    Author   :  changyukun
##################################################################



THIRDPART_OBJS_C := $(patsubst %.c,%.o,$(wildcard *.c))
THIRDPART_OBJS_CPP := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

ifeq "$(XXX_TEMP)" "1"
all: 	thirdpartobj_c thirdpartobj_cpp install temp
else
all: 	thirdpartobj_c thirdpartobj_cpp install
endif


thirdpartobj_c : $(THIRDPART_OBJS_C)
thirdpartobj_cpp : $(THIRDPART_OBJS_CPP)

install: thirdpartobj_c thirdpartobj_cpp
	cp $(THIRDPART_OBJS_C) $(THIRDPART_OBJS_CPP) $(DES_DIR) -f

