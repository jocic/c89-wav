GCC=gcc

OUTPUT=./assets/unit-tests

STD=c89

unit-tests:
	gcc -I source -o $(OUTPUT) -std=$(STD)\
		source/bin.h \
		source/format.h \
		tests/binary-io.h \
		tests/wav-format.h \
		tests/functionality.h \
		source/bin.c \
		source/format.c \
		tests/binary-io.c \
		tests/wav-format.c \
		tests/functionality.c \
		main.c

install:
	gcc -c -fPIC \
		source/bin.c \
		source/format.c

	ar rcs c89-wav.a \
		bin.o \
		format.o
	
	rm ./bin.o ./format.o
	
	mv c89-wav.a /usr/lib
	
	mkdir /usr/include/c89-wav

	cp source/bin.h /usr/include/c89-wav
	cp source/format.h /usr/include/c89-wav
