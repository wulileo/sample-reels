all : cookbook
.PHONY : all

cookbook : cookbook.c
	gcc cookbook.c -o cookbook -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient
	
clean :
	@rm cookbook