#publisher Makefile

cc=g++

VPATH=../

obj=Publisher.o Station.o
CPP_source=Publisher.cpp Station.cpp

ICE_Lib=-lIceUtil -lIceStorm -lIce


publisher:$(obj)
	$(cc) -o $@ $(ICE_Lib) $(obj)
	cp $@ ../
Publisher.o:$(CPP_source) Station.h
	$(cc) -c $<
Station.o:Station.cpp
	$(cc) -c $<

.PHONY:clean

clean:
	rm *.o publisher ../publisher

	
