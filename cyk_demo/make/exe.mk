##################################################################
#####	File Name   : 
#####	Description :  
#####	    Author   :  changyukun
##################################################################


APPOBJS := $(patsubst %.c,%.o,$(wildcard *.c))

all: 	appobj

appobj:	$(APPOBJS)
	$(CXX) $(APPOBJS) $(CFG_LIB_DIR) -Wl,--start-group $(CFG_LIB) -Wl,--end-group $(CFG_FLAGS_EXE) -o $(FILE_ELF).out -Wl,-Map=$(FILE_MAP).map
	$(STRIP) $(FILE_ELF).out

