##################################################################
#####	File Name   : 
#####	Description :  
#####	    Author   :  changyukun
##################################################################


SUPPORTOBJS := $(patsubst %.c,%.o,$(wildcard *.c)) ./*.o

ifeq "$(XXX_TEMP)" "1"
all: supportobj install temp
else
all: supportobj install
endif

supportobj:	$(SUPPORTOBJS)
install: supportobj
	$(AR) -r $(LIB_FILE) $(SUPPORTOBJS)

