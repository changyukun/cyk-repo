##################################################################
#####	File Name   : 
#####	Description :  
#####	    Author   :  changyukun
##################################################################


.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)
$(SUBDIRS):
	make -C $@

clean:
	find . -type f \
		\( -name '*.bak' \
		-o -name '*.o'  \) -print \
		| xargs rm -f

