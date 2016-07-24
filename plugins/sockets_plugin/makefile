GCC = g++
OUTFILE = "../../../trunk/bin/plugins/sockets.so"

COMPILE_FLAGS = -g -c -O2 -fno-strict-aliasing -MD -w -DLINUX -I../squirrel/

all:
	$(GCC) $(COMPILE_FLAGS) ./*.cpp
	$(GCC) $(COMPILE_FLAGS) ../squirrel/*.cpp
	$(GCC) -O2 -fPIC -lpthread -lstdc++ -shared -o $(OUTFILE) *.o
	-rm -f *~ *.o

