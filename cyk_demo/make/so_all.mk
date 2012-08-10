##################################################################
#####	File Name   : 
#####	Description :  
#####	    Author   :  changyukun
##################################################################


CFILE := $(wildcard *.c)
CPLUSPLUSFILE := $(wildcard *.cpp)

CFLAGS_SO = $(filter-out -c,$(CPPFLAGS))

ifeq "$(XXX_TEMP)" "1"
all: install temp
else
all: install
endif


install: 
	$(CXX) -shared $(CFLAGS_SO) $(M_FLAG) -o $(LIB_FILE) $(CPLUSPLUSFILE) $(CFILE) $(CFG_LIB_DIR) $(LIB_LIB)



