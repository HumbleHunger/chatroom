CC=gcc
OBJ = server
all:
	$(CC)  -o  $(OBJ)  *.h    *.c     -lpthread  -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient
#	rm -rf *.o
 
clean:
	-rm -rf *.o
