TDIR=.

all: p3

%.o: %.c
	g++ p3.cpp -g -c -o -fopenmp $@ $<

p3: p3.o
	g++ -g  -o p3 p3.o

clean:
	rm -f p3 *.o
