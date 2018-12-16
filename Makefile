#Top  Makefile

all:
	$(MAKE) -C ./pub  
	$(MAKE) -C ./sub  

clean:
	$(MAKE) -C ./pub clean
	$(MAKE) -C ./sub clean

.PHONY:all clean



