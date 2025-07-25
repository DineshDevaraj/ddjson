
all:
	echo "Valid Usage: make src, make test or make clean"

src: 
	make -f Makefile.src

test:
	make -f Makefile.test

clean:
	make -f Makefile.src clean
	make -f Makefile.test clean
