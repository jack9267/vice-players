GCC = g++
OUTFILE = "../bin/rcon"

COMPILE_FLAGS = -c -O2 -fno-strict-aliasing -w -DLINUX

all:
	$(GCC) $(COMPILE_FLAGS) ./main_linux.cpp rconclient.cpp
	$(GCC) $(COMPILE_FLAGS) ../RakNet/*.cpp
	$(GCC) -O2 -o $(OUTFILE) *.o -lpthread
	-rm -f *~ *.o
