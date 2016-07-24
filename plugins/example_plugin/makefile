GCC = g++
OUTFILE = "../../../trunk/bin/plugins/example.so"

COMPILE_FLAGS = -c -O2 -fno-strict-aliasing -w -DLINUX -I../squirrel/

all:
	$(GCC) $(COMPILE_FLAGS) ./*.cpp
	$(GCC) $(COMPILE_FLAGS) ../squirrel/*.cpp
	$(GCC) -O2 -shared -o $(OUTFILE) *.o
	-rm -f *~ *.o

