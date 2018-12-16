#subcriber Makefile

cc=g++
CPP_source=Menu.cpp Subcriber.cpp SubMenu.cpp Station.cpp myFile.cpp
obj=Menu.o Subcriber.o SubMenu.o Station.o myFile.o
Ice_lib=-lIce -lIceUtil -lIceStorm

subcriber:$(obj)
	$(cc) -o $@ $(Ice_lib) $(obj)
	cp $@ ../

.PHONY:clean

subcriber.o:Subcriber.cpp SubMenu.cpp Menu.cpp myFile.cpp
	$(cc) -c -g $<

clean:
	rm *.o subcriber ../subcriber 
